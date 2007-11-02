#include "kbSizer.h"

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "comctl32")



int APIENTRY _tWinMain(HINSTANCE /*hInst*/, HINSTANCE, LPTSTR /*lpCmdLine*/, int /*nCmdShow*/) {
    const LPTSTR MUTEX_NAME = TEXT("_KBSIZE_");
	CreateMutex(NULL, TRUE, MUTEX_NAME);
	if(GetLastError() == ERROR_ALREADY_EXISTS) {
		return 0;
	}


    Application application;
    
    return application.Run();
}
