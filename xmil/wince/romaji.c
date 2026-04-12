/***********************************************************************
 *
 * ローマ字→カナ変換処理
 *
 ************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "romaji.h"

#ifndef TRUE
#define TRUE	(1)
#endif
#ifndef FALSE
#define FALSE	(0)
#endif

#define COUNTOF(arr)	(int)(sizeof(arr)/sizeof((arr)[0]))

/* ローマ字 → カナ 変換テーブル */
typedef struct {
	const char *s;
	unsigned char      list[4];
} romaji_list;

#include "romaji-table.h"

/* ↑ romaji-table.h 内にて、以下のテーブルが定義してある */
/*
static const romaji_list list_NN;
static const romaji_list list_tu;
static const romaji_list list_msime[];
*/

/*----------------------------------------------------------------------
 * ワーク (サスペンド情報には残す必要なし)
 *----------------------------------------------------------------------*/

/* ローマ字変換前のバッファ */
static char input_buf[4];				/* 入力済みの文字のバッファ */
static int  input_size;					/* 入力済みの文字の数 */


/* ローマ字変換後のバッファ(キュー) */
/* ローマ字入力されたカナはキューに蓄えられ、順次 I/O ポートに送られていく */
#define ROMAJI_QUE_SIZE		(64)
static int   romaji_set;
static int   romaji_ptr;
static unsigned char romaji_que[ ROMAJI_QUE_SIZE ];

/*----------------------------------------------------------------------
 * ローマ字カナ変換テーブルの作成 (1回だけ呼び出しておく)
 *----------------------------------------------------------------------*/
static romaji_list list[280];
static int         nr_list;

static int romajicmp(const void *p1, const void *p2) {
	return strcmp(((const romaji_list *)p1)->s, ((const romaji_list *)p2)->s);
}

void romaji_init(void) {
	int i, nr_p;
	const romaji_list *p;

	romaji_clear();
	nr_list = 0;

	p = list_msime;
	nr_p = COUNTOF(list_msime);

	for (i = 0; i < nr_p; i++) {
		list[ nr_list ++ ] = *p ++;
		if (nr_list >= COUNTOF(list)) {
			break;
		}
	}

	qsort(&list, nr_list, sizeof(romaji_list), romajicmp);
}

/*----------------------------------------------------------------------
 * ローマ字変換のワーク初期化 (ローマ字変換モード開始時に呼び出す)
 *----------------------------------------------------------------------*/
void romaji_clear(void) {
	/* 変換後のカナをオン出力中だったら、オフ出力する */

	/* ワークを全てクリア */
	romaji_set  = 0;
	romaji_ptr  = 0;
	input_size  = 0;
}

/*----------------------------------------------------------------------
 * 入力をローマ字に変換し、キューに蓄える処理関数
 *----------------------------------------------------------------------*/
static void set_romaji_que(const unsigned char *p) {
	unsigned char c;
	while ((c = *p++)) {
		romaji_que[ romaji_set++ ] = c;
		romaji_set &= (ROMAJI_QUE_SIZE - 1);
	}
}

int romaji_input(int key) {
	int i, j;

	if (islower(key)) {
		/* 小文字は大文字に変換して使う */
		key = toupper(key);
	} else if (isupper(key)) {
		/* 大文字はそのまま使う */
		;
	} else {
		/* それ以外は使わない */
		return key;
	}

	input_buf[ input_size ] = key;
	input_size ++;

	while (input_size) {

		int          list_size = nr_list;
		romaji_list *list_p   = list;

		int same   = FALSE;
		int nearly = FALSE;

		for (i = 0; i < list_size; i++, list_p++) {

			const char *s1 = input_buf;
			const char *s2 = list_p->s;

			for (j = 0; j < input_size; j++, s1++, s2++) {
				if (*s1 != *s2) {
					j = 0;
					break;
				}
			}

			if (j == 0) {
				/* 不一致 */
				/* 途中まで一致してたのに一致しなくなったら中断 */
				if (nearly) {
					break;
				}
				/* list->s のソート済みが前提 */
			} else {
				if (*s2 == '\0') {
					/* 完全に一致 */
					same   = TRUE;
					break;
				} else {
					/* 途中まで一致 */
					nearly = TRUE;
				}
			}
		}

		if (same) {
			/* 完全一致の場合 */
			/* キューにセット */
			set_romaji_que(list_p->list);
			/* 入力を捨てる */
			input_size = 0;
			break;

		} else if (nearly) {
			/* 途中まで一致の場合 */
			/* 入力はそのまま */
			break;

		} else {
			/* 不一致の場合 */

			if (input_buf[0] == 'N') {
				/* んをキューにセット*/
				set_romaji_que(list_NN.list);

			} else if (input_size >= 2 &&
					   input_buf[0] == input_buf[1]) {
				/* っをキューにセット*/
				set_romaji_que(list_tu.list);
			}

			/* 入力をずらす */
			input_size --;
			memmove(&input_buf[0], &input_buf[1], input_size);

			/* 一致するまでチェックしなおす */
		}
	}

	if (input_size >= (int)sizeof(input_buf)) {
		/* バッファオーバー防止 */
		input_size = 0;
	}

	return 0;
}

/* キューから1ストローク分のデータを取り出す関数 */
int romaji_get_queue(unsigned char *out_key) {
	if (romaji_set != romaji_ptr) {
		*out_key = romaji_que[romaji_ptr];
		romaji_ptr = (romaji_ptr + 1) % ROMAJI_QUE_SIZE;
			return 1; /* データあり */
	}

	return 0; /* データなし（空） */
}
