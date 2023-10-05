#include	"compiler.h"
#include	"strres.h"
#include	"profile.h"
#include	"dosio.h"
#include	"xmil.h"
#include	"ini.h"
#include	"pccore.h"


static const OEMCHAR ini_title[] = OEMTEXT("Xmillennium");


enum {
	PFRO_BOOL			= PFTYPE_BOOL + PFFLAG_RO,
	PFMAX_UINT8			= PFTYPE_UINT8 + PFFLAG_MAX,
	PFAND_UINT8			= PFTYPE_UINT8 + PFFLAG_AND,
	PFROAND_HEX32		= PFTYPE_HEX32 + PFFLAG_RO + PFFLAG_AND
};

static const PFTBL iniitem[] = {

	// OSàÀë∂Å`
#if !defined(GX_DLL)
	PFVAL("WindposX", PFTYPE_SINT32,	&xmiloscfg.winx),
	PFVAL("WindposY", PFTYPE_SINT32,	&xmiloscfg.winy),
#endif
	PFVAL("s_NOWAIT", PFTYPE_BOOL,		&xmiloscfg.NOWAIT),
	PFVAL("SkpFrame", PFTYPE_UINT8,		&xmiloscfg.DRAW_SKIP),
#if defined(WIN32_PLATFORM_PSPC)
	PFVAL("pbindcur", PFTYPE_UINT8,		&xmiloscfg.bindcur),
	PFVAL("pbindbtn", PFTYPE_UINT8,		&xmiloscfg.bindbtn),
#endif


	// xmil
	PFMAX("IPL_TYPE", PFMAX_UINT8,		&xmilcfg.ROM_TYPE,		3),
	PFVAL("Resolute", PFTYPE_HEX8,		&xmilcfg.DIP_SW),

	PFVAL("DispSync", PFTYPE_BOOL,		&xmilcfg.DISPSYNC),
	PFVAL("Real_Pal", PFTYPE_BOOL,		&xmilcfg.RASTER),
	PFVAL("skipline", PFTYPE_BOOL,		&xmilcfg.skipline),
	PFVAL("skplight", PFTYPE_UINT16,	&xmilcfg.skiplight),

	PFVAL("SampleHz", PFTYPE_UINT16,	&xmilcfg.samplingrate),
	PFVAL("Latencys", PFTYPE_UINT16,	&xmilcfg.delayms),
	PFVAL("OPMsound", PFTYPE_BOOL,		&xmilcfg.SOUND_SW),
	PFVAL("Seek_Snd", PFTYPE_BOOL,		&xmilcfg.MOTOR),
	PFMAX("Seek_Vol", PFMAX_UINT8,		&xmilcfg.MOTORVOL,		100),

	PFVAL("MouseInt", PFTYPE_BOOL,		&xmilcfg.MOUSE_SW),
	PFVAL("btnRAPID", PFTYPE_BOOL,		&xmilcfg.BTN_RAPID),
	PFVAL("btn_MODE", PFTYPE_BOOL,		&xmilcfg.BTN_MODE)};


void initgetfile(OEMCHAR *path, UINT size) {

	file_cpyname(path, modulefile, size);
	file_cutext(path);
	file_catname(path, OEMTEXT(".cfg"), size);
}

void initload(void) {

	OEMCHAR	path[MAX_PATH];

	initgetfile(path, NELEMENTS(path));
	profile_iniread(path, ini_title, iniitem, NELEMENTS(iniitem), NULL);
}

void initsave(void) {

	OEMCHAR	path[MAX_PATH];

	initgetfile(path, NELEMENTS(path));
	profile_iniwrite(path, ini_title, iniitem, NELEMENTS(iniitem), NULL);
}

