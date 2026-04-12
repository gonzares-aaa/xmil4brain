#ifndef ROMAJI_H_INCLUDED
#define ROMAJI_H_INCLUDED

/* ƒ[ƒ}š“ü—Íˆ— */
#ifdef __cplusplus
extern "C" {
#endif

void romaji_init(void);
void romaji_clear(void);
int  romaji_input(int key);
int  romaji_get_queue(unsigned char *out_key);

#ifdef __cplusplus
}
#endif

#endif /* ROMAJI_H_INCLUDED */

