#pragma once
#include <includes.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class c_menu {
private:
    using present_t = HRESULT(__stdcall*)(ID3D11Device*, const RECT*, const RECT*, HWND, const RGNDATA*);
    using reset_t = HRESULT(__stdcall*)(ID3D11Device*, D3DPRESENT_PARAMETERS*);
    using cursor_t = BOOL(WINAPI*)(int, int);
public:
    present_t o_present;
    reset_t o_reset;
    cursor_t o_cursor;
    bool ShowScripts = false;
    bool release();
    void initialize();
    void draw();
    void shutdown(bool before);
    int iCurrentTab;
    bool bMainMnState;
    void style();
};

inline c_menu* menu = new c_menu();


 
class QMenu {
public:


    static bool VerifyLuaInjector;
    static bool DumperVerify;
    static bool luaActived12;
    static char luaCode[9999];
    static char DisableCode[9999];
    static bool luaDumperActived12;
    static bool autoInjectEnabled;
    static bool ActivedLuaOk;
    static void* CSCHookECXxdq;
    static void RenderCursor();
    static void __fastcall DisableSetCursorPos(void* ECX, void* EDX);
    static void UpdateResourceList();
    static void OnResourceSelect(const std::wstring& resourceName);
    static bool modifyCode;
    static ImGuiKey MenuKeyBinding;
    static bool HitKillInPlayer;
    static bool ServerLagWeaponPlayer;
    static bool ResendSendBulletSignature;

    static bool SilentAimPlayer;
    static bool SilentAimCheckVisible;
    static bool SilentAimFovDraw;

    static std::vector<std::wstring> resourceList;
    static std::wstring selectedResource;
    static std::unordered_map<std::wstring, std::vector<char>> storeScripts;

    static std::vector<std::string> codeHistory;
    static int currentHistoryIndex;
    static std::map<std::string, std::string> savedCodes;
    static std::string codeNameInput;

    struct ModificationInfo {
        std::string scriptName;
        std::string timestamp;
        std::string status;
    };

    struct InjectedScriptInfo {
        std::string scriptName;
        std::string timestamp;
        std::string status;
    };

    static std::vector<ModificationInfo> modificationsList;
    static std::vector<InjectedScriptInfo> injectedScriptsList;
};

extern bool Script_Send;
void AddResources(const char* ScriptName);
void addScript(const std::string& title, const std::string& content);
void Reset_Script_And_Resources();
extern ImFont* g_museoFont;

