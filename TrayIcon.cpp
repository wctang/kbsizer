#include "kbSizer.h"

#include <shlwapi.h>


TrayIcon::TrayIcon() : IsInstalled(false) {
}

TrayIcon::~TrayIcon() {
    RemoveIcon();
}

void TrayIcon::Init(HWND hwnd, int uid, HICON hIcon, UINT msg, LPCTSTR tooltip) {
    ZeroMemory(&tnd, sizeof(NOTIFYICONDATA));
    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = hwnd;
    tnd.uID = uid;
    tnd.hIcon  = hIcon;
    tnd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    tnd.uVersion = NOTIFYICON_VERSION;
    tnd.uCallbackMessage = msg;
    StrCpy(tnd.szTip, tooltip);
}

void TrayIcon::InstallIcon() {
    if(!IsInstalled) {
        Shell_NotifyIcon(NIM_ADD, &tnd);
        Shell_NotifyIcon(NIM_SETVERSION, &tnd);
        
        IsInstalled = true;
    }
}

void TrayIcon::ChangeIcon(HICON hIcon) {
    tnd.hIcon  = hIcon;
    if(IsInstalled) {
        Shell_NotifyIcon(NIM_MODIFY, &tnd);
    } else {
        InstallIcon();
    }
}


void TrayIcon::RemoveIcon() {
    if(IsInstalled) {
        Shell_NotifyIcon(NIM_DELETE, &tnd);
        IsInstalled = false;
    }
}

void TrayIcon::RefreshIcon() {
    RemoveIcon();
    InstallIcon();
}
