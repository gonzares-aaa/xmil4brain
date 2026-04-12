#include	"compiler.h"
#if defined(WIN32_PLATFORM_PSPC)
#include	<gx.h>
#endif
#include	"winkbd.h"
#include	"keystat.h"
#include	<string.h> // memcpyを使用するために追加

// ★ローマ字入力対応 ここから追加 
#include "romaji.h"
bool g_KanaMode = false; // カナキーのトグル状態
bool g_RomajiMode = true; // ローマ字入力モード
static bool g_CharSwitch = false; // 文字切替キーのトグル状態
// ★ここまで追加

#define		NC		0xff

#if defined(WIN32_PLATFORM_PSPC)
static UINT8 key106[256] =
#else
static const UINT8 key106[256] =
#endif
			//	    ,    ,    ,STOP,    ,    ,    ,    		; 0x00
		{		  NC,  NC,  NC,0x60,  NC,  NC,  NC,  NC,
			//	  BS, TAB,    ,    , CLR, ENT,    ,    		; 0x08
				0x0e,0x0f,  NC,  NC,  NC,0x1c,  NC,  NC,
			//	 SFT,CTRL, ALT,PAUS,CAPS,KANA,    ,    		; 0x10
				0x70,0x74,0x73,0x60,0x70,0x72,  NC,  NC,
			//	 FIN, KAN,    , ESC,XFER,NFER,    ,  MD		; 0x18
				  NC,  NC,  NC,0x00,0x35,0x51,  NC,  NC,
			//	 SPC,RLUP,RLDN, END,HOME,  ←,  ↑,  →		; 0x20
				0x34,0x37,0x36,0x3f,0x3e,0x3b,0x3a,0x3c,
			//	  ↓, SEL, PNT, EXE,COPY, INS, DEL, HLP		; 0x28
				0x3d,  NC,  NC,  NC,  NC,0x38,0x39,  NC,
			//	  ０,  １,  ２,  ３,  ４,  ５,  ６,  ７		; 0x30
				0x0a,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
			//	  ８,  ９,    ,    ,    ,    ,    ,    		; 0x38
				0x08,0x09,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,  Ａ,  Ｂ,  Ｃ,  Ｄ,  Ｅ,  Ｆ,  Ｇ		; 0x40
				  NC,0x1d,0x2d,0x2b,0x1f,0x12,0x20,0x21,
			//	  Ｈ,  Ｉ,  Ｊ,  Ｋ,  Ｌ,  Ｍ,  Ｎ,  Ｏ		; 0x48
				0x22,0x17,0x23,0x24,0x25,0x2f,0x2e,0x18,
			//	  Ｐ,  Ｑ,  Ｒ,  Ｓ,  Ｔ,  Ｕ,  Ｖ,  Ｗ		; 0x50
				0x19,0x10,0x13,0x1e,0x14,0x16,0x2c,0x11,
			//	  Ｘ,  Ｙ,  Ｚ,LWIN,RWIN, APP,    ,    		; 0x58
				0x2a,0x15,0x29,  NC,  NC,  NC,  NC,  NC,
			//	<０>,<１>,<２>,<３>,<４>,<５>,<６>,<７>		; 0x60
				0x4e,0x4a,0x4b,0x4c,0x46,0x47,0x48,0x42,
			//	<８>,<９>,<＊>,<＋>,<，>,<－>,<．>,<／>		; 0x68
				0x43,0x44,0x45,0x49,  NC,0x40,0x50,0x41,
			//	 f.1, f.2, f.3, f.4, f.5, f.6, f.7, f.8		; 0x70
#if defined(SIZE_QVGA)
				0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
#else
				0x62,0x63,0x64,0x65,0x66,0x67,0x66,0x69,
#endif
			//	 f.9, f10, f11, f12, f13, f14, f15, f16		; 0x78
#if defined(SIZE_QVGA)
				0x6a,0x6b,  NC,  NC,  NC,  NC,  NC,  NC,
#else
				0x6a,0x6b,  NC,0x72,  NC,  NC,  NC,  NC,
#endif
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0x80
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0x88
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	HELP, ALT,<＝>,    ,    ,    ,    ,    		; 0x90
				  NC,0x73,0x4d,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0x98
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0xa0
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0xa8
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0xb0
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,  ：,  ；,  ，,  －,  ．,  ／		; 0xb8
#if defined(SIZE_QVGA)
				  NC,  NC,0x27,0x26,0x30,0x0b,0x31,0x34,
#else
				  NC,  NC,0x27,0x26,0x30,0x0b,0x31,0x32,
#endif
			//	  ＠,    ,    ,    ,    ,    ,    ,    		; 0xc0
				0x1a,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0xc8
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0xd0
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,  ［,  ￥,  ］,  ＾,    		; 0xd8
#if defined(SIZE_QVGA)
				  NC,  NC,  NC,0x1b,0x72,0x28,0x0c,  NC,
#else
				  NC,  NC,  NC,0x1b,0x0d,0x28,0x0c,  NC,
#endif
			//	    ,    ,  ＿,    ,    ,    ,    ,    		; 0xe0
				  NC,  NC,0x33,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0xe8
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
			//	CAPS,    ,KANA,    ,    ,    ,    ,    		; 0xf0
				0x71,  NC,0x72,  NC,  NC,  NC,  NC,  NC,
			//	    ,    ,    ,    ,    ,    ,    ,    		; 0xf8
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC};

#if defined(WIN32_PLATFORM_PSPC)
static UINT8 key106ext[256] =
#else
static const UINT8 key106ext[256] =
#endif
		{		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,0x44,0x4c,0x4a,0x42,0x46,0x43,0x48,
				0x4b,  NC,  NC,  NC,  NC,0x4e,0x50,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
				  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC};

// ★追加：レイヤーマップ機能 (テーブル駆動方式)
static UINT8 key106_mapB[256];
static UINT8 key106_mapC[256];

void winkbd_init_keymaps() {
	// --- マップB (ローマ字OFF: 従来のBrain文字切替) 作成 ---
	memcpy(key106_mapB, key106, sizeof(key106));
	key106_mapB[0x51] = 0x01; // Q -> 1
	key106_mapB[0x57] = 0x02; // W -> 2
	key106_mapB[0x45] = 0x03; // E -> 3
	key106_mapB[0x52] = 0x04; // R -> 4
	key106_mapB[0x54] = 0x05; // T -> 5
	key106_mapB[0x59] = 0x06; // Y -> 6
	key106_mapB[0x55] = 0x07; // U -> 7
	key106_mapB[0x49] = 0x08; // I -> 8
	key106_mapB[0x4F] = 0x09; // O -> 9
	key106_mapB[0x50] = 0x0A; // P -> 0

	key106_mapB[0x44] = 0x0C; // D -> ^
	key106_mapB[0x46] = 0x0D; // F -> \ (ー)
	key106_mapB[0x47] = 0x1A; // G -> @ (゛)
	key106_mapB[0x48] = 0x1B; // H -> [ (゜)
	key106_mapB[0x4A] = 0x26; // J -> ;
	key106_mapB[0x4B] = 0x27; // K -> :
	key106_mapB[0x4C] = 0x28; // L -> ]

	key106_mapB[0x56] = 0x30; // V -> ,
	key106_mapB[0x42] = 0x31; // B -> .
	key106_mapB[0x4E] = 0x32; // N -> /
	key106_mapB[0x4D] = 0x33; // M -> _

	// --- マップC (ローマ字ON: カナ中の数字) 作成 ---
	memcpy(key106_mapC, key106_mapB, sizeof(key106));
	key106_mapC[0x51] = 0x4A; // Q -> テンキー 1
	key106_mapC[0x57] = 0x4B; // W -> テンキー 2
	key106_mapC[0x45] = 0x4C; // E -> テンキー 3
	key106_mapC[0x52] = 0x46; // R -> テンキー 4
	key106_mapC[0x54] = 0x47; // T -> テンキー 5
	key106_mapC[0x59] = 0x48; // Y -> テンキー 6
	key106_mapC[0x55] = 0x42; // U -> テンキー 7
	key106_mapC[0x49] = 0x43; // I -> テンキー 8
	key106_mapC[0x4F] = 0x44; // O -> テンキー 9
	key106_mapC[0x50] = 0x4E; // P -> テンキー 0
}

void winkbd_set_romajimode(bool mode) {
	g_RomajiMode = mode;
	romaji_clear(); // モードが切り替わった瞬間に、入力途中の文字を完全に破棄する
}
	
UINT8 GetMappedKey(WPARAM wParam) {
	UINT8 index = wParam & 0xff;

	if (!g_CharSwitch) {
		return key106[index];      // 文字切替OFFなら常にマップA
	}

	// --- ここから文字切替ONの場合 ---
	
	// ローマ字入力設定がON、かつ、現在カナモードの時だけ「マップC」を使う
	if (g_RomajiMode && g_KanaMode) {
		return key106_mapC[index]; // マップC (テンキー化＆記号)
	} else {
		// それ以外（ローマ字設定OFF、またはカナモードじゃない時）は「マップB」
		return key106_mapB[index]; // マップB (Brain本来の記号配列)
	}
}

void winkbd_keydown(WPARAM wParam, LPARAM lParam) {
	UINT8 data;

	// ★マップを解決してスキャンコードを取得
	data = GetMappedKey(wParam);

	// ★カナロックの同期処理
	if (data == 0x72) {
		if ((lParam & 0x40000000) == 0) {
			g_KanaMode = !g_KanaMode;
			romaji_clear(); 
		}
	}
	
	// ★ローマ字入力対応
	if (g_RomajiMode && g_KanaMode && data != NC) {
		// 文字切替がOFF(マップA)のときだけ、ローマ字エンジンに流す
		// （マップCが発動している時は素通りし、下部の送信処理へ直接向かう）
		if (!g_CharSwitch) {
			if (wParam >= 0x41 && wParam <= 0x5A) {
				romaji_input((char)wParam);
				return;
			}
			// A～Z 以外のキーが押された場合（かつ、Shift/Ctrl/Altの修飾キー単独押しではない場合）
			// 入力途中のローマ字バッファ（亡霊）をすべて破棄してキャンセルする
			if (wParam != 0x10 && wParam != 0x11 && wParam != 0x12) {
				romaji_clear(); 
			}
		}
	}

	if (data != NC) {
		if ((!(lParam & 0x01000000)) &&
				(key106ext[wParam & 0xff] != NC)) {			// ver0.28
			keystat_senddata(0x70);							// PC/AT only!
			data = key106ext[wParam & 0xff];
		}
		keystat_senddata(data);
	}
	else {													// ver0.28
		if (wParam == 0x0c) {
			keystat_senddata(0x70);							// PC/AT only
			keystat_senddata(0x47);
		}
	}
}

void winkbd_keyup(WPARAM wParam, LPARAM lParam) {
	UINT8 data;

// ★文字切替キーのトグル処理
#if defined(SIZE_QVGA)
	if ((wParam & 0xff) == 0x21) {	// 0x21 = [^前見出]キー(第2世代Brainのみ)
#else
	if ((wParam & 0xff) == 0x7A) {	// 0x7A = [文字切替]キー(第3世代Brainのみ)
#endif
		g_CharSwitch = !g_CharSwitch;
		romaji_clear(); // 文字切替が行われたら、入力途中の文字を破棄(キャンセル)する
	}

	data = GetMappedKey(wParam);

	// ★ローマ字入力対応 離上処理の無視
	if (g_RomajiMode && g_KanaMode && data != NC) {
		if (!g_CharSwitch) {
			if (wParam >= 0x41 && wParam <= 0x5A) {
				return; // アルファベットの離上は無視
			}
		}
	}
	
	if (data != NC) {
		if ((!(lParam & 0x01000000)) &&
				(key106ext[wParam & 0xff] != NC)) {		// ver0.28
			keystat_senddata(0x70 | 0x80);				// PC/AT only
			data = key106ext[wParam & 0xff];
		}
		keystat_senddata((UINT8)(data | 0x80));
	}
	else {												// ver0.28
		if (wParam == 0x0c) {
			keystat_senddata(0x70 | 0x80);				// PC/AT only
			keystat_senddata(0x47 | 0x80);
		}
	}
}


// ---- PocketPC keys

#if defined(WIN32_PLATFORM_PSPC)

extern	GXKeyList	gx_keylist;

typedef struct {
	short	*ptr[4];
} KEYADRS;

typedef struct {
	UINT8	key[4];
} KEYSET;

typedef struct {
	KEYADRS	curadrs;
	KEYADRS	btnadrs;
	KEYSET	curset[2];
	KEYSET	btnset[2];
} PPCBTNTBL;

typedef struct {
	KEYSET	cur;
	KEYSET	btn;
} PPCBTNDEF;


static const PPCBTNTBL ppcbtntbl = {
			{&gx_keylist.vkUp, &gx_keylist.vkDown,
			 &gx_keylist.vkLeft, &gx_keylist.vkRight},

			{&gx_keylist.vkA, &gx_keylist.vkB,
			 &gx_keylist.vkC, &gx_keylist.vkStart},

			{{0x3a, 0x3d, 0x3b, 0x3c},			// cur
			 {0x43, 0x4b, 0x46, 0x48}},			// tenkey

			{{0x1c, 0x34,   NC,   NC},		// RET/SP
			 {0x29, 0x2a,   NC,   NC}}};	// ZX

static	PPCBTNDEF	ppcbtndef;

static void getbind(KEYSET *bind, const UINT8 *tbl, const KEYADRS *adrs) {

	int		i;
	int		key;

	for (i=0; i<4; i++) {
		key = (*adrs->ptr[i]) & 0xff;
		bind->key[i] = tbl[key];
	}
}

static void setbind(UINT8 *tbl, const KEYSET *bind, const KEYADRS *adrs) {

	int		i;
	int		key;

	for (i=0; i<4; i++) {
		key = (*adrs->ptr[i]) & 0xff;
		if (tbl[key] != NC) {
			keystat_keyup(tbl[key]);
		}
		tbl[key] = bind->key[i];
	}
}

void winkbd_bindinit(void) {

	getbind(&ppcbtndef.cur, key106ext, &ppcbtntbl.curadrs);
	getbind(&ppcbtndef.btn, key106, &ppcbtntbl.btnadrs);
}

void winkbd_bindcur(UINT type) {

const KEYSET	*bind;

	switch(type) {
		case 0:
		default:
			bind = &ppcbtndef.cur;
			break;

		case 1:
			bind = ppcbtntbl.curset + 0;
			break;

		case 2:
			bind = ppcbtntbl.curset + 1;
			break;
	}
	setbind(key106ext, bind, &ppcbtntbl.curadrs);
}

void winkbd_bindbtn(UINT type) {

const KEYSET	*bind;

	switch(type) {
		case 0:
		default:
			bind = &ppcbtndef.btn;
			break;

		case 1:
			bind = ppcbtntbl.btnset + 0;
			break;

		case 2:
			bind = ppcbtntbl.btnset + 1;
			break;
	}
	setbind(key106, bind, &ppcbtntbl.btnadrs);
}
#endif

// --- ローマ字入力対応 ---
void winkbd_process_romaji(void) {
	static int wait_frames = 0;
	static UINT8 current_key = 0;
	static bool is_shift = false;

	if (!(g_RomajiMode && g_KanaMode)) return;

	if (wait_frames > 0) {
		wait_frames--;
		if (wait_frames == 0) {
			keystat_senddata((UINT8)(current_key | 0x80)); // Break送信
			if (is_shift) {
				keystat_senddata(0x70 | 0x80); // SHIFT Break
			}
		}
		return; 
	}

	UINT8 packed;
	if (romaji_get_queue(&packed)) { 
		is_shift = (packed & 0x80) != 0;
		current_key = packed & 0x7F;

		if (is_shift) {
			keystat_senddata(0x70); // SHIFT Make
		}
		keystat_senddata(current_key); // カナ Make

		wait_frames = 2; // 実機で取りこぼす場合はここを 3 や 4 に増やしてください
	}
}