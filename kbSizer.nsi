; Need Process plugin

!define NAME       "kbSizer"
!ifndef VERSION
!define VERSION    "0.1"
!endif
!define HELPPAGE   "http://sourceforge.net/docman/index.php?group_id=164820"

!include "MUI.nsh"

Name "${NAME}"

OutFile ${NAME}-${VERSION}-setup.exe
InstallDir $PROGRAMFILES\${NAME}
InstallDirRegKey HKLM SOFTWARE\${NAME} ""

XPStyle on
SetCompressor /FINAL lzma

;--------------------------------
;Interface Configuration
;--------------------------------
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP   "${NSISDIR}\Contrib\Graphics\Header\orange.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall.bmp"
!define MUI_ABORTWARNING

;--------------------------------
; Pages
;--------------------------------
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define      MUI_FINISHPAGE_RUN $INSTDIR\${NAME}.exe
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
; Languages
;--------------------------------
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "TradChinese"


;--------------------------------
; install
;--------------------------------
Function .OnInit
    Processes::KillProcess ${NAME}.exe
FunctionEnd

Section "Install"
    SetOutPath $INSTDIR

    File ${NAME}.exe
    File res.dll
    File changes.txt

    WriteRegStr HKLM SOFTWARE\${NAME} Install_Dir $INSTDIR

    WriteRegStr   HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${NAME} DisplayName ${NAME}
    WriteRegStr   HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${NAME} UninstallString '"$INSTDIR\Uninstall.exe"'
    WriteRegDWORD HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${NAME} NoModify 1
    WriteRegDWORD HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${NAME} NoRepair 1
    WriteUninstaller $INSTDIR\Uninstall.exe
SectionEnd

Section "Start Menu Shortcuts"
	SetShellVarContext all
    CreateDirectory $SMPROGRAMS\${NAME}
    CreateShortCut  $SMPROGRAMS\${NAME}\${NAME}.lnk $INSTDIR\${NAME}.exe
    CreateShortCut  "$SMPROGRAMS\${NAME}\Uninstall ${NAME}.lnk" $INSTDIR\Uninstall.exe
SectionEnd




;--------------------------------
; Uninstaller
;--------------------------------
Function un.OnInit
    Processes::KillProcess ${NAME}.exe
FunctionEnd

Section "Uninstall"
    DeleteRegKey   HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}
    DeleteRegValue HKCU SOFTWARE\Microsoft\Windows\CurrentVersion\Run ${NAME}
    DeleteRegKey   HKLM SOFTWARE\${NAME}

	SetShellVarContext all
    Delete $SMPROGRAMS\${NAME}\*.*
    RMDir  $SMPROGRAMS\${NAME}

    Delete $INSTDIR\*.*
    RMDir  $INSTDIR
SectionEnd

