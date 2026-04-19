#ifndef QUICKSAVE_H
#define QUICKSAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#define QUICKSAVE_SLOTS 10

void quicksave_getpath(OEMCHAR *path, int slot);
int quicksave_save(int slot);
int quicksave_load(int slot);
int quicksave_is_available(int slot);

#ifdef __cplusplus
}
#endif

#endif