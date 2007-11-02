#include "kbSizer.h"

LangRes::LangRes() : hRes(NULL) {
    hRes = LoadLibrary(_TEXT("res.dll"));
    if(hRes == NULL) {
        hRes = GetModuleHandle(NULL);
    }
    
    defLangId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
}

LangRes::~LangRes() {
    if(hRes && hRes != GetModuleHandle(NULL)) {
        FreeLibrary(hRes);
    }
}

BOOL CALLBACK LangRes::EnumLangsFunc( HMODULE /*hModule*/, LPCTSTR /*lpszType*/, LPCTSTR /*lpszName*/, WORD wIDLanguage, LONG_PTR lParam) {
    if(wIDLanguage == static_cast<WORD>(lParam)) {
        return FALSE;
    }
    return TRUE;
}

BOOL LangRes::EnumLanguages(ENUMRESLANGPROC func, LONG_PTR param) {
    return EnumResourceLanguages(hRes, RT_STRING, MAKEINTRESOURCE(1), func, param);
}

BOOL LangRes::IsMatchingLangId(WORD langId) {
    return !EnumResourceLanguages(hRes, RT_STRING, MAKEINTRESOURCE(1), LangRes::EnumLangsFunc, static_cast<LONG_PTR>(langId));
}

WORD LangRes::SetCurrentLangId(WORD langId) {
    WORD last = defLangId;
    defLangId = langId;
    return last;
}
WORD LangRes::GetCurrentLangId() {
    return defLangId;
}


LPCWSTR LangRes::LoadString(WORD langid, int idStr, int *nLen) {
    LPCWSTR res = NULL;
    HRSRC hrsrc = FindResourceEx(hRes, RT_STRING, MAKEINTRESOURCE(idStr / 16 + 1), langid);
    if (hrsrc) {
        HGLOBAL hglob = LoadResource(hRes, hrsrc);
        if (hglob) {
            LPCWSTR ptstr = reinterpret_cast<LPCWSTR> (LockResource(hglob));
            if (ptstr) {
                for (int i = 0; i < (idStr & 15); i++) {
                    ptstr += 1 + static_cast<UINT>(*ptstr);
                }
                int len = (BUFLEN-1 > ptstr[0]) ? ptstr[0] : BUFLEN-1;
                memcpy(strbuf, ptstr+1, len * sizeof(WCHAR));
                strbuf[len] = 0;
                UnlockResource(ptstr);

                if(nLen != NULL) *nLen = len;
                res = strbuf;
            }
            FreeResource(hglob);
        }
    }
    return res;
}
LPCWSTR LangRes::LoadString(int idStr, int *nLen) {
    return LoadString(defLangId, idStr, nLen);
}



TemplateRes LangRes::LoadTemplateRes(LPCTSTR type, int idDlg) {
    HRSRC hrsrc = FindResourceEx(hRes, type, MAKEINTRESOURCE(idDlg), defLangId);
    HGLOBAL hglob = 0;
    if (hrsrc) {
        hglob = LoadResource(hRes, hrsrc);
    }
    return TemplateRes(hglob);
}
