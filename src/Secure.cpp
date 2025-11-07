#include "includes.h"
#include "Secure.h"

CSecure* pSecure = new CSecure();

void CSecure::Add(DWORD dwAddress, BYTE byteSize)
{
	for (auto i = 0; i < byteSize; i++)
	{
		if (isAddressSecured(dwAddress + i) == -1)
		{
			g_Memory.dwAddress = dwAddress + i;
			g_Memory.origByte = *(BYTE*)(dwAddress + i);
			vecMemory.push_back(g_Memory);
		}
	}
}

int CSecure::isAddressSecured(DWORD dwAddress)
{
	for (size_t i = 0; i < vecMemory.size(); i++)
	{
		if (vecMemory[i].dwAddress == dwAddress)
			return i;
	}
	return -1;
}

void CSecure::memcpy_safe(void* _dest, const void* _src, uint32_t len)
{
	Add((DWORD)_dest, len);
	MemoryXD::memcpy_safe(_dest, _src, len);
}

void CSecure::HookInstallCall(DWORD dwInstallAddress, DWORD dwHookFunction)
{
	Add(dwInstallAddress, 5);
	MemoryXD::HookInstallCall(dwInstallAddress, dwHookFunction);
}

void CSecure::CreateJump(BYTE* pAddress, DWORD dwJumpTo, DWORD dwLen)
{
	Add((DWORD)pAddress, (BYTE)dwLen);
	MemoryXD::CreateJump(pAddress, dwJumpTo, dwLen);
}