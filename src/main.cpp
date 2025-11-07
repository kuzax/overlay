#include <includes.h>
#include <menu/menu.h>
#include <netc/netc.h>
#include <client/client.h>
#include <game/cnet/cnet.h>
#include <SlientAim.h>
#include <crosshair.h>
#include <data/variables.h>
#include <console/console.h>
#include <data/elements.h>
#include <windows.h>
#include <winhttp.h>
#include <thread>
#include <wininet.h> 

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "wininet.lib")

#include <game/spoofer/spoofer.h>
#include "xorstr.h"
#include <Psapi.h>
#include <winternl.h>
#include <cwctype>

bool wFindString(const std::wstring& strHaystack, const std::wstring& strNeedle)
{
    auto it = std::search(strHaystack.begin(), strHaystack.end(), strNeedle.begin(), strNeedle.end(),
        [](wchar_t ch1, wchar_t ch2) { return std::towupper(ch1) == std::towupper(ch2); });
    return (it != strHaystack.end());
}

typedef NTSTATUS(__stdcall* LdrLoadDll_t)(PWCHAR PathToFile, ULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle);
LdrLoadDll_t oLdrLoadDll = nullptr;

NTSTATUS __stdcall hkLdrLoadDll(PWCHAR PathToFile, ULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle)
{
    NTSTATUS result = oLdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);
    std::wstring wstrModuleFileName = std::wstring(ModuleFileName->Buffer, ModuleFileName->Length);
    if (wFindString(wstrModuleFileName, xorstr_(L"netc.dll")))
    {
        static bool bOnceAC = false;
        if (!bOnceAC)
        {
            bOnceAC = true;
            Spoofer::InstallHooks();
        }
    }
    return result;
}

void Hook_Spinbot()
{
    unsigned long VAR_SystemState;
    while (true)
    {
        Sleep(100);

        VAR_SystemState = *(unsigned long*)0xC8D4C0;

        switch (VAR_SystemState)
        {
        case 9:
            if (!pSpinbot) {
                try {
                    pSpinbot = new Spinbot();
                    console->success("Spinbot initialized");
                }
                catch (...) {
                    console->error("Failed to initialize Spinbot");
                }
            }

            if (!pCustomCrosshair) {
                try {
                    pCustomCrosshair = new CustomCrosshair();
                    console->success("Custom Crosshair initialized");
                }
                catch (...) {
                    console->error("Failed to initialize Custom Crosshair");
                }
            }
            break;
        }
    }
}

void Initialize()
{
    console->initialize();

    var->winapi.mh_status = MH_Initialize();
    if (var->winapi.mh_status != MH_OK)
    {
        console->error("Failed to initialize MinHook: %d", var->winapi.mh_status);
        return;
    }

    oLdrLoadDll = (LdrLoadDll_t)GetProcAddress(GetModuleHandleA(xorstr_("ntdll.dll")), xorstr_("LdrLoadDll"));
    MH_RemoveHook(oLdrLoadDll);
    MH_CreateHook(oLdrLoadDll, &hkLdrLoadDll, reinterpret_cast<LPVOID*>(&oLdrLoadDll));
    MH_EnableHook(MH_ALL_HOOKS);

    if (!netc->release())
    {
        console->error("Failed to initialize network hooks\n");
        return;
    }

    if (!menu->release())
    {
        console->error("Failed to initialize DirectX hooks\n");
        return;
    }

    // Spinbot ve Crosshair thread'i baþlat
    std::thread(Hook_Spinbot).detach();

    std::thread([&]()
        {
            while (true)
            {
                HMODULE hCnet = GetModuleHandleA("core.dll");
                if (hCnet != nullptr)
                {
                    if (!element->content.loaded_core)
                    {
                        cnet->release();
                    }
                }
            }
        }
    ).detach();

    std::thread([&]()
        {
            while (true)
            {
                HMODULE hClient = GetModuleHandleA("client.dll");
                if (hClient != nullptr)
                {
                    if (!element->content.loaded_client)
                    {
                        client->release();
                    }
                }
                else
                {
                    if (element->content.loaded_client)
                    {
                        client->shutdown();
                    }
                }
            }
        }
    ).detach();
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        var->winapi.hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Initialize, nullptr, 0, nullptr);
        break;
    }
    case DLL_PROCESS_DETACH:
    {
        if (pSpinbot) {
            delete pSpinbot;
            pSpinbot = nullptr;
        }

        if (pCustomCrosshair) {
            delete pCustomCrosshair;
            pCustomCrosshair = nullptr;
        }
        break;
    }
    }
    return TRUE;
}