// supa kewl see ess goh chit
// sdk is from different places, usually the actual source leak, some oss base, or (rarely) myself
// everything else is mine

//todo
//fix health esp
//improve smextrack
//autowall
//configs
//key control
//dropdown control
//refactor menu fw
//fov for aimbot
//colorpicker
//basic desync
#include <Windows.h>
#include <iostream>
#include "offsets.h"
#include "sdk.h"
#include "globals.h"
#include "modules.h"
#include "utils.h"
#include "hooks.h"
#include "netvar.h"
BOOL WINAPI initCamelia(HMODULE threadHandle)
{
    AllocConsole();
    SetConsoleCtrlHandler(NULL, true);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "built on " << __DATE__ << std::endl;
    std::cout << "reached main loop (wooh!)" << std::endl;
    if (!utils::setup())
        std::cout << "failed in setup" << std::endl;
    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(10);
    }
    std::cout << "reached end :(";
    std::cout << "destructing hooks :(" << std::endl;
    hooks::destructHooks();
    std::cout << "deallocating myself :(" << std::endl;
    std::cout << "bye! come back soon!" << std::endl;
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(threadHandle, 0); // does not actually free library if manual mapped (?)
    return FALSE;
}

BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)initCamelia, handle, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

