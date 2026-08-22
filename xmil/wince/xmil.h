
typedef struct {
#if !defined(GX_DLL)
	int		winx;
	int		winy;
#endif

	UINT8	NOWAIT;
	UINT8	DRAW_SKIP;

#if defined(WIN32_PLATFORM_PSPC)
	UINT8	bindcur;
	UINT8	bindbtn;
#endif
	UINT8	MUTE_SW;		/* Mute機能ON/OFFフラグ */
	UINT8	ROMAJI_SW;		/* ローマ字入力ON/OFFフラグ */
	UINT8	KANA_SW;		/* カナ入力モードON/OFFフラグ */
} XMILOSCFG;


#if defined(SIZE_QVGA)
#if defined(SUPPORT_SOFTKBD)
enum {
	FULLSCREEN_WIDTH	= 320,
	FULLSCREEN_HEIGHT	= 200 + 80
};
#else
enum {
	FULLSCREEN_WIDTH	= 320,
	FULLSCREEN_HEIGHT	= 200
};
#endif
#else
#if defined(SUPPORT_SOFTKBD)
enum {
	FULLSCREEN_WIDTH	= 640,
	FULLSCREEN_HEIGHT	= 400 + 80
};
#else
enum {
	FULLSCREEN_WIDTH	= 640,
	FULLSCREEN_HEIGHT	= 400
};
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern	XMILOSCFG	xmiloscfg;
#ifdef __cplusplus
}
#endif

extern	HWND		hWndMain;
extern	HINSTANCE	hInst;
extern	HINSTANCE	hPreI;
extern	OEMCHAR		modulefile[MAX_PATH];

