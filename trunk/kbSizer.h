#pragma once

#include <windows.h>
#include <tchar.h>

class TrayIcon {
public:
    TrayIcon();
    ~TrayIcon();

    void Init(HWND hwnd, int uid, HICON hIcon, UINT msg, LPCTSTR tooltip);
    void InstallIcon();
    void ChangeIcon(HICON hIcon);
    void RefreshIcon();
private:
    bool IsInstalled;
    NOTIFYICONDATA tnd;

    void RemoveIcon();
};

const int BUFLEN = 128;


class TemplateRes {
public:
    TemplateRes(HGLOBAL hglob) : m_hglob(hglob), m_lpDlgTmpl(NULL) {
        if(m_hglob != 0) {
            m_lpDlgTmpl = LockResource(m_hglob);
        }
    }
    ~TemplateRes() {
        if(m_lpDlgTmpl != NULL) {
            UnlockResource(m_lpDlgTmpl);
        }
        if(m_hglob != 0) {
            FreeResource(m_hglob);
        }
    }
    operator LPCDLGTEMPLATE() const {
        return static_cast<LPCDLGTEMPLATE>(m_lpDlgTmpl);
    }
    operator const MENUTEMPLATE*() const {
        return static_cast<const MENUTEMPLATE*>(m_lpDlgTmpl);
    }
     
private:
    HGLOBAL m_hglob;
    LPVOID m_lpDlgTmpl;
};

class LangRes {
public:
    LangRes();
    ~LangRes();

    BOOL EnumLanguages(ENUMRESLANGPROC func, LONG_PTR param);
    WORD SetCurrentLangId(WORD langId);
    WORD GetCurrentLangId();
    BOOL IsMatchingLangId(WORD langId);
    LPCWSTR LoadString(int idStr, int *nLen);
    LPCWSTR LoadString(WORD langid, int idStr, int *nLen);
    TemplateRes LoadTemplateRes(LPCTSTR type, int idDlg);

    HINSTANCE GetModule() {
        return hRes;
    }

private:
    static BOOL CALLBACK EnumLangsFunc( HMODULE hModule, LPCTSTR lpszType, LPCTSTR lpszName, WORD wIDLanguage, LONG_PTR lParam);

    HINSTANCE hRes;
    WORD defLangId;
    WCHAR strbuf[BUFLEN];
};


#define NO_MOVE_MARK (-65535)
struct SettingItem {
    TCHAR name[128];
    RECT rect;
};

enum MODE {MODE_MOVE, MODE_MOUSE}; 

class Application {
public:
    Application();

    BOOL SaveLangSetting(WORD langId);
    BOOL SavePresetSetting(SettingItem *sizelist);

    int Run() {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return (int) msg.wParam;
    }
private:
    static void RegHotKey(HWND hwnd);
    static void UnRegHotKey(HWND hwnd);
    static LRESULT CALLBACK Main_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    static BOOL Main_OnCreate(HWND hwnd, LPCREATESTRUCT);
    static void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    static void Main_OnTrayMsg(HWND hwnd, int id);
    static void Main_OnHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk);

    BOOL isEnable;
    MODE currMode;
    TrayIcon trayIcon;
    LangRes res;
    SettingItem m_sizelist[9];

    HWND CreateMainWindow();
    void ShowContextMenu(HWND hwnd);
    BOOL InitSetting();
};


class Configuration {
public:
    static const UINT WM_USER_SET    = WM_USER + 1;
    static BOOL ShowDialog(Application *pApp, LangRes* pRes, SettingItem* configlist);
private:
    static HWND m_hwndConfig;
    static Application *m_pApp;
    static LangRes* m_pRes;

    static SettingItem m_configlist[9];

    static void ShowHelp(HWND hDlg);
    static BOOL CALLBACK EnumLangsFunc( HMODULE hModule, LPCTSTR lpszType, LPCTSTR lpszName, WORD wIDLanguage, LONG_PTR lParam);
    static INT_PTR CALLBACK ConfigDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK MainConfigDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK PresetConfigDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
