#pragma once
#include "includes.h"
#define SIG_OFFSET 0xC00

struct Signature {
    const char* moduleName;
    const char* name;
    const char* signature;
    const char* mask;
    DWORD address;
};

class Module {
public:
    Module()
    {
        baseAddr = 0;
        size = 0;
    }

    Module(DWORD baseAd, DWORD sz)
    {
        baseAddr = baseAd;
        size = sz;
    }

    DWORD baseAddr;
    DWORD size;
};

class Memory
{
private:
    static std::vector<Signature*> signatures;

public:
    static Module* GetModule(const char* module_name);

    static int GetBaseAddress() {
        static int addr = reinterpret_cast<int>(GetModuleHandleA(NULL));
        return addr;
    }

    static int GetGlobalAddress(int address) {
        return GetBaseAddress() - 0x400000 + address;
    }

    static const int GetExternalAddress(const char* processName, int shift, int address) {
        int addr = reinterpret_cast<int>(GetModuleHandleA(processName));
        return (GetBaseAddress() - 0x400000) + (addr - shift + address);
    }


    // Gets the virtual method table from the object @self
    static inline void** GetVMT(const void* self) {
        return *(void***)(self);
    }

    // Gets the virtual method from @self in the table index @index 
    static inline void* GetVMT(const void* self, size_t index) {
        return GetVMT(self)[index];
    }

    static bool ReadVirtualMemory(ULONG ReadAddress, void* buffer, UINT ReadSize)
    {
        HMODULE hModule = GetModuleHandle("client.dll");
        if (hModule == NULL) return FALSE;

        SIZE_T bytesRead;
        ReadProcessMemory(hModule, (LPCVOID)ReadAddress, buffer, ReadSize, &bytesRead);

        return bytesRead > 0;
    }

    template <class T>
    static T Read(ULONG addr) {
        T _read;
        ReadVirtualMemory(addr, &_read, sizeof(T));
        return _read;
    }

    template <typename... Args>
    static void Call(unsigned int address, Args... args) {
        reinterpret_cast<void(__cdecl*)(Args...)>(address)(args...);
    }

    template <typename... Args>
    static void CallModule(unsigned int address, Args... args) {
        HMODULE hModule = GetModuleHandle("client.dll");
        if (hModule == NULL) return;
        reinterpret_cast<void(__cdecl*)(Args...)>(reinterpret_cast<uintptr_t>(hModule) + address)(args...);
    }

    template <typename... Args>
    static void CallStd(unsigned int address, Args... args) {
        reinterpret_cast<void(__stdcall*)(Args...)>(address)(args...);
    }

    template <typename Ret, typename... Args>
    static Ret CallStdAndReturn(unsigned int address, Args... args) {
        return reinterpret_cast<Ret(__stdcall*)(Args...)>(address)(args...);
    }

    template <typename Ret, typename... Args>
    static Ret CallAndReturn(unsigned int address, Args... args) {
        return reinterpret_cast<Ret(__cdecl*)(Args...)>(address)(args...);
    }

    template <typename Ret, typename... Args>
    static Ret CallModuleAndReturn(unsigned int address, Args... args) {
        HMODULE hModule = GetModuleHandle("client.dll");
        if (hModule == NULL) return NULL;
        return reinterpret_cast<Ret(__cdecl*)(Args...)>(reinterpret_cast<uintptr_t>(hModule) + address)(args...);
    }

    template <typename C, typename... Args>
    static void CallMethod(unsigned int address, C _this, Args... args) {
        reinterpret_cast<void(__thiscall*)(C, Args...)>(address)(_this, args...);
    }

    template <typename Ret, typename C, typename... Args>
    static Ret CallMethodAndReturn(unsigned int address, C _this, Args... args) {
        return reinterpret_cast<Ret(__thiscall*)(C, Args...)>(address)(_this, args...);
    }

    template <typename Ret, typename C, typename... Args>
    static Ret CallModuleMethodAndReturn(unsigned int address, C _this, Args... args) {
        HMODULE hModule = GetModuleHandle("client.dll");
        if (hModule == NULL) return NULL;
        return reinterpret_cast<Ret(__thiscall*)(C, Args...)>(reinterpret_cast<uintptr_t>(hModule) + address)(_this, args...);
    }

    template <unsigned int tableIndex, typename C, typename... Args>
    static void CallVirtualMethod(C _this, Args... args) {
        reinterpret_cast<void(__thiscall*)(C, Args...)>((*reinterpret_cast<void***>(_this))[tableIndex])(_this, args...);
    }

    template <typename Ret, unsigned int tableIndex, typename C, typename... Args>
    static Ret CallVirtualMethodAndReturn(C _this, Args... args) {
        return reinterpret_cast<Ret(__thiscall*)(C, Args...)>((*reinterpret_cast<void***>(_this))[tableIndex])(_this, args...);
    }

    static unsigned int FindPattern(const char* name, const char* pattern, const char* mask, const char* moduleName = "client.dll") {
        for (auto& sig : signatures) {
            if (strcmp(sig->name, name) == 0 && strcmp(sig->moduleName, moduleName) == 0) {
                return sig->address;
            }
        }

        DWORD address = SigScan::FindPattern(moduleName, pattern, mask) - SIG_OFFSET;
        signatures.push_back(new Signature{ moduleName, name, pattern, mask, address });

        return address;
    }
};


#pragma once

namespace MemoryXD
{
    int memcpy_safe(void* dest, const void* src, uint32_t len, int check = NULL, const void* checkdata = NULL);
    int memset_safe(void* _dest, int c, uint32_t len);
    int memcmp_safe(const void* _s1, const void* _s2, uint32_t len);
    void* memdup(const void* src, int len);

    void HookInstallCall(DWORD dwInstallAddress, DWORD dwHookFunction);
    void CreateJump(BYTE* pAddress, DWORD dwJumpTo, DWORD dwLen);

    bool isBadPtr_handlerAny(void* pointer, ULONG size, DWORD dwFlags);
    bool isBadPtr_readAny(void* pointer, ULONG size);
    bool isBadPtr_writeAny(void* pointer, ULONG size);

    int __page_size_get(void);
    int __page_write(void* _dest, const void* _src, uint32_t len);
    int __page_read(void* _dest, const void* _src, uint32_t len);
};