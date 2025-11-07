#pragma once
#include <Windows.h>
#include <Psapi.h>
#pragma comment (lib, "Psapi.lib")

class Signaturee
{
public:
	static MODULEINFO GetModuleInfo(const char* szModule);
	static DWORD FindPattern(const char* module, const char* pattern, const char* mask);
};