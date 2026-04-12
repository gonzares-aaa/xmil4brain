#ifndef ROMAJI_TABLE_H
#define ROMAJI_TABLE_H

// 修正版 X1 スキャンコード定義 (Xmilのkey106配列に完全準拠)
#define X1_K_A    0x03 // あ
#define X1_K_I    0x12 // い
#define X1_K_U    0x04 // う
#define X1_K_E    0x05 // え
#define X1_K_O    0x06 // お
#define X1_K_KA   0x14 // か
#define X1_K_KI   0x21 // き
#define X1_K_KU   0x22 // く
#define X1_K_KE   0x27 // け
#define X1_K_KO   0x2d // こ
#define X1_K_SA   0x2a // さ
#define X1_K_SI   0x1f // し
#define X1_K_SU   0x13 // す
#define X1_K_SE   0x19 // せ
#define X1_K_SO   0x2b // そ
#define X1_K_TA   0x10 // た
#define X1_K_TI   0x1d // ち
#define X1_K_TU   0x29 // つ
#define X1_K_TE   0x11 // て
#define X1_K_TO   0x1e // と
#define X1_K_NA   0x16 // な
#define X1_K_NI   0x17 // に
#define X1_K_NU   0x01 // ぬ
#define X1_K_NE   0x30 // ね
#define X1_K_NO   0x24 // の
#define X1_K_HA   0x20 // は
#define X1_K_HI   0x2c // ひ
#define X1_K_HU   0x02 // ふ
#define X1_K_HE   0x0c // へ
#define X1_K_HO   0x0b // ほ
#define X1_K_MA   0x23 // ま
#define X1_K_MI   0x2e // み
#define X1_K_MU   0x28 // む
#define X1_K_ME   0x32 // め
#define X1_K_MO   0x2f // も
#define X1_K_YA   0x07 // や
#define X1_K_YU   0x08 // ゆ
#define X1_K_YO   0x09 // よ
#define X1_K_RA   0x18 // ら
#define X1_K_RI   0x25 // り
#define X1_K_RU   0x31 // る
#define X1_K_RE   0x26 // れ
#define X1_K_RO   0x33 // ろ
#define X1_K_WA   0x0a // わ
#define X1_K_NN   0x15 // ん
#define X1_K_DAK  0x1a // ゛(濁点)
#define X1_K_HAN  0x1b // ゜(半濁点)

// スキャンコードとSHIFT同時押しフラグ(0x80)を合成するマクロ
#define RJ(code, shift) ((unsigned char)(((code) & 0x7F) | ((shift) ? 0x80 : 0x00)))

// MS-IME準拠 ローマ字→X1カナ変換テーブル (QUASI88完全移植版・NNバグ修正済み)
static const romaji_list list_msime[] = {
	// --- ア行 ---
	{ "A",  { RJ(X1_K_A, 0), 0, 0, 0 } },
	{ "I",  { RJ(X1_K_I, 0), 0, 0, 0 } },
	{ "U",  { RJ(X1_K_U, 0), 0, 0, 0 } },
	{ "E",  { RJ(X1_K_E, 0), 0, 0, 0 } },
	{ "O",  { RJ(X1_K_O, 0), 0, 0, 0 } },

	// --- カ行 ---
	{ "KA", { RJ(X1_K_KA, 0), 0, 0, 0 } },
	{ "KI", { RJ(X1_K_KI, 0), 0, 0, 0 } },
	{ "KU", { RJ(X1_K_KU, 0), 0, 0, 0 } },
	{ "KE", { RJ(X1_K_KE, 0), 0, 0, 0 } },
	{ "KO", { RJ(X1_K_KO, 0), 0, 0, 0 } },

	// --- サ行 ---
	{ "SA", { RJ(X1_K_SA, 0), 0, 0, 0 } },
	{ "SI", { RJ(X1_K_SI, 0), 0, 0, 0 } },
	{ "SU", { RJ(X1_K_SU, 0), 0, 0, 0 } },
	{ "SE", { RJ(X1_K_SE, 0), 0, 0, 0 } },
	{ "SO", { RJ(X1_K_SO, 0), 0, 0, 0 } },

	// --- タ行 ---
	{ "TA", { RJ(X1_K_TA, 0), 0, 0, 0 } },
	{ "TI", { RJ(X1_K_TI, 0), 0, 0, 0 } },
	{ "TU", { RJ(X1_K_TU, 0), 0, 0, 0 } },
	{ "TE", { RJ(X1_K_TE, 0), 0, 0, 0 } },
	{ "TO", { RJ(X1_K_TO, 0), 0, 0, 0 } },

	// --- ナ行 ---
	{ "NA", { RJ(X1_K_NA, 0), 0, 0, 0 } },
	{ "NI", { RJ(X1_K_NI, 0), 0, 0, 0 } },
	{ "NU", { RJ(X1_K_NU, 0), 0, 0, 0 } },
	{ "NE", { RJ(X1_K_NE, 0), 0, 0, 0 } },
	{ "NO", { RJ(X1_K_NO, 0), 0, 0, 0 } },

	// --- ハ行 ---
	{ "HA", { RJ(X1_K_HA, 0), 0, 0, 0 } },
	{ "HI", { RJ(X1_K_HI, 0), 0, 0, 0 } },
	{ "HU", { RJ(X1_K_HU, 0), 0, 0, 0 } },
	{ "HE", { RJ(X1_K_HE, 0), 0, 0, 0 } },
	{ "HO", { RJ(X1_K_HO, 0), 0, 0, 0 } },

	// --- マ行 ---
	{ "MA", { RJ(X1_K_MA, 0), 0, 0, 0 } },
	{ "MI", { RJ(X1_K_MI, 0), 0, 0, 0 } },
	{ "MU", { RJ(X1_K_MU, 0), 0, 0, 0 } },
	{ "ME", { RJ(X1_K_ME, 0), 0, 0, 0 } },
	{ "MO", { RJ(X1_K_MO, 0), 0, 0, 0 } },

	// --- ヤ行 ---
	{ "YA", { RJ(X1_K_YA, 0), 0, 0, 0 } },
	{ "YU", { RJ(X1_K_YU, 0), 0, 0, 0 } },
	{ "YO", { RJ(X1_K_YO, 0), 0, 0, 0 } },

	// --- ラ行 ---
	{ "RA", { RJ(X1_K_RA, 0), 0, 0, 0 } },
	{ "RI", { RJ(X1_K_RI, 0), 0, 0, 0 } },
	{ "RU", { RJ(X1_K_RU, 0), 0, 0, 0 } },
	{ "RE", { RJ(X1_K_RE, 0), 0, 0, 0 } },
	{ "RO", { RJ(X1_K_RO, 0), 0, 0, 0 } },

	// --- ワ行・ん ---
	{ "WA", { RJ(X1_K_WA, 0), 0, 0, 0 } },
	{ "WO", { RJ(X1_K_WA, 1), 0, 0, 0 } }, // を (SHIFT+わ)
	{ "NN", { RJ(X1_K_NN, 0), 0, 0, 0 } }, // ん
	{ "N'", { RJ(X1_K_NN, 0), 0, 0, 0 } }, // ん

	// --- ガ行 ---
	{ "GA", { RJ(X1_K_KA, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "GI", { RJ(X1_K_KI, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "GU", { RJ(X1_K_KU, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "GE", { RJ(X1_K_KE, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "GO", { RJ(X1_K_KO, 0), RJ(X1_K_DAK, 0), 0, 0 } },

	// --- ザ行 ---
	{ "ZA", { RJ(X1_K_SA, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "ZI", { RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "ZU", { RJ(X1_K_SU, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "ZE", { RJ(X1_K_SE, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "ZO", { RJ(X1_K_SO, 0), RJ(X1_K_DAK, 0), 0, 0 } },

	// --- ダ行 ---
	{ "DA", { RJ(X1_K_TA, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "DI", { RJ(X1_K_TI, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "DU", { RJ(X1_K_TU, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "DE", { RJ(X1_K_TE, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "DO", { RJ(X1_K_TO, 0), RJ(X1_K_DAK, 0), 0, 0 } },

	// --- バ行 ---
	{ "BA", { RJ(X1_K_HA, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "BI", { RJ(X1_K_HI, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "BU", { RJ(X1_K_HU, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "BE", { RJ(X1_K_HE, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "BO", { RJ(X1_K_HO, 0), RJ(X1_K_DAK, 0), 0, 0 } },

	// --- パ行 ---
	{ "PA", { RJ(X1_K_HA, 0), RJ(X1_K_HAN, 0), 0, 0 } },
	{ "PI", { RJ(X1_K_HI, 0), RJ(X1_K_HAN, 0), 0, 0 } },
	{ "PU", { RJ(X1_K_HU, 0), RJ(X1_K_HAN, 0), 0, 0 } },
	{ "PE", { RJ(X1_K_HE, 0), RJ(X1_K_HAN, 0), 0, 0 } },
	{ "PO", { RJ(X1_K_HO, 0), RJ(X1_K_HAN, 0), 0, 0 } },

	// --- C系・CH系 ---
	{ "CA", { RJ(X1_K_KA, 0), 0, 0, 0 } },
	{ "CI", { RJ(X1_K_SI, 0), 0, 0, 0 } },
	{ "CU", { RJ(X1_K_KU, 0), 0, 0, 0 } },
	{ "CE", { RJ(X1_K_SE, 0), 0, 0, 0 } },
	{ "CO", { RJ(X1_K_KO, 0), 0, 0, 0 } },
	{ "CHA",{ RJ(X1_K_TI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "CHI",{ RJ(X1_K_TI, 0), 0, 0, 0 } },
	{ "CHU",{ RJ(X1_K_TI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "CHE",{ RJ(X1_K_TI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "CHO",{ RJ(X1_K_TI, 0), RJ(X1_K_YO, 1), 0, 0 } },
	{ "CYA",{ RJ(X1_K_TI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "CYI",{ RJ(X1_K_TI, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "CYU",{ RJ(X1_K_TI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "CYE",{ RJ(X1_K_TI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "CYO",{ RJ(X1_K_TI, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- D系・DH系 ---
	{ "DHA",{ RJ(X1_K_TE, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YA, 1), 0 } },
	{ "DHI",{ RJ(X1_K_TE, 0), RJ(X1_K_DAK, 0), RJ(X1_K_I, 1), 0 } },
	{ "DHU",{ RJ(X1_K_TE, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YU, 1), 0 } },
	{ "DHE",{ RJ(X1_K_TE, 0), RJ(X1_K_DAK, 0), RJ(X1_K_E, 1), 0 } },
	{ "DHO",{ RJ(X1_K_TE, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YO, 1), 0 } },
	{ "DYA",{ RJ(X1_K_TI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YA, 1), 0 } },
	{ "DYI",{ RJ(X1_K_TI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_I, 1), 0 } },
	{ "DYU",{ RJ(X1_K_TI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YU, 1), 0 } },
	{ "DYE",{ RJ(X1_K_TI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_E, 1), 0 } },
	{ "DYO",{ RJ(X1_K_TI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YO, 1), 0 } },

	// --- F系・FY系 ---
	{ "FA", { RJ(X1_K_HU, 0), RJ(X1_K_A, 1), 0, 0 } },
	{ "FI", { RJ(X1_K_HU, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "FU", { RJ(X1_K_HU, 0), 0, 0, 0 } },
	{ "FE", { RJ(X1_K_HU, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "FO", { RJ(X1_K_HU, 0), RJ(X1_K_O, 1), 0, 0 } },
	{ "FYA",{ RJ(X1_K_HU, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "FYI",{ RJ(X1_K_HU, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "FYU",{ RJ(X1_K_HU, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "FYE",{ RJ(X1_K_HU, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "FYO",{ RJ(X1_K_HU, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- G系 ---
	{ "GYA",{ RJ(X1_K_KI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YA, 1), 0 } },
	{ "GYI",{ RJ(X1_K_KI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_I, 1), 0 } },
	{ "GYU",{ RJ(X1_K_KI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YU, 1), 0 } },
	{ "GYE",{ RJ(X1_K_KI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_E, 1), 0 } },
	{ "GYO",{ RJ(X1_K_KI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YO, 1), 0 } },

	// --- H系 ---
	{ "HYA",{ RJ(X1_K_HI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "HYI",{ RJ(X1_K_HI, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "HYU",{ RJ(X1_K_HI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "HYE",{ RJ(X1_K_HI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "HYO",{ RJ(X1_K_HI, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- J系・JY系 ---
	{ "JA", { RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YA, 1), 0 } },
	{ "JI", { RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "JU", { RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YU, 1), 0 } },
	{ "JE", { RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_E, 1), 0 } },
	{ "JO", { RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YO, 1), 0 } },
	{ "JYA",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YA, 1), 0 } },
	{ "JYI",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_I, 1), 0 } },
	{ "JYU",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YU, 1), 0 } },
	{ "JYE",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_E, 1), 0 } },
	{ "JYO",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YO, 1), 0 } },

	// --- K系 ---
	{ "KYA",{ RJ(X1_K_KI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "KYI",{ RJ(X1_K_KI, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "KYU",{ RJ(X1_K_KI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "KYE",{ RJ(X1_K_KI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "KYO",{ RJ(X1_K_KI, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- L系 (小文字) ---
	{ "LA", { RJ(X1_K_A, 1), 0, 0, 0 } },
	{ "LI", { RJ(X1_K_I, 1), 0, 0, 0 } },
	{ "LU", { RJ(X1_K_U, 1), 0, 0, 0 } },
	{ "LE", { RJ(X1_K_E, 1), 0, 0, 0 } },
	{ "LO", { RJ(X1_K_O, 1), 0, 0, 0 } },
	{ "LYA",{ RJ(X1_K_YA, 1), 0, 0, 0 } },
	{ "LYI",{ RJ(X1_K_I, 1), 0, 0, 0 } },
	{ "LYU",{ RJ(X1_K_YU, 1), 0, 0, 0 } },
	{ "LYE",{ RJ(X1_K_E, 1), 0, 0, 0 } },
	{ "LYO",{ RJ(X1_K_YO, 1), 0, 0, 0 } },
	{ "LTU",{ RJ(X1_K_TU, 1), 0, 0, 0 } },

	// --- M系 ---
	{ "MYA",{ RJ(X1_K_MI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "MYI",{ RJ(X1_K_MI, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "MYU",{ RJ(X1_K_MI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "MYE",{ RJ(X1_K_MI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "MYO",{ RJ(X1_K_MI, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- N系 ---
	{ "NYA",{ RJ(X1_K_NI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "NYI",{ RJ(X1_K_NI, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "NYU",{ RJ(X1_K_NI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "NYE",{ RJ(X1_K_NI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "NYO",{ RJ(X1_K_NI, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- P系 ---
	{ "PYA",{ RJ(X1_K_HI, 0), RJ(X1_K_HAN, 0), RJ(X1_K_YA, 1), 0 } },
	{ "PYI",{ RJ(X1_K_HI, 0), RJ(X1_K_HAN, 0), RJ(X1_K_I, 1), 0 } },
	{ "PYU",{ RJ(X1_K_HI, 0), RJ(X1_K_HAN, 0), RJ(X1_K_YU, 1), 0 } },
	{ "PYE",{ RJ(X1_K_HI, 0), RJ(X1_K_HAN, 0), RJ(X1_K_E, 1), 0 } },
	{ "PYO",{ RJ(X1_K_HI, 0), RJ(X1_K_HAN, 0), RJ(X1_K_YO, 1), 0 } },

	// --- Q系・QW系・QY系 ---
	{ "QA", { RJ(X1_K_KU, 0), RJ(X1_K_A, 1), 0, 0 } },
	{ "QI", { RJ(X1_K_KU, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "QU", { RJ(X1_K_KU, 0), 0, 0, 0 } },
	{ "QE", { RJ(X1_K_KU, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "QO", { RJ(X1_K_KU, 0), RJ(X1_K_O, 1), 0, 0 } },
	{ "QWA",{ RJ(X1_K_KU, 0), RJ(X1_K_A, 1), 0, 0 } },
	{ "QWI",{ RJ(X1_K_KU, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "QWU",{ RJ(X1_K_KU, 0), RJ(X1_K_U, 1), 0, 0 } },
	{ "QWE",{ RJ(X1_K_KU, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "QWO",{ RJ(X1_K_KU, 0), RJ(X1_K_O, 1), 0, 0 } },
	{ "QYA",{ RJ(X1_K_KU, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "QYI",{ RJ(X1_K_KU, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "QYU",{ RJ(X1_K_KU, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "QYE",{ RJ(X1_K_KU, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "QYO",{ RJ(X1_K_KU, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- R系 ---
	{ "RYA",{ RJ(X1_K_RI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "RYI",{ RJ(X1_K_RI, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "RYU",{ RJ(X1_K_RI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "RYE",{ RJ(X1_K_RI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "RYO",{ RJ(X1_K_RI, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- S系・SH系 ---
	{ "SHA",{ RJ(X1_K_SI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "SHI",{ RJ(X1_K_SI, 0), 0, 0, 0 } },
	{ "SHU",{ RJ(X1_K_SI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "SHE",{ RJ(X1_K_SI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "SHO",{ RJ(X1_K_SI, 0), RJ(X1_K_YO, 1), 0, 0 } },
	{ "SYA",{ RJ(X1_K_SI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "SYI",{ RJ(X1_K_SI, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "SYU",{ RJ(X1_K_SI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "SYE",{ RJ(X1_K_SI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "SYO",{ RJ(X1_K_SI, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- T系・TH系・TS系 ---
	{ "THA",{ RJ(X1_K_TE, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "THI",{ RJ(X1_K_TE, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "THU",{ RJ(X1_K_TE, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "THE",{ RJ(X1_K_TE, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "THO",{ RJ(X1_K_TE, 0), RJ(X1_K_YO, 1), 0, 0 } },
	{ "TSA",{ RJ(X1_K_TU, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "TSI",{ RJ(X1_K_TU, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "TSU",{ RJ(X1_K_TU, 0), 0, 0, 0 } },
	{ "TSE",{ RJ(X1_K_TU, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "TSO",{ RJ(X1_K_TU, 0), RJ(X1_K_YO, 1), 0, 0 } },
	{ "TYA",{ RJ(X1_K_TI, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "TYI",{ RJ(X1_K_TI, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "TYU",{ RJ(X1_K_TI, 0), RJ(X1_K_YU, 1), 0, 0 } },
	{ "TYE",{ RJ(X1_K_TI, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "TYO",{ RJ(X1_K_TI, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- V系・VY系 ---
	{ "VA", { RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_A, 1), 0 } },
	{ "VI", { RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_I, 1), 0 } },
	{ "VU", { RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), 0, 0 } },
	{ "VE", { RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_E, 1), 0 } },
	{ "VO", { RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_O, 1), 0 } },
	{ "VYA",{ RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YA, 1), 0 } },
	{ "VYI",{ RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_I, 1), 0 } },
	{ "VYU",{ RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YU, 1), 0 } },
	{ "VYE",{ RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_E, 1), 0 } },
	{ "VYO",{ RJ(X1_K_U, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YO, 1), 0 } },

	// --- W系・WH系 ---
	{ "WI", { RJ(X1_K_U, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "WU", { RJ(X1_K_U, 0), 0, 0, 0 } },
	{ "WE", { RJ(X1_K_U, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "WHA",{ RJ(X1_K_U, 0), RJ(X1_K_YA, 1), 0, 0 } },
	{ "WHI",{ RJ(X1_K_U, 0), RJ(X1_K_I, 1), 0, 0 } },
	{ "WHU",{ RJ(X1_K_U, 0), 0, 0, 0 } },
	{ "WHE",{ RJ(X1_K_U, 0), RJ(X1_K_E, 1), 0, 0 } },
	{ "WHO",{ RJ(X1_K_U, 0), RJ(X1_K_YO, 1), 0, 0 } },

	// --- X系 (小文字) ---
	{ "XA", { RJ(X1_K_A, 1), 0, 0, 0 } },
	{ "XI", { RJ(X1_K_I, 1), 0, 0, 0 } },
	{ "XU", { RJ(X1_K_U, 1), 0, 0, 0 } },
	{ "XE", { RJ(X1_K_E, 1), 0, 0, 0 } },
	{ "XO", { RJ(X1_K_O, 1), 0, 0, 0 } },
	{ "XTU",{ RJ(X1_K_TU, 1), 0, 0, 0 } },
	{ "XYA",{ RJ(X1_K_YA, 1), 0, 0, 0 } },
	{ "XYI",{ RJ(X1_K_I, 1), 0, 0, 0 } },
	{ "XYU",{ RJ(X1_K_YU, 1), 0, 0, 0 } },
	{ "XYE",{ RJ(X1_K_E, 1), 0, 0, 0 } },
	{ "XYO",{ RJ(X1_K_YO, 1), 0, 0, 0 } },

	// --- Y系 ---
	{ "YI", { RJ(X1_K_I, 0), 0, 0, 0 } },
	{ "YE", { RJ(X1_K_I, 0), RJ(X1_K_E, 1), 0, 0 } },

	// --- Z系・ZY系 ---
	{ "ZYA",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YA, 1), 0 } },
	{ "ZYI",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_I, 1), 0 } },
	{ "ZYU",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YU, 1), 0 } },
	{ "ZYE",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_E, 1), 0 } },
	{ "ZYO",{ RJ(X1_K_SI, 0), RJ(X1_K_DAK, 0), RJ(X1_K_YO, 1), 0 } }
};

// 小さい「っ」の定義
static const romaji_list list_tu = { "TU", { RJ(X1_K_TU, 1), 0, 0, 0 } };

// 単独の「ん」の定義
static const romaji_list list_NN = { "NN", { RJ(X1_K_NN, 0), 0, 0, 0 } };

#endif // ROMAJI_TABLE_H