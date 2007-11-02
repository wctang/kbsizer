#include "kbSizer.h"

#include <windowsx.h>
#include <shlwapi.h>
#include "res.h"


HWND edit_subclass_numonly(HWND hwnd, bool canNegative);



void RefreshList(HWND hwndList, SettingItem *pList) {
    TCHAR buf[128];
    ListBox_ResetContent(hwndList);
    for(int i = 0; i< 9; i++) {
        if(pList[i].rect.left == NO_MOVE_MARK && pList[i].rect.right == -1) {
            wnsprintf(buf, 128, TEXT("[%d]"), i+1);
        } else if(pList[i].rect.left == NO_MOVE_MARK) {
            wnsprintf(buf, 128, TEXT("[%d] %s. No Move. Resize to %dx%d"), i+1, pList[i].name, pList[i].rect.right, pList[i].rect.bottom);
        } else if(pList[i].rect.right == -1) {
            wnsprintf(buf, 128, TEXT("[%d] %s. Move to (%d,%d). No Resize."), i+1, pList[i].name, pList[i].rect.left, pList[i].rect.top);
        } else {
            wnsprintf(buf, 128, TEXT("[%d] %s. Move to (%d,%d). Resize to %dx%d"), i+1, pList[i].name, pList[i].rect.left, pList[i].rect.top, pList[i].rect.right, pList[i].rect.bottom);
        }
        int nlist = ListBox_GetCount(hwndList);
        ListBox_InsertString(hwndList, nlist, buf);
    }
}


void Configuration::ShowHelp(HWND hDlg) {
    ShellExecute(hDlg,TEXT("open"), m_pRes->LoadString(IDS_HELPLINK, NULL),TEXT(""),TEXT(""), SW_SHOW );
}



HWND Configuration::m_hwndConfig;
Application* Configuration::m_pApp;
LangRes* Configuration::m_pRes;
SettingItem Configuration::m_configlist[9];



BOOL CALLBACK Configuration::EnumLangsFunc( HMODULE /*hModule*/, LPCTSTR /*lpszType*/, LPCTSTR /*lpszName*/, WORD wIDLanguage, LONG_PTR lParam) {
    HWND hwndLangs = reinterpret_cast<HWND>(lParam);

    int idx = ComboBox_AddString(hwndLangs, m_pRes->LoadString(wIDLanguage, 0, NULL));
    ComboBox_SetItemData(hwndLangs, idx, wIDLanguage);

    int currLangid = m_pRes->GetCurrentLangId();
    if(wIDLanguage == currLangid) {
        ComboBox_SetCurSel(hwndLangs, idx);
    }

    return TRUE;
}

INT_PTR CALLBACK Configuration::MainConfigDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            m_hwndConfig = hDlg;
            m_pRes->EnumLanguages(EnumLangsFunc, reinterpret_cast<LONG_PTR>(GetDlgItem(hDlg, IDC_LANGUAGES)));
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDC_LANGUAGES:
                    if(HIWORD(wParam) == LBN_SELCHANGE) {
                        SendMessage(GetParent(hDlg), PSM_CHANGED, reinterpret_cast<WPARAM>(hDlg), 0);
                    }
                    return TRUE;
            }
            break;

        case WM_NOTIFY:
            switch (reinterpret_cast<NMHDR *>(lParam)->code) {
                case PSN_APPLY: {
                    HWND hwndLangs = GetDlgItem(hDlg, IDC_LANGUAGES);
                    int curr = ComboBox_GetCurSel(hwndLangs);
                    WORD langid = static_cast<WORD>(ComboBox_GetItemData(hwndLangs, curr));

                    m_pApp->SaveLangSetting(langid);
                    return TRUE;
                }
                case PSN_HELP:
                    ShowHelp(hDlg);
                    return TRUE;
            }
            break;
    }
    return FALSE;
}


void SelectMoveRadio(HWND hDlg, BOOL bMove) {
    if(bMove) {
        Button_SetCheck(GetDlgItem(hDlg, IDC_MOVEPOS), TRUE);
        Edit_Enable(GetDlgItem(hDlg, IDC_POS_X), TRUE);
        Edit_Enable(GetDlgItem(hDlg, IDC_POS_Y), TRUE);
    } else {
        Button_SetCheck(GetDlgItem(hDlg, IDC_MOVEPOS), FALSE);
        Edit_Enable(GetDlgItem(hDlg, IDC_POS_X), FALSE);
        Edit_SetText(GetDlgItem(hDlg, IDC_POS_X), TEXT(""));
        Edit_Enable(GetDlgItem(hDlg, IDC_POS_Y), FALSE);
        Edit_SetText(GetDlgItem(hDlg, IDC_POS_Y), TEXT(""));
    }
}

void SelectResizeRadio(HWND hDlg, BOOL bMove) {
    if(bMove) {
        Button_SetCheck(GetDlgItem(hDlg, IDC_RESIZE), TRUE);
        Edit_Enable(GetDlgItem(hDlg, IDC_POS_WIDTH), TRUE);
        Edit_Enable(GetDlgItem(hDlg, IDC_POS_HEIGHT), TRUE);
    } else {
        Button_SetCheck(GetDlgItem(hDlg, IDC_RESIZE), FALSE);
        Edit_Enable(GetDlgItem(hDlg, IDC_POS_WIDTH), FALSE);
        Edit_SetText(GetDlgItem(hDlg, IDC_POS_WIDTH), TEXT(""));
        Edit_Enable(GetDlgItem(hDlg, IDC_POS_HEIGHT), FALSE);
        Edit_SetText(GetDlgItem(hDlg, IDC_POS_HEIGHT), TEXT(""));
    }
}

INT_PTR CALLBACK EditPresetConfigDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static SettingItem *pSetItem;
    switch (message) {
        case WM_INITDIALOG:
            pSetItem = reinterpret_cast<SettingItem *>(lParam);

            edit_subclass_numonly(GetDlgItem(hDlg, IDC_POS_X), true);
            edit_subclass_numonly(GetDlgItem(hDlg, IDC_POS_Y), true);
            edit_subclass_numonly(GetDlgItem(hDlg, IDC_POS_WIDTH), false);
            edit_subclass_numonly(GetDlgItem(hDlg, IDC_POS_HEIGHT), false);

            if(pSetItem->rect.left != NO_MOVE_MARK || pSetItem->rect.right != -1) {
                Edit_SetText(GetDlgItem(hDlg, IDC_SETTING_NAME), pSetItem->name);
            } else {
                Edit_SetText(GetDlgItem(hDlg, IDC_SETTING_NAME), TEXT(""));
            }

            if(pSetItem->rect.left == NO_MOVE_MARK) {
                SelectMoveRadio(hDlg, FALSE);
            } else {
                SelectMoveRadio(hDlg, TRUE);
                TCHAR buf[128] = {0};
                wnsprintf(buf, 128, TEXT("%d"), pSetItem->rect.left);
                Edit_SetText(GetDlgItem(hDlg, IDC_POS_X), buf);
                wnsprintf(buf, 128, TEXT("%d"), pSetItem->rect.top);
                Edit_SetText(GetDlgItem(hDlg, IDC_POS_Y), buf);
            }
            if(pSetItem->rect.right == -1) {
                SelectResizeRadio(hDlg, FALSE);
            } else {
                SelectResizeRadio(hDlg, TRUE);
                TCHAR buf[128] = {0};
                wnsprintf(buf, 128, TEXT("%d"), pSetItem->rect.right);
                Edit_SetText(GetDlgItem(hDlg, IDC_POS_WIDTH), buf);
                wnsprintf(buf, 128, TEXT("%d"), pSetItem->rect.bottom);
                Edit_SetText(GetDlgItem(hDlg, IDC_POS_HEIGHT), buf);
            }
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDC_MOVEPOS:
                    SelectMoveRadio(hDlg, Button_GetCheck(GetDlgItem(hDlg, IDC_MOVEPOS)));
                    return TRUE;
                case IDC_RESIZE:
                    SelectResizeRadio(hDlg, Button_GetCheck(GetDlgItem(hDlg, IDC_RESIZE)));
                    return TRUE;
                case IDC_DELFROMLIST:
                    EndDialog(hDlg, LOWORD(wParam));
                    return TRUE;
                case IDOK: {
                    bool setting_ok = true;

                    if( Button_GetCheck(GetDlgItem(hDlg, IDC_MOVEPOS))) {
                        TCHAR buf[128];
                        Edit_GetText(GetDlgItem(hDlg, IDC_POS_X), buf, 128);
                        pSetItem->rect.left = StrToInt(buf);
                        Edit_GetText(GetDlgItem(hDlg, IDC_POS_Y), buf, 128);
                        pSetItem->rect.top = StrToInt(buf);
                    } else {
                        pSetItem->rect.left = pSetItem->rect.top = NO_MOVE_MARK;
                    }
                    if( Button_GetCheck(GetDlgItem(hDlg, IDC_RESIZE))) {
                        TCHAR buf[128];
                        Edit_GetText(GetDlgItem(hDlg, IDC_POS_WIDTH), buf, 128);
                        pSetItem->rect.right = StrToInt(buf);
                        Edit_GetText(GetDlgItem(hDlg, IDC_POS_HEIGHT), buf, 128);
                        pSetItem->rect.bottom = StrToInt(buf);
                        if(pSetItem->rect.right < 0 || pSetItem->rect.bottom < 0) {
                            setting_ok = false;  // TODO setting error
                        }
                    } else {
                        pSetItem->rect.right = pSetItem->rect.bottom = -1;
                    }

                    if(pSetItem->rect.left == NO_MOVE_MARK && pSetItem->rect.right == -1) {
                        setting_ok = false;
                    }

                    Edit_GetText(GetDlgItem(hDlg, IDC_SETTING_NAME), pSetItem->name, sizeof(pSetItem->name));

                    EndDialog(hDlg, LOWORD(wParam));
                    return TRUE;
                }
                case IDCANCEL:
                    EndDialog(hDlg, LOWORD(wParam));
                    return TRUE;
            }
            break;

    }
    return FALSE;
}





INT_PTR CALLBACK Configuration::PresetConfigDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG: {
            HWND hwndlist = GetDlgItem(hDlg, IDC_SIZELIST);
            RefreshList(hwndlist, m_configlist);

            ListBox_SetCurSel(hwndlist, 0);
            SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_SIZELIST, LBN_SELCHANGE), (LPARAM)hDlg);

            return TRUE;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDC_SIZELIST:
                    switch(HIWORD(wParam)) {
                        case LBN_DBLCLK: {
                            HWND hwndlist = GetDlgItem(hDlg, IDC_SIZELIST);
                            int cursel = ListBox_GetCurSel(hwndlist);
                            SettingItem si = m_configlist[cursel];
                            TemplateRes templ = m_pRes->LoadTemplateRes(RT_DIALOG, IDD_PRESET_CONFIG);
                            int id = DialogBoxIndirectParam(m_pRes->GetModule(), templ, GetParent(hDlg), EditPresetConfigDlgProc, reinterpret_cast<LPARAM>(&si));
                            if(id == IDOK) {
                                m_configlist[cursel] = si;
                                RefreshList(hwndlist, m_configlist);
                                ListBox_SetCurSel(hwndlist, cursel);
                                SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_SIZELIST, LBN_SELCHANGE), reinterpret_cast<LPARAM>(hDlg));
                                SendMessage(GetParent(hDlg), PSM_CHANGED, reinterpret_cast<WPARAM>(hDlg), 0);
                            } else if( id == IDC_DELFROMLIST) {
                                memset(m_configlist[cursel].name, 0, sizeof(m_configlist[cursel].name));
                                m_configlist[cursel].rect.left = NO_MOVE_MARK;
                                m_configlist[cursel].rect.top  = NO_MOVE_MARK;
                                m_configlist[cursel].rect.right = -1;
                                m_configlist[cursel].rect.bottom = -1;
                                RefreshList(hwndlist, m_configlist);
                                ListBox_SetCurSel(hwndlist, cursel);
                                SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_SIZELIST, LBN_SELCHANGE), reinterpret_cast<LPARAM>(hDlg));
                                SendMessage(GetParent(hDlg), PSM_CHANGED, reinterpret_cast<WPARAM>(hDlg), 0);
                            }
                            return TRUE;
                        }
                    }
                    break;

                case IDC_MOVEUP: {
                    HWND hwndlist = GetDlgItem(hDlg, IDC_SIZELIST);
                    int cursel = ListBox_GetCurSel(hwndlist);
                    if(cursel != LB_ERR && cursel > 0) {
                        SettingItem tt = m_configlist[cursel-1];
                        m_configlist[cursel-1] = m_configlist[cursel];
                        m_configlist[cursel] = tt;
                        RefreshList(hwndlist, m_configlist);
                        ListBox_SetCurSel(hwndlist, cursel-1);

                        SendMessage(GetParent(hDlg), PSM_CHANGED, reinterpret_cast<WPARAM>(hDlg), 0);
                    }
                    return (INT_PTR)TRUE;
                }
                case IDC_MOVEDOWN: {
                    HWND hwndlist = GetDlgItem(hDlg, IDC_SIZELIST);
                    int cursel = ListBox_GetCurSel(hwndlist);
                    if(cursel != LB_ERR && cursel < ListBox_GetCount(hwndlist)-1) {
                        SettingItem tt = m_configlist[cursel+1];
                        m_configlist[cursel+1] = m_configlist[cursel];
                        m_configlist[cursel] = tt;
                        RefreshList(hwndlist, m_configlist);
                        ListBox_SetCurSel(hwndlist, cursel+1);

                        SendMessage(GetParent(hDlg), PSM_CHANGED, reinterpret_cast<WPARAM>(hDlg), 0);
                    }
                    return (INT_PTR)TRUE;
                }
            }
            break;

        case WM_NOTIFY:
            switch (reinterpret_cast<NMHDR *>(lParam)->code) {
                case PSN_APPLY:
                    m_pApp->SavePresetSetting(m_configlist);
                    return TRUE;
            }
            break;
    }
    return FALSE;
}

BOOL Configuration::ShowDialog(Application *pApp, LangRes* pRes, SettingItem* configlist) {
    if(m_hwndConfig != NULL) {
        BringWindowToTop(GetParent(m_hwndConfig));
        return TRUE;
    }

    m_hwndConfig = NULL;

    m_pApp = pApp;
    m_pRes = pRes;
    memcpy(m_configlist, configlist, sizeof(m_configlist));

    
    TemplateRes dlgres[2] = {
        pRes->LoadTemplateRes(RT_DIALOG, IDD_MAIN),
        pRes->LoadTemplateRes(RT_DIALOG, IDD_CONFIG),
    };
    TCHAR configtitle[2][128] = {0};
    StrCpy( configtitle[0], pRes->LoadString(IDS_CONFIG_TITLE_MAIN, NULL));
    StrCpy( configtitle[1], pRes->LoadString(IDS_CONFIG_TITLE_PRESET, NULL));
 
    PROPSHEETPAGE psp[2] = {0};
    psp[0].dwSize = sizeof (PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USETITLE|PSP_DLGINDIRECT|PSP_HASHELP;
    psp[0].hInstance = pRes->GetModule();
    psp[0].pResource = dlgres[0];
    psp[0].pfnDlgProc = MainConfigDlgProc;
    psp[0].pszTitle = configtitle[0];
    psp[1].dwSize = sizeof (PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USETITLE|PSP_DLGINDIRECT|PSP_HASHELP;
    psp[1].hInstance = pRes->GetModule();
    psp[1].pResource = dlgres[1];
    psp[1].pfnDlgProc = PresetConfigDlgProc;
    psp[1].pszTitle = configtitle[1];

    PROPSHEETHEADER psh = {0};
    psh.dwSize = sizeof (PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE|PSH_HASHELP|PSH_USEPAGELANG;
    psh.hwndParent = NULL;
    psh.hInstance = pRes->GetModule();
    psh.pszCaption = pRes->LoadString(IDS_CONFIG_TITLE, NULL);
    psh.nPages = sizeof (psp) / sizeof (PROPSHEETPAGE);
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;

    PropertySheet (&psh);

    m_hwndConfig = NULL;
    return TRUE;
}


