// dllmain.cpp : Defines the entry point for the DLL application.

#include <Windows.h>
#include <iostream>
#include <string>

#include "InitTarget.h"



DWORD WINAPI MainThread(LPVOID lpReserved)
{
    // Création console de débugging
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$", "r", stdin);

    std::cout << "[+] DLL Injectee avec succes !" << std::endl;
    std::cout << "[+] Appuie sur la touche FIN (END) pour ejecter la dll." << std::endl;

    debugValue userDebugValue = PromptDebugValue();

    SetThreadContextExtraction(userDebugValue);


    AddVectoredExceptionHandler(CALL_FIRST, MyVehHandler);

    while (!GetAsyncKeyState(VK_END))
    {


        Sleep(100);
    }


    // Sortie
    std::cout << "fermeture de la dll.." << std::endl;
    Sleep(1000);


    if (f != nullptr)
    {
        fclose(f);
    }

    RemoveVectoredExceptionHandler(MyVehHandler);

    FreeConsole();

    FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
    return 0;
}

// Le point d'entrée de la DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Désactive les appels de thread inutiles pour optimiser
        DisableThreadLibraryCalls(hModule);

        // Lance la fonction MainThread en arrière-plan
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        break;
    }
    return TRUE;
}