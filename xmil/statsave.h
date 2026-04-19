
enum {
	STATFLAG_SUCCESS	= 0,
	STATFLAG_DISKCHG	= 0x0001,
	STATFLAG_VERCHG		= 0x0002,
	STATFLAG_WARNING	= 0x0080,
	STATFLAG_VERSION	= 0x0100,
	STATFLAG_FAILURE	= -1
};

typedef struct {
	char		index[10];
	UINT16		ver;
	UINT32		size;
} STFLAGHDR;

typedef struct {
	STFLAGHDR	hdr;
	UINT		pos;
	OEMCHAR		*err;
	int			errlen;
} _STFLAGH, *STFLAGH;

typedef struct {
	char	index[10];
	UINT16	ver;
	UINT16	type;
	void	*arg1;
	UINT	arg2;
} SFENTRY;


#ifdef __cplusplus
extern "C" {
#endif

int statflag_read(STFLAGH sfh, void *ptr, UINT size);
int statflag_write(STFLAGH sfh, const void *ptr, UINT size);
void statflag_seterr(STFLAGH sfh, const OEMCHAR *str);

int statsave_save(const OEMCHAR *filename);
int statsave_check(const OEMCHAR *filename, OEMCHAR *buf, UINT size);
int statsave_load(const OEMCHAR *filename);

typedef struct {
	OEMCHAR	fdd0[MAX_PATH];
	OEMCHAR	fdd1[MAX_PATH];
	OEMCHAR	cmt[MAX_PATH];
	UINT16	year;
	UINT16	month;
	UINT16	day;
	UINT16	hour;
	UINT16	minute;
} QS_METAINFO;

extern QS_METAINFO qs_meta;
int statsave_read_meta(const OEMCHAR *filename, QS_METAINFO *meta);

#ifdef __cplusplus
}
#endif

