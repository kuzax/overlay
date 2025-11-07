#include "cnet.h"
#include "globals.h"
#include "FindSignature.h"
#include "../../xorstr.h"
#include "../../imguis/Imnotif.h"
#include "../../data/elements.h"
#include <console/console.h>


typedef void(__thiscall* GetDxStatus_t)(void* ECX, SDxStatus* outStatus);
GetDxStatus_t oGetDxStatus = nullptr;

typedef unsigned int(__cdecl* GetWMITotalPhysicalMemory_t)();
GetWMITotalPhysicalMemory_t oGetWMITotalPhysicalMemory = nullptr;

void __fastcall hkGetDxStatus(void* ECX, void* EDX, SDxStatus* outStatus)
{
    oGetDxStatus(ECX, outStatus);

    if (element->dxSpoof.bSystemSpoofer)
    {
        outStatus->videoCard.strName = element->dxSpoof.szVideoCardName;
        outStatus->videoCard.iInstalledMemoryKB = element->dxSpoof.iVideoCardRAM;
        outStatus->settings.bWindowed = element->dxSpoof.bWindowedMode;
        outStatus->settings.bAllowScreenUpload = element->dxSpoof.bAllowScreenUpload;
        outStatus->videoMemoryKB.iFreeForMTA = element->dxSpoof.iFreeForMTA;
        outStatus->videoMemoryKB.iUsedByRenderTargets = element->dxSpoof.iUsedByRenderTargets;



    }
}




void c_cnet::release()
{
	Signaturee scan;


	oGetDxStatus = (GetDxStatus_t)scan.FindPattern(xorstr_("core.dll"), xorstr_("\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF1\x89\x75\x00\x8B\x7D\x00\x8B\x86"), xorstr_("xxxx?x????xx????xxx?x????xxxx?xxxxx?xx????xxxx?xx?xx"));
	MH_CreateHook(oGetDxStatus, hkGetDxStatus, reinterpret_cast<LPVOID*>(&oGetDxStatus));



    if (!element->content.loaded_core) {
        element->content.loaded_core = true;

        console->success("content initialized successfully.\n");
    }
    else {
        console->warning("content already initialized. Skipping initialization.\n");
    }	
    
  

}