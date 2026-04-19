#include "compiler.h"
#include <windows.h>
#include "dosio.h"
#include "pccore.h"
#include "statsave.h"
#include "quicksave.h"

// グローバルから modulefile のアクセスが必要
extern OEMCHAR modulefile[MAX_PATH];

// ファイルパスの生成
void quicksave_getpath(OEMCHAR *path, int slot) {
    OEMCHAR filename[MAX_PATH];
    
//	MessageBox(NULL, modulefile, TEXT("0:modulefile"), MB_OK);

    // modulefileをコピー
    file_cpyname(path, modulefile, MAX_PATH);

//	MessageBox(NULL, path, TEXT("1:path"), MB_OK);

    // ファイル名を削除してディレクトリ名だけにする
    file_cutname(path);
//	MessageBox(NULL, path, TEXT("2:file_cutname"), MB_OK);
    
    // ファイル名を作成: xmilstat.001, xmilstat.002, ... など
    OEMSPRINTF(filename, OEMTEXT("xmilstat.%03d"), slot + 1);
//	MessageBox(NULL, filename, TEXT("3:filename"), MB_OK);
    
    // 最終パス = path + filename
    file_catname(path, filename, MAX_PATH);
//	MessageBox(NULL, path, OEMTEXT("4:final path"), MB_OK);
}

// クイックセーブ
int quicksave_save(int slot) {
    OEMCHAR path[MAX_PATH];
    int ret;
    
    if (slot < 0 || slot >= QUICKSAVE_SLOTS) {
        return STATFLAG_FAILURE;
    }
    
    quicksave_getpath(path, slot);
    
    // statsave_save を使用してセーブ
    ret = statsave_save(path);
    
    return ret;
}

// クイックロード
int quicksave_load(int slot) {
    OEMCHAR path[MAX_PATH];
    int ret;
    
    if (slot < 0 || slot >= QUICKSAVE_SLOTS) {
        return STATFLAG_FAILURE;
    }
    
    quicksave_getpath(path, slot);
    
    // セーブファイルが存在するか確認
    if (!quicksave_is_available(slot)) {
        return STATFLAG_FAILURE;
    }
    
    // statsave_load を使用してロード
    ret = statsave_load(path);
    
    return ret;
}

// セーブが存在するか確認
int quicksave_is_available(int slot) {
    OEMCHAR path[MAX_PATH];
    HANDLE hFile;
    
    if (slot < 0 || slot >= QUICKSAVE_SLOTS) {
        return 0;
    }
    
    quicksave_getpath(path, slot);
    
    hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
                       NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        return 0;
    }
    
    CloseHandle(hFile);
    return 1;
}