#include "compiler.h"
#include "pccore.h"
#include "iocore.h"
#include "nevent.h"
#include "dosio.h"
#include "cmt.h"

// 新設した構造体の実体を宣言
CMT_STAT cmt_stat = { 0 };

// 既存のコードを変更せずに済むよう、マクロでエイリアスを張る
#define tape            (cmt_stat.table)
#define tape_filename   (cmt_stat.filename)
#define motor_interval  (cmt_stat.motor_interval)

/* ========================================================================
 * テープデータのファイル読み込み処理 (dosio使用)
 * ======================================================================== */
static BYTE *cmt_data_ptr(DWORD pos) {
	static BYTE dummy_data = 0;
	FILEH hdr;
	DWORD cur_ptr;
	DWORD page;
	DWORD header_offset;

	if (!tape.header.frequency) return &dummy_data;

	page = pos / CMT_PAGE_SIZE;
	pos %= CMT_PAGE_SIZE;
	
	/* キャッシュヒットしたらそのまま返す */
	if (tape.page == page) return &tape.buf[pos];

	hdr = file_open_rb(tape_filename);
	if (hdr == FILEH_INVALID) return &dummy_data;

	/* ヘッダの有無で実データの開始位置(オフセット)をずらす */
	header_offset = tape.has_header ? CMT_HEADER_SIZE : 0;
	cur_ptr = page * CMT_PAGE_SIZE + header_offset;

	if (cur_ptr + CMT_PAGE_SIZE > tape.filesize) {
		tape.buf_size = (UINT16)(tape.filesize - cur_ptr);
	} else {
		tape.buf_size = CMT_PAGE_SIZE;
	}

	file_seek(hdr, cur_ptr, FSEEK_SET);
	if (file_read(hdr, tape.buf, tape.buf_size) != tape.buf_size) {
		file_close(hdr);
		return &dummy_data;
	}

	file_close(hdr);
	tape.page = page;
	return &tape.buf[pos];
}

/* ========================================================================
 * テープモーター駆動タイマーコールバック (実機のボーレートに同期)
 * ======================================================================== */
static void cmt_play_callback(NEVENTID id) {
	if (tape.header.datasize > 0 && tape.header.position < tape.header.datasize - 1) {
		tape.header.position++;
		tape.sensor |= 1; /* モーター回転中 */
		
		/* 次の1ビット分のイベントを予約 */
		nevent_set(NEVENT_CMT, motor_interval, cmt_play_callback, NEVENT_RELATIVE);
	} else {
		/* テープ終端に達したら停止 */
		cmt_ctrl(0x01); /* CMT_STOP */
	}
}

/* ========================================================================
 * Z80メインCPUからのポート読み取り
 * ======================================================================== */
REG8 cmt_read(void) {
	BYTE data, bitmask;
	DWORD pos;

	if (tape.cur_cmd == 0x02) { /* CMT_PLAY */
		pos = tape.header.position;
		bitmask = 0x80 >> (pos & 7);
		
		data = *cmt_data_ptr(pos / 8);
		
		if (data & bitmask) {
			return 0x02; /* X1のテープ読み込みポートのビット仕様 */
		}
	}
	return 0x00;
}

/* ========================================================================
 * SubCPUとのハンドシェイク・ステータス処理
 * ======================================================================== */
REG8 cmt_tape_stat(void) {
	/* 余計なフラグを立てず、純粋なセンサー状態（テープあり等の情報）だけを返す */
	return tape.sensor;
}

REG8 cmt_ctrl_stat(void) {
	return tape.cur_cmd;
}

REG8 cmt_test(void) {
	/* オリジナルXmilにあった「ロードを強制アボートさせる罠」を完全撤去！
	 * センサーのBit 0 (モーター回転中フラグ) を見て、
	 * 回っていれば 0、止まっていれば 1 を正しく返すようにする。
	 */
	if (tape.sensor & 1) {
		return 0; /* モーター回転中 (IPLはこれを見て安心する) */
	}
	return 1; /* 停止中 */
}

void cmt_ctrl(REG8 cmd) {
//	OEMCHAR dbgmsg[256];
//	OEMSPRINTF(dbgmsg, OEMTEXT("CMD: 0x%02X\nPOS: %d\nSIZE: %d\nSENS: %d\nFREQ: %d"), 
//				cmd, tape.header.position, tape.header.datasize, tape.sensor, tape.header.frequency);
//	MessageBox(NULL, dbgmsg, OEMTEXT("CMT DEBUG"), MB_OK | MB_ICONINFORMATION);
	
	/* ステータス要求(0x80, 0x81)は状態を変更せずに処理を抜ける */
	if (cmd == 0x80 || cmd == 0x81) return;

	/* テープが入っていない場合の不正なコマンドを拒否 */
	if (tape.header.frequency == 0 && cmd != 0x00 && cmd != 0x01) {
		tape.cur_cmd = 0x01;
		return;
	}

	tape.cur_cmd = cmd;

	switch(cmd) {
		case 0x00: /* EJECT */
			nevent_reset(NEVENT_CMT);
			tape.sensor &= ~7;      /* モーターOFF(bit0), テープなし(bit1), プロテクト(bit2)をクリア */
			tape.header.frequency = 0;
			tape.filesize = 0;
			tape.has_header = 0;
			tape_filename[0] = '\0';
			break;

		case 0x01: /* STOP */
			nevent_reset(NEVENT_CMT);
			tape.sensor &= ~1;      /* 【重要】モーターOFF(bit0)のみ！テープあり(bit1)は維持する */
			break;

		case 0x02: /* PLAY */
			if (tape.header.frequency > 0 && (tape.sensor & 2)) {
				SINT32 delay_clock; /* 初回の遅延クロック */

				tape.sensor |= 1;
				
#if defined(FIX_Z80A)
				motor_interval = (2000000 * 2) / tape.header.frequency;
				delay_clock = (2000000 * 2 / 1000) * 500; /* 500msの遅延 */
#else
				motor_interval = pccore.realclock / tape.header.frequency;
				delay_clock = (pccore.realclock / 1000) * 500; /* 500msの遅延 */
#endif
				/* 初回のみ500ms遅らせてタイマー起動 */
				nevent_set(NEVENT_CMT, delay_clock, cmt_play_callback, NEVENT_RELATIVE);
			}
			break;

		case 0x03: /* FF */
			if (tape.header.datasize > 0) {
				tape.header.position = tape.header.datasize; /* 一瞬で終端(EOT)へ飛ぶ */
			}
			cmt_ctrl(0x01); /* 早送り完了としてそのままSTOP状態へ遷移させる */
			break;

		case 0x04: /* REW */
			tape.header.position = 0; /* 再生位置を一瞬で先頭(0)に戻す */
			cmt_ctrl(0x01);           /* 巻き戻し完了としてそのままSTOP状態へ遷移させる */
			break;

		case 0x05: /* AFF (早送り頭出し) */
		case 0x06: /* AREW (巻き戻し頭出し) */
			break;
	}
}

void cmt_reset(void) {
	cmt_ctrl(0x01); /* エミュレータのリセット時はSTOP状態にする */
}

/* ========================================================================
 * テープファイルのセットアップ (ファイル選択ダイアログ等から呼ばれる)
 * ======================================================================== */
void cmt_set(const OEMCHAR *fname) {
	FILEH hdr;
	
	cmt_ctrl(0x01); /* まずSTOP */
	
	/* ファイル名が空の場合はイジェクト処理を行う */
	if (fname == NULL || fname[0] == '\0') {
		cmt_ctrl(0x00);
		return;
	}

	if (fname && fname[0]) {
		file_cpyname(tape_filename, fname, NELEMENTS(tape_filename));
	}

	hdr = file_open_rb(tape_filename);
	if (hdr == FILEH_INVALID) return;

	tape.filesize = file_getsize(hdr);
	if (tape.filesize >= CMT_HEADER_SIZE) {
		file_read(hdr, &tape.header, CMT_HEADER_SIZE);
	} else {
		file_close(hdr);
		return;
	}
	file_close(hdr);

	/* エンディアンに依存しない確実なシグネチャ判定 ("TAPE") */
	if (memcmp(&tape.header.index, "TAPE", 4) == 0) {
		tape.has_header = 1;
		tape.header.position = 0; /* 再生位置を強制リセット */
		
		if (tape.header.datasize == 0 || tape.header.datasize > (tape.filesize * 8)) {
			tape.header.datasize = (tape.filesize - CMT_HEADER_SIZE) * 8;
		}
		if (tape.header.frequency == 0) {
			tape.header.frequency = 8000;
		}
	} else {
		/* ヘッダ無しのベタデータ(raw .tap) */
		tape.has_header = 0;
		tape.header.frequency = 8000;
		tape.header.datasize = tape.filesize * 8;
		tape.header.position = 0;
	}

	tape.sensor |= 6; /* bit1:テープあり, bit2:ライトプロテクト解除 */
	tape.page = (UINT32)-1;
}

/* ========================================================================
 * 未使用関数のスタブ (リンクエラー回避用)
 * ======================================================================== */
void cmt_write(REG8 dat) { (void)dat; }
