#ifndef IO_CMT_H
#define IO_CMT_H

#pragma pack(push, 1)
typedef struct {
	UINT32 index;
	char   name[17];
	UINT8  reserve[5];
	UINT8  protect;
	UINT8  format;
	UINT32 frequency;
	UINT32 datasize;
	UINT32 position;
} X1TAPE_HEADER;
#pragma pack(pop)

#define CMT_HEADER_SIZE 40  /* sizeof(X1TAPE_HEADER) ではなく絶対値にする */
#define CMT_PAGE_SIZE   0x4000

typedef struct {
	X1TAPE_HEADER header;
	UINT8  buf[CMT_PAGE_SIZE];
	UINT32 filesize;
	UINT32 page;
	UINT16 buf_size;
	UINT8  sensor;
	UINT8  cur_cmd;
	UINT8  has_header;
} CMT_TABLE;

// テープ関連の全状態をまとめる構造体
typedef struct {
	CMT_TABLE table;
	OEMCHAR   filename[MAX_PATH];
	SINT32    motor_interval;
} CMT_STAT;

extern CMT_STAT cmt_stat; // 外部公開

typedef struct {
	UINT8	cmd;
	/* UINT8	stop; */
} CMT;


#ifdef __cplusplus
extern "C" {
#endif

REG8 cmt_read(void);
void cmt_write(REG8 dat);
void cmt_ctrl(REG8 cmd);
REG8 cmt_tape_stat(void);
REG8 cmt_ctrl_stat(void);
REG8 cmt_test(void);

void cmt_reset(void);

void cmt_set(const OEMCHAR *fname);

#ifdef __cplusplus
}
#endif

#endif
