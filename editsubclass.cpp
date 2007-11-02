#include "kbSizer.h"
#include <windowsx.h>

static LPCTSTR NUMORGPROC        = TEXT("_KBSIZER_NUMORGPROC_");
static LPCTSTR NUMORGPROC_CANNEG = TEXT("_KBSIZER_NUMORGPROC_CANNEG");


static LRESULT NumOnly_CallOrigWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return (CallWindowProc((WNDPROC)GetProp(hwnd, NUMORGPROC), hwnd, message, wParam, lParam));
}

static void NumOnly_OnChar(HWND hwnd, TCHAR ch, int cRepeat) {
    bool canNegative = (GetProp(hwnd, NUMORGPROC_CANNEG) != 0);
    int n = Edit_GetTextLength(hwnd);
    
    if( _istcntrl(ch) || _istdigit(ch) || (canNegative && (n == 0) && (ch == TEXT('-')))) {
        FORWARD_WM_CHAR(hwnd, ch, cRepeat, NumOnly_CallOrigWndProc);
    } else {
        MessageBeep(0);
    }
}

static LRESULT WINAPI edit_subclass_numonly_wndproc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        HANDLE_MSG(hwnd, WM_CHAR, NumOnly_OnChar);
    }

    return CallWindowProc((WNDPROC)GetProp(hwnd, NUMORGPROC), hwnd, message, wParam, lParam);
}

HWND edit_subclass_numonly(HWND hwnd, bool canNegative) {
    SetProp(hwnd, NUMORGPROC, SubclassWindow(hwnd, edit_subclass_numonly_wndproc));
    SetProp(hwnd, NUMORGPROC_CANNEG, (HANDLE)canNegative);
    return hwnd;
}


