#include "kbSizer.h"
#include <shlwapi.h>

#include "version.h"

void ShowAbout(HWND /*hwndParent*/) {
    TCHAR content[256] = {0};
    wnsprintf(content, 256, TEXT("kbSizer %s\n\nwctang@gmail.com"), VERSION);
    MessageBox(NULL, content, TEXT("kbSizer"), MB_OK);
}
