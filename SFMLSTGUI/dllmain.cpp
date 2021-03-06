#include "dllmain.h"



//------------------------[windows]---------------------
#ifdef _WINDOWS
#include <Windows.h>

HMODULE module_module;

void DLLProcessAttach(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    module_module = hModule;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DLLProcessAttach(hModule, ul_reason_for_call, lpReserved);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#endif
//-----------------------------------------------------------