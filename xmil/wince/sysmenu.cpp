#include	"compiler.h"
#include	"strres.h"
#include	"xmil.h"
#include	"fontmng.h"
#include	"scrnmng.h"
#include	"sysmng.h"
#include	"taskmng.h"
#include	"winkbd.h"
#include	"z80core.h"
#include	"pccore.h"
#include	"iocore.h"
#include	"keystat.h"
#include	"diskdrv.h"
#include	"vramhdl.h"
#include	"menuicon.h"
#include	"menubase.h"
#include	"menudlg.h"
#include	"menusys.h"
#include	"menustr.h"
#include	"sysmenu.h"
#include	"sysmenu.res"
#include	"sysmenu.str"
#include	"filesel.h"
#include	"dlgcfg.h"
// #include	"dlgscr.h"
#include	"dlgabout.h"
#if defined(SUPPORT_STATSAVE)
#include	"quicksave.h"
#include	"statsave.h"
#include	"romaji.h"
#endif
#include "cmt.h"

static DWORD mute_before_vol;	// Mute前のボリューム

extern void winkbd_set_romajimode(bool);


// FDD名を取得するヘルパー関数
static const OEMCHAR* get_filename_only(const OEMCHAR* path) {
    if (!path || !path[0]) return OEMTEXT("(None)");
    const OEMCHAR* p = _tcsrchr(path, '\\');
    if (!p) p = _tcsrchr(path, '/');
    return p ? p + 1 : path;
}

// --- スタックオーバーフローを防ぐ独立関数（セーブ用） ---
static BOOL do_quicksave_dialog(int slot) {
    if (quicksave_is_available(slot)) {
        OEMCHAR filename[MAX_PATH];
        quicksave_getpath(filename, slot);

        QS_METAINFO meta;
        ZeroMemory(&meta, sizeof(meta)); // ゴミデータ一掃

        OEMCHAR msg[1024]; 
        if (statsave_read_meta(filename, &meta) == STATFLAG_SUCCESS) {
            OEMSPRINTF(msg, OEMTEXT("Overwrite Slot %d?\n[Existing Data]\n%04d/%02d/%02d %02d:%02d\nFDD0: %s\nFDD1: %s\nTape: %s"), 
                slot + 1, (int)meta.year, (int)meta.month, (int)meta.day, (int)meta.hour, (int)meta.minute, 
                get_filename_only(meta.fdd0), get_filename_only(meta.fdd1), get_filename_only(meta.cmt));
        } else {
            OEMSPRINTF(msg, OEMTEXT("Overwrite Slot %d?\n(Old format data exists)"), slot + 1);
        }

        if (MessageBox(NULL, msg, OEMTEXT("Confirm Overwrite"), MB_YESNO | MB_ICONQUESTION) != IDYES) {
            return FALSE;
        }
    }

    if (quicksave_save(slot) != STATFLAG_SUCCESS) {
        MessageBox(NULL, OEMTEXT("Save failed."), OEMTEXT("Error"), MB_OK | MB_ICONSTOP);
        return FALSE;
    }
    return TRUE;
}

// --- スタックオーバーフローを防ぐ独立関数（ロード用） ---
static BOOL do_quickload_dialog(int slot) {
    if (!quicksave_is_available(slot)) {
        MessageBox(NULL, OEMTEXT("No save data in this slot."), OEMTEXT("Information"), MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }

    OEMCHAR filename[MAX_PATH];
    quicksave_getpath(filename, slot);

    QS_METAINFO meta;
    ZeroMemory(&meta, sizeof(meta)); // ゴミデータ一掃

    OEMCHAR msg[1024];
    if (statsave_read_meta(filename, &meta) == STATFLAG_SUCCESS) {
        OEMSPRINTF(msg, OEMTEXT("Load Slot %d?\n\n%04d/%02d/%02d %02d:%02d\nFDD0: %s\nFDD1: %s\nTape: %s"), 
            slot + 1, (int)meta.year, (int)meta.month, (int)meta.day, (int)meta.hour, (int)meta.minute, 
            get_filename_only(meta.fdd0), get_filename_only(meta.fdd1), get_filename_only(meta.cmt));
    } else {
        OEMSPRINTF(msg, OEMTEXT("Load Slot %d?\n(Old format data)"), slot + 1);
    }

    if (MessageBox(NULL, msg, OEMTEXT("Confirm Load"), MB_YESNO | MB_ICONQUESTION) != IDYES) {
        return FALSE;
    }

    if (quicksave_load(slot) != STATFLAG_SUCCESS) {
        MessageBox(NULL, OEMTEXT("Load failed."), OEMTEXT("Error"), MB_OK | MB_ICONSTOP);
        return FALSE;
    }
    return TRUE;
}

static void sys_cmd(MENUID id) {

	UINT	update;

	update = 0;
	switch(id) {
		case MID_IPLRESET:
			pccore_reset();
			break;

		case MID_NMIRESET:
			Z80_NMI();
			break;

		case MID_MUTE:	// Mute処理
			if (!xmiloscfg.MUTE_SW) {	// MuteされていないのでMuteにする
				waveOutGetVolume(NULL, &mute_before_vol);	// Mute前のボリューム値を退避
				waveOutSetVolume(NULL, 0);					// ボリュームを0にする
			} else {		// Mute前に戻す
				if (mute_before_vol != 0) {
					waveOutSetVolume(NULL, mute_before_vol);	// 退避していたボリューム値に設定
				}
			}
			xmiloscfg.MUTE_SW ^= 1;
			update |= SYS_UPDATECFG;
		break;

		case MID_ROMAJI:	// ローマ字入力切替処理
			xmiloscfg.ROMAJI_SW ^= 1;
			update |= SYS_UPDATECFG;
			winkbd_set_romajimode((xmiloscfg.ROMAJI_SW & 1));
		break;

		case MID_CONFIG:
			menudlg_create(DLGCFG_WIDTH, DLGCFG_HEIGHT, mstr_cfg, dlgcfg_cmd);
			break;

		case MID_FDD0OPEN:
			filesel_fdd(0);
			break;

		case MID_FDD0EJECT:
			diskdrv_setfdd(0, NULL, 0);
			break;

		case MID_FDD1OPEN:
			filesel_fdd(1);
			break;

		case MID_FDD1EJECT:
			diskdrv_setfdd(1, NULL, 0);
			break;

		case MID_CMTOPEN:
			filesel_cmt();
			break;

		case MID_CMTSTOP:
			cmt_ctrl(0x01);
			break;

		case MID_CMTREW:
			cmt_ctrl(0x04);
			break;

		case MID_CMTEJECT:
			cmt_ctrl(0x00);
			break;
		
		case MID_X1ROM:
			xmilcfg.ROM_TYPE = 1;
			update = SYS_UPDATECFG;
			break;

		case MID_TURBO:
			xmilcfg.ROM_TYPE = 2;
			update = SYS_UPDATECFG;
			break;

#if defined(SUPPORT_TURBOZ)
		case MID_TURBOZ:
			xmilcfg.ROM_TYPE = 3;
			update = SYS_UPDATECFG;
			break;
#endif

		case MID_BOOT2D:
			xmilcfg.DIP_SW &= ~DIPSW_BOOTMEDIA;
			update = SYS_UPDATECFG;
			break;

		case MID_BOOT2HD:
			xmilcfg.DIP_SW |= DIPSW_BOOTMEDIA;
			update = SYS_UPDATECFG;
			break;

		case MID_HIGHRES:
			xmilcfg.DIP_SW &= ~DIPSW_RESOLUTE;
			update = SYS_UPDATECFG;
			break;

		case MID_LOWRES:
			xmilcfg.DIP_SW |= DIPSW_RESOLUTE;
			update = SYS_UPDATECFG;
			break;

		case MID_DISPSYNC:
			xmilcfg.DISPSYNC ^= 1;
			update |= SYS_UPDATECFG;
			break;

		case MID_RASTER:
			xmilcfg.RASTER ^= 1;
			update |= SYS_UPDATECFG;
			break;

		case MID_NOWAIT:
			xmiloscfg.NOWAIT ^= 1;
			update |= SYS_UPDATECFG;
			break;

		case MID_AUTOFPS:
			xmiloscfg.DRAW_SKIP = 0;
			update |= SYS_UPDATECFG;
			break;

		case MID_60FPS:
			xmiloscfg.DRAW_SKIP = 1;
			update |= SYS_UPDATECFG;
			break;

		case MID_30FPS:
			xmiloscfg.DRAW_SKIP = 2;
			update |= SYS_UPDATECFG;
			break;

		case MID_20FPS:
			xmiloscfg.DRAW_SKIP = 3;
			update |= SYS_UPDATECFG;
			break;

		case MID_15FPS:
			xmiloscfg.DRAW_SKIP = 4;
			update |= SYS_UPDATECFG;
			break;

#if defined(WIN32_PLATFORM_PSPC)
		case MID_CURDEF:
			winkbd_bindcur(0);
			xmiloscfg.bindcur = 0;
			update |= SYS_UPDATEOSCFG;
			break;

		case MID_CUR1:
			winkbd_bindcur(1);
			xmiloscfg.bindcur = 1;
			update |= SYS_UPDATEOSCFG;
			break;

		case MID_CUR2:
			winkbd_bindcur(2);
			xmiloscfg.bindcur = 2;
			update |= SYS_UPDATEOSCFG;
			break;

		case MID_BTNDEF:
			winkbd_bindbtn(0);
			xmiloscfg.bindbtn = 0;
			update |= SYS_UPDATEOSCFG;
			break;

		case MID_BTN1:
			winkbd_bindbtn(1);
			xmiloscfg.bindbtn = 1;
			update |= SYS_UPDATEOSCFG;
			break;

		case MID_BTN2:
			winkbd_bindbtn(2);
			xmiloscfg.bindbtn = 2;
			update |= SYS_UPDATEOSCFG;
			break;
#endif

		case MID_KEY:
			xmilcfg.KEY_MODE = 0;
			keystat_resetjoykey();
			update |= SYS_UPDATECFG;
			break;

		case MID_JOY1:
			xmilcfg.KEY_MODE = 1;
			keystat_resetjoykey();
			update |= SYS_UPDATECFG;
			break;

		case MID_JOY2:
			xmilcfg.KEY_MODE = 2;
			keystat_resetjoykey();
			update |= SYS_UPDATECFG;
			break;

#if 0
		case MID_MOUSEKEY:
			xmilcfg.KEY_MODE = 3;
			keystat_resetjoykey();
			update |= SYS_UPDATECFG;
			break;
#endif

#if defined(SUPPORT_TURBOZ) || defined(SUPPORT_OPM)
#if defined(SUPPORT_OPMx2)
		case MID_OPM_NONE:
		case MID_OPM_1:
		case MID_OPM_2:
			xmilcfg.SOUND_SW = (UINT8)(id - MID_OPM_NONE);
			update = SYS_UPDATECFG;
			break;

#else	/* defined(SUPPORT_OPMx2) */
		case MID_FMBOARD:
			xmilcfg.SOUND_SW ^= 1;
			update = SYS_UPDATECFG;
			break;
#endif	/* defined(SUPPORT_OPMx2) */
#endif	/* defined(SUPPORT_TURBOZ) || defined(SUPPORT_OPM) */

		case MID_SEEKSND:
			xmilcfg.MOTOR ^= 1;
			update |= SYS_UPDATECFG;
			break;

		case MID_JOYX:
			xmilcfg.BTN_MODE ^= 1;
			update |= SYS_UPDATECFG;
			break;

		case MID_RAPID:
			xmilcfg.BTN_RAPID ^= 1;
			update |= SYS_UPDATECFG;
			break;

		case MID_ABOUT:
			menudlg_create(DLGABOUT_WIDTH, DLGABOUT_HEIGHT,
													mstr_about, dlgabout_cmd);
			break;

#if defined(SUPPORT_STATSAVE)
		// Quick Save 処理
		case MID_QUICKSAVE_1: case MID_QUICKSAVE_2: case MID_QUICKSAVE_3:
		case MID_QUICKSAVE_4: case MID_QUICKSAVE_5: case MID_QUICKSAVE_6:
		case MID_QUICKSAVE_7: case MID_QUICKSAVE_8: case MID_QUICKSAVE_9:
		case MID_QUICKSAVE_10: 
			do_quicksave_dialog(id - MID_QUICKSAVE_1);
			break;
		
		// Quick Load 処理
		case MID_QUICKLOAD_1: case MID_QUICKLOAD_2: case MID_QUICKLOAD_3:
		case MID_QUICKLOAD_4: case MID_QUICKLOAD_5: case MID_QUICKLOAD_6:
		case MID_QUICKLOAD_7: case MID_QUICKLOAD_8: case MID_QUICKLOAD_9:
		case MID_QUICKLOAD_10:
			if (do_quickload_dialog(id - MID_QUICKLOAD_1)) {
				/* 1. キー押下状態を解放 */
				keystat_allrelease();

				/* 2. ローマ字変換途中のバッファをクリア */
				romaji_init();

				/* 3. 復元された ROMAJI_SW / KANA_SW に合わせてキーマップとカナ状態を再同期 */
				winkbd_set_romajimode((xmiloscfg.ROMAJI_SW & 1));

				/* 4. 復元された MUTE_SW に合わせて音量を安全に同期 */
				if (xmiloscfg.MUTE_SW) {
					waveOutSetVolume(NULL, 0);
				} else if (mute_before_vol != 0) {
					waveOutSetVolume(NULL, mute_before_vol);
				}
			}
			break;
#endif

#if defined(MENU_TASKMINIMIZE)
		case SID_MINIMIZE:
			taskmng_minimize();
			break;
#endif
		case MID_EXIT:
		case SID_CLOSE:
			taskmng_exit();
			break;
	}

	sysmng_update(update);
}


// ----

BRESULT sysmenu_create(void) {

	if (menubase_create() != SUCCESS) {
		goto smcre_err;
	}
	menuicon_regist(MICON_XMIL, &xmilicon);
	if (menusys_create(s_main, sys_cmd, MICON_XMIL, str_xmil)) {
		goto smcre_err;
	}

	/* 起動時の本体音量を退避 */
	waveOutGetVolume(NULL, &mute_before_vol);
	if (xmiloscfg.MUTE_SW) {
		waveOutSetVolume(NULL, 0);				// 設定がMuteならボリュームを0にする
	}

	winkbd_set_romajimode((xmiloscfg.ROMAJI_SW & 1));

#if defined(SUPPORT_SOFTKBD)
	menusys_setstyle(MENUSTYLE_BOTTOM);
#endif
	return(SUCCESS);

smcre_err:
	return(FAILURE);
}

void sysmenu_destroy(void) {

	menubase_close();
	menubase_destroy();
	menusys_destroy();
}

BRESULT sysmenu_menuopen(UINT menutype, int x, int y) {

	UINT8	b;

	menusys_setcheck(MID_MUTE, (xmiloscfg.MUTE_SW & 1));
	menusys_setcheck(MID_ROMAJI, (xmiloscfg.ROMAJI_SW & 1));

	b = xmilcfg.ROM_TYPE;
	menusys_setcheck(MID_X1ROM, (b == 1));
	menusys_setcheck(MID_TURBO, (b == 2));
	menusys_setcheck(MID_TURBOZ, (b == 3));
	b = xmilcfg.DIP_SW & DIPSW_BOOTMEDIA;
	menusys_setcheck(MID_BOOT2D, (b == 0));
	menusys_setcheck(MID_BOOT2HD, (b != 0));
	b = xmilcfg.DIP_SW & DIPSW_RESOLUTE;
	menusys_setcheck(MID_HIGHRES, (b == 0));
	menusys_setcheck(MID_LOWRES, (b != 0));
	menusys_setcheck(MID_DISPSYNC, (xmilcfg.DISPSYNC & 1));
	menusys_setcheck(MID_RASTER, (xmilcfg.RASTER & 1));
	menusys_setcheck(MID_NOWAIT, (xmiloscfg.NOWAIT & 1));
	b = xmiloscfg.DRAW_SKIP;
	menusys_setcheck(MID_AUTOFPS, (b == 0));
	menusys_setcheck(MID_60FPS, (b == 1));
	menusys_setcheck(MID_30FPS, (b == 2));
	menusys_setcheck(MID_20FPS, (b == 3));
	menusys_setcheck(MID_15FPS, (b == 4));
#if defined(WIN32_PLATFORM_PSPC)
	b = xmiloscfg.bindcur;
	menusys_setcheck(MID_CURDEF, (b == 0));
	menusys_setcheck(MID_CUR1, (b == 1));
	menusys_setcheck(MID_CUR2, (b == 2));
	b = xmiloscfg.bindbtn;
	menusys_setcheck(MID_BTNDEF, (b == 0));
	menusys_setcheck(MID_BTN1, (b == 1));
	menusys_setcheck(MID_BTN2, (b == 2));
#endif
	b = xmilcfg.KEY_MODE;
	menusys_setcheck(MID_KEY, (b == 0));
	menusys_setcheck(MID_JOY1, (b == 1));
	menusys_setcheck(MID_JOY2, (b == 2));
	menusys_setcheck(MID_MOUSEKEY, (b == 3));
#if defined(SUPPORT_TURBOZ) || defined(SUPPORT_OPM)
	b = xmilcfg.SOUND_SW;
#if defined(SUPPORT_OPMx2)
	menusys_setcheck(MID_OPM_NONE, (b == 0));
	menusys_setcheck(MID_OPM_1, (b == 1));
	menusys_setcheck(MID_OPM_2, (b == 2));
#else	/* defined(SUPPORT_OPMx2) */
	menusys_setcheck(MID_FMBOARD, (b != 0));
#endif	/* defined(SUPPORT_OPMx2) */
#endif	/* defined(SUPPORT_TURBOZ) || defined(SUPPORT_OPM) */
	menusys_setcheck(MID_SEEKSND, (xmilcfg.MOTOR & 1));
	menusys_setcheck(MID_JOYX, (xmilcfg.BTN_MODE & 1));
	menusys_setcheck(MID_RAPID, (xmilcfg.BTN_RAPID & 1));
	return(menusys_open(x, y));
}

