#include "kbSizer.h"

#include <windowsx.h>
#include <shlwapi.h>
#include "res.h"

static LPCTSTR APP_NAME = _T("kbSizer");


#define HK_UP                           (1)
#define HK_LEFT                         (2)
#define HK_DOWN                         (3)
#define HK_RIGHT                        (4)
#define HK_LBUTTON                      (5)
#define HK_RBUTTON                      (6)

#define HK_HIEHGT_DEC                   (11)
#define HK_WIDTH_DEC                    (12)
#define HK_HEIGHT_ENC                   (13)
#define HK_WIDTH_ENC                    (14)
#define HK_ON_TOPMOST                   (15)
#define HK_ALPHA_UP                     (16)
#define HK_ALPHA_DOWN                   (17)
#define HK_SWITCH_MODE                  (18)

#define HK_SIZE_0                       (20)
#define HK_SIZE_1                       (21)
#define HK_SIZE_2                       (22)
#define HK_SIZE_3                       (23)
#define HK_SIZE_4                       (24)
#define HK_SIZE_5                       (25)
#define HK_SIZE_6                       (26)
#define HK_SIZE_7                       (27)
#define HK_SIZE_8                       (28)
#define HK_SIZE_9                       (29)

#define HK_NUMPAD_0                     (30)
#define HK_NUMPAD_1                     (31)
#define HK_NUMPAD_2                     (32)
#define HK_NUMPAD_3                     (33)
#define HK_NUMPAD_4                     (34)
#define HK_NUMPAD_5                     (35)
#define HK_NUMPAD_6                     (36)
#define HK_NUMPAD_7                     (37)
#define HK_NUMPAD_8                     (38)
#define HK_NUMPAD_9                     (39)
#define HK_NUMPAD_DOT                   (40)

#define HK_NUMPAD_SIZE_0                (50)
#define HK_NUMPAD_SIZE_1                (51)
#define HK_NUMPAD_SIZE_2                (52)
#define HK_NUMPAD_SIZE_3                (53)
#define HK_NUMPAD_SIZE_4                (54)
#define HK_NUMPAD_SIZE_5                (55)
#define HK_NUMPAD_SIZE_6                (56)
#define HK_NUMPAD_SIZE_7                (57)
#define HK_NUMPAD_SIZE_8                (58)
#define HK_NUMPAD_SIZE_9                (59)


#define NUM_BLOCK						(16)

#define LOAD_TRAYICON(IDI) (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR)


UINT HotKeyMap[][3] = {
    {HK_UP,         MOD_WIN,             VK_UP},
    {HK_LEFT,       MOD_WIN,             VK_LEFT},
    {HK_DOWN,       MOD_WIN,             VK_DOWN},
    {HK_RIGHT,      MOD_WIN,             VK_RIGHT},
    {HK_HIEHGT_DEC, MOD_WIN|MOD_CONTROL, VK_UP},
    {HK_WIDTH_DEC,  MOD_WIN|MOD_CONTROL, VK_LEFT},
    {HK_HEIGHT_ENC, MOD_WIN|MOD_CONTROL, VK_DOWN},
    {HK_WIDTH_ENC,  MOD_WIN|MOD_CONTROL, VK_RIGHT},
    {HK_ON_TOPMOST, MOD_WIN,             VK_HOME},
    {HK_ALPHA_UP,   MOD_WIN,             VK_PRIOR},
    {HK_ALPHA_DOWN, MOD_WIN,             VK_NEXT},
    {HK_LBUTTON,    MOD_WIN,             'Z'},
    {HK_RBUTTON,    MOD_WIN,             'X'},
    {HK_SWITCH_MODE,MOD_WIN,             VK_SPACE},


    {HK_SIZE_1,     MOD_WIN,             '1'},
    {HK_SIZE_2,     MOD_WIN,             '2'},
    {HK_SIZE_3,     MOD_WIN,             '3'},
    {HK_SIZE_4,     MOD_WIN,             '4'},
    {HK_SIZE_5,     MOD_WIN,             '5'},
    {HK_SIZE_6,     MOD_WIN,             '6'},
    {HK_SIZE_7,     MOD_WIN,             '7'},
    {HK_SIZE_8,     MOD_WIN,             '8'},
    {HK_SIZE_9,     MOD_WIN,             '9'},
    {HK_SIZE_0,     MOD_WIN,             '0'},

    {HK_NUMPAD_1,   MOD_WIN,             VK_NUMPAD1},
    {HK_NUMPAD_2,   MOD_WIN,             VK_NUMPAD2},
    {HK_NUMPAD_3,   MOD_WIN,             VK_NUMPAD3},
    {HK_NUMPAD_4,   MOD_WIN,             VK_NUMPAD4},
    {HK_NUMPAD_5,   MOD_WIN,             VK_NUMPAD5},
    {HK_NUMPAD_6,   MOD_WIN,             VK_NUMPAD6},
    {HK_NUMPAD_7,   MOD_WIN,             VK_NUMPAD7},
    {HK_NUMPAD_8,   MOD_WIN,             VK_NUMPAD8},
    {HK_NUMPAD_9,   MOD_WIN,             VK_NUMPAD9},
    {HK_NUMPAD_0,   MOD_WIN,             VK_NUMPAD0},
    {HK_NUMPAD_DOT, MOD_WIN,             VK_DECIMAL},

    {HK_NUMPAD_SIZE_1, MOD_WIN|MOD_CONTROL, VK_NUMPAD1},
    {HK_NUMPAD_SIZE_2, MOD_WIN|MOD_CONTROL, VK_NUMPAD2},
    {HK_NUMPAD_SIZE_3, MOD_WIN|MOD_CONTROL, VK_NUMPAD3},
    {HK_NUMPAD_SIZE_4, MOD_WIN|MOD_CONTROL, VK_NUMPAD4},
    {HK_NUMPAD_SIZE_5, MOD_WIN|MOD_CONTROL, VK_NUMPAD5},
    {HK_NUMPAD_SIZE_6, MOD_WIN|MOD_CONTROL, VK_NUMPAD6},
    {HK_NUMPAD_SIZE_7, MOD_WIN|MOD_CONTROL, VK_NUMPAD7},
    {HK_NUMPAD_SIZE_8, MOD_WIN|MOD_CONTROL, VK_NUMPAD8},
    {HK_NUMPAD_SIZE_9, MOD_WIN|MOD_CONTROL, VK_NUMPAD9},
};











const int TRAYICONID = 1;
const int SWM_TRAYMSG = WM_APP;

extern void ShowAbout(HWND hwndParent);


static void RestoreWindowIfMaxized(HWND hwnd) {
	if(IsZoomed(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE);
	}
}

static int widthScr = 0;
static int heightScr = 0;


int align(int x, int direction) {
	int dx = widthScr/NUM_BLOCK;
	int dy = heightScr/NUM_BLOCK;
	
	switch( direction) {
		case HK_UP:    return (x>0) ?  ((x-dy+1)/dy)*dy : ((x-dy-1)/dy)*dy;
		case HK_LEFT:  return (x>0) ?  ((x-dx+1)/dx)*dx : ((x-dx-1)/dx)*dx;
		case HK_DOWN:  return (x>=0) ? ((x+dy+1)/dy)*dy : ((x+dy-1)/dy)*dy;
		case HK_RIGHT: return (x>=0) ? ((x+dx+1)/dx)*dx : ((x+dx-1)/dx)*dx;
	}
	return 0;
}


BOOL MoveWindowWithNumpad(int idx, HWND hwnd, RECT r) {
    switch(idx) {
        case 1: SetWindowPos(hwnd, NULL, 0,                             (heightScr-(r.bottom-r.top)),   0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
        case 2: SetWindowPos(hwnd, NULL, (widthScr-(r.right-r.left))/2, (heightScr-(r.bottom-r.top)),   0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
        case 3: SetWindowPos(hwnd, NULL, (widthScr-(r.right-r.left)),   (heightScr-(r.bottom-r.top)),   0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
        case 4: SetWindowPos(hwnd, NULL, 0,                             (heightScr-(r.bottom-r.top))/2, 0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
        case 5: SetWindowPos(hwnd, NULL, (widthScr-(r.right-r.left))/2, (heightScr-(r.bottom-r.top))/2, 0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
        case 6: SetWindowPos(hwnd, NULL, (widthScr-(r.right-r.left)),   (heightScr-(r.bottom-r.top))/2, 0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
        case 7: SetWindowPos(hwnd, NULL, 0,                             0,                              0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
        case 8: SetWindowPos(hwnd, NULL, (widthScr-(r.right-r.left))/2, 0,                              0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
        case 9: SetWindowPos(hwnd, NULL, (widthScr-(r.right-r.left)),   0,                              0, 0, SWP_NOZORDER|SWP_NOSIZE); break;
    }
    return TRUE;
}




HANDLE hEvent = NULL;
BOOL   bThreadRunning = FALSE;

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
    hEvent = CreateEvent(NULL, NULL, FALSE, NULL);
    POINT p;
    BOOL shortkey[7];
    double diff = 0;
    int lastdirect = 0;

    while(true) {
        WaitForSingleObject(hEvent, INFINITE);
        bThreadRunning = TRUE;
        diff = 1;
        lastdirect = 0;

        while(true) {
            shortkey[0] = (GetAsyncKeyState(VK_LWIN)&0x08000)>>15;
            shortkey[1] = (GetAsyncKeyState(VK_UP)&0x08000)>>15;
            shortkey[2] = (GetAsyncKeyState(VK_DOWN)&0x08000)>>15;
            shortkey[3] = (GetAsyncKeyState(VK_LEFT)&0x08000)>>15;
            shortkey[4] = (GetAsyncKeyState(VK_RIGHT)&0x08000)>>15;
            shortkey[5] = (GetAsyncKeyState('Z')&0x08000)>>15;
            shortkey[6] = (GetAsyncKeyState('X')&0x08000)>>15;
            int direct = ((shortkey[1])|(shortkey[2]<<1)|(shortkey[3]<<2)|(shortkey[4]<<3)|(shortkey[5]<<4)|(shortkey[6]<<5));
            if(direct != lastdirect) {
                diff = 1;
                if((lastdirect&0x010) && !(direct&0x010)) {
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, NULL);
                }
                if((lastdirect&0x020) && !(direct&0x020)) {
                    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, NULL);
                }
                lastdirect = direct;
            }
            if(!(shortkey[0]) || !direct) {
                break;
            }


            GetCursorPos(&p);
            int step = static_cast<int>(diff);
            if(shortkey[1]) p.y -= step;
            if(shortkey[2]) p.y += step;
            if(shortkey[3]) p.x -= step;
            if(shortkey[4]) p.x += step;
            SetCursorPos(p.x, p.y);
            Sleep(5);

            if(diff < 2) diff += 0.1;
            else if(diff < 4) diff += 0.05;
            else if(diff < 10) diff += 0.1;
            else if(diff < 20) diff += 0.2;
        }
        bThreadRunning = FALSE;
    }
    return TRUE;
}



LPCTSTR PROPSTR = TEXT("_KBSIZER_ALPHA_");

BOOL checkWidnow(HWND hwnd) {
    LONG wnStl = GetWindowLong(hwnd, GWL_STYLE);
    if((wnStl & WS_CAPTION) != WS_CAPTION) return FALSE;
    return TRUE;
}

void Application::Main_OnHotKey(HWND hwnd, int idHotKey, UINT /*fuModifiers*/, UINT /*vk*/) {
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    widthScr = workArea.right - workArea.left;
    heightScr = workArea.bottom - workArea.top;
	HWND hw = GetForegroundWindow();
	RECT r;
	GetWindowRect(hw, &r);

    Application *pThis = reinterpret_cast<Application*>(GetWindowLong(hwnd, GWL_USERDATA));
	switch(idHotKey) {
        case HK_UP:
            if(pThis->currMode == MODE_MOVE) {
                if(!checkWidnow(hw)) return;
                RestoreWindowIfMaxized(hw);
                if(r.bottom > 0)
                    MoveWindow(hw, r.left, align(r.top, HK_UP), r.right-r.left, r.bottom-r.top, true);
            } else if(pThis->currMode == MODE_MOUSE) {
                if(!bThreadRunning) SetEvent(hEvent);
            }
			break;
		case HK_LEFT:
            if(pThis->currMode == MODE_MOVE) {
                if(!checkWidnow(hw)) return;
			    RestoreWindowIfMaxized(hw);
			    if(r.right > 0)
				    MoveWindow(hw, align(r.left, HK_LEFT), r.top, r.right-r.left, r.bottom-r.top, true);
            } else if(pThis->currMode == MODE_MOUSE) {
                if(!bThreadRunning) SetEvent(hEvent);
            }
			break;
		case HK_DOWN:
            if(pThis->currMode == MODE_MOVE) {
                if(!checkWidnow(hw)) return;
                RestoreWindowIfMaxized(hw);
                if(r.top < heightScr)
                    MoveWindow(hw, r.left, align(r.top, HK_DOWN), r.right-r.left, r.bottom-r.top, true);
            } else if(pThis->currMode == MODE_MOUSE) {
                if(!bThreadRunning) SetEvent(hEvent);
            }
			break;
		case HK_RIGHT:
            if(pThis->currMode == MODE_MOVE) {
                if(!checkWidnow(hw)) return;
                RestoreWindowIfMaxized(hw);
                if(r.left < widthScr)
                    MoveWindow(hw, align(r.left, HK_RIGHT), r.top, r.right-r.left, r.bottom-r.top, true);
            } else if(pThis->currMode == MODE_MOUSE) {
                if(!bThreadRunning) SetEvent(hEvent);
            }
			break;
        case HK_LBUTTON:
            if(pThis->currMode == MODE_MOUSE) {
                if(!bThreadRunning) {
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, NULL);
                    SetEvent(hEvent);
                }
            }
            break;
        case HK_RBUTTON:
            if(pThis->currMode == MODE_MOUSE) {
                if(!bThreadRunning) {
                    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, NULL);
                    SetEvent(hEvent);
                }
            }
            break;
        case HK_SWITCH_MODE:
            if(pThis->currMode == MODE_MOVE) {
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_MODE_MOUSE, hwnd), 0);
            } else {
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDM_MODE_MOVE, hwnd), 0);
			}
            break;
		case HK_HIEHGT_DEC:
            if(!checkWidnow(hw)) return;
			RestoreWindowIfMaxized(hw);
			MoveWindow(hw, r.left, r.top, r.right-r.left, align(r.bottom, HK_UP)-r.top, true);
			break;
		case HK_WIDTH_DEC:
            if(!checkWidnow(hw)) return;
			RestoreWindowIfMaxized(hw);
			MoveWindow(hw, r.left, r.top, align(r.right, HK_LEFT)-r.left, r.bottom-r.top, true);
			break;
		case HK_HEIGHT_ENC:
            if(!checkWidnow(hw)) return;
			RestoreWindowIfMaxized(hw);
			MoveWindow(hw, r.left, r.top, r.right-r.left, align(r.bottom, HK_DOWN)-r.top, true);
			break;
		case HK_WIDTH_ENC:
            if(!checkWidnow(hw)) return;
			RestoreWindowIfMaxized(hw);
			MoveWindow(hw, r.left, r.top, align(r.right, HK_RIGHT)-r.left, r.bottom-r.top, true);
			break;
        case HK_ON_TOPMOST:
            if(!checkWidnow(hw)) return;
            if(GetWindowLong(hw, GWL_EXSTYLE) & WS_EX_TOPMOST) {
                SetWindowPos(hw, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOREDRAW|SWP_NOSIZE);
            } else {
                SetWindowPos(hw, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOREDRAW|SWP_NOSIZE);
            }
            break;
        case HK_ALPHA_UP:
        case HK_ALPHA_DOWN: {
            if(!checkWidnow(hw)) return;
            int hwalpha = reinterpret_cast<int>(GetProp(hw, PROPSTR));
            LONG exst = GetWindowLong(hw, GWL_EXSTYLE);
            
            if(exst & WS_EX_LAYERED && hwalpha != 0) {
                if( idHotKey == HK_ALPHA_UP) {
                    hwalpha = (hwalpha + 20)/20*20;
                } else {
                    hwalpha = (hwalpha - 20)/20*20;
                }
                if(hwalpha > 0) {
                    if(hwalpha >= 255) {
                        SetWindowLong(hw, GWL_EXSTYLE, exst & ~WS_EX_LAYERED );
                        RemoveProp(hw, PROPSTR);
                    } else {
                        SetLayeredWindowAttributes(hw, NULL, static_cast<BYTE>(hwalpha), LWA_ALPHA);
                        SetProp(hw, PROPSTR, reinterpret_cast<HANDLE>(hwalpha));
                    }
                }
            } else if(idHotKey == HK_ALPHA_DOWN) {
                SetWindowLong(hw, GWL_EXSTYLE, exst | WS_EX_LAYERED);
                SetLayeredWindowAttributes(hw, NULL, 240, LWA_ALPHA);
                hwalpha = 240;
                SetProp(hw, PROPSTR, reinterpret_cast<HANDLE>(hwalpha));
            }
            break;
        }

        case HK_SIZE_0: {
            //pThis->res.ShowDialogBox(GetModuleHandle(NULL), IDD_REGHOTKEY, NULL, ConfigDlgProc, NULL);
            break;
        }

        case HK_SIZE_1:
        case HK_SIZE_2:
        case HK_SIZE_3:
        case HK_SIZE_4:
        case HK_SIZE_5:
        case HK_SIZE_6:
        case HK_SIZE_7:
        case HK_SIZE_8:
        case HK_SIZE_9: {
            if(!checkWidnow(hw)) return;
            SettingItem *psi = pThis->m_sizelist + (idHotKey - HK_SIZE_1);
            if(psi->rect.left == NO_MOVE_MARK && psi->rect.right == -1) {
                // do nothing
            } else if(psi->rect.left == NO_MOVE_MARK) {
                RestoreWindowIfMaxized(hw);
                MoveWindow(hw, r.left, r.top, psi->rect.right, psi->rect.bottom, true);
            } else if(psi->rect.right == -1) {
                RestoreWindowIfMaxized(hw);
                MoveWindow(hw, psi->rect.left, psi->rect.top, r.right-r.left, r.bottom-r.top, true);
            } else {
                RestoreWindowIfMaxized(hw);
                MoveWindow(hw, psi->rect.left, psi->rect.top, psi->rect.right, psi->rect.bottom, true);
            }
            break;
        }

        case HK_NUMPAD_1:
        case HK_NUMPAD_2:
        case HK_NUMPAD_3:
        case HK_NUMPAD_4:
        case HK_NUMPAD_5:
        case HK_NUMPAD_6:
        case HK_NUMPAD_7:
        case HK_NUMPAD_8:
        case HK_NUMPAD_9:
            if(!checkWidnow(hw)) return;
            RestoreWindowIfMaxized(hw);
            MoveWindowWithNumpad(idHotKey-HK_NUMPAD_0, hw, r);
            break;
        case HK_NUMPAD_0:
            if(!checkWidnow(hw)) return;
            if(IsZoomed(hw)) {
                ShowWindow(hw, SW_RESTORE);
            } else {
                ShowWindow(hw, SW_MAXIMIZE);
            }
            break;
        case HK_NUMPAD_DOT:
            if(!checkWidnow(hw)) return;
            ShowWindow(hw, SW_MINIMIZE);
            break;


        case HK_NUMPAD_SIZE_1:
        case HK_NUMPAD_SIZE_2:
        case HK_NUMPAD_SIZE_3:
        case HK_NUMPAD_SIZE_4:
        case HK_NUMPAD_SIZE_5:
        case HK_NUMPAD_SIZE_6:
        case HK_NUMPAD_SIZE_7:
        case HK_NUMPAD_SIZE_8:
        case HK_NUMPAD_SIZE_9: {
            if(!checkWidnow(hw)) return;
            static bool isnext = 1;
            static int  firstkey = 0;
            static SHORT keystat[4] = {GetKeyState(VK_LCONTROL), GetKeyState(VK_LWIN), GetKeyState(VK_RCONTROL), GetKeyState(VK_RWIN)};
            SHORT currkeystat[4] = {GetKeyState(VK_LCONTROL), GetKeyState(VK_LWIN), GetKeyState(VK_RCONTROL), GetKeyState(VK_RWIN)};

            if( ((keystat[0] & 0x01) != (currkeystat[0] & 0x01)) || ((keystat[1] & 0x01) != (currkeystat[1] & 0x01))
                    || ((keystat[2] & 0x01) != (currkeystat[2] & 0x01)) || ((keystat[3] & 0x01) != (currkeystat[3] & 0x01))) {
                isnext = 0;
            } else {
                isnext = isnext ? 0 : 1;
            }
            memcpy(keystat, currkeystat, sizeof(currkeystat));

            if(isnext == 0) {
                RestoreWindowIfMaxized(hw);
                MoveWindowWithNumpad(idHotKey-HK_NUMPAD_SIZE_0, hw, r);
                firstkey = idHotKey;
            } else {
                int kk = ((firstkey-HK_NUMPAD_SIZE_0) << 4) + (idHotKey-HK_NUMPAD_SIZE_0);
                switch(kk) {
                    case 0x11:            SetWindowPos(hw, NULL, 0,            heightScr*2/3, widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x22:            SetWindowPos(hw, NULL, widthScr/3,   heightScr*2/3, widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x33:            SetWindowPos(hw, NULL, widthScr*2/3, heightScr*2/3, widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x44:            SetWindowPos(hw, NULL, 0,            heightScr/3,   widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x55:            SetWindowPos(hw, NULL, widthScr/3,   heightScr/3,   widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x66:            SetWindowPos(hw, NULL, widthScr*2/3, heightScr/3,   widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x77:            SetWindowPos(hw, NULL, 0,            0,             widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x88:            SetWindowPos(hw, NULL, widthScr/3,   0,             widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x99:            SetWindowPos(hw, NULL, widthScr*2/3, 0,             widthScr/3, heightScr/3, SWP_NOZORDER); break;
                    case 0x12: case 0x21: SetWindowPos(hw, NULL, 0,            heightScr*2/3, widthScr/2, heightScr/3, SWP_NOZORDER); break;
                    case 0x13: case 0x31: SetWindowPos(hw, NULL, 0,            heightScr*2/3, widthScr,   heightScr/3, SWP_NOZORDER); break;
                    case 0x14: case 0x41: SetWindowPos(hw, NULL, 0,            heightScr/2,   widthScr/3, heightScr/2, SWP_NOZORDER); break;
                    case 0x15: case 0x51: 
                    case 0x24: case 0x42: SetWindowPos(hw, NULL, 0,            heightScr/2,   widthScr/2, heightScr/2, SWP_NOZORDER); break;
                    case 0x16: case 0x61: 
                    case 0x34: case 0x43: SetWindowPos(hw, NULL, 0,            heightScr/2,   widthScr,   heightScr/2, SWP_NOZORDER); break;
                    case 0x17: case 0x71: SetWindowPos(hw, NULL, 0,            0,             widthScr/3, heightScr,   SWP_NOZORDER); break;
                    case 0x18: case 0x81: 
                    case 0x27: case 0x72: SetWindowPos(hw, NULL, 0,            0,             widthScr/2, heightScr,   SWP_NOZORDER); break;
                    case 0x19: case 0x91: 
                    case 0x37: case 0x73: SetWindowPos(hw, NULL, 0,            0,             widthScr,   heightScr,   SWP_NOZORDER); break;
                    case 0x23: case 0x32: SetWindowPos(hw, NULL, widthScr/2,   heightScr*2/3, widthScr/2, heightScr/3, SWP_NOZORDER); break;
                    case 0x25: case 0x52: SetWindowPos(hw, NULL, widthScr/3,   heightScr/2,   widthScr/3, heightScr/2, SWP_NOZORDER); break;
                    case 0x26: case 0x62: 
                    case 0x35: case 0x53: SetWindowPos(hw, NULL, widthScr/2,   heightScr/2,   widthScr/2, heightScr/2, SWP_NOZORDER); break;
                    case 0x28: case 0x82: SetWindowPos(hw, NULL, widthScr/3,   0,             widthScr/3, heightScr,   SWP_NOZORDER); break;
                    case 0x29: case 0x92: 
                    case 0x38: case 0x83: SetWindowPos(hw, NULL, widthScr/2,   0,             widthScr/2, heightScr,   SWP_NOZORDER); break;
                    case 0x36: case 0x63: SetWindowPos(hw, NULL, widthScr*2/3, heightScr/2,   widthScr/3, heightScr/2, SWP_NOZORDER); break;
                    case 0x39: case 0x93: SetWindowPos(hw, NULL, widthScr*2/3, 0,             widthScr/3, heightScr,   SWP_NOZORDER); break;
                    case 0x45: case 0x54: SetWindowPos(hw, NULL, 0,            heightScr/3,   widthScr/2, heightScr/3, SWP_NOZORDER); break;
                    case 0x46: case 0x64: SetWindowPos(hw, NULL, 0,            heightScr/3,   widthScr,   heightScr/3, SWP_NOZORDER); break;
                    case 0x47: case 0x74: SetWindowPos(hw, NULL, 0,            0,             widthScr/3, heightScr/2, SWP_NOZORDER); break;
                    case 0x48: case 0x84: 
                    case 0x57: case 0x75: SetWindowPos(hw, NULL, 0,            0,             widthScr/2, heightScr/2, SWP_NOZORDER); break;
                    case 0x49: case 0x94: 
                    case 0x67: case 0x76: SetWindowPos(hw, NULL, 0,            0,             widthScr,   heightScr/2, SWP_NOZORDER); break;
                    case 0x56: case 0x65: SetWindowPos(hw, NULL, widthScr/2,   heightScr/3,   widthScr/2, heightScr/3, SWP_NOZORDER); break;
                    case 0x58: case 0x85: SetWindowPos(hw, NULL, widthScr/3,   0,             widthScr/3, heightScr/2, SWP_NOZORDER); break;
                    case 0x59: case 0x95: 
                    case 0x68: case 0x86: SetWindowPos(hw, NULL, widthScr/2,   0,             widthScr/2, heightScr/2, SWP_NOZORDER); break;
                    case 0x69: case 0x96: SetWindowPos(hw, NULL, widthScr*2/3, 0,             widthScr/3, heightScr/2, SWP_NOZORDER); break;
                    case 0x78: case 0x87: SetWindowPos(hw, NULL, 0,            0,             widthScr/2, heightScr/3, SWP_NOZORDER); break;
                    case 0x79: case 0x97: SetWindowPos(hw, NULL, 0,            0,             widthScr,   heightScr/3, SWP_NOZORDER); break;
                    case 0x89: case 0x98: SetWindowPos(hw, NULL, widthScr/2,   0,             widthScr/2, heightScr/3, SWP_NOZORDER); break;
                }
            }
            break;
        }
	}
}


static BOOL IsAutoRun() {
    TCHAR filepath[4][MAX_PATH];
    if( SHRegGetPath(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), APP_NAME, filepath[0], 0) == ERROR_SUCCESS) {
        PathCanonicalize(filepath[1], filepath[0]);
        GetModuleFileName(NULL, filepath[2], MAX_PATH);
        PathCanonicalize(filepath[3], filepath[2]);
        if(!StrCmp(filepath[1], filepath[3])) {
            return TRUE;
        } else {
            SHDeleteValue(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), APP_NAME);
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

#define SUBMENU_MODE_POS  (2)

void Application::ShowContextMenu(HWND hwnd) {
    TemplateRes menu = res.LoadTemplateRes(RT_MENU, IDR_CTXMENU);

    HMENU hMainMenu = LoadMenuIndirect(menu);
    HMENU hMenu     = GetSubMenu(hMainMenu, 0);

    if(isEnable) {
        CheckMenuItem(hMenu, IDM_ENABLE, MF_CHECKED);
    }
    if(IsAutoRun()) {
        CheckMenuItem(hMenu, IDM_AUTORUN, MF_CHECKED);
    }


    TCHAR strbuf[128] = {0};
    if(currMode == MODE_MOVE) {
        CheckMenuItem(hMenu, IDM_MODE_MOVE, MF_CHECKED);
        GetMenuString(hMenu, IDM_MODE_MOVE, strbuf, 128, 0);
    } else {
        CheckMenuItem(hMenu, IDM_MODE_MOUSE, MF_CHECKED);
        GetMenuString(hMenu, IDM_MODE_MOUSE, strbuf, 128, 0);
    }
    
    MENUITEMINFO menuinfo = {sizeof(MENUITEMINFO), MIIM_STRING};
    menuinfo.dwTypeData = strbuf;
    menuinfo.cch = _tcslen(strbuf);
    SetMenuItemInfo(hMenu, SUBMENU_MODE_POS, TRUE, &menuinfo);

	POINT pt;
	GetCursorPos(&pt);

    // KB:Q135788 
    SetForegroundWindow(hwnd);
    TrackPopupMenu(hMenu, TPM_LEFTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
    PostMessage(hwnd, WM_NULL, 0, 0);

    DestroyMenu(hMenu); 
}

/* void Cls_OnTrayMsg(HWND hwnd, int id) */
#define HANDLE_SWM_TRAYMSG(hwnd, wParam, lParam, fn) \
	((fn)((hwnd), (int)(lParam)), 0L)

void Application::Main_OnTrayMsg(HWND hwnd, int id) {
	switch(id) {
        case WM_LBUTTONDBLCLK:
            ShowAbout(hwnd);            
            break;
		case WM_CONTEXTMENU:
			reinterpret_cast<Application*>(GetWindowLong(hwnd, GWL_USERDATA))->ShowContextMenu(hwnd);
			break;
	}
}





void Application::RegHotKey(HWND hwnd) {
    int num = sizeof(HotKeyMap)/sizeof(int[3]);
    for(int i = 0; i< num; ++i) {
        RegisterHotKey(hwnd, HotKeyMap[i][0], HotKeyMap[i][1], HotKeyMap[i][2]);
    }
}

void Application::UnRegHotKey(HWND hwnd) {
    int num = sizeof(HotKeyMap)/sizeof(int[3]);
    for(int i = 0; i< num; ++i) {
        UnregisterHotKey(hwnd, HotKeyMap[i][0]);
    }
}

BOOL Application::Main_OnCreate(HWND hwnd, LPCREATESTRUCT) {
    CloseHandle(CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL));

    RegHotKey(hwnd);
    return TRUE;
}





static LPCTSTR WNDCLSNAME = _T("_WNDCLS_KBSIZER_");
static LPCTSTR REGKEY     = _T("Software\\kbSizer");
static LPCTSTR REGVAL_LANGID   = _T("LANGID");
static LPCTSTR REGVAL_SIZELIST = _T("SIZELIST");
static LPCTSTR REGVAL_SIZELIST_DEF = _T("640x480:*,*:640x480;800x600:*,*:800x600;1024x768:0,0:1024x768;:*,*:-1x-1;:*,*:-1x-1;:*,*:-1x-1;:*,*:-1x-1;:*,*:-1x-1;:*,*:-1x-1;");


Application::Application() : isEnable(TRUE), currMode(MODE_MOVE) {
    HWND hwnd = CreateMainWindow();
    trayIcon.Init(hwnd, TRAYICONID, LOAD_TRAYICON(IDI_TRAY_KEYBOARD), SWM_TRAYMSG, APP_NAME);

    InitSetting();

    trayIcon.InstallIcon();
}







BOOL ParseSizeItem(LPTSTR pStr, LPTSTR *pName, int *pl, int *pt, int *pw, int *ph) {
    int l, t, w, h;

    if(*pStr == 0) {
        *pName = NULL;
        *pl = *pt = *pw = *ph = -1;
        return TRUE;
    }

    LPTSTR pNameStr = pStr;
    pStr = StrChr(pNameStr, TEXT(':'));
    *pStr = 0;
    ++pStr;


    LPTSTR pMid = StrChr(pStr, TEXT(':'));
    if(pMid == NULL) return FALSE;
    *pMid = 0;
    ++pMid;

    LPTSTR pPos = StrChr(pStr, TEXT(','));
    if(pPos != NULL) {
        *pPos = 0;
        ++pPos;
        if(!StrCmp(pStr, TEXT("*"))) {
            l = t = NO_MOVE_MARK;
        } else {
            l = StrToInt(pStr);
            t = StrToInt(pPos);
        }
    } else {
        return FALSE;
    }

    LPTSTR pSiz = StrChr(pMid, TEXT('x'));
    if(pSiz != NULL) {
        *pSiz = 0;
        ++pSiz;
        w = StrToInt(pMid);
        h = StrToInt(pSiz);
    } else {
        return FALSE;
    }

    *pName = pNameStr;
    *pl = l;
    *pt = t;
    *pw = w;
    *ph = h;
    return TRUE;
}

BOOL ParseSettingString(LPCTSTR setting, SettingItem *configlist) {
    LPTSTR pDeli = NULL;
    TCHAR buf[1024];
    LPCTSTR pData = (LPCTSTR)setting;

    for(int i = 0; i< 9; ++i) {
        pDeli = StrChr(pData, TEXT(';'));
        if(pDeli == NULL)
            return FALSE;

        StrCpyN(buf, pData, pDeli-pData+1);
        buf[pDeli-pData] = 0;
        LPTSTR pName;
        int l, t, w, h;
        if(!ParseSizeItem(buf, &pName, &l, &t, &w, &h)) {
            return FALSE;
        }

        StrCpyN(configlist[i].name, pName, sizeof(configlist[i].name));
        configlist[i].rect.left = l;
        configlist[i].rect.top = t;
        configlist[i].rect.right = w;
        configlist[i].rect.bottom = h;

        pData = pDeli + 1;
    }
    return TRUE;
}

BOOL Application::InitSetting() {
    WORD langid = 0;
    DWORD type = 0;
    DWORD regdata = 0;
    DWORD regsize = sizeof(DWORD);
    if( SHGetValue(HKEY_CURRENT_USER, REGKEY, REGVAL_LANGID, &type, &regdata, &regsize) == ERROR_SUCCESS) {
        if(type == REG_DWORD) {
            langid = (WORD)regdata;
        } else {
            SHDeleteValue(HKEY_CURRENT_USER, REGKEY, REGVAL_LANGID);
        }
    }
    SaveLangSetting(langid);


    LPCTSTR pstrval = NULL;
    TCHAR strbuf[1024];
    DWORD strlen = 1024;
    if( SHGetValue(HKEY_CURRENT_USER, REGKEY, REGVAL_SIZELIST, &type, strbuf, &strlen) == ERROR_SUCCESS) {
        if(type == REG_SZ) {
            pstrval = strbuf;
        } else {
            SHDeleteValue(HKEY_CURRENT_USER, REGKEY, REGVAL_SIZELIST);
        }
    }
    if(pstrval == NULL)
        pstrval = REGVAL_SIZELIST_DEF;

    memset(m_sizelist, 0, sizeof(m_sizelist));
    if(!ParseSettingString(pstrval, m_sizelist)) {
        pstrval = REGVAL_SIZELIST_DEF;
        ParseSettingString(pstrval, m_sizelist);
    }

    SavePresetSetting(m_sizelist);

    return TRUE;
}




HWND Application::CreateMainWindow() {
    WNDCLASSEX wcex		= { sizeof(WNDCLASSEX) }; 
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= Main_WndProc;
    wcex.hInstance		= GetModuleHandle(NULL);
    wcex.lpszClassName	= WNDCLSNAME;
    RegisterClassEx(&wcex);

    //return CreateWindowEx(0, WNDCLSNAME, WNDCLSNAME, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, hInstance, 0);
    HWND hwnd = CreateWindowEx(0, WNDCLSNAME, WNDCLSNAME, 0, 0, 0, 0, 0, 0, 0, GetModuleHandle(NULL), 0);
    SetWindowLong(hwnd, GWL_USERDATA, (LONG)this);
    return hwnd;
}


const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

LRESULT CALLBACK Application::Main_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        HANDLE_MSG(hwnd, WM_CREATE, Main_OnCreate);
        HANDLE_MSG(hwnd, WM_HOTKEY, Main_OnHotKey);
        HANDLE_MSG(hwnd, WM_COMMAND, Main_OnCommand);
        HANDLE_MSG(hwnd, SWM_TRAYMSG, Main_OnTrayMsg);
        default:
            // http://channel9.msdn.com/ShowPost.aspx?PostID=65017
            if(message == WM_TASKBARCREATED) {
                ((Application*)GetWindowLong(hwnd, GWL_USERDATA))->trayIcon.RefreshIcon();
            }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}





void Application::Main_OnCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT /*codeNotify*/) {
    Application* pThis = reinterpret_cast<Application*>(GetWindowLong(hwnd, GWL_USERDATA));
    switch (id) {
        case IDM_ENABLE:
            pThis->isEnable = !pThis->isEnable;
            if(pThis->isEnable) {
                RegHotKey(hwnd);
                pThis->trayIcon.ChangeIcon(LOAD_TRAYICON(IDI_TRAY_KEYBOARD));
            } else {
                UnRegHotKey(hwnd);
                pThis->trayIcon.ChangeIcon(LOAD_TRAYICON(IDI_TRAY_DISABLE));
            }
            break;

        case IDM_MODE_MOVE:
            if(pThis->currMode != MODE_MOVE) {
                pThis->currMode = MODE_MOVE;
                pThis->trayIcon.ChangeIcon(LOAD_TRAYICON(IDI_TRAY_KEYBOARD));
            }
            break;
        case IDM_MODE_MOUSE:
            if(pThis->currMode != MODE_MOUSE) {
                pThis->currMode = MODE_MOUSE;
                pThis->trayIcon.ChangeIcon(LOAD_TRAYICON(IDI_TRAY_MOUSE));
            }
            break;
        case IDM_AUTORUN: {
            TCHAR filepath[2][MAX_PATH];
            if( SHGetValue(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), APP_NAME, NULL, NULL, NULL) == ERROR_SUCCESS) {
                SHDeleteValue(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), APP_NAME);
            } else {
                GetModuleFileName(NULL, filepath[0], MAX_PATH);
                PathCanonicalize(filepath[1], filepath[0]);
                SHRegSetPath(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), APP_NAME, filepath[1], 0);
            }
            break;
        }
        case IDM_CONF:
            Configuration::ShowDialog(pThis, &(pThis->res), pThis->m_sizelist);
            break;
        case IDM_ABOUT:
            ShowAbout(hwnd);
            break;
        case IDM_EXIT:
            PostQuitMessage(0);
            break;
    }
}




BOOL Application::SaveLangSetting(WORD langid) {
    if(langid == 0 || !res.IsMatchingLangId(langid)) {
        langid = GetUserDefaultLangID();
        if( !res.IsMatchingLangId(langid)) {
            langid = GetSystemDefaultLangID();
            if( !res.IsMatchingLangId(langid)) {
                langid = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
            }
        }
    }

    res.SetCurrentLangId(langid);

    HKEY hKey;
    if( RegCreateKeyEx(HKEY_CURRENT_USER, REGKEY, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        DWORD data = langid;
        RegSetValueEx(hKey, REGVAL_LANGID, 0, REG_DWORD, (PBYTE)&data, sizeof(DWORD));
        RegCloseKey(hKey);
    }
    return TRUE;
}


BOOL Application::SavePresetSetting(SettingItem *sizelist) {
    if(sizelist == NULL || memcmp(m_sizelist, sizelist, sizeof(m_sizelist)) == 0) {
        return TRUE;
    }

    memcpy(m_sizelist, sizelist, sizeof(m_sizelist));

    TCHAR buf[1024];
    LPTSTR pData = buf;
    for(int i = 0; i< 9; i++) {
        if(sizelist[i].rect.left == NO_MOVE_MARK) {
            pData += wnsprintf(pData, 1024, TEXT("%s:*,*:%dx%d;"), sizelist[i].name, sizelist[i].rect.right, sizelist[i].rect.bottom);
        } else {
            pData += wnsprintf(pData, 1024, TEXT("%s:%d,%d:%dx%d;"), sizelist[i].name, sizelist[i].rect.left, sizelist[i].rect.top, sizelist[i].rect.right, sizelist[i].rect.bottom);
        }
    }

    SHSetValue(HKEY_CURRENT_USER, REGKEY, REGVAL_SIZELIST, REG_SZ, buf, (pData-buf)*sizeof(TCHAR));
    return TRUE;
}
