#pragma once
#include <string>
#include <Windows.h>

class DllInjector {
public:
    DllInjector();
    ~DllInjector();
    bool InjectDll(const std::string& processName, const std::string& dllPath);

private:
    DWORD GetProcessIdByName(const std::string& processName);
    bool InjectIntoProcess(HANDLE hProcess, const std::string& dllPath);
};
