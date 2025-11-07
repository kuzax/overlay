#pragma once
#include <includes.h>
#include <sdk.h>
#include <fstream>
#include <filesystem>
#include <string>

// c_netc sýnýfý tanýmý
class c_netc {
private:
    typedef bool(__thiscall* send_packet_t)(void* ecx, unsigned char ucPacketID, void* bitStream, int packetPriority, int packetReliability, int packetOrdering);
    typedef int(__cdecl* send_report_t)(char arg_list, void* a2, int a3, int a4, int a5);
    typedef void(__thiscall* send_report_2_t)(int ecx, int ArgList, DWORD* a3, int a4, int a5, void* a6);
    typedef HANDLE(__stdcall* driver_send_report_t)(LPCWSTR pszLogFileName, ACCESS_MASK fDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES psaLogFile, ULONG fCreateDisposition, ULONG fFlagsAndAttributes);
    typedef void(__thiscall* client_kick_t)(int ecx, int EAX, unsigned int id, int argument4, DWORD* argument5, int argument6);
    typedef void(__thiscall* local_kick_t)(int ecx, char reason);
    typedef void(__thiscall* client_ban_t)(int ecx, int a2, int* client_id, int* reason, char ban_flags, int time);
    typedef void(__fastcall* SendKickID_t)(int ECX, int EDX, int ID);
    typedef void(__fastcall* SendReportAndKick_t)(void** ECX, DWORD* Packet, int reportID);
    typedef int(__thiscall* stop_network_t)(int ecx);
    typedef const char* (__thiscall* get_connected_server_t)(void* ecx, bool includePort);
    typedef bool(__stdcall* deobfuscate_script_t)(char* cpInBuffer, UINT uiInSize, char** pcpOutBuffer, UINT* puiOutSize, char* szScriptName);

public:
    void* c_net_manager = nullptr;

    send_packet_t o_send_packet;
    send_report_t o_send_report;
    SendKickID_t oSendKickID;
    SendReportAndKick_t oSendReportAndKick;
    send_report_2_t o_send_report_2;
    driver_send_report_t o_driver_send_report;
    client_kick_t o_client_kick;
    local_kick_t o_local_kick;
    client_ban_t o_client_ban;
    stop_network_t o_stop_network;
    deobfuscate_script_t o_deobfuscate_script;
    get_connected_server_t o_get_connected_server;
    bool Send_Script_Packet =false ;    
    bool Ghost_Mode =false ;
    bool release();
    std::string GenerateChecksum();
    char randomHexChar();
    static char spoofBuffer[33];  
    static char originalSerial[33];
    std::string mySerial;
    std::string GenerateRandomSerial();
    bool isValidMD5(const std::string& str);
    void DumpAllScripts();

    bool updated_serial = false;

    char serial_input[33] = "";
    char serial_to_spoof[33] = "";
    
};

inline c_netc* netc = new c_netc();


void generate_new_serial();
std::string GenerateRandomString(size_t length);

std::string utf8_to_cp1251(const char* str);

using Serial_Generate4ByteCheckSum_t = unsigned int(__thiscall*)(int*, std::string*);
extern Serial_Generate4ByteCheckSum_t Serial_Generate4ByteCheckSum_c;

unsigned int __fastcall Serial_Generate4ByteCheckSum(int* ECX, int* EDX, std::string* Checksum);