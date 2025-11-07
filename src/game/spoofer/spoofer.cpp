#include "spoofer.h"
#include "serialhook.h"
#include "globals.h"
#include "../../xorstr.h"
#include "../../imguis/Imnotif.h"
#include "../../data/elements.h"
#include <comdef.h>
#include <psapi.h>
#include <strsafe.h>
#include <wincred.h>
#include "FindSignature.h"
#include <console/console.h>
#include <wincred.h>

void* vMainRuntimeSpooferECX = nullptr;
bool bSpoofing = false;

BOOL RegDelnodeRecurse(HKEY hKeyRoot, LPSTR lpSubKey)
{
    LPSTR    lpEnd;
    LONG     lResult;
    DWORD    dwSize;
    CHAR     szName[MAX_PATH];
    HKEY     hKey;
    FILETIME ftWrite;
    lResult = RegDeleteKeyA(hKeyRoot, lpSubKey);
    if (lResult == ERROR_SUCCESS)
        return TRUE;
    lResult = RegOpenKeyExA(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
    if (lResult != ERROR_SUCCESS)
    {
        if (lResult == ERROR_FILE_NOT_FOUND)
            return TRUE;
        else
            return FALSE;
    }
    lpEnd = lpSubKey + lstrlenA(lpSubKey);
    if (*(lpEnd - 1) != TEXT('\\'))
    {
        *lpEnd = TEXT('\\');
        lpEnd++;
        *lpEnd = TEXT('\0');
    }
    dwSize = MAX_PATH;
    lResult = RegEnumKeyExA(hKey, 0, szName, &dwSize, NULL, NULL, NULL, &ftWrite);
    if (lResult == ERROR_SUCCESS)
    {
        do
        {
            *lpEnd = TEXT('\0');
            StringCchCatA(lpSubKey, MAX_PATH * 2, szName);
            if (!RegDelnodeRecurse(hKeyRoot, lpSubKey))
                break;
            dwSize = MAX_PATH;
            lResult = RegEnumKeyExA(hKey, 0, szName, &dwSize, NULL, NULL, NULL, &ftWrite);
        } while (lResult == ERROR_SUCCESS);
    }
    lpEnd--;
    *lpEnd = TEXT('\0');
    RegCloseKey(hKey);
    lResult = RegDeleteKeyA(hKeyRoot, lpSubKey);
    if (lResult == ERROR_SUCCESS)
        return TRUE;
    return FALSE;
}
BOOL RegDelnode(HKEY hKeyRoot, LPCSTR lpSubKey)
{
    CHAR szDelKey[MAX_PATH * 2];
    StringCchCopyA(szDelKey, MAX_PATH * 2, lpSubKey);
    return RegDelnodeRecurse(hKeyRoot, szDelKey);
}

//Netc.dll functions

char* EncryptSerial(char* szIn)
{
    char* szSerial = new char[64];
    memset(szSerial, 0, 33);
    strcpy(szSerial, szIn);

    int i = 0;
    do
    {
        szSerial[i] = ((i ^ szSerial[i] ^ ((144 - 36 * (i & 3)) / 5) ^ 0x13 ^ (1 << (i & 7))) & 0x7F) | 0x80;
        i++;
    } while (i < 32);

    return szSerial;
}

void DecryptSerial(char* szIn)
{
    char szData[128];
    memset(szData, 0, 128);
    strcpy(szData, szIn);

    int         i = 0;
    std::string strIn(szIn, 33);
    std::string strOut = "";

    do
    {
        szData[i] = (i ^ szData[i] ^ ((144 - 36 * (i & 3)) / 5) ^ 0x13 ^ (1 << (i & 7))) & 0x7F;
        i++;
    } while (i < 32);
}

typedef int(__thiscall* GetLocalIP_t)(DWORD** ECX);
GetLocalIP_t oGetLocalIP = nullptr;
int __fastcall hkGetLocalIP(DWORD** ECX, void* EDX)
{
    int Result = oGetLocalIP(ECX);
    if (bSpoofing)
    {
        int RandomValue = -1000 + std::rand() % (2000 + 1);
        int RandomIP = Result + RandomValue;
        return RandomIP;
    }
    return Result;
}

typedef void(__thiscall* MainRuntimeSpoofer_t)(void* ECX);
MainRuntimeSpoofer_t oMainRuntimeSpoofer = nullptr;
void __fastcall hkMainRuntimeSpoofer(void* ECX, void* EDX)
{
    static bool bOnce = false;
    if (!bOnce)
    {
        bOnce = true;
        vMainRuntimeSpooferECX = ECX;
    }
    return oMainRuntimeSpoofer(ECX);
}

typedef void** (__cdecl* GenerateNT2Public_t)(void* a1, int a2, int* a3, int a4);
GenerateNT2Public_t oGenerateNT2Public = nullptr;
typedef void** (__cdecl* GenerateNTPublic_t)(void* a1, int a2, int* a3, int a4);
GenerateNTPublic_t oGenerateNTPublic = nullptr;

typedef char(__cdecl* GenerateCheckSum_t)(SString* a1);
GenerateCheckSum_t oGenerateCheckSum = nullptr;

char* szTempSerial = new char[64];
void** __cdecl hkGenerateNT2Public(void* a1, int a2, int* a3, int a4)
{
    if (strlen(szTempSerial) == 0 || !bSpoofing)
        return oGenerateNT2Public(a1, a2, a3, a4);

    void** pData = oGenerateNT2Public(a1, a2, a3, a4);

    char* szData = (char*)*pData;

    DecryptSerial(szData);
    strncpy(szData, EncryptSerial(szTempSerial), 32);
    SStringX strCheckSum(szData, 32);
    oGenerateCheckSum(&strCheckSum);
    strncpy(szData, strCheckSum.data(), 36);
    return pData;
}

void** __cdecl hkGenerateNTPublic(void* a1, int a2, int* a3, int a4)
{
    if (strlen(szTempSerial) == 0 || !bSpoofing)
    {
        return oGenerateNTPublic(a1, a2, a3, a4);
    }

    char* szSerial = new char[128];
    memset(szSerial, 0, 128);
    strcpy(szSerial, szTempSerial);
    void** pData = oGenerateNTPublic(a1, a2, a3, a4);
    char* szData = (char*)*pData;

    strncpy(szData, szTempSerial, 32);
    return pData;
}

void Spoofer::CleanBullshits()
{
    CredDelete(xorstr_("SSO_POP_DEVICE"), CRED_TYPE_GENERIC, 0);
    CredDelete(xorstr_("SSO_RND_DEVICE"), CRED_TYPE_GENERIC, 0);

    RegDelnode(HKEY_CURRENT_USER, xorstr_("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID2"));
    RegDelnode(HKEY_CURRENT_USER, xorstr_("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections"));
}

DWORD* life_ecx = nullptr;
int life_edx = 0;

typedef int(__fastcall* tGenerateNT2Public)(DWORD* a1, int a2);
tGenerateNT2Public cGenerateNT2Public = nullptr;

int __fastcall spoofrealtime(DWORD* ecx, int edx)
{
    life_ecx = ecx;
    life_edx = edx;

    if (!cGenerateNT2Public)
        return 0;

    return cGenerateNT2Public(ecx, edx);
}

bool bSerialChanged = false; // global veya class member olabilir
char szLastSpoofedSerial[64] = { 0 }; 

void Spoofer::ChangeSerial(int iMethod)
{
    if (GetModuleHandleA("client.dll"))
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "You need to disconnect from server!", "Error!" });
        return;
    }

    if (iMethod == 1 && strlen(element->spoofer.szCustomSerial) != 32)
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "Serial length must be 32!", "Error!" });
        return;
    }

    if (oGetLocalIP == nullptr || oGenerateNT2Public == nullptr || oGenerateNTPublic == nullptr || oGenerateCheckSum == nullptr )
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "Spoofer couldnt initialize. Please contact support!", "Error!" });
        return;
    }

    CleanBullshits();
    memset(szTempSerial, 0, sizeof(szTempSerial));

    bSpoofing = true;

    switch (iMethod)
    {
    case 1: // Custom Serial
        strcpy(szTempSerial, element->spoofer.szCustomSerial);
        break;

    case 2: // Random Serial
    {
        const char* randomSerial = GetRandomSerial();
        strcpy(szTempSerial, randomSerial);
        break;
    }
    }

    spoofrealtime(life_ecx, life_edx);
    bSpoofing = false;

    if (strlen(szTempSerial) > 0 && strcmp(element->spoofer.szLastSpoofedSerial, szTempSerial) != 0)
    {
        strcpy(element->spoofer.szLastSpoofedSerial, szTempSerial);
        element->spoofer.isSerialChanged = true;

        ImGui::InsertNotification({ ImGuiToastType::Success, 3000, "Your Serial has been successfully changed.", "Success!" });
    }
    else
    {
        ImGui::InsertNotification({ ImGuiToastType::Warning, 3000, "Serial has not changed.", "Warning!" });
    }

    memset(szTempSerial, 0, sizeof(szTempSerial));
}

void Spoofer::InstallHooks()
{
    Signaturee scan;


    cGenerateNT2Public = (tGenerateNT2Public)scan.FindPattern(xorstr_("netc.dll"), xorstr_("\x53\x8B\xDC\x83\xEC\x00\x83\xE4\x00\x83\xC4\x00\x55\x8B\x6B\x00\x89\x6C\x24\x00\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x53\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x89\x8D\x00\x00\x00\x00\xC7\x85"), xorstr_("xxxxx?xx?xx?xxx?xxx?xxx?x????xx????xxxx????x????xxxx?xxxxx?xx????xx????xx"));
    if (cGenerateNT2Public != nullptr)
    {
        MH_CreateHook((LPVOID)cGenerateNT2Public, &spoofrealtime, reinterpret_cast<LPVOID*>(&cGenerateNT2Public));
        MH_EnableHook(MH_ALL_HOOKS);

        console->success(xorstr_("'spoofrealtime' address found\n"));
    }

    oGetLocalIP = (GetLocalIP_t)scan.FindPattern(xorstr_("netc.dll"), xorstr_("\x8B\x49\x00\x6A\x00\x8B\x01"), xorstr_("xx?x?xx"));
    if (oGetLocalIP != nullptr)
    {
        MH_CreateHook(oGetLocalIP, hkGetLocalIP, reinterpret_cast<LPVOID*>(&oGetLocalIP));
        console->success("'oGetLocalIP' address founded.\n");
    }
    else
    {
        console->error("'oGetLocalIP' address not founded.\n");
    }




    oGenerateCheckSum = (GenerateCheckSum_t)scan.FindPattern(xorstr_("netc.dll"), xorstr_("\x55\x8B\xEC\x53\x56\x57\x8B\x7D\x00\x33\xF6\x32\xDB"), xorstr_("xxxxxxxx?xxxx"));
    if (oGenerateCheckSum != nullptr)
    {
        console->success("'oGenerateCheckSum' address founded.\n");
    }
    else
    {
        console->error("'oGenerateCheckSum' address not founded.\n");
    }

    oGenerateNT2Public = (GenerateNT2Public_t)scan.FindPattern(xorstr_("netc.dll"), xorstr_("\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\x45\x00\x0F\x57\xC0\x8B\x7D\x00\x33\xF6\x89\x85\x00\x00\x00\x00\x0F\x11\x45\x00\x89\x85\x00\x00\x00\x00\x89\xBD\x00\x00\x00\x00\x89\x75\x00\xC7\x45\x00\x00\x00\x00\x00\xC6\x45\x00\x00\x8B\x4F\x00\x33\xD2\x8B\x3F\x8B\xC1\x89\x75\x00\x89\x95\x00\x00\x00\x00\x2B\xC7\x0F\x84\x00\x00\x00\x00\x2B\xCF\x3B\xD1\x73\x00\x0F\xB6\xF2\xB8\x00\x00\x00\x00\x8B\xCE\x83\xE1\x00\x2B\xC1\x69\xC8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xF7\xE9\x8B\x85\x00\x00\x00\x00\xD1\xFA\x8B\xCA\xC1\xE9\x00\x0F\xBE\x04\x07\x03\xCA\x33\xC8\x33\xCE\x83\xE6\x00\x83\xF1\x00\x0F\xBB\xF1\xEB\x00\x32\xC9\x0F\xB6\xF1\x56\xE8\x00\x00\x00\x00\x83\xC4\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x8A\x8E\x00\x00\x00\x00\x8B\x7D"), xorstr_("xxxx?x????xx????xxx????x????xxxx?xxxxx?xx????xx?xxxxx?xxxx????xxx?xx????xx????xx?xx?????xx??xx?xxxxxxxx?xx????xxxx????xxxxx?xxxx????xxxx?xxxx????x????xxxx????xxxxxx?xxxxxxxxxxxx?xx?xxxx?xxxxxxx????xx?xxxx????xx????xx"));
    if (oGenerateNT2Public != nullptr)
    {
        MH_CreateHook(oGenerateNT2Public, hkGenerateNT2Public, reinterpret_cast<LPVOID*>(&oGenerateNT2Public));
        console->success("'oGenerateNT2Public' address founded.\n");
    }
    else
    {
        console->error("'oGenerateNT2Public' address not founded.\n");
    }

    oGenerateNTPublic = (GenerateNTPublic_t)scan.FindPattern(xorstr_("netc.dll"), xorstr_("\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\x45\x00\x0F\x57\xC0\x8B\x7D\x00\x33\xF6\x89\x85\x00\x00\x00\x00\x0F\x11\x45\x00\x89\x85\x00\x00\x00\x00\x89\xBD\x00\x00\x00\x00\x89\x75\x00\xC7\x45\x00\x00\x00\x00\x00\xC6\x45\x00\x00\x8B\x4F\x00\x33\xD2\x8B\x3F\x8B\xC1\x89\x75\x00\x89\x95\x00\x00\x00\x00\x2B\xC7\x0F\x84\x00\x00\x00\x00\x2B\xCF\x3B\xD1\x73\x00\x0F\xB6\xF2\xB8\x00\x00\x00\x00\x8B\xCE\x83\xE1\x00\x2B\xC1\x69\xC8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xF7\xE9\x8B\x85\x00\x00\x00\x00\xD1\xFA\x8B\xCA\xC1\xE9\x00\x0F\xBE\x04\x07\x03\xCA\x33\xC8\x33\xCE\x83\xE6\x00\x83\xF1\x00\x0F\xBB\xF1\xEB\x00\x32\xC9\x0F\xB6\xF1\x56\xE8\x00\x00\x00\x00\x83\xC4\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x8A\x8E\x00\x00\x00\x00\x8B\x75"), xorstr_("xxxx?x????xx????xxx????x????xxxx?xxxxx?xx????xx?xxxxx?xxxx????xxx?xx????xx????xx?xx?????xx??xx?xxxxxxxx?xx????xxxx????xxxxx?xxxx????xxxx?xxxx????x????xxxx????xxxxxx?xxxxxxxxxxxx?xx?xxxx?xxxxxxx????xx?xxxx????xx????xx"));
    if (oGenerateNTPublic != nullptr)
    {
        MH_CreateHook(oGenerateNTPublic, hkGenerateNTPublic, reinterpret_cast<LPVOID*>(&oGenerateNTPublic));
        console->success("'oGenerateNTPublic' address founded.\n");
    }
    else
    {
        console->error("'oGenerateNTPublic' address not founded.\n");
    }
}




