// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <iostream>
#include "Memory/Memory.h"
#include "libSMMWE.h"


DWORD WINAPI ModThread(HMODULE hModule)
{

    // Register some SMM:WE functions
    SMMWE::RegisterMethods(mem::gameBase + 0xABDA40, mem::gameBase + 0xEEE090, mem::gameBase + 0xF06CE0, mem::gameBase + 0xEF1530, mem::gameBase + 0x1460);

    // Hook into SMM:WE
    SMMWE::Hook();

    // Keep running until the game fully loaded the texture
    while (SMMWE::running) {}
    SMMWE::RejectHook();

    // Exit
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ModThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

