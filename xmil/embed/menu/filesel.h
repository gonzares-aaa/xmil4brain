/**
 * @file	filesel.h
 * @brief	Interface of the selection of files
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void filesel_fdd(REG8 drv);
void filesel_cmt(void); /* Tape file support */

#ifdef __cplusplus
}
#endif

