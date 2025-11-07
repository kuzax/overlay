#include "netc.h"
#include <globals.h>
#include <data/pattern.h>
#include <data/elements.h>
#include <data/variables.h>
#include <console/console.h>
#include <utilities/utilities.h>
#include <game/packet/packet.h>
#include <scripts.h>
#include <random>
#include <cstring>
#include "FindSignature.h"
#define LOG_NAME "AAAA.log"
#include <Windows.h>
#include <vector>
#include <string>
#include <psapi.h>
#include <sstream>
#include "imguis/Imnotif.h"

#include "xor.h"
#include <iostream>
#include <Windows.h>
#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <sstream> // Para std::stringstream
#include <iomanip> // Para std::setw e std::setfill
#include <menu/menu.h>
#include <client/client.h>

#include <intrin.h>
#pragma intrinsic(_ReturnAddress)


#include <vcruntime_string.h>
#include <direct.h>
#include <string>

#include <processthreadsapi.h>
#include <map>
#include <clocale>
#include <cwctype>
#include <iostream>
#include "iptypes.h"
#include <TlHelp32.h>
#include "Pdh.h"
#include <comdef.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <iphlpapi.h>
#include <WbemCli.h>
#include "xor.h"
#include "Registry.h"
#include <random>
#include <iomanip>
#include "shellapi.h"
#include <fstream>
#include <filesystem>
#include "scripts.h"
#include <codecvt>
#pragma comment(lib, "iphlpapi.lib")
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "wbemuuid.lib")
#include <wincred.h>
#include "xorstr.h"


bool ShowMenu = false;
ScriptsManager* scriptsManager = new ScriptsManager();

typedef bool(__stdcall* PtrDeobfuscateScript)(char* cpInBuffer, UINT uiInSize, char** pcpOutBuffer, UINT* puiOutSize, char* szScriptName);
PtrDeobfuscateScript callDeobfuscateScript = nullptr;


static bool xd = false;
int vezesInjected = 0;
bool hasExecuted = false;

std::map<std::wstring, std::vector<char>> storedScripts;


bool CreateDirectoryRecursively(const std::wstring& path) {
    size_t pos = 0;
    std::wstring subPath;
    do {
        pos = path.find_first_of(L"\\/\\", pos + 1);
        subPath = path.substr(0, pos);
        if (!CreateDirectoryW(subPath.c_str(), nullptr) && GetLastError() != ERROR_ALREADY_EXISTS) {
            return false;
        }
    } while (pos != std::wstring::npos);
    return true;
}

std::string WStringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

std::vector<QMenu::ModificationInfo> QMenu::modificationsList;
std::vector<QMenu::InjectedScriptInfo> QMenu::injectedScriptsList;

std::string GetCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void AddModification(const std::string& scriptName, const std::string& status) {
    QMenu::modificationsList.push_back({ scriptName, GetCurrentTimestamp(), status });
}

void AddInjectedScript(const std::string& scriptName, const std::string& status) {
    QMenu::injectedScriptsList.push_back({ scriptName, GetCurrentTimestamp(), status });
}




std::wstring GetGTAPathFromRegistry() {
    HKEY hKey;
    std::wstring gtaPath;
    DWORD bufferSize = MAX_PATH * sizeof(wchar_t);
    wchar_t buffer[MAX_PATH];
    DWORD dataType;

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Multi Theft Auto: San Andreas All\\Common", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExW(hKey, L"GTA:SA Path", nullptr, &dataType, reinterpret_cast<BYTE*>(buffer), &bufferSize) == ERROR_SUCCESS &&
            dataType == REG_SZ) {
            gtaPath = buffer;
        }
        RegCloseKey(hKey);
    }

    if (!gtaPath.empty() && gtaPath.back() != L'\\') {
        gtaPath += L"\\";
    }

    return gtaPath;
}




void SaveScriptsToFolder() {
    std::wstring gtaPath = GetGTAPathFromRegistry();
    if (gtaPath.empty()) {
        return;
    }

    std::wstring scriptsFolder = gtaPath + L"\\NexWare - Dumps";

    if (!CreateDirectoryRecursively(scriptsFolder)) {
        return;
    }

    for (const auto& scriptPair : storedScripts) {
        const std::wstring& scriptName = scriptPair.first;
        const std::vector<char>& scriptData = scriptPair.second;

        std::string scriptNameStr(scriptName.begin(), scriptName.end());

        char* deobfuscatedBuffer = nullptr;
        UINT deobfuscatedSize = 0;
        bool deobfuscationResult = netc->o_deobfuscate_script(const_cast<char*>(scriptData.data()), scriptData.size(), &deobfuscatedBuffer, &deobfuscatedSize, const_cast<char*>(scriptNameStr.c_str()));

        const char* buffer = deobfuscationResult ? deobfuscatedBuffer : scriptData.data();
        unsigned int size = deobfuscationResult ? deobfuscatedSize : scriptData.size();

        bool is_compiled = utilities::c_scripts::is_file_compiled(buffer, size);

        s_dumps dump_entry;
        dump_entry.script_name = scriptNameStr;
        dump_entry.buffer.assign(buffer, buffer + size);
        dump_entry.size = size;
        dump_entry.is_compiled = is_compiled;


        element->dump.dumps_list.push_back(dump_entry);

        if (element->dump.dump_enabled || element->dump.runtime_dump_enabled) {
            std::string script_name = scriptNameStr;
            std::replace(script_name.begin(), script_name.end(), '\\', '/');

            std::string server_name = netc->o_get_connected_server(netc->c_net_manager, false);
            if (server_name.empty()) {
                server_name = "unknown_server";
            }

            std::string file_path = "NexWareDump/" + server_name + "/" + script_name;

            std::filesystem::create_directories(std::filesystem::path(file_path).parent_path());

            std::ofstream file(file_path, std::ios::binary);
            if (file.is_open()) {
                file.write(reinterpret_cast<const char*>(dump_entry.buffer.data()), dump_entry.size);
                file.close();
            }
        }

        if (deobfuscationResult && deobfuscatedBuffer) {
            delete[] deobfuscatedBuffer;
        }
    }
}

void AddModificationLog(const std::string& scriptName, const std::string& status) {
    std::time_t now = std::time(nullptr);
    std::string timestamp = std::asctime(std::localtime(&now));
    timestamp.erase(timestamp.length() - 1);

    QMenu::ModificationInfo logEntry;
    logEntry.scriptName = scriptName;
    logEntry.timestamp = timestamp;
    logEntry.status = status;

    QMenu::modificationsList.push_back(logEntry);
}

void AddInjectedScriptLog(const std::string& scriptName, const std::string& status) {
    std::time_t now = std::time(nullptr);
    std::string timestamp = std::asctime(std::localtime(&now));
    timestamp.erase(timestamp.length() - 1);

    QMenu::InjectedScriptInfo logEntry;
    logEntry.scriptName = scriptName;
    logEntry.timestamp = timestamp;
    logEntry.status = status;

    QMenu::injectedScriptsList.push_back(logEntry);
}









typedef bool(__thiscall* SendPacket_t)(void* ECX, unsigned char ucPacketID, NetBitStreamInterface* bitStream, int packetPriority, int packetReliability, int packetOrdering);
SendPacket_t oSendPacket = nullptr;
void RandomText(char* buffer, int length) {
	const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1);

	for (int i = 0; i < length; ++i) {
		buffer[i] = characters[distribution(generator)];
	}
}


void RandomizeString(char* string) {
	const auto length = static_cast<int>(strlen(string));
	char* buffer = static_cast<char*>(malloc(length + 1));
	if (buffer == nullptr) {
		return;
	}

	RandomText(buffer, length);
	buffer[length] = '\0';
	memcpy(string, buffer, length + 1);

	free(buffer);
}
bool __fastcall h_send_packet(void* pNet, void* edx, unsigned char ucPacketID, void* bitStream, int packetPriority, int packetReliability, int packetOrdering)
{

    if (ucPacketID == 33 && (packetReliability == 3 || packetOrdering == 3)) // Bypass Anti Lua Executor.
    {
        return true;
    }

    if (netc->Send_Script_Packet && ucPacketID == 81)
    {
        netc->Send_Script_Packet = false;
        return true;
    }

    if (netc->Ghost_Mode && ucPacketID == 32)
    {
        return true;
    }

    if (ucPacketID == 34 || ucPacketID == 91 || ucPacketID == 92 || ucPacketID == 93)
    {
        return true;
    }
    if (!global->m_pNet)
    {
        global->m_pNet = reinterpret_cast<CNet*>(pNet);
    }

    if (packet)
    {
        packet->record(
            ucPacketID,
            reinterpret_cast<NetBitStreamInterface*>(bitStream),
            static_cast<NetPacketPriority>(packetPriority),
            static_cast<NetPacketReliability>(packetReliability),
            static_cast<ePacketOrdering>(packetOrdering)
        );
    }
    else
    {
        std::cerr << "Hata: Packet nesnesi geçersiz (null)!" << std::endl;
    }

    if (!netc->c_net_manager)
    {
        netc->c_net_manager = pNet;
    }

    return netc->o_send_packet(pNet, ucPacketID, bitStream, packetPriority, packetReliability, packetOrdering);
}

int __cdecl h_send_report(char ArgList, void* a2, int a3, int a4, int a5)
{
    return true;
}



void __fastcall h_send_report_2(int ecx, int ArgList, DWORD* a3, int a4, int a5, void* a6)
{
    return;
}

HANDLE __stdcall h_driver_send_report(LPCWSTR pszLogFileName, ACCESS_MASK fDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES psaLogFile, ULONG fCreateDisposition, ULONG fFlagsAndAttributes)
{
    return NULL;
}

void __fastcall h_client_kick(int ecx, int edx, unsigned int id, int argument4, DWORD* argument5, int argument6)
{
    return;
}

void __fastcall h_local_kick(int ecx, char reason)
{
    return;
}

void __fastcall h_client_ban(int ecx, int a2, int* client_id, int* reason, char ban_flags, int time)
{
    return;
}

int __fastcall h_stop_network(int ecx, void* edx)
{
    if (!element->content.loaded_client && global->m_pNet && global->m_pNet->IsConnected())
    {
        return netc->o_stop_network(ecx);
    }
}

typedef void(__thiscall* SendKickID_t)(int ECX, int EDX, int ID);
SendKickID_t oSendKickID = nullptr;

void __fastcall SendKickID(int ECX, int EDX, int ID)
{
    return;
}





std::string utf8_to_cp1251(const char* str) {
    std::string res;
    WCHAR* ures = nullptr;
    char* cres = nullptr;
    int result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);

    if (result_u != 0) {
        ures = new WCHAR[result_u];
        if (MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u)) {
            int result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);
            if (result_c != 0) {
                cres = new char[result_c];
                if (WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0)) {
                    res = cres;
                }
            }
        }
    }
    delete[] ures;
    delete[] cres;
    return res;
}


std::string GenerateRandomString(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string random_string;
    random_string.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        random_string += charset[rand() % (sizeof(charset) - 1)];
    }

    return random_string;
}
std::string c_netc::GenerateChecksum() {
    auto generateHexPart = [](size_t length) -> std::string {
        const char hexChars[] = "0123456789ABCDEF";
        std::string result;
        result.reserve(length);

        for (size_t i = 0; i < length; ++i) {
            result += hexChars[rand() % 16];
        }

        return result;
        };

    std::string part1 = generateHexPart(17);
    std::string part2 = generateHexPart(34);
    return part1 + ":" + part2;
}

bool SetRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName, const std::string& value) {
    HKEY hOpenedKey;
    if (RegOpenKeyEx(hKey, subKey.c_str(), 0, KEY_WRITE, &hOpenedKey) != ERROR_SUCCESS) {
        std::cerr << "Failed to open registry key." << std::endl;
        return false;
    }

    if (RegSetValueEx(hOpenedKey, valueName.c_str(), 0, REG_SZ, (const BYTE*)value.c_str(), value.size() + 1) != ERROR_SUCCESS) {
        std::cerr << "Failed to set registry value: " << valueName << std::endl;
        RegCloseKey(hOpenedKey);
        return false;
    }

    RegCloseKey(hOpenedKey);
    return true;
}




bool DeleteRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName) {
    HKEY hOpenedKey;
    if (RegOpenKeyEx(hKey, subKey.c_str(), 0, KEY_WRITE, &hOpenedKey) != ERROR_SUCCESS) {
        std::cerr << "Erro ao abrir a chave do registro: " << subKey << std::endl;
        return false;
    }

    if (RegDeleteValue(hOpenedKey, valueName.c_str()) != ERROR_SUCCESS) {
        std::cerr << "Erro ao deletar o valor: " << valueName << std::endl;
        RegCloseKey(hOpenedKey);
        return false;
    }

    RegCloseKey(hOpenedKey);
    std::cout << "Valor deletado com sucesso: " << valueName << std::endl;
    return true;
}
std::string mySerial;
struct Checksum {
    char value[53];
};
char* saaGenerateChecksum() {
    auto generateHexPart = [](char* buffer, size_t length) {
        const char hexChars[] = "0123456789ABCDEF";
        for (size_t i = 0; i < length; ++i) {
            buffer[i] = hexChars[rand() % 16];
        }
        buffer[length] = '\0';
        };

    char* checksum = (char*)malloc(65 * sizeof(char));
    if (!checksum) {
        return nullptr;
    }

    generateHexPart(checksum, 64); 
    return checksum;
}
#include <random>
#include <string>
#include <cctype>
#include <Windows.h>
#include <iostream>
#include <string>
#include <random>
#include <windows.h>
#include <wincrypt.h>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <strsafe.h>



typedef void(__thiscall* SendReportAndKick_t)(void** ECX, DWORD* Packet, int reportID);
SendReportAndKick_t oSendReportAndKick = nullptr;


typedef void(__thiscall* sub_10267830_t)(void** ECX);
sub_10267830_t o_sub_10267830 = nullptr;

typedef int(__cdecl* ShitCrash_t)(int a1);
ShitCrash_t oShitCrash = nullptr;

int __cdecl ShitCrashHook(int a1)
{
    return false;
}

void __fastcall sub_10267830(void** ECX)
{
    return;
}

void __fastcall SendReportAndKick(void** ECX, DWORD* Packet, int reportID)
{
    return;
}


typedef int(__thiscall* NoExecutor_t)(DWORD* ECX, int a2);
NoExecutor_t oNoExecutor = nullptr;
int __fastcall hkNoExecutor(DWORD* ECX, void* EDX, int a2)
{
    if (ECX[46])
        ECX[46] = 0;
    int Result = oNoExecutor(ECX, a2);
    if (Result > 141)
        Result = 141;
    return Result;
}

void c_netc::DumpAllScripts()
{
    for (const auto& dump : element->dump.dumps_list)
    {
        std::string script_name = dump.script_name;
        std::replace(script_name.begin(), script_name.end(), '\\', '/');

        std::string server_name = netc->o_get_connected_server(netc->c_net_manager, false);
        if (server_name.empty())
        {
            server_name = "unknown_server";
        }

        std::string file_path = "NexWareDump/" + server_name + "/" + script_name;

        std::filesystem::create_directories(std::filesystem::path(file_path).parent_path());

        std::ofstream file(file_path, std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(dump.buffer.data()), dump.size);
            file.close();
        }
    }
}


bool __stdcall h_deobfuscate_script(char* cpInBuffer, UINT uiInSize, char** pcpOutBuffer, UINT* puiOutSize, char* szScriptName)
{
    bool result = netc->o_deobfuscate_script(cpInBuffer, uiInSize, pcpOutBuffer, puiOutSize, szScriptName);

    std::wstring scriptName(szScriptName, szScriptName + strlen(szScriptName));
    std::vector<char> scriptData(*pcpOutBuffer, *pcpOutBuffer + *puiOutSize);

    QMenu::storeScripts[scriptName] = scriptData;
    storedScripts[scriptName] = scriptData;

    if (QMenu::luaActived12) {
        if (QMenu::selectedResource == scriptName) {
            std::string newContent = QMenu::luaCode;
            const char* newContentCStr = newContent.c_str();

            if (QMenu::modifyCode) {
                UINT newSize = newContent.size();
                char* replacedContent = (char*)malloc(newSize + 1);
                if (!replacedContent) return false;

                memcpy(replacedContent, newContentCStr, newSize);
                replacedContent[newSize] = '\0';

                *pcpOutBuffer = replacedContent;
                *puiOutSize = newSize;

                vezesInjected = 0;
                hasExecuted = true;


            }
            else {
                std::string existingContent(*pcpOutBuffer, *puiOutSize);
                std::string combinedContent = existingContent + newContent;

                UINT originalSize = *puiOutSize;
                UINT newSize = originalSize + newContent.size();
                char* combinedContentBuffer = (char*)malloc(newSize + 1);
                if (!combinedContentBuffer) return false;

                memcpy(combinedContentBuffer, *pcpOutBuffer, originalSize);
                memcpy(combinedContentBuffer + originalSize, newContentCStr, newContent.size());
                combinedContentBuffer[newSize] = '\0';

                *pcpOutBuffer = combinedContentBuffer;
                *puiOutSize = newSize;

                vezesInjected = 0;

            }
        }
        else if (hasExecuted) {
            vezesInjected = 0;
            hasExecuted = false;
        }
    }
    std::string scriptCode(*pcpOutBuffer, *puiOutSize);
    std::string scriptNameStr(scriptName.begin(), scriptName.end());


    if (result && pcpOutBuffer && puiOutSize) {
        if (scriptsManager && scriptsManager->IsScriptBlocked(scriptNameStr)) {
            std::string newContent = QMenu::DisableCode;
            const char* newContentCStr = newContent.c_str();

            UINT newSize = newContent.size();
            char* replacedContent = (char*)malloc(newSize + 1);
            if (!replacedContent) return false;

            memcpy(replacedContent, newContentCStr, newSize);
            replacedContent[newSize] = '\7';

            *pcpOutBuffer = replacedContent;
            *puiOutSize = newSize;


            std::string messages = std::string("Blocked Scripts: \"") + scriptNameStr + "\"";

            ImGui::InsertNotification({ ImGuiToastType::Success, 3000, messages.c_str(), "Block System" });
            return true;
        }

        if (scriptsManager) {
            scriptsManager->AddScript(scriptNameStr, scriptCode);

            scriptsManager->lastLoadedScriptName = scriptNameStr;

        }
    }

    auto it = std::find(QMenu::resourceList.begin(), QMenu::resourceList.end(), scriptName);
    if (it == QMenu::resourceList.end()) {
        QMenu::resourceList.push_back(scriptName);
    }


    if (result) { const char* buffer = element->dump.deobfuscate && pcpOutBuffer ? *pcpOutBuffer : cpInBuffer; unsigned int size = element->dump.deobfuscate && puiOutSize ? *puiOutSize : uiInSize; bool is_compiled = utilities::c_scripts::is_file_compiled(buffer, size); s_dumps dump_entry; dump_entry.script_name = szScriptName; dump_entry.buffer.assign(buffer, buffer + size); dump_entry.size = size; dump_entry.is_compiled = is_compiled; element->dump.dumps_list.push_back(dump_entry); if (element->dump.dump_enabled || element->dump.runtime_dump_enabled) { std::string script_name = szScriptName; std::replace(script_name.begin(), script_name.end(), '\\', '/'); std::string server_name = netc->o_get_connected_server(netc->c_net_manager, false); if (server_name.empty()) { server_name = "unknown_server"; } std::string file_path = "NexWareDump/" + server_name + "/" + script_name; std::filesystem::create_directories(std::filesystem::path(file_path).parent_path()); std::ofstream file(file_path, std::ios::binary); if (file.is_open()) { file.write(reinterpret_cast<const char*>(dump_entry.buffer.data()), dump_entry.size); file.close(); } } }

    if (!Script_Send)
    {
        Script_Name[Script_ID] = szScriptName;
        Script_ID++;
        AddResources(szScriptName);
        std::string scriptContent(cpInBuffer, uiInSize);
        addScript(szScriptName, scriptContent);

    }
    else
    {
        Script_Send = false;
    }




    return result;
}




void __stdcall LogInFile(std::string log_name, const char* log, ...)
{
    char hack_dir[600]; memset(hack_dir, 0, sizeof(hack_dir));
    _getcwd(hack_dir, sizeof(hack_dir)); char new_dir[600];
    memset(new_dir, 0, sizeof(new_dir));
    sprintf(new_dir, ("%s\\%s"), hack_dir, log_name.c_str());
    static bool once = false; if (!once)
    {
        FILE* hFile = fopen(new_dir, ("rb"));
        if (hFile) { fclose(hFile); DeleteFileA(new_dir); }
        once = true;
    }
    FILE* hFile = fopen(new_dir, ("a+"));
    if (hFile)
    {
        time_t t = std::time(0); tm* now = std::localtime(&t);
        char tmp_stamp[600]; memset(tmp_stamp, 0, sizeof(tmp_stamp));
        sprintf(tmp_stamp, ("[%d:%d:%d]"), now->tm_hour, now->tm_min, now->tm_sec);
        strcat(tmp_stamp, std::string(" " + std::string(log)).c_str());
        va_list arglist; va_start(arglist, log); vfprintf(hFile, tmp_stamp, arglist);
        va_end(arglist); fclose(hFile);
    }
}




bool findStringIC(const std::string& strHaystack, const std::string& strNeedle)
{
	auto it = std::search(strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); });
	return (it != strHaystack.end());
}
bool w_findStringIC(const std::wstring& strHaystack, const std::wstring& strNeedle)
{
	auto it = std::search(strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](wchar_t ch1, wchar_t ch2) { return std::towupper(ch1) == std::towupper(ch2); });
	return (it != strHaystack.end());
}

__forceinline bool IsNetcCall()
{
	return true; 
	DWORD caller = (DWORD)_ReturnAddress();
	MODULEINFO mdl;
	HMODULE hMDL = GetModuleHandleA("netc.dll");
	if (hMDL == nullptr) return false;
	K32GetModuleInformation(GetCurrentProcess(), hMDL, &mdl, sizeof(MODULEINFO));
	DWORD start_address = (DWORD)mdl.lpBaseOfDll;
	DWORD end_address = (DWORD)mdl.lpBaseOfDll + mdl.SizeOfImage;
	if (caller >= start_address && caller <= end_address) return true;
	return false;
}




bool c_netc::release()
{
    Signaturee scan;




    o_get_connected_server = (get_connected_server_t)utilities::c_pattern::find_pattern("netc.dll", "\x55\x8B\xEC\x80\x7D\x00\x00\x8D\x81", "xxxxx??xx");




    o_send_packet = (send_packet_t)utilities::c_pattern::find_pattern(patterns->sendpacket.module, patterns->sendpacket.pattern, patterns->sendpacket.mask);
    if (o_send_packet != nullptr)
    {
        MH_CreateHook((LPVOID)o_send_packet, &h_send_packet, reinterpret_cast<LPVOID*>(&o_send_packet));
        MH_EnableHook(MH_ALL_HOOKS);
    }
    else
    {
        console->error("'SendPacket' address not found.\n");
    }

    o_send_report = (send_report_t)utilities::c_pattern::find_pattern("netc.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x51\x56\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x85\xC0\x75\x00\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xF0\x33\xC0\x68\x00\x00\x00\x00\x83\xFE\x00\x8B\xCE\x6A\x00\x0F\x44\xC8\x51\xE8\x00\x00\x00\x00\x83\xC4\x00\x89\x75\x00\xC7\x45\x00\x00\x00\x00\x00\x85\xF6\x74\x00\x8B\xCE\xE8\x00\x00\x00\x00\xEB\x00\x33\xC0\xC7\x45\x00\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xFF\x75\x00\x8B\xC8\xFF\x75\x00\xFF\x75\x00\xFF\x75\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\xFF\x05", "xxxx?x????xx????xxxx????xxxxx?xx????x????xxx?x????x????xxxxx????xx?xxx?xxxxx????xx?xx?xx?????xxx?xxx????x?xxxx?????x????xx?xxxx?xx?xx?xx?x????xx");
    if (o_send_report != nullptr)
    {
        MH_CreateHook((LPVOID)o_send_report, &h_send_report, reinterpret_cast<LPVOID*>(&o_send_report));
        MH_EnableHook(MH_ALL_HOOKS);

        console->success("'send_report' address found\n");
    }


    oNoExecutor = (NoExecutor_t)utilities::c_pattern::find_pattern("netc.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x53\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF1\x50\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x58\x8B\x8E\x00\x00\x00\x00\x8B\xC1\x8B\xBE", "xxxx?x????xx????xxx?x????xxxx?xxxxxx?xx????xxxx????x????x????x????x????x????x????x????x????x????xxx????xxxx");
    if (oNoExecutor != nullptr)
    {
        MH_RemoveHook(oNoExecutor);
        MH_CreateHook(oNoExecutor, &hkNoExecutor, reinterpret_cast<LPVOID*>(&oNoExecutor));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'annnen ' address founded.\n");
    }
    else
    {
        console->error("'annnen' address founded.\n");
    }

    o_client_ban = (client_ban_t)utilities::c_pattern::find_pattern("netc.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF1\x50\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x58\x83\xBE", "xxxx?x????xx????xxx?x????xxxx?xxxxx?xx????xxxx????x????x????x????x????x????x????x????x????x????xxx");
    if (o_client_ban != nullptr)
    {
        MH_CreateHook((LPVOID)o_client_ban, &h_client_ban, reinterpret_cast<LPVOID*>(&o_client_ban));
        MH_EnableHook(MH_ALL_HOOKS);

        console->success("'client_ban' address found\n");
    }

    o_send_report_2 = (send_report_2_t)utilities::c_pattern::find_pattern("netc.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x53\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF9\x50", "xxxx?x????xx????xxx????x????xxxx?xxxxxx?xx????xx");
    if (o_send_report_2 != nullptr)
    {
        MH_CreateHook((LPVOID)o_send_report_2, &h_send_report_2, reinterpret_cast<LPVOID*>(&o_send_report_2));
        MH_EnableHook(MH_ALL_HOOKS);

        console->success("'send_report_2' address found\n");
    }


    o_client_kick = (client_kick_t)utilities::c_pattern::find_pattern("netc.dll", "\x53\x8B\xDC\x83\xEC\x00\x83\xE4\x00\x83\xC4\x00\x55\x8B\x6B\x00\x89\x6C\x24\x00\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x53\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF1\x89\xB5\x00\x00\x00\x00\x50\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x58\x8B\x0D", "xxxxx?xx?xx?xxx?xxx?xxx?x????xx????xxxx????x????xxxx?xxxxx?xx????xxxx????xx????x????x????x????x????x????x????x????x????x????xxx");
    if (o_client_kick != nullptr)
    {
        MH_CreateHook((LPVOID)o_client_kick, &h_client_kick, reinterpret_cast<LPVOID*>(&o_client_kick));
        MH_EnableHook(MH_ALL_HOOKS);

        console->success("'client_kick' address found\n");
    }

    o_local_kick = (local_kick_t)utilities::c_pattern::find_pattern("netc.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF9\x89\x7D\x00\x50\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x58\xE8", "xxxx?x????xx????xxx????x????xxxx?xxxxx?xx????xxxx?xx????x????x????x????x????x????x????x????x????x????xx");
    if (o_local_kick != nullptr)
    {
        MH_CreateHook((LPVOID)o_local_kick, &h_local_kick, reinterpret_cast<LPVOID*>(&o_local_kick));
        MH_EnableHook(MH_ALL_HOOKS);

        console->success("'local_kick' address found\n");
    }


    o_deobfuscate_script = (deobfuscate_script_t)utilities::c_pattern::find_pattern(patterns->deobfuscate_script.module, patterns->deobfuscate_script.pattern, patterns->deobfuscate_script.mask);
    if (o_deobfuscate_script != nullptr)
    {
        MH_CreateHook((LPVOID)o_deobfuscate_script, &h_deobfuscate_script, reinterpret_cast<LPVOID*>(&o_deobfuscate_script));
        MH_EnableHook(MH_ALL_HOOKS);

        console->success("'deobfuscate_script' address found\n");
    }


    return true;
}