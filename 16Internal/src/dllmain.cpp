#define NOMINMAX
#include <windows.h>
#include <iostream>
#include "utils/utils.hpp"
#include "hooks/hooks.h"

DWORD WINAPI OnDllAttach(HMODULE hModule)
{
    using namespace std;

    Utils::AttachConsole();

    Hooks::Init();

    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        Sleep(150);
    }

    Hooks::Unhook();
    Sleep(100);
    Utils::DetachConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)OnDllAttach, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}