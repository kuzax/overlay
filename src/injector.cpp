#include "injector.h"
#include <iostream>
#include <thread>
#include <TlHelp32.h>

DllInjector::DllInjector() {}

DllInjector::~DllInjector() {}

bool DllInjector::InjectDll(const std::string& processName, const std::string& dllPath) {
    DWORD processId = GetProcessIdByName(processName);
    if (processId == 0) {
        std::cerr << "Process not found!" << std::endl;
        return false;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) {
        std::cerr << "Unable to open process!" << std::endl;
        return false;
    }

    bool result = InjectIntoProcess(hProcess, dllPath);
    CloseHandle(hProcess);

    return result;
}

DWORD DllInjector::GetProcessIdByName(const std::string& processName) {
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (!Process32First(snapshot, &pe32)) {
        CloseHandle(snapshot);
        return 0;
    }

    do {
        if (processName == pe32.szExeFile) {
            CloseHandle(snapshot);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(snapshot, &pe32));

    CloseHandle(snapshot);
    return 0;
}

bool DllInjector::InjectIntoProcess(HANDLE hProcess, const std::string& dllPath) {
    // DLL yolu doðruluðunu kontrol et
    if (dllPath.empty()) {
        std::cerr << "Invalid DLL path!" << std::endl;
        return false;
    }

    // Bellek tahsisi
    LPVOID baseAddress = VirtualAllocEx(hProcess, nullptr, dllPath.size() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!baseAddress) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return false;
    }

    // Belleðe DLL yolunu yaz
    if (!WriteProcessMemory(hProcess, baseAddress, dllPath.c_str(), dllPath.size() + 1, nullptr)) {
        std::cerr << "WriteProcessMemory failed!" << std::endl;
        return false;
    }

    // LoadLibraryA adresini al
    FARPROC loadLibraryAddr = GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryA");
    if (!loadLibraryAddr) {
        std::cerr << "GetProcAddress failed!" << std::endl;
        return false;
    }

    // Hedef iþlemde yeni bir thread baþlat
    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, baseAddress, 0, nullptr);
    if (!hThread) {
        std::cerr << "CreateRemoteThread failed!" << std::endl;
        return false;
    }

    // Thread'in bitmesini bekle
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return true;
}
