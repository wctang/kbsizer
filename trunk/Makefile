PROJECT = kbSizer
VERSION = 1.1

APPVER = 5.0
_WIN32_IE = 0x0500


!IFNDEF DEBUG
NODEBUG  = 1
!ENDIF

TARGETOS = WINNT
CHARSET  = /D "_UNICODE" /D "UNICODE"

BUILD_DIR = build
OBJS = /about.obj /kbSizer.obj /TrayIcon.obj /main.obj /LangRes.obj /config.obj /editsubclass.obj /kbSizer.res
RESOBJS = /res.res


!include <win32.mak>
optlibs  = $(optlibs) shell32.lib
cflags   = $(cflags) $(CHARSET)




all: prep $(PROJECT).exe res.dll

release: $(PROJECT)-$(VERSION)-setup.exe $(PROJECT)-$(VERSION).zip 

rebuild: clean all

prep:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR) 

clean:
    @if exist $(BUILD_DIR) rmdir /Q/S $(BUILD_DIR)
	@if exist $(PROJECT) rmdir /Q/S $(PROJECT)
    @if exist *.pdb del *.pdb
    @if exist *.ncb del *.ncb
    @if exist *.suo del /A:H *.suo
    @if exist *.aps del *.aps
    @if exist *.ilk del *.ilk
    @if exist *.user del *.user
    @if exist *.manifest del *.manifest
    @if exist *.exe del *.exe
    @if exist *.dll del *.dll
    @if exist *.zip del *.zip
    @if exist version.h del version.h

$(PROJECT)-$(VERSION)-setup.exe: all
    "C:\Program Files\NSIS\makensis.exe" /V1 /DVERSION=$(VERSION) $(PROJECT).nsi

$(PROJECT)-$(VERSION).zip: all
    mkdir $(PROJECT)
    copy $(PROJECT).exe $(PROJECT)
    copy res.dll $(PROJECT)
    copy changes.txt $(PROJECT)
    "C:\Program Files\WinRAR\WinRAR.exe" a $@ $(PROJECT)

$(PROJECT).exe: version.h $(OBJS:/=build/)
    $(link) $(ldebug) $(guilflags) /machine:x86 $(guilibsmt) $(OBJS:/=build/) -out:$@

res.dll: $(RESOBJS:/=build/)
    $(link) $(ldebug) -dll /NOENTRY /machine:x86 $** -out:$@

version.h:
    echo LPCTSTR VERSION = TEXT("$(VERSION)"); > version.h

.cpp{$(BUILD_DIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvarsmt) $** /Fo$@

$(BUILD_DIR)/kbSizer.res: res.rc
    $(rc) $(rcvars) $(rcflags) /d AFX_RESOURCE_DLL /d AFX_TARG_ENU /fo $@ $**

.rc{$(BUILD_DIR)}.res:
    $(rc) $(rcvars) $(rcflags) /d AFX_RESOURCE_DLL /d AFX_TARG_ENU /d AFX_TARG_CHT /fo $@ $**
