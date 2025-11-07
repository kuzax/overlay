#include "menu/menu.h"

#include "imgui/blur.hpp"
#include "imgui/bytes.hpp"
#include "imgui/gui.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include <console/console.h>
#include <data/elements.h>
#include <data/variables.h>
#include <globals.h>
#include <utilities/utilities.h>

#include "byte_array.h"
#include "includes.h"
#include <client/client.h>
#include <config.h>
#include <game/aimbot/aimbot.h>
#include <game/manager/manager.h>
#include <game/packet/packet.h>
#include <game/player/player.h>
#include <game/visual/visual.h>
#include <game/visual/visual.h>
#include <game/weapon/weapon.h>
#include <game/spoofer/spoofer.h>
#include <game/Radar/radar.h>
#include <injector.h>
#include <scripts.h>
#include <SlientAim.h>
#include <crosshair.h>
std::vector<int> original_indices;
static std::vector<size_t> savedPackets;



bool Script_Send;
static char searchBuffer[256] = "";
static char kuzaxsearchbuffer[256] = "";
static char input_buffer[128] = "";

DWORD* ClimaGame = (DWORD*)0xB70153;
float ClimaGameValue = 16;
bool Jump = false;
static std::vector<const char*> g_dump_filtered_items;
static std::vector<int>         g_dump_filtered_indices;
bool Reviver = false;  
bool bWeatherEnabled = false;
bool KeyBindRepararMotor = false;

static bool showBlockedScripts = false;
static std::string blockedScriptCode;
static int selectedBlockedScriptIndex = -1;
static bool isInitialized = false;
bool QMenu::luaActived12 = false;
char QMenu::luaCode[9999] = "outputChatBox(\"Powered by NexWare: \" .. getResourceName(getThisResource()))";
char QMenu::DisableCode[9999] = "asflşka";
bool QMenu::luaDumperActived12 = false;
bool QMenu::autoInjectEnabled = false;
bool QMenu::modifyCode = true;
bool CheckBoxLua = false; static int selectedAmmo = 0;  
static int ammoAmount = 100;  
bool Velocidade = false;
#define Adds_Speed   0xB7CB64
bool iWeaponGroup = false;
static char selectedScriptCode[1024 * 64] = "";
int selectedScript = 0;
DWORD* CPedd = (DWORD*)0xB6F5F0;
DWORD* NitroVeiculo = (DWORD*)0x969165;
DWORD* tCongelar = (DWORD*)((*CPedd) + 0x598);
#define Adds_Speed   0xB7CB64
DWORD* CVehiclee = (DWORD*)0xB6F980;
float* MotorVehicle = (float*)((*CVehiclee) + 1216);

bool removehudcheck = false;
DllInjector myInjector;
char dllPath[256] = "";
static bool vehicleTrollTriggered = false;
static int currentSelectionIndex = 1;
std::string GetKeyName(int keyCode)
{
    char name[128];
    if (GetKeyNameTextA(MapVirtualKeyA(keyCode, MAPVK_VK_TO_VSC) << 16, name, 128) > 0)
        return std::string(name);
    return "Unknown";
}
static std::vector<size_t> displayedPackets;  
bool VerificationBulletproofEngine = false;
bool VehicleInvisible = false;
bool HandlingVehicleBool = false;
bool NitroActived = false;
static bool CrosshairEnabled = false;
struct Combat {
    bool bNoReload;
    bool bAllowCBug;
    bool bCBugFreeze;
    bool bCBugAnim;
    bool bCBugText;
    bool bCBugWeapon;
    bool bRapidFire;
    float fRapidFire;
    bool bNoRecoil;    
    bool bNoSpread;
    bool bFastCrosshair;
    bool bWeaponSwitcher;
    bool bFastSwitch;
    bool bAutoScroll;
    bool bAutoCBug;
};
static float fSpeed = 1.0f;
struct Config {
    Combat g_Combat;
};
bool streamproof = false;
bool fakelag_enabled = false;
bool fakeconnection = false;

bool sendCorruptedData = false;  
bool sendData = false;  


Config g_Config;


#define ALPHA    ( ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
#define NO_ALPHA ( ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )

IDirect3DTexture9* avatar{ };
IDirect3DTexture9* mapimage{ };
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

void FastCrosshair()
{
    static bool bFastCrosshair = false;
    if (g_Config.g_Combat.bFastCrosshair)
    {
        pSecure->memcpy_safe((void*)0x0058E1D9, "\xEB", 1);
        bFastCrosshair = true;
    }
    else if (bFastCrosshair)
    {
        MemoryXD::memcpy_safe((void*)0x0058E1D9, "\x74", 1);

        bFastCrosshair = false;
    }
}





void LoadMTASerial()
{
    HKEY hKey;
    const char* regPath = R"(SOFTWARE\WOW6432Node\Multi Theft Auto: San Andreas All\1.6\Settings\general)";
    DWORD bufferSize = sizeof(element->spoofer.szCurrentSerial);
    DWORD type = 0;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueExA(hKey, "serial", nullptr, &type, reinterpret_cast<LPBYTE>(element->spoofer.szCurrentSerial), &bufferSize) == ERROR_SUCCESS)
        {
            if (type != REG_SZ)
                element->spoofer.szCurrentSerial[0] = '\0'; 
        }
        RegCloseKey(hKey);
    }
}


void NoReloadXD()
{
    DWORD* MunicaoInfinita = (DWORD*)0x969178;
    if (g_Config.g_Combat.bNoReload == true) {
        if (*MunicaoInfinita == 0) {
            *MunicaoInfinita = 1;
        }
    }

    if (g_Config.g_Combat.bNoReload == false) {
        if (*MunicaoInfinita == 1) {
            *MunicaoInfinita = 0;
        }
    }
}


static bool waitingForKey = false; 
static bool waitingForLagKey = false; 
static int airbreakKey = 0;  
static int lagKey = 0; 

static bool waitingForHealthKey = false;
static int healthArmorKey = 0;
static float playerHealth = 100.0f;
static float playerArmor = 100.0f;

void CheckAirbreakKey()
{
    if (airbreakKey != 0 && (GetAsyncKeyState(airbreakKey) & 1))
    {

        element->player.airbreak = !element->player.airbreak;
    }
}




void CheckHealthArmorKey()
{
        if (element->player.enableHealthKeybind && healthArmorKey != 0)
        {
            if (GetAsyncKeyState(healthArmorKey) & 0x1)
            {
                CClientPlayer* local = manager->get_local_player();
                if (local && local->GetGamePlayer())
                {
                    CPlayerPed* pPlayerPed = local->GetGamePlayer();
                    pPlayerPed->SetHealth(playerHealth);
                    pPlayerPed->SetArmor(playerArmor);
                }
            }
        }

}

const char* const KeyNames[] = {
    "Unknown",
    "VK_LBUTTON",
    "VK_RBUTTON",
    "VK_CANCEL",
    "VK_MBUTTON",
    "VK_XBUTTON1",
    "VK_XBUTTON2",
    "Unknown",
    "VK_BACK",
    "VK_TAB",
    "Unknown",
    "Unknown",
    "VK_CLEAR",
    "VK_RETURN",
    "Unknown",
    "Unknown",
    "VK_SHIFT",
    "VK_CONTROL",
    "VK_MENU",
    "VK_PAUSE",
    "VK_CAPITAL",
    "VK_KANA",
    "Unknown",
    "VK_JUNJA",
    "VK_FINAL",
    "VK_KANJI",
    "Unknown",
    "VK_ESCAPE",
    "VK_CONVERT",
    "VK_NONCONVERT",
    "VK_ACCEPT",
    "VK_MODECHANGE",
    "VK_SPACE",
    "VK_PRIOR",
    "VK_NEXT",
    "VK_END",
    "VK_HOME",
    "VK_LEFT",
    "VK_UP",
    "VK_RIGHT",
    "VK_DOWN",
    "VK_SELECT",
    "VK_PRINT",
    "VK_EXECUTE",
    "VK_SNAPSHOT",
    "VK_INSERT",
    "VK_DELETE",
    "VK_HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "VK_LWIN",
    "VK_RWIN",
    "VK_APPS",
    "Unknown",
    "VK_SLEEP",
    "VK_NUMPAD0",
    "VK_NUMPAD1",
    "VK_NUMPAD2",
    "VK_NUMPAD3",
    "VK_NUMPAD4",
    "VK_NUMPAD5",
    "VK_NUMPAD6",
    "VK_NUMPAD7",
    "VK_NUMPAD8",
    "VK_NUMPAD9",
    "VK_MULTIPLY",
    "VK_ADD",
    "VK_SEPARATOR",
    "VK_SUBTRACT",
    "VK_DECIMAL",
    "VK_DIVIDE",
    "VK_F1",
    "VK_F2",
    "VK_F3",
    "VK_F4",
    "VK_F5",
    "VK_F6",
    "VK_F7",
    "VK_F8",
    "VK_F9",
    "VK_F10",
    "VK_F11",
    "VK_F12",
    "VK_F13",
    "VK_F14",
    "VK_F15",
    "VK_F16",
    "VK_F17",
    "VK_F18",
    "VK_F19",
    "VK_F20",
    "VK_F21",
    "VK_F22",
    "VK_F23",
    "VK_F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_NUMLOCK",
    "VK_SCROLL",
    "VK_OEM_NEC_EQUAL",
    "VK_OEM_FJ_MASSHOU",
    "VK_OEM_FJ_TOUROKU",
    "VK_OEM_FJ_LOYA",
    "VK_OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_LSHIFT",
    "VK_RSHIFT",
    "VK_LCONTROL",
    "VK_RCONTROL",
    "VK_LMENU",
    "VK_RMENU"
};

namespace Theme {
    
    constexpr ImVec4 BACKGROUND_PRIMARY = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);  
    constexpr ImVec4 BACKGROUND_SECONDARY = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);  
    constexpr ImVec4 BACKGROUND_TERTIARY = ImVec4(0.19f, 0.19f, 0.21f, 1.00f);  
    constexpr ImVec4 BACKGROUND_ELEVATED = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);  

    
    constexpr ImVec4 GRADIENT_TOP = ImVec4(0.13f, 0.13f, 0.15f, 1.00f);
    constexpr ImVec4 GRADIENT_BOTTOM = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);

    
    constexpr ImVec4 ACCENT_BLUE = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);  
    constexpr ImVec4 ACCENT_BLUE_HOVER = ImVec4(0.50f, 0.80f, 1.00f, 0.90f);  
    constexpr ImVec4 ACCENT_BLUE_ACTIVE = ImVec4(0.30f, 0.60f, 0.95f, 1.00f);  
    constexpr ImVec4 ACCENT_GLOW = ImVec4(0.40f, 0.70f, 1.00f, 0.25f);  

    
    constexpr ImVec4 BORDER_COLOR = ImVec4(0.32f, 0.32f, 0.34f, 0.80f);  
    constexpr ImVec4 BORDER_HIGHLIGHT = ImVec4(0.45f, 0.45f, 0.47f, 0.50f);  
    constexpr ImVec4 SHADOW_SOFT = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);  
    constexpr ImVec4 SHADOW_SHARP = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);  

    
    constexpr ImVec4 TEXT_PRIMARY = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);  
    constexpr ImVec4 TEXT_SECONDARY = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);  
    constexpr ImVec4 TEXT_MUTED = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);  
    constexpr ImVec4 TEXT_DISABLED = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);  
}


bool key_bind(const char* label, int& k, const ImVec2& size_arg)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    const bool focus_requested = ImGui::FocusableItemRegister(window, g.ActiveId == id);
    const bool active = g.ActiveId == id;

    if (hovered) {
        ImGui::SetHoveredID(id);
        g.MouseCursor = ImGuiMouseCursor_TextInput;
    }

    const bool user_clicked = hovered && io.MouseClicked[0];

    if (focus_requested || user_clicked) {
        if (g.ActiveId != id) {
            
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            memset(io.KeysDown, 0, sizeof(io.KeysDown));
            k = 0;
        }
        ImGui::SetActiveID(id, window);
        ImGui::FocusWindow(window);
    }
    else if (io.MouseClicked[0]) {
        if (g.ActiveId == id)
            ImGui::ClearActiveID();
    }

    bool value_changed = false;
    int key = k;

    if (active) {
        
        for (int i = 0; i < 5; i++) {
            if (io.MouseDown[i]) {
                switch (i) {
                case 0: key = VK_LBUTTON; break;
                case 1: key = VK_RBUTTON; break;
                case 2: key = VK_MBUTTON; break;
                case 3: key = VK_XBUTTON1; break;
                case 4: key = VK_XBUTTON2; break;
                }
                value_changed = true;
                ImGui::ClearActiveID();
                break;
            }
        }

        
        if (!value_changed) {
            for (int i = VK_BACK; i <= VK_RMENU; i++) {
                if (io.KeysDown[i]) {
                    key = i;
                    value_changed = true;
                    ImGui::ClearActiveID();
                    break;
                }
            }
        }

        
        if (IsKeyPressedMap(ImGuiKey_Escape)) {
            k = 0;
            ImGui::ClearActiveID();
        }
        else {
            k = key;
        }
    }

    
    char buf_display[64] = "None";

    
    ImVec4 bg_gradient_top, bg_gradient_bottom, border_color, text_color, glow_color;
    float animation_factor = 1.0f;

    if (active) {
        
        bg_gradient_top = ImVec4(0.35f, 0.65f, 1.00f, 0.95f);
        bg_gradient_bottom = ImVec4(0.25f, 0.55f, 0.90f, 0.95f);
        border_color = Theme::ACCENT_BLUE;
        text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        glow_color = Theme::ACCENT_GLOW;
        animation_factor = 1.2f;
        strcpy_s(buf_display, "[Press a key]");
    }
    else if (hovered) {
        
        bg_gradient_top = ImVec4(0.22f, 0.22f, 0.24f, 0.98f);
        bg_gradient_bottom = ImVec4(0.18f, 0.18f, 0.20f, 0.98f);
        border_color = Theme::ACCENT_BLUE_HOVER;
        text_color = Theme::TEXT_PRIMARY;
        glow_color = ImVec4(0.40f, 0.70f, 1.00f, 0.15f);
        animation_factor = 1.05f;
        if (k != 0) {
            strcpy_s(buf_display, KeyNames[k]);
        }
    }
    else {
        bg_gradient_top = ImVec4(0.17f, 0.17f, 0.19f, 0.96f);
        bg_gradient_bottom = ImVec4(0.13f, 0.13f, 0.15f, 0.96f);
        border_color = Theme::BORDER_COLOR;
        text_color = (k != 0) ? Theme::TEXT_PRIMARY : Theme::TEXT_MUTED;
        glow_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        if (k != 0) {
            strcpy_s(buf_display, KeyNames[k]);
        }
    }

    for (int i = 3; i >= 0; i--) {
        float shadow_offset = (i + 1) * 0.8f;
        float shadow_alpha = (4.0f - i) / 4.0f * 0.25f * animation_factor;

        window->DrawList->AddRectFilled(
            frame_bb.Min + ImVec2(shadow_offset, shadow_offset),
            frame_bb.Max + ImVec2(shadow_offset, shadow_offset),
            ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, shadow_alpha)),
            style.FrameRounding + shadow_offset * 0.3f
        );
    }

    if (active || hovered) {
        float glow_intensity = active ? 1.0f : 0.6f;
        for (int i = 0; i < 3; i++) {
            float glow_offset = (i + 1) * 1.5f;
            float glow_alpha = glow_color.w * (3.0f - i) / 3.0f * glow_intensity;

            window->DrawList->AddRect(
                frame_bb.Min - ImVec2(glow_offset, glow_offset),
                frame_bb.Max + ImVec2(glow_offset, glow_offset),
                ImGui::GetColorU32(ImVec4(glow_color.x, glow_color.y, glow_color.z, glow_alpha)),
                style.FrameRounding + glow_offset * 0.4f,
                0,
                0.8f
            );
        }
    }

    
    window->DrawList->AddRectFilledMultiColor(
        frame_bb.Min,
        frame_bb.Max,
        ImGui::GetColorU32(bg_gradient_top),
        ImGui::GetColorU32(bg_gradient_top),
        ImGui::GetColorU32(bg_gradient_bottom),
        ImGui::GetColorU32(bg_gradient_bottom)
    );

    
    if (active || hovered) {
        ImVec4 glass_overlay = active ?
            ImVec4(1.0f, 1.0f, 1.0f, 0.08f) :
            ImVec4(1.0f, 1.0f, 1.0f, 0.04f);

        window->DrawList->AddRectFilled(
            frame_bb.Min,
            ImVec2(frame_bb.Max.x, frame_bb.Min.y + (frame_bb.Max.y - frame_bb.Min.y) * 0.5f),
            ImGui::GetColorU32(glass_overlay),
            style.FrameRounding,
            ImDrawFlags_RoundCornersTop
        );
    }

    
    
    window->DrawList->AddRect(
        frame_bb.Min - ImVec2(0.5f, 0.5f),
        frame_bb.Max + ImVec2(0.5f, 0.5f),
        ImGui::GetColorU32(Theme::SHADOW_SOFT),
        style.FrameRounding + 0.5f,
        0,
        0.8f
    );

    
    window->DrawList->AddRect(
        frame_bb.Min,
        frame_bb.Max,
        ImGui::GetColorU32(border_color),
        style.FrameRounding,
        0,
        1.0f
    );

    
    if (hovered || active) {
        window->DrawList->AddRect(
            frame_bb.Min + ImVec2(0.5f, 0.5f),
            frame_bb.Max - ImVec2(0.5f, 0.5f),
            ImGui::GetColorU32(Theme::BORDER_HIGHLIGHT),
            style.FrameRounding - 0.5f,
            0,
            0.6f
        );
    }

    
    if (!active && !hovered) {
        window->DrawList->AddRect(
            frame_bb.Min + ImVec2(1, 1),
            frame_bb.Max - ImVec2(1, 1),
            ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.15f)),
            style.FrameRounding - 1.0f,
            0,
            0.4f
        );
    }

    
    const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Max.x, frame_bb.Max.y);
    ImVec2 text_pos = frame_bb.Min + style.FramePadding;

    ImGui::PushStyleColor(ImGuiCol_Text, text_color);
    ImGui::RenderTextClipped(
        text_pos,
        frame_bb.Max - style.FramePadding,
        buf_display,
        NULL,
        NULL,
        style.ButtonTextAlign,
        &clip_rect
    );
    ImGui::PopStyleColor();

    
    std::string label_str = label;
    if (label_size.x > 0 && label_str.find("##") == std::string::npos) {
        ImVec2 label_pos = ImVec2(
            frame_bb.Max.x + style.ItemInnerSpacing.x,
            frame_bb.Min.y + style.FramePadding.y
        );

        ImGui::PushStyleColor(ImGuiCol_Text, Theme::TEXT_PRIMARY);
        ImGui::RenderText(label_pos, label);
        ImGui::PopStyleColor();
    }

    return value_changed;
}

void weathersa()
{
    if (ClimaGameValue > 16 or ClimaGameValue < 16) {
        *ClimaGame = ClimaGameValue;
    }
}

static constexpr float WORLD_MIN_X = -3000.0f;
static constexpr float WORLD_MAX_X = 3000.0f;
static constexpr float WORLD_MIN_Y = -3000.0f;
static constexpr float WORLD_MAX_Y = 3000.0f;



ImVec2 WorldToMapCoords(CVector worldPos, ImVec2 mapPos, float mapSize) {
    // Dünya koordinatlarını 0-1 arası normalize et
    float normalizedX = (worldPos.fX - WORLD_MIN_X) / WORLD_SIZE;
    float normalizedY = (worldPos.fY - WORLD_MIN_Y) / WORLD_SIZE;

    // Y koordinatını ters çevir
    normalizedY = 1.0f - normalizedY;

    return ImVec2(
        mapPos.x + (normalizedX * mapSize),
        mapPos.y + (normalizedY * mapSize)
    );
}

void speedxd()
{
    if (Velocidade)
    {
        *(float*)Adds_Speed = fSpeed;
    }
    else
    {
        *(float*)Adds_Speed = 1.0f;
    }
}

void ScriptsManager::BlockScript(const std::string& scriptName)
{
    blockedScripts.insert(scriptName);
}


bool ScriptsManager::IsScriptBlocked(const std::string& scriptName) const
{
    return blockedScripts.find(scriptName) != blockedScripts.end();
}


void ScriptsManager::UnblockScript(const std::string& scriptName)
{
    blockedScripts.erase(scriptName);
}

int currentID = 0;
int selectedResourceIndex = 0;


struct ScriptEntry {
    int id;
    std::string LoadScriptFromBufferScriptName;
    std::string DeobfuscateScriptName;
};

std::vector<ScriptEntry> scriptList;
std::vector<std::string> resources;
std::unordered_map<std::string, std::string> scripts;




void Reset_Script_And_Resources()
{
    scripts.clear();
    scriptList.clear();
    resources.clear();
}



bool containsInvalidCharacters(const std::string& content) {
    for (char c : content) {
        if (c < 32 || c > 126) {
            return true;
        }
    }
    return false;
}

bool KIsLuaObfuscatedScript(const void* pData, unsigned int uiLength)
{
    const unsigned char* pCharData = (const unsigned char*)pData;
    return (uiLength > 0 && pCharData[0] == 0x1C);            
}



void AddResources(const char* ScriptName) {
    std::string strScriptName(ScriptName);
    auto it = std::find(resources.begin(), resources.end(), strScriptName);
    if (it == resources.end()) {
        resources.push_back(strScriptName);
    }
}

void addScript(const std::string& title, const std::string& content) {
    if (content.empty() || title.empty()) {
        return;
    }
    if (scripts.find(title) != scripts.end()) {
        return;
    }
    if (containsInvalidCharacters(content)) {
        scripts[title] = "Obfuscated Script";
        return;
    }
    if (!KIsLuaObfuscatedScript(content.c_str(), content.length())) {
        scripts[title] = content;
    }
    else {
        scripts[title] = "Obfuscated Script";
    }
}

void AddLoadScriptToScriptList(const std::string& loadScriptName) {
    for (auto& entry : scriptList) {
        if (entry.id == currentID) {
            entry.LoadScriptFromBufferScriptName = loadScriptName;
            
            return;
        }
    }
    ScriptEntry newEntry = { currentID, loadScriptName, "" };
    scriptList.push_back(newEntry);
}

void AddDeobfuscateScriptToScriptList(const std::string& deobfuscateScriptName) {
    for (auto& entry : scriptList) {
        if (entry.id == currentID) {
            entry.DeobfuscateScriptName = deobfuscateScriptName;
            currentID++; 
            return;
        }
    }
    ScriptEntry newEntry = { currentID, "", deobfuscateScriptName };
    scriptList.push_back(newEntry);
    currentID++;
}

std::string cp1251_to_utf8(const char* str)
{
    std::string res;
    WCHAR* ures = nullptr;
    char* cres = nullptr;

    int result_u = MultiByteToWideChar(1251, 0, str, -1, nullptr, 0);
    if (result_u != 0)
    {
        ures = new WCHAR[result_u];
        if (MultiByteToWideChar(1251, 0, str, -1, ures, result_u))
        {
            int result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, nullptr, 0, 0, 0);
            if (result_c != 0)
            {
                cres = new char[result_c];
                if (WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0))
                {
                    res = cres;
                }
            }
        }
    }

    delete[] ures;
    delete[] cres;

    return res;
}

int stringIndexBul(const std::vector<std::string>& vektor, const std::string& str) {
    auto it = std::find(vektor.begin(), vektor.end(), str);
    if (it != vektor.end()) {
        return static_cast<int>(std::distance(vektor.begin(), it));
    }
    return -1;
}


static bool fovLoopEnabled = false;
static bool aspectLoopEnabled = false;
static float FovValue = 10.0f;

static float AspectValue = 1.3333334f;
static float TurnMassValue = 3000.0;
static float MassValue = 1500.0;
static float DragMulltipler = 1.00;


static float TractionMultiplier = 1.0f;
static float NumOfGears = 5.0f;
static float EngineAcceleration = 20.0f;
static float EngineInertia = 10.0f;
static float MaxVelocity = 200.0f;
static float BrakeDeceleration = 8.0f;
static float BrakeBias = 0.5f;
static float SteeringLock = 35.0f;
static float TractionLoss = 0.9f;
static float TractionBias = 0.5f;
static float SuspensionForceLevel = 1.5f;
static float SuspensionDamping = 0.2f;
static float SuspensionHighSpeedDamping = 0.0f;
static float SuspensionUpperLimit = 0.3f;
static float SuspensionLowerLimit = -0.2f;
static float SuspensionFrontRearBias = 0.5f;
static float SuspensionAntiDiveMultiplier = 0.0f;
static float CollisionDamageMultiplier = 0.5f;
static float SeatOffsetDistance = 0.25f;
static float Monetary = 10000.0f;



void RemoveHud()
{
    DWORD* RemoverHud = (DWORD*)0xA444A0;

    if (removehudcheck == true) {
        if (*RemoverHud == 1) {
            *RemoverHud = 0;
        }
    }
    else {
        if (removehudcheck == false) {
            if (*RemoverHud == 0) {
                *RemoverHud = 1;
            }
        }
    }
}


 

void NoRecoil()
{
    if (g_Config.g_Combat.bNoRecoil)
        pSecure->memcpy_safe((void*)0x8D610F, "\xBE\x00\x00\x00\x00", 5);
    else
        MemoryXD::memcpy_safe((void*)0x8D610F, "\xBE\x00\x00\x40\x3F", 5);
}

bool activeMoviment = false;
int MovimentVelocityStride = 7;
bool randompack = false;  
int duration = 10;         

void VelocityMoviment()
{
    float floatMovimento = MovimentVelocityStride;
    DWORD* CPed = (DWORD*)0xB6F5F0;
    float* tAtrasoMovimento = (float*)((*CPed) + 0x560);

    if (activeMoviment == true) {
        if (MovimentVelocityStride > -1) {
            *tAtrasoMovimento = floatMovimento;
        }
    }
}



static bool c_GodMode = false;
static std::thread godThread;
static bool godThreadRunning = false;






float fSpinbotSpeed = 10.0f;  // Dönüş hızı
float fSpinbotAngle = 0.0f;   // Mevcut açı


void antiwalk(CPed* ped, CClientPlayer* local) {
    bool bW = GetAsyncKeyState('W') & 0x8000;
    bool bA = GetAsyncKeyState('A') & 0x8000;
    bool bS = GetAsyncKeyState('S') & 0x8000;
    bool bD = GetAsyncKeyState('D') & 0x8000;
    if (!bW && !bA && !bS && !bD) return;

    CCamera* camera = global->m_pGame->GetCamera();
    if (!camera) return;

    // Kamera açısını al
    float cameraAngle = camera->GetCameraRotation();

    // GTA'nın world rotasyonunda Y ekseni ters çalıştığı için burayı ayarlıyoruz
    cameraAngle = -cameraAngle + static_cast<float>(M_PI); // 180° tersine çevir

    float moveX = 0.0f, moveY = 0.0f;
    if (bW) moveY -= 1.0f;  // W artık geri
    if (bS) moveY += 1.0f;  // S artık ileri
    if (bD) moveX -= 1.0f;  // D artık sola
    if (bA) moveX += 1.0f;  // A artık sağa

    float length = sqrt(moveX * moveX + moveY * moveY);
    if (length > 0.0f) {
        moveX /= length;
        moveY /= length;
    }

    // Kamera açısına göre döndür (Y ekseni ters olarak hesaba katıldı)
    float finalX = moveX * cos(cameraAngle) - moveY * sin(cameraAngle);
    float finalY = moveX * sin(cameraAngle) + moveY * cos(cameraAngle);

    float speed = 0.15f;

    CVector currentPos;
    CClientEntity* local_entity = static_cast<CClientEntity*>(local);
    local_entity->GetPosition(currentPos);

    currentPos.fX += finalX * speed;
    currentPos.fY += finalY * speed;

    ped->SetPosition(&currentPos);
}


void SpinPlayer(CPed* ped, float angle) {
        CClientPlayer* local = manager->get_local_player();
        if (local) {
            CPed* ped = local->GetGamePlayer();
            CClientPed* localPed = static_cast<CClientPed*>(local);
            if (ped) {
                fSpinbotAngle += fSpinbotSpeed;
                if (fSpinbotAngle >= 360.0f)
                    fSpinbotAngle = 0.0f;

                float radians = fSpinbotAngle * 3.14159f / 180.0f;
                ped->SetCurrentRotation(radians);
                ped->SetTargetRotation(radians);


            }
        }
    }

void Update() {
    if (!pSpinbot) return;
    if (!pSpinbot->bEnabled) return;       
    if (pSpinbot->bHookActive) return;     

    CClientPlayer* local = manager->get_local_player();
    if (!local) return;

    CPed* ped = local->GetGamePlayer();
    if (!ped) return;

    fSpinbotAngle += fSpinbotSpeed;
    if (fSpinbotAngle >= 360.0f)
        fSpinbotAngle = 0.0f;
    SpinPlayer(ped, fSpinbotAngle);
    antiwalk(ped, local);
}

void PrepareScriptCodeForExecution()
{
    if (element->dump.item_current >= 0 && element->dump.item_current < element->dump.dumps_list.size())
    {
        const auto& selected_dump = element->dump.dumps_list[element->dump.item_current];

        if (selected_dump.is_blocked)
        {
            strcpy(QMenu::luaCode, "askfljkla");  
        }
        else
        {
            std::string script_text(selected_dump.buffer.begin(), selected_dump.buffer.end());
            strcpy(QMenu::luaCode, script_text.c_str());
        }
    }
    else
    {
        strcpy(QMenu::luaCode, "");
    }
}





void GodModeThread()
{
    godThreadRunning = true;

    while (c_GodMode)
    {
        CClientPlayer* local = manager->get_local_player();
        if (local && local->GetGamePlayer())
        {
            CPlayerPed* pPlayerPed = local->GetGamePlayer();
            pPlayerPed->SetHealth(playerHealth);
           
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
    }

    godThreadRunning = false;
}
 


static bool b_veh_speed_boost = false;  
static float speed_multiplier = 1.0f;   


void vehicle_speed_boost()
{
    
    if (!b_veh_speed_boost)
        return;

    
    DWORD* vehicle = (DWORD*)0xB6F980;

    
    if (!vehicle)
        return;

    
    float* f_speed_x = (float*)((*vehicle) + 68);
    float* f_speed_y = (float*)((*vehicle) + 72);
    float* f_speed_z = (float*)((*vehicle) + 76);

    float* f_spin_x = (float*)((*vehicle) + 80);
    float* f_spin_y = (float*)((*vehicle) + 84);
    float* f_spin_z = (float*)((*vehicle) + 88);

    
    if (GetAsyncKeyState(VK_SHIFT))
    {
        *f_speed_x += *f_speed_x * speed_multiplier; 
        *f_speed_y += *f_speed_y * speed_multiplier; 
    }

    
    if (GetAsyncKeyState(VK_CONTROL))
    {
        *f_speed_x = 0.0f;
        *f_speed_y = 0.0f;
        *f_speed_z = 0.0f;
        *f_spin_x = 0.0f;
        *f_spin_y = 0.0f;
        *f_spin_z = 0.0f;
    }
}

bool InputText(const char* label, std::string& str, ImGuiInputTextFlags flags = 0)
{
    char buffer[256];
    std::strncpy(buffer, str.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    bool result = ImGui::InputText(label, buffer, sizeof(buffer), flags);

    if (result)
    {
        str = std::string(buffer);
    }

    return result;
}


LRESULT WINAPI h_wndproc(HWND handle, UINT message, WPARAM word_param, LPARAM long_param)
{
    ImGui_ImplWin32_WndProcHandler(handle, message, word_param, long_param);

    const auto& io = ImGui::GetIO();
    if (visual->overlayEnabled && (io.WantCaptureMouse || io.WantCaptureKeyboard))
    {
        return true;
    }

    return CallWindowProcA(var->winapi.wnd_proc, handle, message, word_param, long_param);
}

#include <chams.h>

HRESULT __stdcall h_present(IDirect3DDevice9* self, const RECT* sourceRect, const RECT* destRect, HWND destWindowOverride, const RGNDATA* dirtyRegion)
{
    var->winapi.device_dx9 = self;
    if (var->winapi.device_dx9)
    {
        menu->initialize();
        menu->draw();



    }
    return menu->o_present(self, sourceRect, destRect, destWindowOverride, dirtyRegion);
}


HRESULT __stdcall h_reset(IDirect3DDevice9* self, D3DPRESENT_PARAMETERS* presentationParameters)
{


    menu->shutdown(true);
    HRESULT result = menu->o_reset(self, presentationParameters);
    menu->shutdown(false);
    return result;
}

BOOL WINAPI h_cursor(int pos_x, int pos_y)
{
    if (var->gui.is_open || visual->overlayEnabled)
        return FALSE;

    return menu->o_cursor(pos_x, pos_y);
}

class PlayerInfo1
{
public:
    char _0x0000[20];
    union
    {
  
    };
    char _0x0018[32];
    float isplayer; 
    char _0x003C[6];
    BYTE especial_flags; 
    char _0x0043[125];
    __int32 proximo_carro; 
    char _0x00C4[936];
    BYTE player_stats; 
    BYTE player_jumping; 
    char _0x046E[1];
    BYTE player_crouch; 
    char _0x0470[208];
    float vida; 
    char _0x0544[4];
    float colete; 
    char _0x054C[500];
    __int32 current_wearpon; 
    char _0x0744[1276];

};




void CreateExplosion(CVector vecExplosion, eExplosionType eType, CClientEntity* Ent = nullptr)
{
    SPositionSync position(false);
    position.data.vecPosition = vecExplosion;

    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        if (Ent)
        {
            pBitStream->WriteBit(true);
            pBitStream->Write(Ent->GetID());
        }
        else
            pBitStream->WriteBit(false);

        pBitStream->Write(&position);

        SExplosionTypeSync explosionType;
        explosionType.data.uiType = eType;
        pBitStream->Write(&explosionType);

        global->m_pNet->SendPacket(PACKET_ID_EXPLOSION, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE);
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }
}

void CreateExplosionVehicle(CVector vecExplosion, eExplosionType eType, CClientVehicle* Ent = nullptr)
{
    SPositionSync position(false);
    position.data.vecPosition = vecExplosion;

    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        if (Ent)
        {
            pBitStream->WriteBit(true);
            pBitStream->Write(Ent->GetID());
        }
        else
            pBitStream->WriteBit(false);

        pBitStream->Write(&position);

        SExplosionTypeSync explosionType;
        explosionType.data.uiType = eType;
        pBitStream->Write(&explosionType);

        global->m_pNet->SendPacket(PACKET_ID_EXPLOSION, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE);
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }
}

void SetPlayerPosition(CClientEntity* pPlayer, CVector vecNewPosition)
{
    if (!pPlayer)
        return;

    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        pBitStream->Write(pPlayer->GetID());

        SPositionSync position(false);
        position.data.vecPosition = vecNewPosition;
        pBitStream->Write(&position);

        global->m_pNet->SendPacket(PACKET_ID_PLAYER_PURESYNC, pBitStream, PACKET_PRIORITY_MEDIUM, PACKET_RELIABILITY_RELIABLE);

        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }
}

void SetCurrentWeaponNoSpread()
{
    CClientPlayer* localPlayer = manager->get_local_player();
    if (!localPlayer) return;

    eWeaponType weaponType = manager->get_weapon_type(localPlayer);
    if (weaponType == WEAPONTYPE_INVALID) return;

    CWeaponInfo* weaponInfo = global->m_pGame->GetWeaponInfo(weaponType);
    if (!weaponInfo) return;


    weaponInfo->SetSpread(0.0f);
}

bool noSpreadEnabled = false;



#include <cstdlib>  
#include <ctime>    


constexpr uintptr_t OFFSET_MASS = 0x0C;
constexpr uintptr_t OFFSET_TURN_MASS = 0x10;
constexpr uintptr_t OFFSET_DRAG_MULT = 0x14;
constexpr uintptr_t OFFSET_ENGINE_TRACTION = 0x18;
constexpr uintptr_t OFFSET_TRACTION_LOSS = 0x20;
constexpr uintptr_t OFFSET_BRAKE_DECELERATION = 0x1C;
constexpr uintptr_t OFFSET_BRAKE_TRACTION_LOSS = 0x28;
constexpr uintptr_t OFFSET_BRAKE_BIAS = 0x24;
constexpr uintptr_t OFFSET_STEERING_LOCK = 0x2C;
constexpr uintptr_t OFFSET_SUSPENSION_FORCE = 0x30;
constexpr uintptr_t OFFSET_SUSPENSION_DAMPING = 0x34;
constexpr uintptr_t OFFSET_HIGH_SPEED_DAMPING = 0x38;
constexpr uintptr_t OFFSET_SUSPENSION_UPPER_LIMIT = 0x3C;
constexpr uintptr_t OFFSET_SUSPENSION_LOWER_LIMIT = 0x40;
constexpr uintptr_t OFFSET_COLLISION_DAMAGE_MULTI = 0x44;


#define VEHICLE_POINTER_ADDR 0xB6F980


CVehicle* GetLocalVehicle()
{
    CVehicle** vehiclePtrPtr = reinterpret_cast<CVehicle**>(VEHICLE_POINTER_ADDR);
    if (!vehiclePtrPtr) return nullptr;
    return *vehiclePtrPtr;
}

inline float& VehicleParam(CVehicle* vehicle, uintptr_t offset)
{
    return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(vehicle) + offset);
}



void c_menu::initialize()
{

    if (var->gui.initialized)
        return;

    var->winapi.device_dx9->GetCreationParameters(&var->winapi.device_par);
    var->winapi.wnd_proc = (WNDPROC)SetWindowLongW(var->winapi.device_par.hFocusWindow, GWL_WNDPROC, (LONG)h_wndproc);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    
    



    io.Fonts->AddFontFromMemoryTTF(museo500_binary, sizeof museo500_binary, 14);
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromMemoryTTF(&font_awesome_binary, sizeof font_awesome_binary, 13, &icons_config, icon_ranges);
    io.Fonts->AddFontFromMemoryTTF(museo900_binary, sizeof museo900_binary, 28);
    font::Font = io.Fonts->AddFontFromMemoryTTF(dia, sizeof dia, 14);





    ImGui_ImplWin32_Init(var->winapi.device_par.hFocusWindow);
    ImGui_ImplDX9_Init(var->winapi.device_dx9);
    ImGui_ImplDX9_InvalidateDeviceObjects();


    var->gui.initialized = true;
}
#include <ctime> 
#include <imguis/Imnotif.h>
#include <netc/netc.h>
#include <codecvt>
#include <gamesdk.h>
#include <imguis/imguipp.h>
#include <chat.h>

static int selectedExplosion = 2;
const char* explosionTypes[] = {
    "Rocket Weak", "Car", "Car Quick",
    "Boat", "Heli", "Mine", "Object", "Tank Grenade", "Small", "Tiny"
};

float fWidth;
float fHeight;
static std::string editorText = "";
void CreateProjectileSync(CVector vecPosition, eWeaponType weaponType, CClientEntity* pTarget = nullptr)
{
    SPositionSync position(false);
    position.data.vecPosition = vecPosition;
    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        if (pTarget)
        {
            pBitStream->WriteBit(true);
            pBitStream->Write(pTarget->GetID());
        }
        else
            pBitStream->WriteBit(false);

        pBitStream->Write(&position);

        SWeaponTypeSync weaponTypeSync;
        weaponTypeSync.data.ucWeaponType = static_cast<unsigned char>(weaponType);
        pBitStream->Write(&weaponTypeSync);

        switch (weaponType)
        {
        case WEAPONTYPE_GRENADE:
        case WEAPONTYPE_TEARGAS:
        {
            
            SFloatSync<7, 17> projectileForce;
            projectileForce.data.fValue = 150.0f;  
            pBitStream->Write(&projectileForce);

            
            SVelocitySync velocity;
            velocity.data.vecVelocity = CVector(50.0f, 50.0f, 20.0f); 
            pBitStream->Write(&velocity);

            
            SFloatSync<7, 17> timeToLive;
            timeToLive.data.fValue = 5.0f;  
            pBitStream->Write(&timeToLive);


            break;
        }
        case WEAPONTYPE_MOLOTOV:
        case WEAPONTYPE_REMOTE_SATCHEL_CHARGE:
        {
            
            SFloatSync<7, 17> projectileForce;
            projectileForce.data.fValue = 50.0f;  
            pBitStream->Write(&projectileForce);

            
            SVelocitySync velocity;
            velocity.data.vecVelocity = CVector(10.0f, 10.0f, 20.0f);  
            pBitStream->Write(&velocity);

            
            SFloatSync<7, 17> timeToLive;
            timeToLive.data.fValue = 3.0f;  
            pBitStream->Write(&timeToLive);

            break;
        }
        case WEAPONTYPE_ROCKET:
        case WEAPONTYPE_ROCKET_HS:
        {
            if (pTarget)
            {
                pBitStream->WriteBit(true);
                pBitStream->Write(pTarget->GetID());
            }
            else
                pBitStream->WriteBit(false);

            
            SVelocitySync velocity;
            velocity.data.vecVelocity = CVector(30.0f, 30.0f, 50.0f);  
            pBitStream->Write(&velocity);

            
            SRotationRadiansSync rotation(true);
            rotation.data.vecRotation = CVector(0.0f, 0.0f, 5.0f);  
            pBitStream->Write(&rotation);

            
            SFloatSync<7, 17> timeToLive;
            timeToLive.data.fValue = 10.0f;  
            pBitStream->Write(&timeToLive);

            break;
        }
        default:
            break;
        }

        global->m_pNet->SendPacket(PACKET_ID_PROJECTILE, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_RELIABLE_ORDERED);

        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }
}



void SuperBreak()
{
    if (!element->vehicle.SuperBreakCheck) 
        return;

    CClientPlayer* localPlayer = global->m_pPlayerManager->GetLocalPlayer();
    if (!localPlayer)
        return;

    CClientVehicle* vehicle = localPlayer->GetOccupiedVehicle();
    if (!vehicle)
        return;

    CVector vecZero(0.0f, 0.0f, 0.0f);
    vehicle->GetGameVehicle()->SetMoveSpeed(vecZero);
    vehicle->GetGameVehicle()->SetTurnSpeed(&vecZero);
}




void inshallah_time11(CVector vecPosition, eWeaponType weaponType, CClientEntity* pTarget = nullptr)
{
    SPositionSync position(false);
    position.data.vecPosition = vecPosition;
    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        if (pTarget)
        {
            pBitStream->WriteBit(true);
            pBitStream->Write(pTarget->GetID());
        }
        else
            pBitStream->WriteBit(false);

        pBitStream->Write(&position);

        SWeaponTypeSync weaponTypeSync;
        weaponTypeSync.data.ucWeaponType = static_cast<unsigned char>(weaponType);
        pBitStream->Write(&weaponTypeSync);

        
        for (int i = 0; i < 1000; i++)  
        {
            pBitStream->Write(&position);  
            pBitStream->Write(&weaponTypeSync);  
        }

        
        for (int i = 0; i < 500; i++)  
        {
            SVelocitySync velocity;
            velocity.data.vecVelocity = CVector(i * 20.0f, i * 10.0f, i * 5.0f);  
            pBitStream->Write(&velocity);
        }

        
        for (int i = 0; i < 500; i++)  
        {
            SRotationRadiansSync rotation(true);
            rotation.data.vecRotation = CVector(i * 2.0f, i * 4.0f, i * 6.0f);  
            pBitStream->Write(&rotation);
        }

        
        for (int i = 0; i < 500; i++)  
        {
            SFloatSync<7, 17> extraData;
            extraData.data.fValue = 0.5f + (float)i * 0.2f;  
            pBitStream->Write(&extraData);
        }

        
        for (int i = 0; i < 1000; i++)  
        {
            SFloatSync<7, 17> stateData;
            stateData.data.fValue = 1.0f + i * 0.3f;  
            pBitStream->Write(&stateData);
        }

        
        for (int i = 0; i < 500; i++)  
        {
            SPositionSync tempPosition(false);
            tempPosition.data.vecPosition = CVector(i * 2.0f, i * 4.0f, i * 6.0f);  
            pBitStream->Write(&tempPosition);
        }

        
        for (int i = 0; i < 1000; i++)  
        {
            pBitStream->Write(&weaponTypeSync);  
        }

        
        for (int i = 0; i < 1000; i++)  
        {
            SVelocitySync velocity;
            velocity.data.vecVelocity = CVector(i * 2.0f, i * 4.0f, i * 6.0f);  
            pBitStream->Write(&velocity);

            SRotationRadiansSync rotation(true);
            rotation.data.vecRotation = CVector(i * 0.2f, i * 0.4f, i * 0.6f);  
            pBitStream->Write(&rotation);
        }

        
        for (int i = 0; i < 1000; i++)  
        {
            SFloatSync<7, 17> timeToLive;
            timeToLive.data.fValue = 10.0f + (float)i * 0.2f;  
            pBitStream->Write(&timeToLive);
        }

        global->m_pNet->SendPacket(PACKET_ID_PROJECTILE, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_RELIABLE_ORDERED);

        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }
}

void SendCustomWeaponFireSync(uint8_t weaponID, const CVector& vecStart, const CVector& vecEnd)
{
    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (!pBitStream)
        return;

    pBitStream->Write(weaponID); 

    pBitStream->Write((const char*)&vecStart, sizeof(CVector)); 
    pBitStream->Write((const char*)&vecEnd, sizeof(CVector));   

    static uint8_t customBulletSyncOrderCounter = 0;
    pBitStream->Write(customBulletSyncOrderCounter++);          

    global->m_pNet->SendPacket(PACKET_ID_WEAPON_BULLETSYNC, pBitStream, PACKET_PRIORITY_MEDIUM, PACKET_RELIABILITY_RELIABLE);
    global->m_pNet->DeallocateNetBitStream(pBitStream);
}


void sendCustomDataToTarget(CVector vecPosition, float speed, float rotationAngle,CClientEntity* pTarget = nullptr)
{
    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        if (pTarget)
        {
            pBitStream->WriteBit(true); 
            pBitStream->Write(pTarget->GetID()); 
        }
        else
        {
            pBitStream->WriteBit(false); 
        }

        
        SPositionSync positionSync(false);
        positionSync.data.vecPosition = vecPosition; 
        pBitStream->Write(&positionSync); 

        
        SFloatSync<7, 17> speedData;
        speedData.data.fValue = speed;  
        pBitStream->Write(&speedData); 

        
        SFloatSync<7, 17> rotationData;
        rotationData.data.fValue = rotationAngle;  
        pBitStream->Write(&rotationData); 

        
        global->m_pNet->SendPacket(PACKET_ID_CUSTOM_DATA, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_RELIABLE_ORDERED);

        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }
}



bool xdXX = false;
bool QMenu::ActivedLuaOk = false;



bool playersListCheckBox = false;

std::vector<std::wstring> QMenu::resourceList;
std::wstring QMenu::selectedResource;
std::unordered_map<std::wstring, std::vector<char>> QMenu::storeScripts;
 
TextEditor editor;
void QMenu::OnResourceSelect(const std::wstring& resourceName) {
    selectedResource = resourceName;
}


std::string ZStringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

void SaveScript(const std::wstring& scriptName, const std::vector<char>& scriptData) {
    QMenu::storeScripts[scriptName] = scriptData;
}


void HandleModuleNotLoaded() {
    if (!GetModuleHandleA("client.dll")) {
        if (!QMenu::selectedResource.empty()) {
            auto it = QMenu::storeScripts.find(QMenu::selectedResource);
            if (it != QMenu::storeScripts.end()) {
                SaveScript(QMenu::selectedResource, it->second);
            }
        }

        std::wstring savedSelectedResource = QMenu::selectedResource;

        QMenu::resourceList.clear();
        QMenu::selectedResource = savedSelectedResource;
    }
}

void RestoreSelectedScript() {
    if (!QMenu::selectedResource.empty()) {
        auto it = QMenu::storeScripts.find(QMenu::selectedResource);
        if (it != QMenu::storeScripts.end()) {
            
        }
    }
}


void ApplyBoost()
{
    if (!element->vehicle.HornBoostCheck)
        return;

    CClientPlayer* localPlayer = global->m_pPlayerManager->GetLocalPlayer();
    if (!localPlayer)
        return;

    CClientVehicle* vehicle = localPlayer->GetOccupiedVehicle();
    if (!vehicle || !vehicle->GetGameVehicle())
        return;

    CVector currentSpeed;
    vehicle->GetGameVehicle()->GetMoveSpeed(&currentSpeed);

    float length = sqrtf(currentSpeed.fX * currentSpeed.fX + currentSpeed.fY * currentSpeed.fY + currentSpeed.fZ * currentSpeed.fZ);
    if (length == 0.0f)
        return;


    
    CVector direction(currentSpeed.fX / length, currentSpeed.fY / length, currentSpeed.fZ / length);

    
    float boostStrength = static_cast<float>(element->vehicle.boost_value) * 0.002f;

    CVector boostVec = direction * boostStrength;
    currentSpeed += boostVec;

    vehicle->GetGameVehicle()->SetMoveSpeed(currentSpeed);
}

void HandleResourceSelectionChange(const std::wstring& newSelection) {
    if (QMenu::selectedResource != newSelection) {
        if (QMenu::modifyCode && !QMenu::selectedResource.empty()) {
            auto it = QMenu::storeScripts.find(QMenu::selectedResource);
            if (it != QMenu::storeScripts.end()) {
                std::vector<char> modifiedScriptData(editor.GetText().begin(), editor.GetText().end());
                SaveScript(QMenu::selectedResource, modifiedScriptData);
            }
        }

        QMenu::selectedResource = newSelection;

        if (QMenu::modifyCode) {
            RestoreSelectedScript();
        }
    }
}

std::vector<std::string> QMenu::codeHistory;
int QMenu::currentHistoryIndex = -1;

std::map<std::string, std::string> QMenu::savedCodes;
std::string QMenu::codeNameInput;

template <typename T>
bool SafeWriteMemory(uintptr_t address, const T& value)
{
    DWORD oldProtect;
    if (VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        std::memcpy(reinterpret_cast<void*>(address), &value, sizeof(T));
        VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(T), oldProtect, &oldProtect);
        return true;
    }
    return false;
}

static float comebackcomebacktothemomentx = 0.0f;
static float comebackcomebacktothemomenty = 0.0f;
static float comebackcomebacktothemomentz = 0.0f;


static CVector tp_pos(0.0f, 0.0f, 0.0f);


void TeleportPlayer(float x, float y, float z)
{
    CClientPlayer* local = manager->get_local_player();
    if (!local)
        return;

    CClientEntity* local_entity = static_cast<CClientEntity*>(local);
    if (!local_entity)
        return;

    CVector vec_position(x, y, z);

   local_entity->SetPosition(vec_position);
}


#include <chrono>


static std::vector<CClientVehicle*> vehiclesToPull;
static size_t vehiclePullIndex = 0;
static std::chrono::steady_clock::time_point lastPullTime;




static int currentDelay = 1000; 

void InitVehiclesToPull()
{
    vehiclesToPull.clear();

    auto allPlayers = manager->get_players();
    std::unordered_set<CClientVehicle*> occupiedVehicles;

    for (auto* player : allPlayers)
    {
        if (!player) continue;

        CClientVehicle* veh = player->GetOccupiedVehicle();
        if (veh && occupiedVehicles.find(veh) == occupiedVehicles.end())
        {
            vehiclesToPull.push_back(veh);
            occupiedVehicles.insert(veh);
        }
    }

    auto allVehicles = manager->get_vehicles();
    for (auto* veh : allVehicles)
    {
        if (!veh) continue;
        if (occupiedVehicles.find(veh) == occupiedVehicles.end())
        {
            vehiclesToPull.push_back(veh);
        }
    }

    vehiclePullIndex = 0;
    lastPullTime = std::chrono::steady_clock::now();

    
    currentDelay = 1000 + (rand() % 1001); 
}

void PullAllVehiclesTick()
{
    CClientEntity* local = static_cast<CClientEntity*>(manager->get_local_player());
    if (!local || vehiclesToPull.empty())
        return;

    auto currentTime = std::chrono::steady_clock::now();

    if (vehiclePullIndex < vehiclesToPull.size() &&
        std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastPullTime).count() >= currentDelay)
    {
        CClientVehicle* v = vehiclesToPull[vehiclePullIndex];
        if (v)
        {
            
            SendVehicleTrailerAttach(v);
        }

        vehiclePullIndex++;
        lastPullTime = currentTime;

        
        currentDelay = 1000 + (rand() % 1001); 
    }
}



#include <game/CallHook/ccallhook.h>






CClientPlayer* local = manager->get_local_player();
CPed* pPedSelf = local->GetGamePlayer();




















void SetFOV(float fFOV)
{
    SafeWriteMemory<float>(0x0858CE0, fFOV);
    SafeWriteMemory<float>(0x0B6F250, fFOV);
}

void SetAspectRatio(float fValue)
{
    SafeWriteMemory<float>(0xC3EFA4, fValue);
}


int inshallah_time = 1;


static bool SuperBreakCheck = false;
static bool HornBoostCheck = false;

void SetSuperBreakEnabled(bool enable)
{
    SuperBreakCheck = enable;
}

void SetHornBoostEnabled(bool enable)
{
    HornBoostCheck = enable;
}




void RenderBlur(ImDrawList* drawList, const ImVec2& pos, const ImVec2& size, float strength)
{


    ImU32 blurColor = ImGui::GetColorU32(ImVec4(0, 0, 0, 0.5f * strength)); 
    drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), blurColor);
}

static bool send_vehicle_checkbox = false;
static bool vehicle_rain = false;
static bool game_started = false;
static bool notified = false;  



void c_menu::draw()
{




    void CheckSetFakeKey();
    {
        if (lagKey != 0 && (GetAsyncKeyState(lagKey) & 1))
        {
            fakelag_enabled = !fakelag_enabled;


            if (fakelag_enabled) {
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Lag Switch On", "Test" });
                global->m_pNet->SetFakeLag(50, 5000, 5000, 32);
            }
            else {
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Lag Switch Off", "Test" });
                global->m_pNet->SetFakeLag(0, 0, 0, 0);
            }
        }
    }



    void CheckSuperBreak();
    {
        if (SuperBreakCheck)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x8000)
                element->vehicle.SuperBreakCheck = true;
            else
                element->vehicle.SuperBreakCheck = false;
        }
    }    
    
    void CheckHornBoost();
    {
        if (HornBoostCheck)
        {
            if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)
                element->vehicle.HornBoostCheck = true;
            else
                element->vehicle.HornBoostCheck = false;
        }
    }





    if (GetAsyncKeyState('H') & 0x8000) {
        if (KeyBindRepararMotor == true) {
            *MotorVehicle = 1000.0f;
        }
    }

    static CClientPlayer* selected_player = nullptr;
    static CClientVehicle* selected_Vehicle = nullptr;



    if (element->binds.menu_bind != 0 && GetAsyncKeyState(element->binds.menu_bind) & 1)
    {
        var->gui.is_open = !var->gui.is_open;
    }

    if (var->gui.initialized && !notified) {
        ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Menu Loaded Successfully", "Test" });
        notified = true; 
    }



    if (!avatar)
        D3DXCreateTextureFromFileInMemoryEx(var->winapi.device_dx9, &porno, sizeof porno, 30, 30, D3DX_DEFAULT, 0,
            D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &avatar);    
    
    if (!mapimage)
        D3DXCreateTextureFromFileInMemoryEx(var->winapi.device_dx9, &radar, sizeof radar,
            2048, 2048,  // Büyük boyut (orijinal texture boyutu ne ise o)
            D3DX_DEFAULT, 0,
            D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
            D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &mapimage);








    if (element->player.HealthKey != 0 && GetAsyncKeyState(element->player.HealthKey) & 0x8000)
    {
        static auto timer = std::chrono::steady_clock::now();

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timer).count() > 300)
        {
            element->player.HealthSet = true;

            ImGui::InsertNotification({
                ImGuiToastType::Success,
                3000,
                "Health has been set!",
                "Health System"
                });

            timer = now;
        }
    }
    
    
    if (element->player.ArmorKey != 0 && GetAsyncKeyState(element->player.ArmorKey) & 0x8000)
    {
        static auto timer = std::chrono::steady_clock::now();

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timer).count() > 300)
        {
            element->player.ArmorSet = true;

            ImGui::InsertNotification({
                ImGuiToastType::Success,
                3000,
                "Armor has been set!",
                "Armor System"
                });

            timer = now;
        }
    }


    if (element->player.FakeLagKey != 0 && GetAsyncKeyState(element->player.FakeLagKey) & 0x8000)
    {
        static auto timer = std::chrono::steady_clock::now();
        static bool keyPressed = false; 

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timer).count() > 300)
        {
            if (!keyPressed)
            {
                element->player.FakeLagSet = !element->player.FakeLagSet; 

                if (element->player.FakeLagSet)
                {
                    ImGui::InsertNotification({
                        ImGuiToastType::Success,
                        3000,
                        "Lag Switch On",
                        "Fake Lag System"
                        });
                }
                else
                {
                    ImGui::InsertNotification({
                        ImGuiToastType::Info,
                        3000,
                        "Lag Switch Off",
                        "Fake Lag System"
                        });
                }

                keyPressed = true;
                timer = now;
            }
        }
        else
        {
            keyPressed = false;
        }
    }
    
    
    if (element->player.godmodekey != 0 && GetAsyncKeyState(element->player.godmodekey) & 0x8000)
    {
        static auto timer = std::chrono::steady_clock::now();

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timer).count() > 300)
        {
            element->player.godmode = !element->player.godmode;

            std::string message = element->player.godmode ? "God Mode Enabled" : "God Mode Disabled";
            auto type = element->player.godmode ? ImGuiToastType::Success : ImGuiToastType::Error;

            ImGui::InsertNotification({ type, 3000, message.c_str(), "God Mode System" });

            timer = now;
        }
    }

    if (element->radar.EnableRadar)
    {
        static auto toggleTimer = std::chrono::steady_clock::now();

        bool togglePressed = false;
        bool holdPressed = false;

        if (element->radar.RadarHoldKey != 0 && (GetAsyncKeyState(element->radar.RadarHoldKey) & 0x8000))
        {
            holdPressed = true;
        }

        if (element->radar.RadarToggleKey != 0 && (GetAsyncKeyState(element->radar.RadarToggleKey) & 0x8000))
        {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - toggleTimer).count() > 300)
            {
                togglePressed = true;
                toggleTimer = now;
            }
        }

        if (holdPressed)
        {
            if (!element->radar.RadarActive)
            {
                element->radar.RadarActive = true;
                ImGui::InsertNotification({
                    ImGuiToastType::Success,
                    2000,
                    "Radar Enabled (Hold)",
                    "Radar System"
                    });
            }
        }
        else if (togglePressed)
        {
            element->radar.RadarActive = !element->radar.RadarActive;

            std::string message = element->radar.RadarActive ? "Radar Enabled" : "Radar Disabled";
            auto type = element->radar.RadarActive ? ImGuiToastType::Success : ImGuiToastType::Info;

            ImGui::InsertNotification({
                type,
                3000,
                message.c_str(),
                "Radar System"
                });
        }
        else if (!holdPressed && element->radar.RadarHoldKey != 0)
        {
            if (element->radar.RadarActive)
            {
                element->radar.RadarActive = false;
                ImGui::InsertNotification({
                    ImGuiToastType::Info,
                    2000,
                    "Radar Disabled (Hold Released)",
                    "Radar System"
                    });
            }
        }
    }

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();




    static int inputValue = 0;

    if (element->radar.EnableRadar && element->radar.RadarActive)
    {
        ImVec2 windowSize = ImGui::GetIO().DisplaySize;
        float mapSize = element->radar.MapSize;

        ImVec2 mapPos;
        float padding = 20.0f;

        switch (element->radar.MapPosition) {
        case 0:
            mapPos = ImVec2(padding, padding);
            break;
        case 1:
            mapPos = ImVec2(windowSize.x - mapSize - padding, padding);
            break;
        case 2:
            mapPos = ImVec2(padding, windowSize.y - mapSize - padding);
            break;
        case 3:
            mapPos = ImVec2(windowSize.x - mapSize - padding, windowSize.y - mapSize - padding);
            break;
        case 4:
        default:
            mapPos = ImVec2(
                (windowSize.x - mapSize) * 0.5f,
                (windowSize.y - mapSize) * 0.5f
            );
            break;
        }

        auto drawList = ImGui::GetBackgroundDrawList();

        drawList->AddRect(mapPos, ImVec2(mapPos.x + mapSize, mapPos.y + mapSize),
            ImColor(255, 255, 255, 100), 0.0f, 0, 2.0f);

        drawList->AddImage(mapimage,
            mapPos,
            ImVec2(mapPos.x + mapSize, mapPos.y + mapSize),
            ImVec2(0, 0), ImVec2(1, 1),
            ImColor(255, 255, 255, 255));

        if (element->radar.PlayerBlip || element->radar.LocalPlayerBlip) {
            std::vector<CClientPlayer*> players = manager->get_players();
            CClientPlayer* localPlayer = manager->get_local_player();

            for (auto* player : players) {
                if (!player) continue;

                bool isLocal = (player == localPlayer);

                if (isLocal && !element->radar.LocalPlayerBlip) continue;
                if (!isLocal && !element->radar.PlayerBlip) continue;

                CClientEntity* entity = static_cast<CClientEntity*>(player);
                CVector pos;
                entity->GetPosition(pos);

                ImVec2 playerMapPos = WorldToMapCoords(pos, mapPos, mapSize);

                ImColor blipColor = isLocal
                    ? ImColor(element->radar.LocalPlayerColor[0], element->radar.LocalPlayerColor[1],
                        element->radar.LocalPlayerColor[2], element->radar.LocalPlayerColor[3])
                    : ImColor(element->radar.PlayerColor[0], element->radar.PlayerColor[1],
                        element->radar.PlayerColor[2], element->radar.PlayerColor[3]);

                ImColor borderColor = ImColor(255, 255, 255, 255);

                drawList->AddCircleFilled(playerMapPos, 6.0f, blipColor);
                drawList->AddCircle(playerMapPos, 6.0f, borderColor, 0, 1.5f);

                const char* playerName = player->GetNick();
                if (playerName && mapSize > 600.0f) {
                    ImColor nameColor = ImColor(element->radar.NameColor[0], element->radar.NameColor[1],
                        element->radar.NameColor[2], element->radar.NameColor[3]);
                    drawList->AddText(ImVec2(playerMapPos.x + 10, playerMapPos.y - 8),
                        nameColor, playerName);
                }
            }
        }

        if (element->radar.VehicleBlip) {
            std::vector<CClientVehicle*> vehicles = manager->get_vehicles();

            for (auto* vehicle : vehicles) {
                if (!vehicle) continue;

                CClientEntity* entity = static_cast<CClientEntity*>(vehicle);
                CVector pos;
                entity->GetPosition(pos);

                ImVec2 vehicleMapPos = WorldToMapCoords(pos, mapPos, mapSize);

                ImColor vehicleColor = ImColor(element->radar.VehicleColor[0], element->radar.VehicleColor[1],
                    element->radar.VehicleColor[2], element->radar.VehicleColor[3]);
                ImColor borderColor = ImColor(255, 255, 255, 255);

                drawList->AddRectFilled(
                    ImVec2(vehicleMapPos.x - 4, vehicleMapPos.y - 4),
                    ImVec2(vehicleMapPos.x + 4, vehicleMapPos.y + 4),
                    vehicleColor
                );
                drawList->AddRect(
                    ImVec2(vehicleMapPos.x - 4, vehicleMapPos.y - 4),
                    ImVec2(vehicleMapPos.x + 4, vehicleMapPos.y + 4),
                    borderColor, 0.0f, 0, 1.5f
                );
            }
        }
    }


    if (element->player.HealthSet)
    {
        CClientPlayer* local = manager->get_local_player();
        if (local && local->GetGamePlayer())
        {
            CPlayerPed* pPlayerPed = local->GetGamePlayer();
            pPlayerPed->SetHealth(inputValue);
            element->player.HealthSet = false;
        }
    }    
    
    if (element->player.ArmorSet)
    {
        CClientPlayer* local = manager->get_local_player();
        if (local && local->GetGamePlayer())
        {
            CPlayerPed* pPlayerPed = local->GetGamePlayer();
            pPlayerPed->SetArmor(inputValue);
            element->player.ArmorSet = false;
        }
    }    
    
    if (manager->is_loaded() && element->player.FakeLagSet)
    {
        global->m_pNet->SetFakeLag(50, 5000, 5000, 32);
    }
    else if (manager->is_loaded() && !element->player.FakeLagSet)
    {
        global->m_pNet->SetFakeLag(0, 0, 0, 0);
    }


    if (var->gui.is_open)
    {
        
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        ImGui::Begin("##BlurBackground", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoBringToFrontOnFocus);

        auto* draw_list = ImGui::GetWindowDrawList();

        
        ImVec2 screen_pos = ImGui::GetWindowPos();
        ImVec2 screen_size = ImGui::GetWindowSize();

        
        draw_list->AddRectFilled(
            screen_pos,
            screen_pos + screen_size,
            IM_COL32(0, 0, 0, 120)  
        );

        
        for (int i = 0; i < 3; i++) {
            float alpha = (3 - i) * 20.0f;
            ImU32 color = IM_COL32(0, 0, 0, (int)alpha);

            draw_list->AddRectFilled(
                screen_pos + ImVec2(i * 2, i * 2),
                screen_pos + screen_size - ImVec2(i * 2, i * 2),
                color
            );
        }

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(3);

        
        static bool bools[50]{};
        static int ints[50]{}, combo = 0;
        static char buf[64];
        static float color[4] = { 1.f, 1.f, 1.f, 1.f };



        ImGui::SetNextWindowSize(var->window.size);
        ImGui::Begin("Hello, world!", NULL, var->window.flags); {
            element->binds.bypass = true;

            auto window = GetCurrentWindow();
            auto draw = window->DrawList;
            auto pos = window->Pos;
            auto size = window->Size;

            gui.m_anim = ImLerp(gui.m_anim, 1.f, 0.045f);
            ImVec2 image_pos = pos + ImVec2(170 / 2 - io.Fonts->Fonts[1]->CalcTextSizeA(
                io.Fonts->Fonts[1]->FontSize, FLT_MAX, 0, "NEVERLOSE").x / 2 + 1 + 10, 15);

            ImVec2 image_size = ImVec2(45, 45);

            ImVec2 text_pos = image_pos + ImVec2(image_size.x + 10, 12);

            float time = ImGui::GetTime();
            ImFont* font = io.Fonts->Fonts[1];
            float base_font_size = font->FontSize * 1.12f;

            ImVec2 base_pos = text_pos - ImVec2(55, 5);
            float spacing = 2.0f;

            auto calc_text_width = [&](const char* text) -> float {
                return font->CalcTextSizeA(base_font_size, FLT_MAX, 0, text).x;
                };

            float nex_width = calc_text_width("Nex");
            float ware_width = calc_text_width("Ware");

            ImVec2 pos_nex = base_pos;
            ImVec2 pos_ware = base_pos + ImVec2(nex_width + spacing, 0);

            const char* nex_text = "Nex";
            const char* ware_text = "Ware";

            
            ImColor base_color_nex = ImColor(0.2f, 0.4f, 1.0f, 1.0f);
            ImColor base_color_ware = ImColor(0.9f, 0.9f, 0.9f, 1.0f);

            float fade_speed = 1.5f; 

            
            float x_cursor = pos_nex.x;
            for (int i = 0; nex_text[i] != '\0'; i++) {
                char c[2] = { nex_text[i], '\0' };
                float char_width = font->CalcTextSizeA(base_font_size, FLT_MAX, 0, c).x;

                
                float phase = fmod(time * fade_speed - i * 0.5f, (float)nex_text[i]);
                float alpha = 0.5f + 0.5f * sinf(time * fade_speed - i * 0.5f);

                alpha = ImClamp(alpha, 0.3f, 1.0f);

                ImColor col = ImColor(base_color_nex.Value.x, base_color_nex.Value.y, base_color_nex.Value.z, alpha);
                draw->AddText(font, base_font_size, ImVec2(x_cursor, pos_nex.y), col, c);
                x_cursor += char_width;
            }

            
            x_cursor = pos_ware.x;
            for (int i = 0; ware_text[i] != '\0'; i++) {
                char c[2] = { ware_text[i], '\0' };
                float char_width = font->CalcTextSizeA(base_font_size, FLT_MAX, 0, c).x;

                float alpha = 0.5f + 0.5f * sinf(time * fade_speed - i * 0.5f);
                alpha = ImClamp(alpha, 0.3f, 1.0f);

                ImColor col = ImColor(base_color_ware.Value.x, base_color_ware.Value.y, base_color_ware.Value.z, alpha);
                draw->AddText(font, base_font_size, ImVec2(x_cursor, pos_ware.y), col, c);
                x_cursor += char_width;
            }


         

            draw->AddImageRounded(avatar, pos + ImVec2(15, size.y - 55), pos + ImVec2(45, size.y - 25),
                ImVec2(0, 0), ImVec2(1, 1), ImColor(1.f, 1.f, 1.f, 1.f), 100);

            draw->AddText(pos + ImVec2(50, size.y - 55), gui.text.to_im_color(), "NexWare Project");
            draw->AddText(pos + ImVec2(50, size.y - 40), gui.text_disabled.to_im_color(), "Version:V1.1");



            SetCursorPos(ImVec2(10, 70));
            BeginChild("##tabs", ImVec2(150, size.y - 120));
            gui.group_title("General");

            if (gui.tab(ICON_FA_CROSSHAIRS, "Combat", gui.m_tab == 0) && gui.m_tab != 0)
                gui.m_tab = 0, gui.m_anim = 0.f;
            Spacing(); 

            if (gui.tab(ICON_FA_USER, "Local Player", gui.m_tab == 1) && gui.m_tab != 1)
                gui.m_tab = 1, gui.m_anim = 0.f;
            Spacing();

            if (gui.tab(ICON_FA_EYE, "Visuals", gui.m_tab == 2) && gui.m_tab != 2)
                gui.m_tab = 2, gui.m_anim = 0.f;
            Spacing(); Spacing(); 

            gui.group_title("Network");
            Spacing();

            if (gui.tab(ICON_FA_TOOLBOX, "Misc", gui.m_tab == 3) && gui.m_tab != 3)
                gui.m_tab = 3, gui.m_anim = 0.f;
            Spacing();            
            
            if (gui.tab(ICON_FA_USERS, "Player List", gui.m_tab == 4) && gui.m_tab != 4)
                gui.m_tab = 4, gui.m_anim = 0.f;
            Spacing();

            if (gui.tab(ICON_FA_CAR, "Vehicles", gui.m_tab == 5) && gui.m_tab != 5)
                gui.m_tab = 5, gui.m_anim = 0.f;
            Spacing(); Spacing();

            gui.group_title("Lua Manager");
            Spacing();

            if (gui.tab(ICON_FA_CODE, "Lua Executor", gui.m_tab == 6) && gui.m_tab != 6)
                gui.m_tab = 6, gui.m_anim = 0.f;
            Spacing();

            if (gui.tab(ICON_FA_FILE, "Lua Files", gui.m_tab == 7) && gui.m_tab != 7)
                gui.m_tab = 7, gui.m_anim = 0.f;
            Spacing();

            if (gui.tab(ICON_FA_SERVER, "Resources Manager", gui.m_tab == 8) && gui.m_tab != 8)
                gui.m_tab = 8, gui.m_anim = 0.f;
            Spacing();

            if (gui.tab(ICON_FA_DATABASE, "Lua Functions", gui.m_tab == 9) && gui.m_tab != 9)
                gui.m_tab = 9, gui.m_anim = 0.f;

            Spacing();

            if (gui.tab(ICON_FA_BOX, "Packets", gui.m_tab == 10) && gui.m_tab != 10)
                gui.m_tab = 10, gui.m_anim = 0.f;
            Spacing(); Spacing();

            gui.group_title("Options");
            Spacing();

            if (gui.tab(ICON_FA_USER_SECRET, "Spoofer", gui.m_tab == 11) && gui.m_tab != 11)
                gui.m_tab = 11, gui.m_anim = 0.f;

            Spacing();

            if (gui.tab(ICON_FA_GEAR, "Settings", gui.m_tab == 12) && gui.m_tab != 12)
                gui.m_tab = 12, gui.m_anim = 0.f;

            EndChild();


            if (gui.m_tab == 0) {
                SetCursorPos(ImVec2(200, 25)); 
                BeginChild("##weapon_subtabs", ImVec2(660, 25));



                for (int i = 0; i < gui.weapon_subtabs.size(); ++i) {
                    if (gui.subtab(gui.weapon_subtabs.at(i), gui.m_weapon_subtab == i, gui.weapon_subtabs.size(),
                        i == 0 ? ImDrawFlags_None : i == gui.weapon_subtabs.size() - 1 ? ImDrawFlags_None : 0)
                        && gui.m_weapon_subtab != i) {
                        gui.m_weapon_subtab = i;
                        gui.m_anim = 0.f;
                    }

                    if (i != gui.weapon_subtabs.size() - 1)
                        SameLine();
                }

                EndChild();
            }            


            


            if (gui.m_tab == 2) {
                SetCursorPos(ImVec2(200, 25));
                BeginChild("##visual_subtabs", ImVec2(660, 25));

                GetWindowDrawList()->AddRectFilled(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.button.to_im_color(), 4);
                GetWindowDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.border.to_im_color(), 4);

                for (int i = 0; i < gui.visual_subtabs.size(); ++i) {
                    if (gui.subtab(gui.visual_subtabs.at(i), gui.m_visual_subtab == i, gui.visual_subtabs.size(),
                        i == 0 ? ImDrawFlags_None : i == gui.visual_subtabs.size() - 1 ? ImDrawFlags_None : 0)
                        && gui.m_visual_subtab != i) {
                        gui.m_visual_subtab = i;
                        gui.m_anim = 0.f;
                    }

                    if (i != gui.visual_subtabs.size() - 1)
                        SameLine();
                }

                EndChild();
            }

            
            if (gui.m_tab == 5) {
                SetCursorPos(ImVec2(200, 25)); 
                BeginChild("##vehicle_subtabs", ImVec2(660, 25));

                GetWindowDrawList()->AddRectFilled(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.button.to_im_color(), 4);
                GetWindowDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.border.to_im_color(), 4);

                for (int i = 0; i < gui.vehicle_subtabs.size(); ++i) {
                    if (gui.subtab(gui.vehicle_subtabs.at(i), gui.m_vehicle_subtab == i, gui.vehicle_subtabs.size(),
                        i == 0 ? ImDrawFlags_None : i == gui.vehicle_subtabs.size() - 1 ? ImDrawFlags_None : 0)
                        && gui.m_vehicle_subtab != i) {
                        gui.m_vehicle_subtab = i;
                        gui.m_anim = 0.f;
                    }

                    if (i != gui.vehicle_subtabs.size() - 1)
                        SameLine();
                }            
                

                EndChild();
            }



            if (gui.m_tab == 6) {
                SetCursorPos(ImVec2(200, 25));
                BeginChild("##executor_subtabs", ImVec2(660, 25));

                GetWindowDrawList()->AddRectFilled(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.button.to_im_color(), 4);
                GetWindowDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.border.to_im_color(), 4);

                for (int i = 0; i < gui.executor_subtabs.size(); ++i) {
                    if (gui.subtab(gui.executor_subtabs.at(i), gui.m_executor_subtab == i, gui.executor_subtabs.size(),
                        i == 0 ? ImDrawFlags_None : i == gui.executor_subtabs.size() - 1 ? ImDrawFlags_None : 0)
                        && gui.m_executor_subtab != i) {
                        gui.m_executor_subtab = i;
                        gui.m_anim = 0.f;
                    }

                    if (i != gui.executor_subtabs.size() - 1)
                        SameLine();
                }

                EndChild();
            }



            if (gui.m_tab == 9) {
                SetCursorPos(ImVec2(200, 25));
                BeginChild("##luafunc_subtabs", ImVec2(700, 25));

                GetWindowDrawList()->AddRectFilled(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.button.to_im_color(), 4);
                GetWindowDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.border.to_im_color(), 4);

                for (int i = 0; i < gui.luafunc_subtabs.size(); ++i) {
                    if (gui.subtab(gui.luafunc_subtabs.at(i), gui.m_luafunc_subtab == i, gui.luafunc_subtabs.size(),
                        i == 0 ? ImDrawFlags_None : i == gui.luafunc_subtabs.size() - 1 ? ImDrawFlags_None : 0)
                        && gui.m_luafunc_subtab != i) {
                        gui.m_luafunc_subtab = i;
                        gui.m_anim = 0.f;
                    }

                    if (i != gui.luafunc_subtabs.size() - 1)
                        SameLine();
                }


                EndChild();
            }            
            
            if (gui.m_tab == 1) {
                SetCursorPos(ImVec2(200, 25));
                BeginChild("##localplayer_subtab", ImVec2(660, 25));

                GetWindowDrawList()->AddRectFilled(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.button.to_im_color(), 4);
                GetWindowDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.border.to_im_color(), 4);

                for (int i = 0; i < gui.localplayer_subtabs.size(); ++i) {
                    if (gui.subtab(gui.localplayer_subtabs.at(i), gui.m_localplayer_subtab == i, gui.localplayer_subtabs.size(),
                        i == 0 ? ImDrawFlags_None : i == gui.localplayer_subtabs.size() - 1 ? ImDrawFlags_None : 0)
                        && gui.m_localplayer_subtab != i) {
                        gui.m_localplayer_subtab = i;
                        gui.m_anim = 0.f;
                    }

                    if (i != gui.localplayer_subtabs.size() - 1)
                        SameLine();
                }


                EndChild();
            }

            if (gui.m_tab == 3) {
                SetCursorPos(ImVec2(200, 25));
                BeginChild("##misc_subtab", ImVec2(660, 25));

                GetWindowDrawList()->AddRectFilled(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.button.to_im_color(), 4);
                GetWindowDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.border.to_im_color(), 4);

                for (int i = 0; i < gui.misc_subtabs.size(); ++i) {
                    if (gui.subtab(gui.misc_subtabs.at(i), gui.m_misc_subtabs == i, gui.misc_subtabs.size(),
                        i == 0 ? ImDrawFlags_None : i == gui.misc_subtabs.size() - 1 ? ImDrawFlags_None : 0)
                        && gui.m_misc_subtabs != i) {
                        gui.m_misc_subtabs = i;
                        gui.m_anim = 0.f;
                    }

                    if (i != gui.misc_subtabs.size() - 1)
                        SameLine();
                }


                EndChild();
            }

            if (gui.m_tab == 11) {
                SetCursorPos(ImVec2(200, 25));
                BeginChild("##spoofer", ImVec2(660, 25));

                GetWindowDrawList()->AddRectFilled(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.button.to_im_color(), 4);
                GetWindowDrawList()->AddRect(GetWindowPos(), GetWindowPos() + GetWindowSize(), gui.border.to_im_color(), 4);

                for (int i = 0; i < gui.spoofer_subtabs.size(); ++i) {
                    if (gui.subtab(gui.spoofer_subtabs.at(i), gui.m_spoofer_subtab == i, gui.spoofer_subtabs.size(),
                        i == 0 ? ImDrawFlags_None : i == gui.spoofer_subtabs.size() - 1 ? ImDrawFlags_None : 0)
                        && gui.m_spoofer_subtab != i) {
                        gui.m_spoofer_subtab = i;
                        gui.m_anim = 0.f;
                    }

                    if (i != gui.spoofer_subtabs.size() - 1)
                        SameLine();
                }


                EndChild();
            }

            SetCursorPos(ImVec2(185, 81 - (5 * gui.m_anim)));
            BeginChild("##childs", ImVec2(size.x - 200, size.y - 96));

            switch (gui.m_tab) {

            case 0:

              if (gui.m_weapon_subtab == 0) {
                gui.group_box(ICON_FA_CROSSHAIRS " Aimbot", ImVec2(320, 210)); {
                    ImGui::Checkbox("Enable", &element->aimbot.state);
                    ImGui::Checkbox("Draw FOV", &element->aimbot.b_fov);

                    ImGui::Separator();
                    ImGui::Text("Target Selection");

                    ImGui::Combo("Target Priority", &element->aimbot.target_priority,
                        "FOV Center\0Closest Distance\0Lowest HP\0");

                    ImGui::Combo("Target Bone", &element->aimbot.bone, "Head\0Body\0");

                    ImGui::SliderInt("Aim FOV", &element->aimbot.i_fov, 15, 1000, nullptr,
                        ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoInput);


                    ImGui::SliderFloat("Aim Smooth", &element->aimbot.smooth, 0.6f, 100.0f, "%.1f",
                        ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoInput);


                    ImGui::Separator();
                    ImGui::Text("Filters");

                    ImGui::Checkbox("Dead Check", &element->aimbot.deadcheck);
                    ImGui::Checkbox("Visible Check", &element->aimbot.visible_check);
                    ImGui::Checkbox("Friend Check", &element->aimbot.team_check);

                    ImGui::Checkbox("Distance Check", &element->aimbot.distance_check);
                    if (element->aimbot.distance_check)
                    {
                        ImGui::SliderFloat("Max Distance", &element->aimbot.max_distance, 10.0f, 500.0f, "%.0fm",
                            ImGuiSliderFlags_AlwaysClamp);
                    }

                    ImGui::SliderFloat("Min Target HP", &element->aimbot.min_hp, 0.0f, 100.0f, "%.0f",
                        ImGuiSliderFlags_AlwaysClamp);
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("0 = Disabled, Only target players above this HP");




                } gui.end_group_box();

                SameLine(),



                gui.group_box(ICON_FA_CROSSHAIRS " Silent Aim", ImVec2(375, 210)); {

                    const char* silent_modes[] = { "Normal Silent", "Bullet Track" };
                    static int current_silent_mode = 0;
                    ImGui::Combo("Silent Mode", &current_silent_mode, silent_modes, 2);

                    // Normal Silent
                    if (current_silent_mode == 0) {


                        ImGui::Checkbox("Enable", &element->manual.state);
                        ImGui::Checkbox("Wall Shot", &element->manual.wallshot_enable);
                        ImGui::Checkbox("Draw FOV", &element->manual.b_fov);
                        ImGui::Combo("Target Bone", &element->manual.bone,
                            "Head\0Neck\0Spine\0Pelvis\0Left Hand\0Right Hand\0Left Foot\0Right Foot\0Chest\0Belly\0\0");
                        ImGui::SliderInt("Silent FOV", &element->manual.i_fov, 15, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoInput);
                        ImGui::Checkbox("Hit Chance", &element->manual.hitchance_enable);
                        if (element->manual.hitchance_enable)
                        {
                            ImGui::SliderInt("Chance %", &element->manual.hitchance_percent, 0, 100);
                        }
                        ImGui::Checkbox("Visible Check", &element->manual.visible_check);
                        ImGui::Checkbox("Dead Check", &element->silent.deadcheck);
                        ImGui::Checkbox("Target Marker", &element->esp.silent_color);
                        if (element->esp.silent_color)
                        {
                            ImGui::ColorEdit4("normalsilent", (float*)&element->esp_color.target_color, ImGuiColorEditFlags_NoInputs);
                        }
                    }
                    else if (current_silent_mode == 1) {

                        if (pSpinbot) {
                            if (pSpinbot->bEnabled) {
                                ImGui::BeginDisabled();
                                ImGui::Checkbox("Enable", &pSpinbot->bBulletTrackEnabled);
                                ImGui::EndDisabled();
                            }
                            else {
                                ImGui::Checkbox("Enable", &pSpinbot->bBulletTrackEnabled);

                                    ImGui::Checkbox("Show FOV", &element->bullettrack.b_fov);
                                    if (element->bullettrack.b_fov) {
                                        ImGui::SliderInt("FOV Size", &element->bullettrack.i_fov, 50, 500);

                                        ImGui::Text("Fov Color");
                                        ImGui::SameLine(120);
                                        ImGui::ColorEdit4("##FOV Color", element->bullettrack.fov_color,
                                            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

                                        ImGui::Checkbox("Rainbow FOV", &element->bullettrack.rgb_fov);

                                        if (element->bullettrack.rgb_fov) {
                                            ImGui::SliderFloat("RGB Speed", &element->bullettrack.rgb_speed, 0.5f, 10.0f, "%.1f");
                                        }
                                    }

                                    ImGui::Checkbox("Rotation Sync", &element->bullettrack.rotation_sync);

                                    const char* bones[] = { "Head", "Pelvis" };
                                    ImGui::Combo("Target Bone", &element->bullettrack.bone, bones, 2);

                                    ImGui::Checkbox("Friend Check", &element->bullettrack.team_check);
                                    ImGui::Checkbox("Visible Check", &element->bullettrack.visible_check);
                                    ImGui::Checkbox("Dead Check", &element->bullettrack.deadcheck);

                                    ImGui::Checkbox("Distance Limit", &element->bullettrack.distance_check);
                                    if (element->bullettrack.distance_check) {
                                        ImGui::SliderFloat("Max Distance", &element->bullettrack.max_distance, 10.0f, 300.0f, "%.0fm");
                                    }
                                
                            }
                        }
                    }


                } gui.end_group_box();


                SetCursorPosY(GetCursorPosY() + 5); 

                gui.group_box(" Weapon Tools", ImVec2(700, 405)); {
                 



                    if (element->chams.enabled) {
                        ImGui::ColorEdit4("Visible Color", element->chams.visible_color,
                            ALPHA);

                        ImGui::ColorEdit4("Hidden Color", element->chams.hidden_color,
                           ALPHA);

                        ImGui::Checkbox("Team Check##chams", &element->chams.team_check);

                        ImGui::Checkbox("Distance Check##chams", &element->chams.distance_check);
                        if (element->chams.distance_check) {
                            ImGui::SliderFloat("Max Distance##chams", &element->chams.max_distance, 10.0f, 300.0f, "%.0fm");
                        }
                    }


                    ImGui::Checkbox("Infinite Ammo", &g_Config.g_Combat.bNoReload);
                    ImGui::Checkbox("Player Slapper/Killer", &element->player.slapper);
                    ImGui::Checkbox("No Recoil", &g_Config.g_Combat.bNoRecoil);

                    ImGui::Checkbox("Bomb Shoot", &element->weapon.one_shot);
                    ImGui::Checkbox("One Shoot", &element->weapon.tek_shot);
                    ImGui::Checkbox("Weapon Lagger", &element->weapon.lagger);

                    ImGui::Checkbox("Fast Crosshair", &g_Config.g_Combat.bFastCrosshair);

                    ImGui::Checkbox("Damage Boost", &element->weapon.damage_boost);
                    ImGui::SliderFloat("Damage Boost Amount", &damageBoost, 1.0f, 30.0f, "%.0f");

                    ImGui::Checkbox("Rapid Fire", &element->weapon.rapidfire);
                    ImGui::SliderFloat("Rapid Fire Speed", &element->weapon.rapidfire_speed, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoInput);

                    ImGui::SeparatorText(ICON_FA_BOMB " Weapon Spawner");


                    ImGui::Combo("Weapon", &element->weapon.selected_weapon, element->weapon.weapon_list, 47);
                    ImGui::SliderInt("Ammo", &element->weapon.ammo_count, 1, 1000);
                    if (ImGui::Button("Spawn", ImVec2(670, 25)))
                    {
                        CClientPlayer* local = manager->get_local_player();
                        if (local)
                        {
                            weapon->give_weapon(local, (eWeaponType)element->weapon.selected_weapon, element->weapon.ammo_count, true);
                            const char* weaponName = element->weapon.weapon_list[element->weapon.selected_weapon];
                            std::string message = std::string("Weapon Spawned: "") + weaponName + """);

                            ImGui::InsertNotification({ ImGuiToastType::Success, 3000, message.c_str(), "Weapon System" });
                        }
                    }


                } gui.end_group_box();


              }else if (gui.m_weapon_subtab == 1) {

                  gui.group_box(ICON_FA_CROSSHAIRS " Crosshair Aimbot", ImVec2(320, 430));
                  {
                      ImGui::Checkbox("Enable Crosshair Aimbot", &element->crosshair.state);
                      ImGui::Checkbox("Crosshair Draw FOV", &element->crosshair.b_fov);

                      ImGui::Separator();
                      ImGui::Text("Target Selection");

                      ImGui::Combo("Target Priority", &element->crosshair.target_priority,
                          "FOV Center\0Closest Distance\0Lowest HP\0");

                      ImGui::Combo("Crosshair Target Bone", &element->crosshair.bone,
                          "Head\0Neck\0Upper Torso\0Spine\0Pelvis\0Belly\0Left Hand\0Right Hand\0Left Foot\0Right Foot\0");

                      ImGui::SliderInt("Crosshair FOV", &element->crosshair.i_fov, 15, 1000, nullptr,
                          ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoInput);

                      ImGui::Separator();
                      ImGui::Text("Movement");

                      ImGui::Checkbox("Smooth Movement", &element->crosshair.smooth_movement);
                      if (ImGui::IsItemHovered())
                          ImGui::SetTooltip("Smooth crosshair transition to target");

                      if (element->crosshair.smooth_movement)
                      {

                          ImGui::SliderFloat("Smooth Speed", &element->crosshair.smooth_speed, 10.0f, 20.0f, "%.1f",
                              ImGuiSliderFlags_AlwaysClamp);
                      }

                      ImGui::Separator();
                      ImGui::Text("Filters");

                      ImGui::Checkbox("Visible Check", &element->crosshair.visible_check);
                      ImGui::Checkbox("Dead Check", &element->crosshair.deadcheck);
                      ImGui::Checkbox("Friend Check", &element->crosshair.team_check);

                      ImGui::Checkbox("Distance Check", &element->crosshair.distance_check);
                      if (element->crosshair.distance_check)
                      {

                          ImGui::SliderFloat("Max Distance", &element->crosshair.max_distance, 10.0f, 500.0f, "%.0fm",
                              ImGuiSliderFlags_AlwaysClamp);

                      }

                      ImGui::SliderFloat("Min Target HP", &element->crosshair.min_hp, 0.0f, 100.0f, "%.0f",
                          ImGuiSliderFlags_AlwaysClamp);
                      if (ImGui::IsItemHovered())
                          ImGui::SetTooltip("0 = Disabled, Only target players above this HP");

                  }gui.end_group_box();

                  SameLine();


                gui.group_box(ICON_FA_CROSSHAIRS " Spinbot", ImVec2(350, 350));
                {
                    if (pSpinbot) {
                        ImGui::Checkbox("Enable Spinbot", &pSpinbot->bEnabled);

    

                            ImGui::SliderFloat("Spin Speed", &element->spinbot.spin_speed, 100.0f, 3600.0f, "%.0f deg/s");

                            ImGui::Checkbox("Show FOV", &element->spinbot.b_fov);
                                ImGui::SliderInt("FOV Size", &element->spinbot.i_fov, 50, 500);

         
                                ImGui::ColorEdit4("FOV Color", element->spinbot.fov_color,
                                ALPHA);
                        

                                ImGui::Checkbox("Rainbow FOV", &element->spinbot.rgb_fov);
                                if (element->spinbot.rgb_fov) {
                                    ImGui::SliderFloat("RGB Speed", &element->spinbot.rgb_speed, 0.5f, 10.0f, "%.1f");
                                }

                            const char* bones[] = { "Head", "Pelvis" };
                            ImGui::Combo("Target Bone", &element->spinbot.bone, bones, 2);

                            ImGui::Checkbox("Friend Check", &element->spinbot.team_check);
                            ImGui::Checkbox("Visible Check", &element->spinbot.visible_check);
                            ImGui::Checkbox("Dead Check", &element->spinbot.deadcheck);

                            ImGui::Checkbox("Distance Limit", &element->spinbot.distance_check);
                            if (element->spinbot.distance_check) {
                                ImGui::SliderFloat("Max Distance", &element->spinbot.max_distance, 10.0f, 300.0f, "%.0fm");
                            }

                        
                    }

                }gui.end_group_box();


                gui.group_box(ICON_FA_CROSSHAIRS " Custom Crosshair", ImVec2(350, 180));
                {

                    ImGui::Checkbox("Enable Custom Crosshair", &element->customcrosshair.enabled);

                        ImGui::Checkbox("Use Image", &element->customcrosshair.useImage);

                        if (element->customcrosshair.useImage) {
                            if (ImGui::Button("Load Crosshair Image", ImVec2(-1, 0))) {
                                pCustomCrosshair->OpenFileDialog();
                            }

                            ImGui::SliderFloat("Image Size", &element->customcrosshair.imageSize, 10.0f, 100.0f);

                            if (ImGui::Button("Remove Image", ImVec2(-1, 0))) {
                                pCustomCrosshair->ReleaseCrosshairImage();
                                element->customcrosshair.useImage = false;
                            }
                        }
                        else {
                            // Shape mode
                            const char* types[] = {
                                "Cross (+)",
                                "Circle (○)",
                                "X Shape",
                            };
                            ImGui::Combo("Crosshair Type", &element->customcrosshair.type, types, 3);
                            ImGui::SliderFloat("Size", &element->customcrosshair.size, 2.0f, 30.0f);
                            ImGui::SliderFloat("Thickness", &element->customcrosshair.thickness, 1.0f, 5.0f);
                            ImGui::SliderFloat("Gap", &element->customcrosshair.gap, 0.0f, 20.0f);
                        }

                        ImGui::ColorEdit4("Crosshair Color", element->customcrosshair.color,
                            ALPHA);
                    

                }gui.end_group_box();
              }
                break;

            case 1:                             
              if (gui.m_localplayer_subtab == 0) {
                gui.group_box(ICON_FA_TOOLBOX " General", ImVec2(320, 210)); {

                    static int godmodeType = 0; 
                    const char* godmodeOptions[] = { "Normal", "Semi","Rage"};
                    



                    ImGui::Checkbox("Godmode", &element->player.godmode);

                    ImGui::Combo("Type", &godmodeType, godmodeOptions, IM_ARRAYSIZE(godmodeOptions));

                    if (element->player.godmode)
                    {
                        if (godmodeType == 0 && !godThreadRunning)
                        {
                            c_GodMode = true;
                            godThread = std::thread(GodModeThread);
                        }
                        else if (godmodeType == 1)
                        {
                            c_GodMode = false;

                            if (godThreadRunning && godThread.joinable())
                                godThread.join();

                            element->player.semimode = true;
                        }

                        else if (godmodeType == 2)
                        {
                            c_GodMode = false;

                            if (godThreadRunning && godThread.joinable())
                                godThread.join();

                            element->player.godmode = true;
                        }
                    }
                    else
                    {
                        c_GodMode = false;
                        if (godThreadRunning && godThread.joinable())
                            godThread.join();
                    }

                    ImGui::Separator();

                    ImGui::Checkbox("Mega Jump", &Jump);
                    if (Jump)
                    {
                        DWORD* pMegaJump = (DWORD*)0x96916C;
                        *pMegaJump = 1;
                    }
                    else
                    {
                        DWORD* pMegaJump = (DWORD*)0x96916C;
                        *pMegaJump = 0;
                    }

                    ImGui::Checkbox("Fast Rotation", &activeMoviment);
                    if (activeMoviment == true) {
                        ImGui::SliderInt("Rotation Value", &MovimentVelocityStride, 7, 100);
                    }

                    ImGui::Checkbox("Fast Sprint", &element->player.sprint);
                    if (element->player.sprint) {
                        ImGui::SliderFloat("Sprint Value", &element->player.sprint_speed, 1.0f, 5.0f);
                    }
                    


                    ImGui::Checkbox("Game Speed", &Velocidade);

                    if (Velocidade)
                    {
                        ImGui::SliderFloat("Game Value", &fSpeed, 1.0f, 10.0f);
                        *(float*)Adds_Speed = fSpeed;
                    }
                    else
                    {
                        *(float*)Adds_Speed = 1.0f;
                    }

                    ImGui::Checkbox("Game Time", &bWeatherEnabled);
                    if (bWeatherEnabled)
                    {
                        ImGui::SliderFloat("Time Value", &ClimaGameValue, 1.0f, 20.0f);
                        if (ClimaGameValue != 16)
                        {
                            *ClimaGame = ClimaGameValue;
                        }
                    }





                } gui.end_group_box();

                SameLine(),

                gui.group_box(ICON_FA_WRENCH " Value Settings", ImVec2(375, 210)); {




                    static char inputValueStr[32];
                    sprintf(inputValueStr, "%d", inputValue); 

                    ImGui::PushItemWidth(300.0f);
                    if (ImGui::InputText("Value", inputValueStr, IM_ARRAYSIZE(inputValueStr), ImGuiInputTextFlags_CharsDecimal)) {
                        inputValue = atoi(inputValueStr); 
                    }
                    ImGui::PopItemWidth();

                    if (ImGui::Button("Set Skin", ImVec2(350, 25))) {
                        CClientPlayer* local = manager->get_local_player();
                        if (local) {
                            manager->set_player_skin(local, inputValue);
                        }
                    }


                    if (ImGui::Button("Set Health", ImVec2(350, 25)) || element->player.HealthSet) {
                        element->player.HealthSet = true;
                    }




                    if (ImGui::Button("Set Armor", ImVec2(350, 25))) {
                        element->player.ArmorSet = true;
                    }
                    
                    if (ImGui::Button("Set Oxygen", ImVec2(350, 25))) {
                        CClientPlayer* local = manager->get_local_player();
                        if (local && local->GetGamePlayer())
                        {

                            CPlayerPed* pPlayerPed = local->GetGamePlayer();
                            pPlayerPed->SetOxygenLevel(inputValue);

                        }
                    }                          
                       


                    if (ImGui::Button("Set Dimension", ImVec2(350, 25))) {

                        CClientPlayer* local = manager->get_local_player();
                        CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                        if (local_entity)
                        {
                            local_entity->SetDimension(inputValue);
                        }
                    }                      
                   
                    

                    if (ImGui::Button("Set Interior", ImVec2(350, 25))) {

                        CClientPlayer* local = manager->get_local_player();
                        CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                        if (local_entity)
                        {
                            local_entity->SetInterior(inputValue);

                        }
                    }


                } gui.end_group_box();

                SetCursorPosY(GetCursorPosY() + 5); 
                gui.group_box(" Other", ImVec2(700, 405)); {


                    ImGui::Checkbox("Enabled", &element->player.airbreak);          
                    
                    ImGui::SliderFloat("Airbreak speed", &element->player.airbreak_speed, 1.0f, 30.0f);

                    ImGui::Separator();

                    std::string buttonLabel;
                    if (waitingForKey)
                        buttonLabel = "Press a Key...";
                    else if (airbreakKey != 0)
                        buttonLabel = GetKeyName(airbreakKey);
                    else
                        buttonLabel = "Bind Key";

                    if (ImGui::Button(buttonLabel.c_str(), ImVec2(670, 25)))
                    {
                        waitingForKey = true;
                    }


                    if (waitingForKey)
                    {
                        for (int i = 0; i < 512; i++)
                        {
                            if (GetAsyncKeyState(i) & 0x8000)  
                            {
                                airbreakKey = i;
                                waitingForKey = false;
                            }
                        }
                    }

                    ImGui::SeparatorText(ICON_FA_EARTH_EUROPE " World Properties");
                    static float color[4] = { 1.f, 1.f, 1.f, 1.f };
                    static float ambient[3] = { 1.f, 1.f, 1.f};

                    static float sunCoreColor[3] = { 1.f, 1.f, 1.f };    
                    static float sunCoronaColor[3] = { 1.f, 1.f, 1.f };  


                    static int farClipValue = 320.00; 
                    static int fogDistanceValue = 200.00;


                    static float gravityValue = 0.00f;




                    if (ImGui::SliderFloat("Gravity", &gravityValue, -1.00f, 1.00f, "%.2f")) {
                        global->m_pMultiPlayer->SetLocalPlayerGravity(gravityValue);
                    }                    
                    
               
                    

                    if (ImGui::SliderFloat("Fov Changer", &FovValue, 10.0f, 230.0f, "%.2f")) {
                        fovLoopEnabled = true; 
                        SetFOV(FovValue);      
                    }                    
                    

                    if (ImGui::SliderFloat("Aspect Ratio", &AspectValue, 0.2f, 2.0f, "%.2f")) {
                        aspectLoopEnabled = true; 
                        SetAspectRatio(AspectValue);
                    }




                    if (ImGui::SliderInt("Draw Distance", &farClipValue, 0, 5000)) {
                        global->m_pMultiPlayer->SetFarClipDistance(farClipValue);
                    }

                    if (ImGui::SliderInt("Fog Distance", &fogDistanceValue, 0, 5000)) {
                        global->m_pMultiPlayer->SetFogDistance(fogDistanceValue);
                    }


                    if (ImGui::ColorEdit4("Ambient Color", ambient, ALPHA)) {
                        global->m_pMultiPlayer->SetAmbientColor(
                            static_cast<unsigned char>(ambient[0] * 255),  
                            static_cast<unsigned char>(ambient[1] * 255),  
                            static_cast<unsigned char>(ambient[2] * 255)   
                        );
                    }

                    if (ImGui::ColorEdit4("Water Color", color, ALPHA)) {
                        global->m_pMultiPlayer->SetWaterColor(
                            static_cast<unsigned char>(color[0] * 255),  
                            static_cast<unsigned char>(color[1] * 255),  
                            static_cast<unsigned char>(color[2] * 255),  
                            static_cast<unsigned char>(color[3] * 255)   
                        );
                    }       

                    bool updated = false;

                    updated |= ImGui::ColorEdit3("Weather Top", sunCoreColor,ALPHA);
                    updated |= ImGui::ColorEdit3("Weather Buttom", sunCoronaColor,ALPHA);

                    if (updated)
                    {
                        global->m_pMultiPlayer->SetSkyColor(
                            static_cast<unsigned char>(sunCoreColor[0] * 255),
                            static_cast<unsigned char>(sunCoreColor[1] * 255),
                            static_cast<unsigned char>(sunCoreColor[2] * 255),
                            static_cast<unsigned char>(sunCoronaColor[0] * 255),
                            static_cast<unsigned char>(sunCoronaColor[1] * 255),
                            static_cast<unsigned char>(sunCoronaColor[2] * 255)
                        );
                    }




                } gui.end_group_box();
              }else if (gui.m_localplayer_subtab == 1) {

                  gui.group_box(ICON_FA_GEARS" Config", ImVec2(320, 310)); {
                      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 12));
                      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 8));
                      ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 0.95f, 0.95f, 0.95f));
                      ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.7f, 0.7f, 0.7f, 0.8f));
                      ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85f, 0.85f, 0.85f, 0.8f));
                      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.9f, 0.9f, 0.9f, 0.9f));
                      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));
                      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.85f, 0.85f, 0.9f));

                      ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(ICON_FA_GEARS " Configuration").x) * 0.5f);
                      ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ICON_FA_GEARS " Configuration");
                      ImGui::Spacing();
                      ImGui::Separator();
                      ImGui::Spacing();

                      ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.6f, 1.0f), ICON_FA_HEART " Health Key");
                      ImGui::SameLine(200);
                      key_bind("##HealthKey", element->player.HealthKey, ImVec2(80, 25));
                      ImGui::Spacing();

                      ImGui::TextColored(ImVec4(0.6f, 0.8f, 0.9f, 1.0f), ICON_FA_SHIELD " Armor Key");
                      ImGui::SameLine(200);
                      key_bind("##ArmorKey", element->player.ArmorKey, ImVec2(80, 25));
                      ImGui::Spacing();

                      ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), ICON_FA_CROWN " Godmode Key");
                      ImGui::SameLine(200);
                      key_bind("##GodmodeKey", element->player.godmodekey, ImVec2(80, 25));

                      ImGui::Spacing();
                      ImGui::Spacing();

                      ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(ICON_FA_CHECK " Settings Active").x) * 0.5f);
                      ImGui::TextColored(ImVec4(0.6f, 0.9f, 0.6f, 1.0f), ICON_FA_CHECK " Settings Active");

                      ImGui::PopStyleColor(6);
                      ImGui::PopStyleVar(5);
                  } gui.end_group_box();



              }
                break;

            case 2:
                if (gui.m_visual_subtab == 0) {
                    gui.group_box(ICON_FA_USERS " Player ESP", ImVec2(350, 622)); {
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
                        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 10));
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 8));

                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.9f, 0.9f, 0.9f, 0.8f));
                        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95f, 0.95f, 0.95f, 0.9f));
                        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.4f, 0.6f, 0.8f, 0.8f));
                        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.3f, 0.5f, 0.7f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.6f, 0.6f, 0.6f, 0.8f));

                        const char* visual_items[] = {
                            "Enable",
                            "Name",
                            "Weapon",
                            "2D Box",
                            "Corner",
                            "Filled Box",
                            "Skeleton",
                            "Health",
                            "Armor",
                            "Top Line",
                            "SnapLine",
                            "Show Distance",
                            "Local Player",
                        };

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.5f, 0.8f, 1.0f));
                        ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.8f, 1.0f), ICON_FA_EYE " ESP Status");
                        ImGui::PopStyleColor();

                        ImGui::Spacing();
                        if (ImGui::Checkbox(visual_items[0], &element->esp.state)) {
                        }

                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ICON_FA_PALETTE " Basic Elements");
                        ImGui::Spacing();

                        ImGui::Checkbox(visual_items[1], &element->esp.name);           
                        ImGui::Checkbox(visual_items[2], &element->esp.weapon);         
                        ImGui::Checkbox(visual_items[7], &element->esp.health);         
                        ImGui::Checkbox(visual_items[8], &element->esp.armor);          
                        ImGui::Checkbox(visual_items[11], &element->esp.distance);      
                        ImGui::Checkbox(visual_items[12], &element->esp.localplayer);   

                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ICON_FA_SQUARE " Box & Shapes");
                        ImGui::Spacing();

                        ImGui::Checkbox(visual_items[3], &element->esp.box);            
                        ImGui::Checkbox(visual_items[4], &element->esp.corner);         
                        ImGui::Checkbox(visual_items[6], &element->esp.bone);           
                        ImGui::Checkbox(visual_items[9], &element->esp.line_top);       
                        ImGui::Checkbox(visual_items[10], &element->esp.line_bottom);   
                        ImGui::Checkbox("CHAMS player", &element->esp.yok);


                        

                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.2f, 1.0f), ICON_FA_STAR " Special Features");
                        ImGui::Spacing();

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.6f, 0.2f, 1.0f));
                        ImGui::Checkbox("(soon)", &element->esp.bos);
                        ImGui::PopStyleColor();

                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ICON_FA_SLIDERS " Settings");
                        ImGui::Spacing();

                        ImGui::SliderFloat("Max Distance", &element->esp.max_distance, 0.0f, 5000.0f, "%.1f m");

                        ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::PopStyleColor(6);
                        ImGui::PopStyleVar(5);
                    } gui.end_group_box();

                ImGui::SameLine();  

                gui.group_box(" ESP Colors", ImVec2(330, 622)); {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 8));
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4f, 0.4f, 0.4f, 0.6f));

                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("ESP Color Settings").x) * 0.5f);
                    ImGui::Text("ESP Color Settings");

                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();

                    ImGui::Text("Player Information");
                    ImGui::Spacing();

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Name");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##name", (float*)&element->esp.name_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Weapon");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##weapon", (float*)&element->esp.weapon_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Distance");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##distance", (float*)&element->esp.distance_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();

                    ImGui::Text("Visual Elements");
                    ImGui::Spacing();

                    const char* chamsTypes[] = { "Crystal", "Glow", "Sparkle" };
                    ImGui::Combo("Chams Type", &element->esp.chamsType, chamsTypes, 3); // 3 seçenek


                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Chams Color");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##chamscolor", (float*)&element->esp.chamsVisibleColor, ALPHA);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Box");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##box", (float*)&element->esp.box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Skeleton");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##skeleton", (float*)&element->esp.bone_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Snapline");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##snapline", (float*)&element->esp.line_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Corners");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##corners", (float*)&element->esp.box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();

                    ImGui::Text("Health & Status");
                    ImGui::Spacing();

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Health Bar");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##health", (float*)&element->esp.health_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Armor");
                    ImGui::SameLine(120);
                    ImGui::ColorEdit4("##armor", (float*)&element->esp.armor_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();

                    float buttonWidth = 80.0f;
                    float totalWidth = (buttonWidth * 2) + 10;
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - totalWidth) * 0.5f);

                    if (ImGui::Button("Reset", ImVec2(buttonWidth, 28))) {
                        element->esp.name_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                        element->esp.weapon_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                        element->esp.distance_color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
                        element->esp.box_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                        element->esp.bone_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                        element->esp.line_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                        element->esp.box_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                        element->esp.health_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                        element->esp.armor_color = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
                    }


                    ImGui::PopStyleColor(2);
                    ImGui::PopStyleVar(3);
                } gui.end_group_box();

              }else if (gui.m_visual_subtab == 1) {

                  gui.group_box(ICON_FA_CAR " Vehicle ESP", ImVec2(350, 622)); {
                      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 10));
                      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 8));

                      ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.9f, 0.9f, 0.9f, 0.8f));
                      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95f, 0.95f, 0.95f, 0.9f));
                      ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
                      ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.4f, 0.6f, 0.8f, 0.8f));
                      ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.3f, 0.5f, 0.7f, 1.0f));
                      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.6f, 0.6f, 0.6f, 0.8f));

                      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.5f, 0.8f, 1.0f));
                      ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.8f, 1.0f), ICON_FA_EYE " Vehicle ESP Status");
                      ImGui::PopStyleColor();

                      ImGui::Spacing();
                      if (ImGui::Checkbox("Enable", &element->esp.veh_state)) {
                      }

                      ImGui::Spacing();
                      ImGui::Spacing();

                      ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ICON_FA_PALETTE " Basic Elements");
                      ImGui::Spacing();

                      ImGui::Checkbox("Name ESP", &element->esp.veh_name);
                      ImGui::Checkbox("Distance ESP", &element->esp.veh_distance);
                      ImGui::Checkbox("Engine Status ESP", &element->esp.veh_engine_status);
                      ImGui::Checkbox("Health ESP", &element->esp.veh_health);

                      ImGui::Spacing();
                      ImGui::Spacing();

                      ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ICON_FA_SQUARE " Box & Shapes");
                      ImGui::Spacing();

                      ImGui::Checkbox("Box ESP", &element->esp.veh_box);
                      if (ImGui::Checkbox("3D Box", &element->esp.veh_box_3d)) {
                          if (element->esp.veh_box_3d) {
                              element->esp.veh_box = false;
                          }
                      }
                      ImGui::Checkbox("Line ESP", &element->esp.veh_line);

                      ImGui::Spacing();
                      ImGui::Spacing();

                      ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.2f, 1.0f), ICON_FA_STAR " Special Features");
                      ImGui::Spacing();

                      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.6f, 0.2f, 1.0f));
                      ImGui::Checkbox("(soon)", &element->esp.am);
                      ImGui::PopStyleColor();

                      ImGui::Spacing();
                      ImGui::Spacing();
                      ImGui::Spacing();
                      ImGui::Spacing();
                      ImGui::Spacing();

                      ImGui::PopStyleColor(7);
                      ImGui::PopStyleVar(5);
                  } gui.end_group_box();

                  ImGui::SameLine();

                  gui.group_box(" Vehicle ESP Colors", ImVec2(400, 555)); {
                      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 8));
                      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));

                      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
                      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4f, 0.4f, 0.4f, 0.6f));

                      ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Vehicle ESP Color Settings").x) * 0.5f);
                      ImGui::Text("Vehicle ESP Color Settings");

                      ImGui::Spacing();
                      ImGui::Separator();
                      ImGui::Spacing();

                      ImGui::Text("Vehicle Information");
                      ImGui::Spacing();

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Name");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##veh_name", (float*)&element->esp.veh_name_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Distance");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##veh_distance", (float*)&element->esp.veh_distance_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Health");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##veh_health", (float*)&element->esp.veh_health_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::Spacing();
                      ImGui::Separator();
                      ImGui::Spacing();

                      ImGui::Text("Visual Elements");
                      ImGui::Spacing();

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Box");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##veh_box", (float*)&element->esp.veh_box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Snapline");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##veh_line", (float*)&element->esp.veh_line_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::Spacing();
                      ImGui::Separator();
                      ImGui::Spacing();

                      float buttonWidth = 80.0f;
                      float totalWidth = (buttonWidth * 2) + 10;
                      ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - totalWidth) * 0.5f);

                      if (ImGui::Button("Reset", ImVec2(buttonWidth, 28))) {
                          element->esp.veh_name_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                          element->esp.veh_distance_color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
                          element->esp.veh_health_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                          element->esp.veh_box_color = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
                          element->esp.veh_line_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                      }


                      ImGui::PopStyleColor(2);
                      ImGui::PopStyleVar(3);
                  } gui.end_group_box();
              
              
              }else if (gui.m_visual_subtab == 2) {

                  gui.group_box(ICON_FA_USER " Ped ESP", ImVec2(350, 622)); {
                      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 10));
                      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 8));

                      ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.9f, 0.9f, 0.9f, 0.8f));
                      ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95f, 0.95f, 0.95f, 0.9f));
                      ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
                      ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.4f, 0.6f, 0.8f, 0.8f));
                      ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.3f, 0.5f, 0.7f, 1.0f));
                      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.6f, 0.6f, 0.6f, 0.8f));

                      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.5f, 0.8f, 1.0f));
                      ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.8f, 1.0f), ICON_FA_EYE " Ped ESP Status");
                      ImGui::PopStyleColor();

                      ImGui::Spacing();
                      if (ImGui::Checkbox("Enable", &element->esp.ped_state)) {
                      }

                      ImGui::Spacing();
                      ImGui::Spacing();

                      ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ICON_FA_PALETTE " Basic Elements");
                      ImGui::Spacing();

                      ImGui::Checkbox("Name ESP", &element->esp.ped_name);
                      ImGui::Checkbox("Distance ESP", &element->esp.ped_distance);

                      ImGui::Spacing();
                      ImGui::Spacing();

                      ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ICON_FA_SQUARE " Visual Elements");
                      ImGui::Spacing();

                      ImGui::Checkbox("Corner ESP", &element->esp.ped_corner);
                      ImGui::Checkbox("Skeleton ESP", &element->esp.ped_skeleton);
                      ImGui::Checkbox("Snapline ESP", &element->esp.ped_line);

                      ImGui::Spacing();
                      ImGui::Spacing();
                      ImGui::Spacing();
                      ImGui::Spacing();
                      ImGui::Spacing();
                      ImGui::Spacing();

                      ImGui::PopStyleColor(6);
                      ImGui::PopStyleVar(5);


                  } gui.end_group_box();

                  ImGui::SameLine();

                  gui.group_box(" Ped ESP Colors", ImVec2(400, 555)); {
                      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
                      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 8));
                      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));

                      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
                      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4f, 0.4f, 0.4f, 0.6f));

                      ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Ped ESP Color Settings").x) * 0.5f);
                      ImGui::Text("Ped ESP Color Settings");

                      ImGui::Spacing();
                      ImGui::Separator();
                      ImGui::Spacing();

                      ImGui::Text("Ped Information");
                      ImGui::Spacing();

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Name");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##ped_name", (float*)&element->esp.ped_name_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Distance");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##ped_distance", (float*)&element->esp.ped_distance_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::Spacing();
                      ImGui::Separator();
                      ImGui::Spacing();

                      ImGui::Text("Visual Elements");
                      ImGui::Spacing();

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Box");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##ped_box", (float*)&element->esp.ped_box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Skeleton");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##ped_skeleton", (float*)&element->esp.ped_bone_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::AlignTextToFramePadding();
                      ImGui::Text("Snapline");
                      ImGui::SameLine(120);
                      ImGui::ColorEdit4("##ped_snapline", (float*)&element->esp.ped_line_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      ImGui::Spacing();
                      ImGui::Spacing();
                      ImGui::Separator();
                      ImGui::Spacing();

                      float buttonWidth = 80.0f;
                      float totalWidth = (buttonWidth * 2) + 10;
                      ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - totalWidth) * 0.5f);

                      if (ImGui::Button("Reset", ImVec2(buttonWidth, 28))) {
                          element->esp.ped_name_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                          element->esp.ped_distance_color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
                          element->esp.ped_box_color = ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
                          element->esp.ped_bone_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                          element->esp.ped_line_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                      }


                      ImGui::PopStyleColor(2);
                      ImGui::PopStyleVar(3);
                  } gui.end_group_box();
              }
              
                break;

                 case 3:

                  if (gui.m_misc_subtabs == 0) {
                     gui.group_box(ICON_FA_GEARS " Misc", ImVec2(730, 622));
                     {
                        

                         ImGui::Text(ICON_FA_KEYBOARD " Fake Lag Key:");
                         key_bind("##FakeLagKey", element->player.FakeLagKey, ImVec2(80, 25));

                         ImGui::Spacing();

                         if (ImGui::Checkbox( " Fake Jetpack", &element->player.fakejetpack))
                         {
                             if (element->player.fakejetpack)
                                 SendFakeJetpack(true);
                         }

                         if (ImGui::Checkbox( " Fake Dead", &element->player.fakedead))
                         {
                             if (element->player.fakedead)
                                 SendFakeDead(0.0);
                         }


                         ImGui::Checkbox(" Block Special property [Map Anticheat]", &element->info.bypass_world);
                         ImGui::Checkbox(" DebugHooks Blocker", &element->info.bypass_lua);


                         if (ImGui::Checkbox( " Fake Duck", &element->player.fakeduck))
                         {
                             if (element->player.fakeduck)
                                 SendFakeDuck(true);
                         }

                         ImGui::Spacing();
                         ImGui::Spacing();
                         ImGui::SeparatorText("Teleport Manager");


                         int ix = static_cast<int>(tp_pos.fX);
                         int iy = static_cast<int>(tp_pos.fY);
                         int iz = static_cast<int>(tp_pos.fZ);

                         ImGui::Text(ICON_FA_ARROWS_LEFT_RIGHT " X:");
                         ImGui::SetNextItemWidth(120);
                         ImGui::InputInt("##x_input", &ix, 1, 10);

           
                         ImGui::Text(ICON_FA_ARROWS_UP_DOWN " Y:");
                         ImGui::SetNextItemWidth(120);
                         ImGui::InputInt("##y_input", &iy, 1, 10);

                         ImGui::Text(ICON_FA_UP_DOWN " Z:");
                         ImGui::SetNextItemWidth(120);
                         ImGui::InputInt("##z_input", &iz, 1, 10);

                         tp_pos.fX = static_cast<float>(ix);
                         tp_pos.fY = static_cast<float>(iy);
                         tp_pos.fZ = static_cast<float>(iz);

                         ImGui::Spacing();

                         ImGui::Text(ICON_FA_CROSSHAIRS " Current Position: X: %.1f | Y: %.1f | Z: %.1f", tp_pos.fX, tp_pos.fY, tp_pos.fZ);

                         ImGui::Spacing();

                         if (ImGui::Button(ICON_FA_ROCKET " Teleport Player", ImVec2(200, 35)))
                         {
                             CClientPlayer* local = manager->get_local_player();
                             if (local)
                             {
                                 CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                                 if (local_entity)
                                     local_entity->SetPosition(tp_pos);
                             }
                         }
                     }
                     gui.end_group_box();


                  }else if (gui.m_misc_subtabs == 1) {

                      gui.group_box(ICON_FA_MAP " Radar Manager", ImVec2(730, 622));
                      {
                          ImGui::Checkbox("Enable Radar", &element->radar.EnableRadar);

                          ImGui::SeparatorText("Keybinds");

                          static bool isBindingToggle = false;
                          static bool isBindingHold = false;

                          std::string toggleKeyText = element->radar.RadarToggleKey != 0
                              ? "Toggle Key: " + std::string(1, (char)element->radar.RadarToggleKey)
                              : "Toggle Key: None";

                          std::string holdKeyText = element->radar.RadarHoldKey != 0
                              ? "Hold Key: " + std::string(1, (char)element->radar.RadarHoldKey)
                              : "Hold Key: None";

                          if (ImGui::Button(toggleKeyText.c_str(), ImVec2(200, 0))) {
                              isBindingToggle = true;
                              isBindingHold = false;
                          }
                          if (isBindingToggle) {
                              ImGui::SameLine();
                              ImGui::Text("Press any key...");
                              for (int i = 0x08; i < 0xFE; i++) {
                                  if (GetAsyncKeyState(i) & 0x8000) {
                                      element->radar.RadarToggleKey = i;
                                      isBindingToggle = false;
                                      break;
                                  }
                              }
                          }

                          if (ImGui::Button(holdKeyText.c_str(), ImVec2(200, 0))) {
                              isBindingHold = true;
                              isBindingToggle = false;
                          }
                          if (isBindingHold) {
                              ImGui::SameLine();
                              ImGui::Text("Press any key...");
                              for (int i = 0x08; i < 0xFE; i++) {
                                  if (GetAsyncKeyState(i) & 0x8000) {
                                      element->radar.RadarHoldKey = i;
                                      isBindingHold = false;
                                      break;
                                  }
                              }
                          }

                          ImGui::SeparatorText("Map Settings");
                          ImGui::Combo("Map Position", &element->radar.MapPosition,
                              "Top Left\0Top Right\0Bottom Left\0Bottom Right\0Center\0");

                          ImGui::SliderFloat("Map Size", &element->radar.MapSize, 400.0f, 1200.0f, "%.0f",
                              ImGuiSliderFlags_AlwaysClamp);

                          ImGui::SeparatorText("Blip Options");
                          ImGui::Checkbox("Show Local Player", &element->radar.LocalPlayerBlip);
                          ImGui::Checkbox("Show Players", &element->radar.PlayerBlip);
                          ImGui::Checkbox("Show Vehicles", &element->radar.VehicleBlip);

                          ImGui::SeparatorText("Color Settings");

                          ImGui::Text("Local Player Color");
                          ImGui::SameLine(160);
                          ImGui::ColorEdit4("##LocalPlayerColor", element->radar.LocalPlayerColor,
                              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                          ImGui::Text("Players Color");
                          ImGui::SameLine(160);
                          ImGui::ColorEdit4("##PlayerColor", element->radar.PlayerColor,
                              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                          ImGui::Text("Vehicles Color");
                          ImGui::SameLine(160);
                          ImGui::ColorEdit4("##VehicleColor", element->radar.VehicleColor,
                              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                          ImGui::Text("Name Color");
                          ImGui::SameLine(160);
                          ImGui::ColorEdit4("##NameColor", element->radar.NameColor,
                              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                      }
                      gui.end_group_box();

                  }

                    break;

                 case 4:

                     gui.group_box(ICON_FA_USERS" Player List", ImVec2(350, 622)); {

                         static char search_buf[64] = "";

                         std::vector<CClientPlayer*> players = manager->get_players();
                         CClientPlayer* local = manager->get_local_player();

                         CVector local_pos;
                         if (local)
                         {
                             CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                             if (local_entity)
                                 local_entity->GetPosition(local_pos);
                         }

                         std::sort(players.begin(), players.end(), [&](CClientPlayer* a, CClientPlayer* b) {
                             if (!a || !b) return false;

                             CVector posA, posB;
                             static_cast<CClientEntity*>(a)->GetPosition(posA);
                             static_cast<CClientEntity*>(b)->GetPosition(posB);

                             float distA = sqrtf(powf(posA.fX - local_pos.fX, 2) + powf(posA.fY - local_pos.fY, 2) + powf(posA.fZ - local_pos.fZ, 2));
                             float distB = sqrtf(powf(posB.fX - local_pos.fX, 2) + powf(posB.fY - local_pos.fY, 2) + powf(posB.fZ - local_pos.fZ, 2));
                             return distA < distB;
                             });

                         ImGui::SetNextItemWidth(100);
                         ImGui::InputTextWithHint(ICON_FA_MAGNIFYING_GLASS, "", search_buf, sizeof(search_buf));

                         for (CClientPlayer* player : players)
                         {
                             if (!player)
                                 continue;

                             CClientEntity* entity = static_cast<CClientEntity*>(player);
                             if (!entity)
                                 continue;

                             CVector player_pos;
                             entity->GetPosition(player_pos);

                             float dx = player_pos.fX - local_pos.fX;
                             float dy = player_pos.fY - local_pos.fY;
                             float dz = player_pos.fZ - local_pos.fZ;
                             float distance = sqrtf(dx * dx + dy * dy + dz * dz);

                             std::string base_name = player->GetNametagText();
                             std::string label = base_name + "##" + std::to_string((uintptr_t)player);

                             if (strlen(search_buf) > 0 && base_name.find(search_buf) == std::string::npos)
                                 continue;

                             bool is_friend = aimbot->is_friend(base_name);
                             if (is_friend)
                                 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 1.0f, 0.3f, 1.0f)); 

                             if (ImGui::Selectable(label.c_str(), selected_player == player))
                             {
                                 selected_player = player;
                             }

                             if (is_friend)
                                 ImGui::PopStyleColor();


                             ImGui::SameLine();

                             ImVec4 color;
                             if (distance <= 150.0f)
                                 color = ImVec4(0.3f, 1.0f, 0.3f, 1.0f); 
                             else if (distance <= 500.0f)
                                 color = ImVec4(1.0f, 1.0f, 0.3f, 1.0f); 
                             else
                                 color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); 

                             char distance_text[32];
                             snprintf(distance_text, sizeof(distance_text), "[%.0f m]", distance);

                             ImGui::PushStyleColor(ImGuiCol_Text, color);
                             ImGui::TextUnformatted(distance_text);
                             ImGui::PopStyleColor();
                         }


                     } gui.end_group_box();

                     SameLine(),

                         gui.group_box(ICON_FA_USERS_GEAR " Player Management", ImVec2(360, 622)); {
                         ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
                         ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
                         ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
                         ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));
                         ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 6));

                         ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.16f, 0.16f, 0.21f, 0.9f));
                         ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.20f, 0.20f, 0.25f, 1.0f));
                         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.8f));
                         ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.36f, 0.69f, 1.0f, 0.9f));
                         ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.16f, 0.49f, 0.88f, 1.0f));
                         ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.26f, 0.98f, 0.45f, 1.0f));
                         ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4f, 0.4f, 0.5f, 0.6f));

                         ImVec2 headerPos = ImGui::GetCursorScreenPos();
                         ImVec2 headerSize = ImVec2(ImGui::GetContentRegionAvail().x, 35);
                         ImDrawList* drawList = ImGui::GetWindowDrawList();

                         ImU32 headerBg1 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.20f, 0.20f, 0.25f, 0.9f));
                         ImU32 headerBg2 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.16f, 0.16f, 0.21f, 0.9f));
                         drawList->AddRectFilledMultiColor(headerPos, ImVec2(headerPos.x + headerSize.x, headerPos.y + headerSize.y),
                             headerBg1, headerBg2, headerBg2, headerBg1);
                         drawList->AddRect(headerPos, ImVec2(headerPos.x + headerSize.x, headerPos.y + headerSize.y),
                             ImGui::ColorConvertFloat4ToU32(ImVec4(0.4f, 0.4f, 0.5f, 0.6f)), 6.0f, 0, 1.2f);

                         ImGui::SetCursorScreenPos(ImVec2(headerPos.x + 12, headerPos.y + 8));
                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
                         ImGui::Text(ICON_FA_CROWN " Quick Actions");
                         ImGui::PopStyleColor();

                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + headerSize.y + 8);

                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

                         if (ImGui::Button(ICON_FA_LOCATION_ARROW " Teleport", ImVec2(ImGui::GetContentRegionAvail().x - 4, 30))) {
                             if (selected_player) {
                                 CClientEntity* entity = static_cast<CClientEntity*>(selected_player);
                                 manager->teleport_to_player(entity);
                             }
                         }

                         const char* friendText = selected_player && aimbot->is_friend(selected_player->GetNametagText())
                             ? ICON_FA_USER_MINUS " Remove Friend" : ICON_FA_USER_PLUS " Add Friend";
                         ImVec4 friendColor = selected_player && aimbot->is_friend(selected_player->GetNametagText())
                             ? ImVec4(0.85f, 0.35f, 0.35f, 0.8f) : ImVec4(0.35f, 0.85f, 0.45f, 0.8f);

                         ImGui::PushStyleColor(ImGuiCol_Button, friendColor);
                         ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(friendColor.x + 0.1f, friendColor.y + 0.1f, friendColor.z + 0.1f, 0.9f));

                         if (ImGui::Button(friendText, ImVec2(ImGui::GetContentRegionAvail().x - 4, 30))) {
                             if (selected_player) {
                                 bool is_friend = aimbot->is_friend(selected_player->GetNametagText());
                                 if (is_friend)
                                     aimbot->remove_friend(selected_player->GetNametagText());
                                 else
                                     aimbot->add_friend(selected_player->GetNametagText());
                             }
                         }
                         ImGui::PopStyleColor(2);
                         ImGui::PopStyleColor();

                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
                         if (ImGui::Checkbox(ICON_FA_EYE " Spectate Target", &element->player.spectate)) {
                             if (selected_player) {
                                 CClientPlayer* player = static_cast<CClientPlayer*>(selected_player);
                                 CClientStreamElement* stream = static_cast<CClientStreamElement*>(player);
                                 SpectateEntity(player, stream);
                             }
                         }
                         ImGui::PopStyleColor();

                         ImGui::Spacing();

                         ImVec2 sectionPos = ImGui::GetCursorPos();
                         ImVec2 sectionSize = ImVec2(ImGui::GetContentRegionAvail().x, 100);

                         ImVec2 sectionScreenPos = ImGui::GetCursorScreenPos();
                         ImU32 sectionBg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.08f, 0.08f, 0.08f, 0.9f)); 
                         drawList->AddRectFilled(sectionScreenPos, ImVec2(sectionScreenPos.x + sectionSize.x, sectionScreenPos.y + sectionSize.y), sectionBg, 6.0f);
                         drawList->AddRect(sectionScreenPos, ImVec2(sectionScreenPos.x + sectionSize.x, sectionScreenPos.y + sectionSize.y),
                             ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.2f, 0.2f, 0.6f)), 6.0f, 0, 1.0f);

                         
                         ImGui::SetCursorPos(ImVec2(sectionPos.x + 12, sectionPos.y + 8));
                         ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), ICON_FA_CAR " Vehicle Functions");

                         ImGui::SetCursorPosX(12);
                         if (ImGui::Checkbox("Vehicle Troll", &send_vehicle_checkbox)) {
                             if (send_vehicle_checkbox && selected_player) {
                                 CClientPlayer* target = static_cast<CClientPlayer*>(selected_player);
                                 if (!vehicleTrollTriggered) {
                                     LaunchVehicleAboveTarget(target);
                                     vehicleTrollTriggered = true;
                                 }
                             }
                             else {
                                 vehicleTrollTriggered = false;
                             }
                         }

                         ImGui::SetCursorPosX(12);
                         ImGui::Checkbox("Vehicle Rain", &element->vehicle.vehicle_rain);
                         if (element->vehicle.vehicle_rain && selected_player)
                         {
                             CClientPlayer* target = static_cast<CClientPlayer*>(selected_player);
                             SetRainTarget(target);
                         }

                         static int selectedSeat = 0;
                         static char seatValueStr[32] = "0";
                         static bool seatInitialized = false;
                         if (!seatInitialized) {
                             sprintf(seatValueStr, "%d", selectedSeat);
                             seatInitialized = true;
                         }
                         ImGui::SetCursorPosX(12);
                         ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), "Seat:");
                         ImGui::SameLine(-10);  
                         ImGui::PushItemWidth(40.0f);
                         ImGui::PushID("seat_input"); 
                         if (ImGui::InputText("", seatValueStr, IM_ARRAYSIZE(seatValueStr), ImGuiInputTextFlags_CharsDecimal)) {
                             selectedSeat = atoi(seatValueStr);
                             if (selectedSeat < 0) { selectedSeat = 0; sprintf(seatValueStr, "%d", selectedSeat); }
                             if (selectedSeat > 3) { selectedSeat = 3; sprintf(seatValueStr, "%d", selectedSeat); }
                         }
                         ImGui::PopID();
                         ImGui::PopItemWidth();
                         ImGui::SameLine(45);  
                         if (ImGui::Button(ICON_FA_RIGHT_TO_BRACKET " Enter", ImVec2(120, 25))) {
                             if (selected_player) {
                                 CClientPlayer* target = static_cast<CClientPlayer*>(selected_player);
                                 WarpIntoSelectedPlayerVehicle(target, selectedSeat, true);
                             }
                         }

                         ImGui::SetCursorPosY(sectionPos.y + sectionSize.y + 8);

                         ImVec2 combatPos = ImGui::GetCursorPos();
                         ImVec2 combatSize = ImVec2(ImGui::GetContentRegionAvail().x, 80);
                         ImVec2 combatScreenPos = ImGui::GetCursorScreenPos();

                         ImU32 combatBg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.08f, 0.08f, 0.08f, 0.9f)); 
                         drawList->AddRectFilled(combatScreenPos, ImVec2(combatScreenPos.x + combatSize.x, combatScreenPos.y + combatSize.y), combatBg, 6.0f);
                         drawList->AddRect(combatScreenPos, ImVec2(combatScreenPos.x + combatSize.x, combatScreenPos.y + combatSize.y),
                             ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.2f, 0.2f, 0.6f)), 6.0f, 0, 1.0f);

                         ImGui::SetCursorPos(ImVec2(combatPos.x + 12, combatPos.y + 8));
                         ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), ICON_FA_CROSSHAIRS " Combat Functions");

                         ImGui::SetCursorPosX(12);

                         if (ImGui::Button(ICON_FA_FIRE " Fire Target", ImVec2(150, 25))) {
                             if (selected_player) {
                                 CClientEntity* entity = static_cast<CClientEntity*>(selected_player);
                                 CClientStreamElement* stream = static_cast<CClientStreamElement*>(selected_player);
                                 StartFirePlayer(selected_player, stream);
                             }
                         }

                         bool disableKill = true;
                         CClientPlayer* local = manager->get_local_player();
                         if (local && local->GetGamePlayer()) {
                             for (int slot = 0; slot < 13; ++slot) {
                                 eWeaponType weaponType = manager->myget_weapon_type(local, static_cast<eWeaponSlot>(slot));
                                 if (weaponType != WEAPONTYPE_INVALID && weaponType != WEAPONTYPE_UNARMED) {
                                     disableKill = false;
                                     break;
                                 }
                             }
                         }

                         if (!disableKill && selected_player && local) {
                             CClientPlayer* target = static_cast<CClientPlayer*>(selected_player);

                             if (local->GetGamePlayer() && target->GetGamePlayer()) {
                                 CClientPed* localPed = static_cast<CClientPed*>(local);
                                 CClientPed* targetPed = static_cast<CClientPed*>(target);

                                 CVector localPos, targetPos;
                                 localPed->GetPosition(localPos);
                                 targetPed->GetPosition(targetPos);

                                 float distance = sqrt(pow(targetPos.fX - localPos.fX, 2) +
                                     pow(targetPos.fY - localPos.fY, 2) +
                                     pow(targetPos.fZ - localPos.fZ, 2));

                                 if (distance > 50.0f) {
                                     disableKill = true;
                                 }
                             }
                             else {
                                 disableKill = true; 
                             }
                         }

                         ImGui::SameLine(175);
                         if (disableKill) {
                             ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 0.5f));
                             ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
                         }
                         bool killPressed = ImGui::Button(ICON_FA_SKULL " Kill Target", ImVec2(150, 25));
                         if (disableKill) {
                             ImGui::PopStyleVar();
                             ImGui::PopStyleColor();
                         }
                         if (disableKill && ImGui::IsItemHovered()) {
                             ImGui::BeginTooltip();
                             ImGui::Text("Requires weapon in inventory");
                             if (selected_player && local) {
                                 CClientPlayer* target = static_cast<CClientPlayer*>(selected_player);

                                 if (local->GetGamePlayer() && target->GetGamePlayer()) {
                                     CClientPed* localPed = static_cast<CClientPed*>(local);
                                     CClientPed* targetPed = static_cast<CClientPed*>(target);

                                     CVector localPos, targetPos;
                                     localPed->GetPosition(localPos);
                                     targetPed->GetPosition(targetPos);

                                     float distance = sqrt(pow(targetPos.fX - localPos.fX, 2) +
                                         pow(targetPos.fY - localPos.fY, 2) +
                                         pow(targetPos.fZ - localPos.fZ, 2));

                                     if (distance > 50.0f) {
                                         ImGui::Text("Target too far away (%.1fm)", distance);
                                     }
                                 }
                                 else {
                                     ImGui::Text("Cannot get position data");
                                 }
                             }
                             ImGui::EndTooltip();
                         }
                         if (killPressed && !disableKill && selected_player) {
                             TryCrashWithVehicle(static_cast<CClientPlayer*>(selected_player), false);
                         }

                         ImGui::SetCursorPosY(combatPos.y + combatSize.y + 8);

                         ImVec2 riskyLocalPos = ImGui::GetCursorPos();        
                         ImVec2 riskyScreenPos = ImGui::GetCursorScreenPos();  
                         ImVec2 riskySize = ImVec2(ImGui::GetContentRegionAvail().x, 90);

                         ImU32 riskyBg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.08f, 0.08f, 0.08f, 0.9f)); 
                         drawList->AddRectFilled(riskyScreenPos, ImVec2(riskyScreenPos.x + riskySize.x, riskyScreenPos.y + riskySize.y), riskyBg, 6.0f);
                         drawList->AddRect(riskyScreenPos, ImVec2(riskyScreenPos.x + riskySize.x, riskyScreenPos.y + riskySize.y),
                             ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.2f, 0.2f, 0.6f)), 6.0f, 0, 1.0f);

                         ImGui::SetCursorPos(ImVec2(riskyLocalPos.x + 12, riskyLocalPos.y + 8));
                         ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), ICON_FA_TRIANGLE_EXCLAMATION " Risky Functions");

                         ImGui::SetCursorPos(ImVec2(riskyLocalPos.x + 12, riskyLocalPos.y + 30));

                         if (ImGui::Button(ICON_FA_BOMB " Explode", ImVec2(110, 25))) {
                             if (selected_player) {
                                 CClientEntity* entity = static_cast<CClientEntity*>(selected_player);
                                 CVector pos; entity->GetPosition(pos);
                                 CreateExplosion(pos, static_cast<eExplosionType>(selectedExplosion));
                             }
                         }

                         ImGui::SetCursorPos(ImVec2(riskyLocalPos.x + 135, riskyLocalPos.y + 30));
                         if (ImGui::Button(ICON_FA_HAND " Slap", ImVec2(80, 25))) {
                             if (selected_player) {
                                 CClientEntity* entity = static_cast<CClientEntity*>(selected_player);
                                 CVector pos; entity->GetPosition(pos);
                                 for (int i = 0; i < 10; i++) {
                                     CreateProjectileSync(pos, WEAPONTYPE_TEARGAS);
                                     CreateProjectileSync(pos, WEAPONTYPE_GRENADE);
                                 }
                             }
                         }

                         ImGui::SetCursorPos(ImVec2(riskyLocalPos.x + 12, riskyLocalPos.y + 58));
                         ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), "Type:");
                         ImGui::SetCursorPos(ImVec2(riskyLocalPos.x + 50, riskyLocalPos.y + 56));
                         ImGui::PushItemWidth(200);
                         ImGui::PushID("explosion_combo"); 
                         ImGui::Combo("", &selectedExplosion, explosionTypes, IM_ARRAYSIZE(explosionTypes));
                         ImGui::PopID();
                         ImGui::PopItemWidth();

                         ImGui::SetCursorPosY(riskyLocalPos.y + riskySize.y + 8);

                         
                         ImVec2 specialLocalPos = ImGui::GetCursorPos();        
                         ImVec2 specialScreenPos = ImGui::GetCursorScreenPos();  
                         ImVec2 specialSize = ImVec2(ImGui::GetContentRegionAvail().x, 90);

                         
                         ImU32 specialBg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.08f, 0.08f, 0.08f, 0.9f)); 
                         drawList->AddRectFilled(specialScreenPos, ImVec2(specialScreenPos.x + specialSize.x, specialScreenPos.y + specialSize.y), specialBg, 6.0f);
                         drawList->AddRect(specialScreenPos, ImVec2(specialScreenPos.x + specialSize.x, specialScreenPos.y + specialSize.y),
                             ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.2f, 0.2f, 0.6f)), 6.0f, 0, 1.0f);

                         ImGui::SetCursorPos(ImVec2(specialLocalPos.x + 12, specialLocalPos.y + 8));
                         ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), ICON_FA_STAR " Special Functions");

                         ImGui::SetCursorPos(ImVec2(specialLocalPos.x + 12, specialLocalPos.y + 40));

                         if (ImGui::Button(ICON_FA_COMPASS " Match Dimension", ImVec2(150, 25))) {
                             if (selected_player)
                             {
                                 CClientEntity* entity = static_cast<CClientEntity*>(selected_player);
                                 int targetDimension = entity->GetDimension();
                                 ChangeCurrentDimension(targetDimension);
                             }
                         }

                         ImGui::SameLine(0, 25);
                         if (ImGui::Button(ICON_FA_DOOR_OPEN " Match Interior", ImVec2(150, 25))) {
                             if (selected_player)
                             {
                                 CClientEntity* entity = static_cast<CClientEntity*>(selected_player);
                                 int targetDimension = entity->GetInterior();
                                 CVector InteriorPos;
                                 entity->GetPosition(InteriorPos);
                                 ChangeCurrentInterior(targetDimension, &InteriorPos);
                             }
                         }

                         ImGui::SetCursorPosY(specialLocalPos.y + specialSize.y + 8);

                         ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4f, 0.4f, 0.5f, 0.8f));
                         ImGui::Separator();
                         ImGui::PopStyleColor();

                         ImGui::PopStyleColor(7);
                         ImGui::PopStyleVar(5);
                     } gui.end_group_box();

                     break;

                     case 5:
                     if (gui.m_vehicle_subtab == 0) {

                         gui.group_box("Vehicle Tools", ImVec2(320, 200)); 
                         {
                             if (ImGui::Button("Repair Engine", ImVec2(150, 25)))
                             {
                                 DWORD CCar = *(DWORD*)0xB6F980;
                                 *(float*)(CCar + 1216) = 1000.0f;
                             }
                             ImGui::SameLine();
                             if (ImGui::Button("Set Engine on Fire", ImVec2(148, 25)))
                             {
                                 DWORD CCar = *(DWORD*)0xB6F980;
                                 *(float*)(CCar + 1216) = 0.0f;
                             }

                             ImGui::Spacing();

                             if (ImGui::Button("Turn On Vehicle", ImVec2(150, 25)))
                             {
                                 DWORD* TurnOnVehicle = reinterpret_cast<DWORD*>((*CVehiclee) + 0x428);
                                 *TurnOnVehicle = 26;
                             }
                             ImGui::SameLine();
                             if (ImGui::Button("Turn Off Vehicle", ImVec2(148, 25)))
                             {
                                 DWORD* TurnOffVehicle = reinterpret_cast<DWORD*>((*CVehiclee) + 0x428);
                                 *TurnOffVehicle = 0;
                             }

                             ImGui::Spacing();

                             if (ImGui::Button("Unlock Vehicle", ImVec2(150, 25)))
                             {
                                 DWORD* VeiculoProximo1 = (DWORD*)((*CVehiclee) + 0x47C);
                                 if (*VeiculoProximo1 != 0) {
                                     DWORD* VeiculoProximo2 = (DWORD*)((*VeiculoProximo1) + 0x120);
                                     if (*VeiculoProximo2 != 0) {
                                         DWORD* VeiculoProximo3 = (DWORD*)((*VeiculoProximo2) + 0x4F8);
                                         *VeiculoProximo3 = 1;
                                     }
                                 }
                             }
                             ImGui::SameLine();
                             if (ImGui::Button("Lock Vehicle", ImVec2(148, 25)))
                             {
                                 DWORD* VeiculoProximo1 = (DWORD*)((*CVehiclee) + 0x47C);
                                 if (*VeiculoProximo1 != 0) {
                                     DWORD* VeiculoProximo2 = (DWORD*)((*VeiculoProximo1) + 0x120);
                                     if (*VeiculoProximo2 != 0) {
                                         DWORD* VeiculoProximo3 = (DWORD*)((*VeiculoProximo2) + 0x4F8);
                                         *VeiculoProximo3 = 2;
                                     }
                                 }
                             }

                             ImGui::Spacing();
                             ImGui::Separator();
                             ImGui::Spacing();

                             
                             ImGui::Checkbox("Vehicle Godmode", &VerificationBulletproofEngine);
                             if (VerificationBulletproofEngine)
                             {
                                 DWORD CCar = *(DWORD*)0xB6F980;
                                 *(float*)(CCar + 1216) = 100000000000.0f;
                             }

                             ImGui::Spacing();
                             ImGui::Separator();
                             ImGui::Spacing();


                             static int modelID = 432;
                             static char modelIDStr[16];
                             sprintf(modelIDStr, "%d", modelID);

                             
                             ImGui::Text("Model ID:");
                             ImGui::SameLine();

                             ImGui::PushItemWidth(200);
                             if (ImGui::InputText("", modelIDStr, IM_ARRAYSIZE(modelIDStr), ImGuiInputTextFlags_CharsDecimal)) {
                                 modelID = atoi(modelIDStr);
                             }
                             ImGui::PopItemWidth();



                             if (ImGui::Button("Spoof Model", ImVec2(290, 25)))
                             {

                                 if (modelID >= 400 && modelID <= 611)
                                 {

                                     ChangeModel(modelID);

                                 }
                             }

                         } gui.end_group_box();

                         SameLine(),

                         gui.group_box(" Vehicle Improved", ImVec2(375, 200)); {

                             ImGui::SeparatorText(ICON_FA_CAR " Vehicle Controls");
                             ImGui::Checkbox("Super Break (SPACE)", &SuperBreakCheck);
                             if (SuperBreakCheck)
                             {
                                 SetSuperBreakEnabled(true);
                             }

                             if (ImGui::Button(NitroActived ? "Remove Nitro" : "Add Nitro") && element->content.loaded_client)
                             {
                                 *NitroVeiculo = NitroActived ? 0 : 1;
                                 NitroActived = !NitroActived;
                             }

                             if (ImGui::Button("Drilling Wheels") && element->content.loaded_client)
                             {
                                 DWORD* Pneu1 = reinterpret_cast<DWORD*>((*CVehiclee) + 0x5A5);
                                 DWORD* Pneu2 = reinterpret_cast<DWORD*>((*CVehiclee) + 0x5A6);
                                 DWORD* Pneu3 = reinterpret_cast<DWORD*>((*CVehiclee) + 0x5A7);
                                 DWORD* Pneu4 = reinterpret_cast<DWORD*>((*CVehiclee) + 0x5A8);
                                 *Pneu1 = 1; *Pneu2 = 1; *Pneu3 = 1; *Pneu4 = 1;
                             }

                             ImGui::SeparatorText(ICON_FA_ROCKET " Boost Features");
                             ImGui::Checkbox("Horn Boost (LSHIFT)", &HornBoostCheck);
                             if (HornBoostCheck)
                             {
                                 SetHornBoostEnabled(true);
                             }
                             ImGui::SliderFloat("Horn Boost Speed", &element->vehicle.boost_value, 1.0f, 30.0f, "%.1f");

                             ImGui::Checkbox("Vehicle Slapper", &element->buzina_boost.bBoostActive);
                             ImGui::SliderFloat("Max Speed", &element->buzina_boost.bMaxSpeed, 30.0f, 1000.0f, "%.0f");

                             ImGui::SeparatorText(ICON_FA_CUBE " Physics & Collision");
                             if (ImGui::Checkbox("Remove Vehicle Collision", &element->vehicle.Cool))
                             {
                                 StartCollisionDisabler();
                             }

                       
                             



                         } gui.end_group_box();


                         SetCursorPosY(GetCursorPosY() + 5); 
                         gui.group_box(" Vehicle Other", ImVec2(700, 405));
                         {
                             ImGui::SeparatorText(" Vehicle Airbreak");
                             ImGui::Checkbox("Airbreak", &element->vehicle.airbreak);
                             ImGui::SliderFloat("Airbreak speed", &element->vehicle.airbreak_speed, 1.0f, 30.0f);
                         }


                         
                         float fullWidth = ImGui::GetContentRegionAvail().x;

                         
                         float buttonWidth = (fullWidth - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
                         float buttonHeight = 35.0f;
                         if (ImGui::Button("Explode Near Vehicle", ImVec2(buttonWidth, buttonHeight)))
                         {
                             CClientEntity* local = static_cast<CClientEntity*>(manager->get_local_player());
                             if (!local) return;

                             CVector localPos;
                             local->GetPosition(localPos);

                             auto allVehicles = manager->get_vehicles();
                             for (auto* veh : allVehicles)
                             {
                                 if (!veh) continue;

                                 CVector vehPos;
                                 veh->GetPosition(vehPos);

                                 float dist = (vehPos - localPos).Length();
                                 if (dist <= 200.0f) 
                                 {
                                     CreateExplosion(vehPos, static_cast<eExplosionType>(selectedExplosion));
                                 }
                             }
                         }

                         ImGui::SameLine();
                         if (ImGui::Button("Teleport Nearest Vehicle", ImVec2(buttonWidth, buttonHeight)))
                         {
                             CClientEntity* local = static_cast<CClientEntity*>(manager->get_local_player());
                             if (!local) return;

                             CVector localPos;
                             local->GetPosition(localPos);

                             CClientVehicle* nearestVehicle = nullptr;
                             float nearestDist = FLT_MAX;

                             auto allVehicles = manager->get_vehicles();
                             for (auto* veh : allVehicles)
                             {
                                 if (!veh) continue;

                                 CVector vehPos;
                                 veh->GetPosition(vehPos);

                                 float dist = (vehPos - localPos).Length();
                                 if (dist < nearestDist)
                                 {
                                     nearestDist = dist;
                                     nearestVehicle = veh;
                                 }
                             }

                             if (nearestVehicle && nearestDist <= 300.0f)
                             {
                                 CVector vehPos;
                                 nearestVehicle->GetPosition(vehPos);

                                 local->SetPosition(vehPos);
                             }
                         }

                         
                         if (ImGui::Button("Pull All Vehicles", ImVec2(buttonWidth, buttonHeight)))
                         {
                             CClientEntity* local = static_cast<CClientEntity*>(manager->get_local_player());
                             if (!local)
                                 return;

                             vehiclesToPull.clear();

                             auto allPlayers = manager->get_players();
                             std::unordered_set<CClientVehicle*> occupiedVehicles;

                             for (auto* player : allPlayers)
                             {
                                 if (!player) continue;

                                 CClientVehicle* veh = player->GetOccupiedVehicle();
                                 if (veh && occupiedVehicles.find(veh) == occupiedVehicles.end())
                                 {
                                     vehiclesToPull.push_back(veh);
                                     occupiedVehicles.insert(veh);
                                 }
                             }

                             auto allVehicles = manager->get_vehicles();
                             for (auto* veh : allVehicles)
                             {
                                 if (!veh) continue;
                                 if (occupiedVehicles.find(veh) == occupiedVehicles.end())
                                 {
                                     vehiclesToPull.push_back(veh);
                                 }
                             }

                             vehiclePullIndex = 0;
                             lastPullTime = std::chrono::steady_clock::now();
                         }
                         ImGui::SameLine();
                         if (ImGui::Button("Get Here Nearest Vehicle", ImVec2(buttonWidth, buttonHeight)))
                         {
                             PullNearestVehicleToLocalPlayer();
                         }



                         ImGui::SeparatorText(ICON_FA_CAR "  Basic Vehicle Properties");

                         if (ImGui::SliderFloat("Turn Mass", &TurnMassValue, 1.0f, 90000.0f, "%.1f")) {
                             SetHandling(HANDLING_TURNMASS, TurnMassValue);
                         }
                         if (ImGui::SliderFloat("Mass", &MassValue, 1.0f, 90000.0f, "%.1f")) {
                             SetHandling(HANDLING_MASS, MassValue);
                         }
                         if (ImGui::SliderFloat("Drag Coeff", &DragMulltipler, 0.00f, 90000.0f, "%.2f")) {
                             SetHandling(HANDLING_DRAGCOEFF, DragMulltipler);
                         }
                         if (ImGui::SliderFloat("Monetary Value", &Monetary, 0.0f, 1000000.0f, "%.0f")) {
                             SetHandling(HANDLING_MONETARY, Monetary);
                         }
                         if (ImGui::SliderFloat("Seat Offset Distance", &SeatOffsetDistance, -1.0f, 1.0f, "%.3f")) {
                             SetHandling(HANDLING_SEATOFFSETDISTANCE, SeatOffsetDistance);
                         }
                         if (ImGui::SliderFloat("Collision Damage Multiplier", &CollisionDamageMultiplier, 0.0f, 10.0f, "%.2f")) {
                             SetHandling(HANDLING_COLLISIONDAMAGEMULTIPLIER, CollisionDamageMultiplier);
                         }

                         ImGui::Spacing();


                         ImGui::SeparatorText(ICON_FA_GEARS "  Engine & Transmission");

                         if (ImGui::SliderFloat("Engine Acceleration", &EngineAcceleration, 0.0f, 100.0f, "%.2f")) {
                             SetHandling(HANDLING_ENGINEACCELERATION, EngineAcceleration);
                         }
                         if (ImGui::SliderFloat("Engine Inertia", &EngineInertia, 0.0f, 50.0f, "%.2f")) {
                             SetHandling(HANDLING_ENGINEINERTIA, EngineInertia);
                         }
                         if (ImGui::SliderFloat("Max Velocity", &MaxVelocity, 10.0f, 50000.0f, "%.1f")) {
                             SetHandling(HANDLING_MAXVELOCITY, MaxVelocity);
                         }
                         if (ImGui::SliderFloat("Number of Gears", &NumOfGears, 1.0f, 10.0f, "%.0f")) {
                             SetHandling(HANDLING_NUMOFGEARS, NumOfGears);
                         }

                         ImGui::Spacing();

        
                         ImGui::SeparatorText(ICON_FA_ROAD "  Handling & Traction");

                         if (ImGui::SliderFloat("Traction Multiplier", &TractionMultiplier, 0.1f, 10.0f, "%.2f")) {
                             SetHandling(HANDLING_TRACTIONMULTIPLIER, TractionMultiplier);
                         }
                         if (ImGui::SliderFloat("Traction Loss", &TractionLoss, 0.0f, 10.0f, "%.2f")) {
                             SetHandling(HANDLING_TRACTIONLOSS, TractionLoss);
                         }
                         if (ImGui::SliderFloat("Traction Bias", &TractionBias, 0.0f, 1.0f, "%.2f")) {
                             SetHandling(HANDLING_TRACTIONBIAS, TractionBias);
                         }
                         if (ImGui::SliderFloat("Steering Lock", &SteeringLock, 0.0f, 90.0f, "%.1f")) {
                             SetHandling(HANDLING_STEERINGLOCK, SteeringLock);
                         }

                         ImGui::Spacing();


                         ImGui::SeparatorText(ICON_FA_CIRCLE_STOP "  Braking");

                         if (ImGui::SliderFloat("Brake Deceleration", &BrakeDeceleration, 0.0f, 50.0f, "%.2f")) {
                             SetHandling(HANDLING_BRAKEDECELERATION, BrakeDeceleration);
                         }
                         if (ImGui::SliderFloat("Brake Bias", &BrakeBias, 0.0f, 1.0f, "%.2f")) {
                             SetHandling(HANDLING_BRAKEBIAS, BrakeBias);
                         }

                         ImGui::Spacing();


                         ImGui::SeparatorText(ICON_FA_SPRAY_CAN "  Suspension");

                         if (ImGui::SliderFloat("Suspension Force Level", &SuspensionForceLevel, 0.0f, 10.0f, "%.2f")) {
                             SetHandling(HANDLING_SUSPENSION_FORCELEVEL, SuspensionForceLevel);
                         }
                         if (ImGui::SliderFloat("Suspension Damping", &SuspensionDamping, 0.0f, 10.0f, "%.2f")) {
                             SetHandling(HANDLING_SUSPENSION_DAMPING, SuspensionDamping);
                         }
                         if (ImGui::SliderFloat("Susp. High-Speed Damping", &SuspensionHighSpeedDamping, 0.0f, 10.0f, "%.2f")) {
                             SetHandling(HANDLING_SUSPENSION_HIGHSPEEDDAMPING, SuspensionHighSpeedDamping);
                         }
                         if (ImGui::SliderFloat("Suspension Upper Limit", &SuspensionUpperLimit, 0.0f, 1.0f, "%.3f")) {
                             SetHandling(HANDLING_SUSPENSION_UPPER_LIMIT, SuspensionUpperLimit);
                         }
                         if (ImGui::SliderFloat("Suspension Lower Limit", &SuspensionLowerLimit, -1.0f, 0.0f, "%.3f")) {
                             SetHandling(HANDLING_SUSPENSION_LOWER_LIMIT, SuspensionLowerLimit);
                         }
                         if (ImGui::SliderFloat("Suspension Front-Rear Bias", &SuspensionFrontRearBias, 0.0f, 1.0f, "%.2f")) {
                             SetHandling(HANDLING_SUSPENSION_FRONTREARBIAS, SuspensionFrontRearBias);
                         }
                         if (ImGui::SliderFloat("Suspension Anti-Dive Mult", &SuspensionAntiDiveMultiplier, 0.0f, 10.0f, "%.2f")) {
                             SetHandling(HANDLING_SUSPENSION_ANTIDIVEMULTIPLIER, SuspensionAntiDiveMultiplier);
                         }


                         gui.end_group_box();




                     }
                     else if (gui.m_vehicle_subtab == 1) {
                     
                         gui.group_box(ICON_FA_CAR" Vehicle List", ImVec2(350, 622)); {

                             static char search_buf[64] = "";

                             std::vector<CClientVehicle*> vehicles = manager->get_vehicles();
                             CClientPlayer* local = manager->get_local_player();
                             CVector local_pos;

                             if (local)
                             {
                                 CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                                 if (local_entity)
                                     local_entity->GetPosition(local_pos);
                             }

                             
                             std::sort(vehicles.begin(), vehicles.end(), [&](CClientVehicle* a, CClientVehicle* b) {
                                 if (!a || !b) return false;

                                 CVector posA, posB;
                                 a->GetPosition(posA);
                                 b->GetPosition(posB);

                                 float distA = sqrtf(
                                     (posA.fX - local_pos.fX) * (posA.fX - local_pos.fX) +
                                     (posA.fY - local_pos.fY) * (posA.fY - local_pos.fY) +
                                     (posA.fZ - local_pos.fZ) * (posA.fZ - local_pos.fZ)
                                 );

                                 float distB = sqrtf(
                                     (posB.fX - local_pos.fX) * (posB.fX - local_pos.fX) +
                                     (posB.fY - local_pos.fY) * (posB.fY - local_pos.fY) +
                                     (posB.fZ - local_pos.fZ) * (posB.fZ - local_pos.fZ)
                                 );

                                 return distA < distB;
                                 });

                             ImGui::SetNextItemWidth(100);
                             ImGui::InputTextWithHint(ICON_FA_MAGNIFYING_GLASS, "", search_buf, sizeof(search_buf));
                             for (CClientVehicle* vehicle : vehicles)
                             {
                                 if (!vehicle)
                                     continue;

                                 CVector vehicle_pos;
                                 vehicle->GetPosition(vehicle_pos);

                                 float dx = vehicle_pos.fX - local_pos.fX;
                                 float dy = vehicle_pos.fY - local_pos.fY;
                                 float dz = vehicle_pos.fZ - local_pos.fZ;
                                 float distance = sqrtf(dx * dx + dy * dy + dz * dz);

                                 std::string vehicle_name = vehicle->GetNamePointer();
                                 std::string label = vehicle_name + "##" + std::to_string((uintptr_t)vehicle);

                                 if (strlen(search_buf) > 0 && vehicle_name.find(search_buf) == std::string::npos)
                                     continue;

                                 
                                 if (ImGui::Selectable(label.c_str(), selected_Vehicle == vehicle))
                                 {
                                     selected_Vehicle = vehicle;
                                 }

                                 
                                 ImGui::SameLine();

                                 ImVec4 color;
                                 if (distance <= 150.0f)
                                     color = ImVec4(0.3f, 1.0f, 0.3f, 1.0f); 
                                 else if (distance <= 500.0f)
                                     color = ImVec4(1.0f, 1.0f, 0.3f, 1.0f); 
                                 else
                                     color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); 

                                 char distance_text[32];
                                 snprintf(distance_text, sizeof(distance_text), "[%.0f m]", distance);

                                 ImGui::PushStyleColor(ImGuiCol_Text, color);
                                 ImGui::TextUnformatted(distance_text);
                                 ImGui::PopStyleColor();
                             }

                         } gui.end_group_box();


                         SameLine(),


                             gui.group_box(ICON_FA_GEARS" Vehicle Management", ImVec2(360, 622)); {

                             if (ImGui::Button("Teleport", ImVec2(100, 23)))
                             {
                                 if (selected_Vehicle)
                                 {
                                     CClientVehicle* vehicle = static_cast<CClientVehicle*>(selected_Vehicle);
                                     manager->teleport_to_vehicle(vehicle);
                                 }
                             }     

                             if (ImGui::Button("Explode", ImVec2(100, 23)))
                             {
                                 if (selected_Vehicle)
                                 {
                                     CClientVehicle* vehicle = static_cast<CClientVehicle*>(selected_Vehicle);
                                     CVector pos;
                                     vehicle->GetPosition(pos);
                                     CreateExplosion(pos, static_cast<eExplosionType>(selectedExplosion));

                                 }
                             }                             
                             
                             if (ImGui::Button("Pull Vehicle", ImVec2(100, 23)))
                             {
                                 if (selected_Vehicle)
                                 {
                                     CClientVehicle* vehicle = static_cast<CClientVehicle*>(selected_Vehicle);
                                     SendVehicleTrailerAttach(selected_Vehicle);
                                 }
                             }
                             if (ImGui::Checkbox("Spectate", &element->player.spectate))
                             {
                                 if (selected_Vehicle)
                                 {
                                     CClientVehicle* vehicle = static_cast<CClientVehicle*>(selected_Vehicle);

                                     CClientStreamElement* stream = static_cast<CClientStreamElement*>(vehicle);

                                     SpectateEntity(vehicle, stream);
                                 }
                             }

                             ImGui::SeparatorText("Sync Functions");

                             if (ImGui::Button("Delete Vehicle", ImVec2(100, 23)))
                             {
                                 if (selected_Vehicle)
                                 {
                                     CClientVehicle* vehicle = static_cast<CClientVehicle*>(selected_Vehicle);
                                     SendUnoccupiedVehicleSync(vehicle);
                                 }
                             }                             
                             
                             ImGui::SameLine();

                             if (ImGui::Button("Fire Vehicle", ImVec2(100, 23)))
                             {
                                 if (selected_Vehicle)
                                 {
                                     CClientVehicle* vehicle = static_cast<CClientVehicle*>(selected_Vehicle);
                                     FireVehicle(vehicle);
                                 }
                             }                             
                             
                             if (ImGui::Button("Teleport To Me", ImVec2(100, 23)))
                             {
                                 if (selected_Vehicle)
                                 {
                                     CClientVehicle* vehicle = static_cast<CClientVehicle*>(selected_Vehicle);
                                     PullVehicleToLocalPlayer(vehicle);
                                 }
                             }






                             } gui.end_group_box();

                     }

                     break;

                     case 6:
                         if (gui.m_executor_subtab == 0) {

                             gui.group_box("Runtime Executor", ImVec2(720, 480));
                             {
                                 ImVec2 contentSize = ImGui::GetContentRegionAvail();

                                 ImGui::BeginChild("NoPaddingChild", contentSize, false,
                                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

                                 ImVec2 zeroVec(0.f, 0.f);
                                 ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, zeroVec);
                                 ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, zeroVec);

                                 var->editor.executor.Render("Lua Editor", contentSize);

                                 ImGui::PopStyleVar(2);
                                 ImGui::EndChild();
                             }
                             gui.end_group_box();
                            
                             
                             gui.group_box(" Resource Selector", ImVec2(250, 130)); {


                                 ImGui::InputText("Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));

                                 std::vector<std::string> filteredResources;
                                 for (const auto& res : resources)
                                 {
                                     if (res.find(searchBuffer) != std::string::npos)
                                         filteredResources.push_back(res);
                                 }

                                 std::vector<const char*> items;
                                 for (const auto& res : filteredResources)
                                     items.push_back(res.c_str());

                                 if (!items.empty())
                                 {
                                     int filteredIndex = 0;
                                     for (size_t i = 0; i < filteredResources.size(); ++i)
                                     {
                                         if (filteredResources[i] == resources[selectedResourceIndex])
                                         {
                                             filteredIndex = static_cast<int>(i);
                                             break;
                                         }
                                     }

                                     if (ImGui::Combo("Resource", &filteredIndex, items.data(), static_cast<int>(items.size())))
                                     {
                                         for (size_t i = 0; i < resources.size(); ++i)
                                         {
                                             if (resources[i] == filteredResources[filteredIndex])
                                             {
                                                 selectedResourceIndex = static_cast<int>(i);
                                                 break;
                                             }
                                         }
                                     }
                                 }




                             } gui.end_group_box();

                             ImGui::SameLine();

                             gui.group_box(" ", ImVec2(500, 130)); {

                                 
                                 ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                                 ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.9f, 1.0f), "Script Editor");
                                 ImGui::Separator();
                                 ImGui::Spacing();

                                 
                                 ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
                                 ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 8));

                                 
                                 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.3f, 0.8f));
                                 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.7f, 0.35f, 1.0f));
                                 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.5f, 0.25f, 1.0f));

                                 if (ImGui::Button("Execute", ImVec2(90, 28)))
                                 {
                                     std::string scriptContent = var->editor.executor.GetText();
                                     std::string utf8_script = cp1251_to_utf8(scriptContent.c_str());
                                     int bulunanIndeks = stringIndexBul(resources, Script_Name[selectedResourceIndex]);

                                     if (bulunanIndeks != -1)
                                     {
                                         netc->Send_Script_Packet = true;
                                         LoadScriptFromBuffer(
                                             Call_ECX[bulunanIndeks],
                                             Call_This[bulunanIndeks],
                                             utf8_script.c_str(),
                                             utf8_script.length(),
                                             Script_Name[bulunanIndeks].c_str()
                                         );
                                         ImGui::InsertNotification({
                                             ImGuiToastType::Success,
                                             3000,
                                             "Code Executed Successfully",
                                             "Script has been executed!"
                                             });
                                     }
                                 }
                                 ImGui::PopStyleColor(3);

                                 ImGui::SameLine();

                                 
                                 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.3f, 0.3f, 0.8f));
                                 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.4f, 0.4f, 1.0f));
                                 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));

                                 if (ImGui::Button("Clear", ImVec2(80, 28)))
                                 {
                                     var->editor.executor.SetText("");
                                     ImGui::InsertNotification({
                                         ImGuiToastType::Info,
                                         2000,
                                         "Editor Cleared",
                                         "All code has been removed."
                                         });
                                 }
                                 ImGui::PopStyleColor(3);

                                 ImGui::SameLine();

                                 
                                 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.7f, 0.8f));
                                 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.6f, 0.8f, 1.0f));
                                 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.4f, 0.6f, 1.0f));

                                 if (ImGui::Button("Copy", ImVec2(80, 28)))
                                 {
                                     std::string currentCode = var->editor.executor.GetText();
                                     ImGui::SetClipboardText(currentCode.c_str());
                                     ImGui::InsertNotification({
                                         ImGuiToastType::Info,
                                         2000,
                                         "Code Copied",
                                         "Script copied to clipboard."
                                         });
                                 }
                                 ImGui::PopStyleColor(3);

                                 ImGui::SameLine();

                                 
                                 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.3f, 0.6f, 0.8f));
                                 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.4f, 0.7f, 1.0f));
                                 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.2f, 0.5f, 1.0f));

                                 if (ImGui::Button("Paste", ImVec2(80, 28)))
                                 {
                                     const char* clip = ImGui::GetClipboardText();
                                     if (clip)
                                     {
                                         var->editor.executor.SetText(clip);
                                         ImGui::InsertNotification({
                                             ImGuiToastType::Info,
                                             2000,
                                             "Code Pasted",
                                             "Code pasted from clipboard."
                                             });
                                     }
                                 }
                                 ImGui::PopStyleColor(3);

                                 ImGui::PopStyleVar(2); 

                             } gui.end_group_box();


                         }
                         else if (gui.m_executor_subtab == 1) {

                             gui.group_box("OverWrite Executor", ImVec2(720, 480)); {

                                 ImVec2 contentinfo = ImGui::GetContentRegionAvail();

                                 ImGui::BeginChild("EditorWindow", contentinfo, false,
                                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

                                 ImVec2 zeroVec(0.f, 0.f);
                                 ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, zeroVec);
                                 ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, zeroVec);

                                 std::string currentEditorText = var->editor.OverWrite.GetText();
                                 if (currentEditorText != QMenu::luaCode) {
                                     var->editor.OverWrite.SetText(QMenu::luaCode);
                                 }

                                 var->editor.OverWrite.Render("Lua Editor", contentinfo);

                                 if (QMenu::luaActived12) {
                                     std::string newTextFromEditor = var->editor.OverWrite.GetText();
                                     if (newTextFromEditor != QMenu::luaCode) {
                                         strcpy(QMenu::luaCode, newTextFromEditor.c_str());
                                     }
                                 }

                                 ImGui::PopStyleVar(2);
                                 ImGui::EndChild();

                             } gui.end_group_box();

                             gui.group_box(" Resource Selector", ImVec2(250, 130)); {


                                 ImGui::InputText("Search", kuzaxsearchbuffer, IM_ARRAYSIZE(kuzaxsearchbuffer));

                                 std::vector<std::string> filteredResources;
                                 for (const auto& res : QMenu::resourceList) {
                                     std::string name = ZStringToString(res);
                                     if (name.find(kuzaxsearchbuffer) != std::string::npos)
                                         filteredResources.push_back(name);
                                 }

                                 std::vector<const char*> items;
                                 for (const auto& res : filteredResources)
                                     items.push_back(res.c_str());

                                 if (!items.empty()) {
                                     int filteredIndex = 0;
                                     for (size_t i = 0; i < filteredResources.size(); ++i) {
                                         if (filteredResources[i] == ZStringToString(QMenu::selectedResource)) {
                                             filteredIndex = static_cast<int>(i);
                                             break;
                                         }
                                     }

                                     if (ImGui::Combo("Resource", &filteredIndex, items.data(), static_cast<int>(items.size()))) {
                                         for (size_t i = 0; i < QMenu::resourceList.size(); ++i) {
                                             if (ZStringToString(QMenu::resourceList[i]) == filteredResources[filteredIndex]) {
                                                 QMenu::selectedResource = QMenu::resourceList[i];
                                                 HandleResourceSelectionChange(QMenu::selectedResource);
                                                 break;
                                             }
                                         }
                                     }
                                 }


                             }gui.end_group_box();

                             ImGui::SameLine();


                             gui.group_box(" ", ImVec2(460, 130)); {

                                 ImGui::Checkbox("Execute (Need Reconnect)", &QMenu::luaActived12);
                             }gui.end_group_box();

                         }

                     break;

                     case 7:
                     {
                         float scriptList_start_y = ImGui::GetCursorPosY();

                         gui.group_box(" Dump Options", ImVec2(220, 140));
                         {
                             static int selected_mode = element->dump.deobfuscate ? 1 : 0;
                             const char* modes[] = { "Input (Recommended)", "Output - Method" };

                             if (ImGui::Combo("Dump Mode", &selected_mode, modes, IM_ARRAYSIZE(modes)))
                             {
                                 element->dump.deobfuscate = (selected_mode == 1);
                             }

                             ImGui::Checkbox("Dump Scripts (Reconnect)", &element->dump.dump_enabled);

                             if (ImGui::Button("Runtime Dumper"))
                             {
                                 netc->DumpAllScripts();
                             }
                         }
                         gui.end_group_box();


                         static ImGuiTextFilter dump_filter;

                         gui.group_box(" Script List", ImVec2(220, 480)); {

                             dump_filter.Draw("Search"); 

                             std::vector<const char*> filtered_items;
                             for (const auto& dump : element->dump.dumps_list)
                             {
                                 if (dump_filter.PassFilter(dump.script_name.c_str()))
                                 {
                                     filtered_items.push_back(dump.script_name.c_str());
                                 }
                             }

                             ImGui::PushItemWidth(250);
                             ImGui::ListBox("DumpsList", &element->dump.item_current, filtered_items.data(), filtered_items.size(), 30);
                             ImGui::PopItemWidth();

                         } gui.end_group_box();



                         ImGui::SameLine();

                         float desired_y = scriptList_start_y - 2.0f;  

                         ImGui::SetCursorPosY(desired_y);

                         static ImGuiTextFilter lua_code_filter;

                         gui.group_box(" Lua Code", ImVec2(496, 480)); {

                             lua_code_filter.Draw("Search");

                             if (element->dump.item_current >= 0 && element->dump.item_current < element->dump.dumps_list.size())
                             {
                                 const auto& selected_dump = element->dump.dumps_list[element->dump.item_current];

                                 if (!selected_dump.is_compiled)
                                 {
                                     std::string script_text(selected_dump.buffer.begin(), selected_dump.buffer.end());

                                     ImVec2 contentSize = ImGui::GetContentRegionAvail();
                                     var->editor.dumper.SetText(script_text);

                                     const char* filter = lua_code_filter.InputBuf;

                                     if (filter[0] != '\0')
                                     {
      
                                         std::string lowered_script = script_text;
                                         std::string lowered_filter = filter;

                                         std::transform(lowered_script.begin(), lowered_script.end(), lowered_script.begin(), ::tolower);
                                         std::transform(lowered_filter.begin(), lowered_filter.end(), lowered_filter.begin(), ::tolower);

                                         size_t found = lowered_script.find(lowered_filter);
                                         if (found != std::string::npos)
                                         {
                                             int line = 0, column = 0;
                                             for (size_t i = 0; i < found; ++i)
                                             {
                                                 if (script_text[i] == '\n')
                                                 {
                                                     line++;
                                                     column = 0;
                                                 }
                                                 else
                                                 {
                                                     column++;
                                                 }
                                             }

                                             TextEditor::Coordinates start(line, column);
                                             TextEditor::Coordinates end(line, column + strlen(filter));

                                             var->editor.dumper.SetSelection(start, end);
                                             var->editor.dumper.SetCursorPosition(start);
                                         }
                                     }

                                     var->editor.dumper.Render("Lua Editor", contentSize);
                                 }
                                 else
                                 {
                                     ImGui::TextWrapped("This script is decompiled.");
                                 }

                             }
                             else
                             {
                                 ImGui::TextDisabled("No script selected.");
                             }

                         } gui.end_group_box();

                         ImGui::SetCursorPosY(desired_y + 490.0f); 
                         ImGui::SetCursorPosX(229.0f);             

                         gui.group_box("Script Options", ImVec2(490, 125)); {

                             const int sel = element->dump.item_current;
                             if (sel >= 0 && sel < (int)element->dump.dumps_list.size())
                             {
                                 const auto& selected_dump = element->dump.dumps_list[sel];
                                 const std::string& scriptName = selected_dump.script_name;

                                 ImGui::Text("Script Selected: %s", scriptName.c_str());
                                 ImGui::Separator();

                                 bool isBlocked = scriptsManager->IsScriptBlocked(scriptName);
                                 bool prevBlocked = isBlocked;

                                 if (ImGui::Checkbox("Undetected Block Script(Reconnect)", &isBlocked)) {
                                     if (isBlocked && !prevBlocked) {
                                         scriptsManager->BlockScript(scriptName);
                                     }
                                     else if (!isBlocked && prevBlocked) {
                                         scriptsManager->UnblockScript(scriptName);
                                     }
                                 }
                             }
                             else
                             {
                                 ImGui::TextDisabled("No script selected.");
                             }

                         } gui.end_group_box();


                         break;
                     }
                         case 8:

                             gui.group_box(ICON_FA_SERVER " Resource List", ImVec2(280, 622));
                             {
                                 std::vector<const char*> items;



                                 for (size_t i = 0; i < element->resource.resources_list.size(); i++)
                                 {
                                     const auto& resource = element->resource.resources_list[i];

                                     if (strlen(element->resource.search_input) == 0 ||
                                         std::string(resource.resource_name).find(element->resource.search_input) != std::string::npos)
                                     {
                                         items.push_back(resource.resource_name.c_str());
                                         original_indices.push_back(i);
                                     }
                                 }

                                 ImGui::InputText("Search", element->resource.search_input, sizeof(element->resource.search_input));
                                 ImGui::SetNextItemWidth(400.0f); 
                                 ImGui::ListBox("", &element->resource.item_current, items.data(), static_cast<int>(items.size()), 22);

                             } gui.end_group_box();


                             SameLine(),

                             gui.group_box(ICON_FA_GEAR " Resource Manager", ImVec2(430, 622));
                             {
                                 bool has_selection = (element->resource.item_current >= 0 && element->resource.item_current < original_indices.size());

                                 if (ImGui::Button("Delete Resource", ImVec2(ImGui::GetContentRegionAvail().x, 30)))
                                 {
                                     if (element->content.loaded_client && has_selection)
                                     {
                                         int selected_index = original_indices[element->resource.item_current];
                                         const auto& selected_resource = element->resource.resources_list[selected_index];

                                         client->stop_resource(selected_resource.net_id);
                                     }
                                 }

                                 if (has_selection)
                                 {
                                     int selected_index = original_indices[element->resource.item_current];
                                     const auto& selected_resource = element->resource.resources_list[selected_index];
                                     ImGui::Separator();
                                     ImGui::TextDisabled("Resource usNetID: %u", selected_resource.net_id);

                                 }
                                 else
                                 {
                                     ImGui::Separator();
                                     ImGui::TextDisabled("No resource selected.");
                                 }
                             }
                             gui.end_group_box();


                             break;

                         case 9:
                         if (gui.m_luafunc_subtab == 0) {
                             gui.group_box(ICON_FA_SERVER " Captured Triggers", ImVec2(280, 622));
                             {
          
                                 ImGui::InputText("Event Name", input_buffer, sizeof(input_buffer));

                                 std::vector<std::string> items;
                                 for (const auto& event : element->event.events_list)
                                 {
                                     std::string label = event.event_name;
                                     if (event.is_blocked)
                                         label += " (blocked)";
                                     items.push_back(label);
                                 }

                                 std::vector<const char*> cstr_items;
                                 for (const auto& item : items)
                                     cstr_items.push_back(item.c_str());

                                 ImGui::SetNextItemWidth(400.0f);
                                 ImGui::ListBox("##EventsItem", &element->event.item_current, cstr_items.data(), cstr_items.size(), 20);


                             } gui.end_group_box();

                             SameLine(),
                             gui.group_box(ICON_FA_GEAR " Trigger Manager", ImVec2(430, 622));
                             {

                                 ImGui::TextColored(ImVec4(1, 0.8f, 0.2f, 1), ICON_FA_BAN " Blacklist Manager");

                                 ImGui::InputText("Blacklist Event", element->event.input_buffer, sizeof(element->event.input_buffer));
                                 if (ImGui::Button(" Add Trigger To Blacklist", ImVec2(402, 25)))
                                 {
                                     if (strlen(element->event.input_buffer) > 0)
                                     {
                                         element->event.blacklist_words.push_back(element->event.input_buffer);
                                         element->event.input_buffer[0] = '\0';
                                     }
                                 }


                                 if (ImGui::BeginListBox("BlacklistWords", ImVec2(-FLT_MIN, 200)))
                                 {
                                     for (int i = 0; i < element->event.blacklist_words.size(); i++)
                                     {
                                         const std::string& word = element->event.blacklist_words[i];
                                         bool selected = (element->event.item_current == i);

                                         if (ImGui::Selectable(word.c_str(), selected))
                                             element->event.item_current = i;
                                     }
                                     ImGui::EndListBox();
                                 }

                                 if (element->event.item_current >= 0 && element->event.item_current < element->event.blacklist_words.size())
                                 {
                                     if (ImGui::Button(ICON_FA_TRASH " Remove", ImVec2(-FLT_MIN, 30)))
                                     {
                                         element->event.blacklist_words.erase(element->event.blacklist_words.begin() + element->event.item_current);
                                         element->event.item_current = -1;
                                     }
                                 }

                                 ImGui::Separator();

                                 ImGui::Text( ICON_FA_SERVER " Selected Event Details");
                                 if (element->event.item_current >= 0 && element->event.item_current < element->event.events_list.size())
                                 {
                                     auto& selected_event = element->event.events_list[element->event.item_current];


                                     ImGui::TextColored(ImVec4(0.25f, 0.5f, 1.0f, 1.0f), "Event Name:");
                                     ImGui::SameLine();
                                     ImGui::TextWrapped("%s", selected_event.event_name.c_str());



                  
                                     ImVec4 status_color = selected_event.is_blocked
                                         ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f) 
                                         : ImVec4(0.2f, 1.0f, 0.2f, 1.0f); 
                                     ImGui::TextColored(status_color, "Status:");
                                     ImGui::SameLine();
                                     ImGui::Text("%s", selected_event.is_blocked ? "Blocked" : "Allowed");


                                     std::string button_label = selected_event.is_blocked ? ICON_FA_UNLOCK " Unblock" : ICON_FA_LOCK " Block";
                                     if (ImGui::Button(button_label.c_str(), ImVec2(-FLT_MIN, 35)))
                                     {
                                         selected_event.is_blocked = !selected_event.is_blocked;
                                     }
                                 }
                                 else
                                 {
                                     ImGui::TextDisabled("No event selected.");
                                 }

                             }
                             gui.end_group_box();



                         }
                         else if (gui.m_luafunc_subtab == 1) {



                             gui.group_box(ICON_FA_SERVER " Captured Client Events", ImVec2(280, 622));
                             {
                                 ImGui::InputText("Event Name", input_buffer, sizeof(input_buffer));

                                 std::vector<std::string> items;
                                 for (const auto& event : element->triggerEvent.events_list)
                                 {
                                     std::string label = event.event_name;
                                     if (event.is_blocked)
                                         label += " (blocked)";
                                     items.push_back(label);
                                 }

                                 std::vector<const char*> cstr_items;
                                 for (const auto& item : items)
                                     cstr_items.push_back(item.c_str());

                                 ImGui::SetNextItemWidth(400.0f);
                                 ImGui::ListBox("##EventsItem", &element->triggerEvent.item_current, cstr_items.data(), cstr_items.size(), 20);


                             } gui.end_group_box();



                             SameLine(),


                             gui.group_box(ICON_FA_GEAR " Client Event Manager", ImVec2(430, 622));
                             {

                                 ImGui::TextColored(ImVec4(1, 0.8f, 0.2f, 1), ICON_FA_BAN " Blacklist Manager");

                                 ImGui::InputText("Blacklist Event", element->triggerEvent.input_buffer, sizeof(element->triggerEvent.input_buffer));
                                 if (ImGui::Button(" Add ClientEvent To Blacklist", ImVec2(402, 25)))
                                 {
                                     if (strlen(element->triggerEvent.input_buffer) > 0)
                                     {
                                         element->triggerEvent.blacklist_words.push_back(element->triggerEvent.input_buffer);
                                         element->triggerEvent.input_buffer[0] = '\0';
                                     }
                                 }


                                 if (ImGui::BeginListBox("BlacklistWords", ImVec2(-FLT_MIN, 200)))
                                 {
                                     for (int i = 0; i < element->triggerEvent.blacklist_words.size(); i++)
                                     {
                                         const std::string& word = element->triggerEvent.blacklist_words[i];
                                         bool selected = (element->triggerEvent.item_current == i);

                                         if (ImGui::Selectable(word.c_str(), selected))
                                             element->triggerEvent.item_current = i;
                                     }
                                     ImGui::EndListBox();
                                 }

                                 if (element->triggerEvent.item_current >= 0 && element->triggerEvent.item_current < element->triggerEvent.blacklist_words.size())
                                 {
                                     if (ImGui::Button(ICON_FA_TRASH " Remove", ImVec2(-FLT_MIN, 30)))
                                     {
                                         element->triggerEvent.blacklist_words.erase(element->triggerEvent.blacklist_words.begin() + element->triggerEvent.item_current);
                                         element->triggerEvent.item_current = -1;
                                     }
                                 }

                                 ImGui::Separator();

                                 ImGui::Text(ICON_FA_SERVER " Selected Event Details");
                                 if (element->triggerEvent.item_current >= 0 && element->triggerEvent.item_current < element->triggerEvent.events_list.size())
                                 {
                                     auto& selected_event = element->triggerEvent.events_list[element->triggerEvent.item_current];


                                     ImGui::TextColored(ImVec4(0.25f, 0.5f, 1.0f, 1.0f), "Event Name:");
                                     ImGui::SameLine();
                                     ImGui::TextWrapped("%s", selected_event.event_name.c_str());




                                     ImVec4 status_color = selected_event.is_blocked
                                         ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f)
                                         : ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
                                     ImGui::TextColored(status_color, "Status:");
                                     ImGui::SameLine();
                                     ImGui::Text("%s", selected_event.is_blocked ? "Blocked" : "Allowed");


                                     std::string button_label = selected_event.is_blocked ? ICON_FA_UNLOCK " Unblock" : ICON_FA_LOCK " Block";
                                     if (ImGui::Button(button_label.c_str(), ImVec2(-FLT_MIN, 35)))
                                     {
                                         selected_event.is_blocked = !selected_event.is_blocked;
                                     }
                                 }
                                 else
                                 {
                                     ImGui::TextDisabled("No event selected.");
                                 }

                             }
                             gui.end_group_box();


                         }
                         else if (gui.m_luafunc_subtab == 2) {




                             gui.group_box(ICON_FA_SERVER " Captured AddEvents", ImVec2(280, 622));
                             {
                                 ImGui::InputText("AddEvent Name", input_buffer, sizeof(input_buffer));

                                 std::vector<std::string> items;
                                 for (const auto& event : element->addevent.events_list)
                                 {
                                     std::string label = event.event_name;
                                     if (event.is_blocked)
                                         label += " (blocked)";
                                     items.push_back(label);
                                 }

                                 std::vector<const char*> cstr_items;
                                 for (const auto& item : items)
                                     cstr_items.push_back(item.c_str());

                                 ImGui::SetNextItemWidth(400.0f);
                                 ImGui::ListBox("##EventsItem", &element->addevent.item_current, cstr_items.data(), cstr_items.size(), 20);


                             } gui.end_group_box();



                             SameLine(),


                              gui.group_box(ICON_FA_GEAR " AddEvent Manager", ImVec2(430, 622));
                             {

                                 ImGui::TextColored(ImVec4(1, 0.8f, 0.2f, 1), ICON_FA_BAN " Blacklist Manager");

                                 ImGui::InputText("Blacklist Event", element->addevent.input_buffer, sizeof(element->addevent.input_buffer));
                                 if (ImGui::Button(" Add AddEvent To Blacklist", ImVec2(402, 25)))
                                 {
                                     if (strlen(element->addevent.input_buffer) > 0)
                                     {
                                         element->addevent.blacklist_words.push_back(element->addevent.input_buffer);
                                         element->addevent.input_buffer[0] = '\0';
                                     }
                                 }


                                 if (ImGui::BeginListBox("BlacklistWords", ImVec2(-FLT_MIN, 200)))
                                 {
                                     for (int i = 0; i < element->addevent.blacklist_words.size(); i++)
                                     {
                                         const std::string& word = element->addevent.blacklist_words[i];
                                         bool selected = (element->addevent.item_current == i);

                                         if (ImGui::Selectable(word.c_str(), selected))
                                             element->addevent.item_current = i;
                                     }
                                     ImGui::EndListBox();
                                 }

                                 if (element->addevent.item_current >= 0 && element->addevent.item_current < element->addevent.blacklist_words.size())
                                 {
                                     if (ImGui::Button(ICON_FA_TRASH " Remove", ImVec2(-FLT_MIN, 30)))
                                     {
                                         element->addevent.blacklist_words.erase(element->addevent.blacklist_words.begin() + element->addevent.item_current);
                                         element->addevent.item_current = -1;
                                     }
                                 }

                                 ImGui::Separator();

                                 ImGui::Text(ICON_FA_SERVER " Selected Event Details");
                                 if (element->addevent.item_current >= 0 && element->addevent.item_current < element->addevent.events_list.size())
                                 {
                                     auto& selected_event = element->addevent.events_list[element->addevent.item_current];


                                     ImGui::TextColored(ImVec4(0.25f, 0.5f, 1.0f, 1.0f), "Event Name:");
                                     ImGui::SameLine();
                                     ImGui::TextWrapped("%s", selected_event.event_name.c_str());





                                     ImVec4 status_color = selected_event.is_blocked
                                         ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f)
                                         : ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
                                     ImGui::TextColored(status_color, "Status:");
                                     ImGui::SameLine();
                                     ImGui::Text("%s", selected_event.is_blocked ? "Blocked" : "Allowed");

                                     
                                     std::string button_label = selected_event.is_blocked ? ICON_FA_UNLOCK " Unblock" : ICON_FA_LOCK " Block";
                                     if (ImGui::Button(button_label.c_str(), ImVec2(-FLT_MIN, 35)))
                                     {
                                         selected_event.is_blocked = !selected_event.is_blocked;
                                     }
                                 }
                                 else
                                 {
                                     ImGui::TextDisabled("No event selected.");
                                 }

                             }
                             gui.end_group_box();







                         }
                         else if (gui.m_luafunc_subtab == 3) {





                             gui.group_box(ICON_FA_SERVER " Captured EventHandler", ImVec2(280, 622));
                             {
                                 ImGui::InputText("EventHandler Name", input_buffer, sizeof(input_buffer));

                                 std::vector<std::string> items;
                                 for (const auto& event : element->addeventhandler.events_list)
                                 {
                                     std::string label = event.event_name;
                                     if (event.is_blocked)
                                         label += " (blocked)";
                                     items.push_back(label);
                                 }

                                 std::vector<const char*> cstr_items;
                                 for (const auto& item : items)
                                     cstr_items.push_back(item.c_str());

                                 ImGui::SetNextItemWidth(400.0f);
                                 ImGui::ListBox("##EventsItem", &element->addeventhandler.item_current, cstr_items.data(), cstr_items.size(), 20);


                             } gui.end_group_box();



                             SameLine(),


                             gui.group_box(ICON_FA_GEAR " EventHandler Manager", ImVec2(430, 622));
                             {

                                 ImGui::TextColored(ImVec4(1, 0.8f, 0.2f, 1), ICON_FA_BAN " Blacklist Manager");

                                 ImGui::InputText("Blacklist Event", element->addeventhandler.input_buffer, sizeof(element->addeventhandler.input_buffer));
                                 if (ImGui::Button(" Add EventHandler To Blacklist", ImVec2(402, 25)))
                                 {
                                     if (strlen(element->addeventhandler.input_buffer) > 0)
                                     {
                                         element->addeventhandler.blacklist_words.push_back(element->addeventhandler.input_buffer);
                                         element->addeventhandler.input_buffer[0] = '\0';
                                     }
                                 }


                                 if (ImGui::BeginListBox("BlacklistWords", ImVec2(-FLT_MIN, 200)))
                                 {
                                     for (int i = 0; i < element->addeventhandler.blacklist_words.size(); i++)
                                     {
                                         const std::string& word = element->addeventhandler.blacklist_words[i];
                                         bool selected = (element->addeventhandler.item_current == i);

                                         if (ImGui::Selectable(word.c_str(), selected))
                                             element->addeventhandler.item_current = i;
                                     }
                                     ImGui::EndListBox();
                                 }

                                 if (element->addeventhandler.item_current >= 0 && element->addeventhandler.item_current < element->addeventhandler.blacklist_words.size())
                                 {
                                     if (ImGui::Button(ICON_FA_TRASH " Remove", ImVec2(-FLT_MIN, 30)))
                                     {
                                         element->addeventhandler.blacklist_words.erase(element->addeventhandler.blacklist_words.begin() + element->addeventhandler.item_current);
                                         element->addeventhandler.item_current = -1;
                                     }
                                 }

                                 ImGui::Separator();

                                 ImGui::Text(ICON_FA_SERVER " Selected Event Details");
                                 if (element->addeventhandler.item_current >= 0 && element->addeventhandler.item_current < element->addeventhandler.events_list.size())
                                 {
                                     auto& selected_event = element->addeventhandler.events_list[element->addeventhandler.item_current];


                                     ImGui::TextColored(ImVec4(0.25f, 0.5f, 1.0f, 1.0f), "Event Name:");
                                     ImGui::SameLine();
                                     ImGui::TextWrapped("%s", selected_event.event_name.c_str());





                                     ImVec4 status_color = selected_event.is_blocked
                                         ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f)
                                         : ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
                                     ImGui::TextColored(status_color, "Status:");
                                     ImGui::SameLine();
                                     ImGui::Text("%s", selected_event.is_blocked ? "Blocked" : "Allowed");


                                     std::string button_label = selected_event.is_blocked ? ICON_FA_UNLOCK " Unblock" : ICON_FA_LOCK " Block";
                                     if (ImGui::Button(button_label.c_str(), ImVec2(-FLT_MIN, 35)))
                                     {
                                         selected_event.is_blocked = !selected_event.is_blocked;
                                     }
                                 }
                                 else
                                 {
                                     ImGui::TextDisabled("No event selected.");
                                 }

                             }
                             gui.end_group_box();









                         }
                         else if (gui.m_luafunc_subtab == 4) {




                             gui.group_box(ICON_FA_SERVER " Captured MtaFunc", ImVec2(280, 622));
                             {
                                 ImGui::InputText("MtaFunc Name", input_buffer, sizeof(input_buffer));

                                 std::vector<std::string> items;
                                 for (const auto& event : element->debugcall.events_list)
                                 {
                                     std::string label = event.event_name;
                                     if (event.is_blocked)
                                         label += " (blocked)";
                                     items.push_back(label);
                                 }

                                 std::vector<const char*> cstr_items;
                                 for (const auto& item : items)
                                     cstr_items.push_back(item.c_str());

                                 ImGui::SetNextItemWidth(400.0f);
                                 ImGui::ListBox("##EventsItem", &element->debugcall.item_current, cstr_items.data(), cstr_items.size(), 20);


                             } gui.end_group_box();



                             SameLine(),


                             gui.group_box(ICON_FA_GEAR " MtaFunc Manager", ImVec2(430, 622));
                             {

                                 ImGui::TextColored(ImVec4(1, 0.8f, 0.2f, 1), ICON_FA_BAN " Blacklist Manager");

                                 ImGui::InputText("Blacklist Event", element->debugcall.input_buffer, sizeof(element->debugcall.input_buffer));
                                 if (ImGui::Button(" Add Mta Functions To Blacklist", ImVec2(402, 25)))
                                 {
                                     if (strlen(element->debugcall.input_buffer) > 0)
                                     {
                                         element->debugcall.blacklist_words.push_back(element->debugcall.input_buffer);
                                         element->debugcall.input_buffer[0] = '\0';
                                     }
                                 }


                                 if (ImGui::BeginListBox("BlacklistWords", ImVec2(-FLT_MIN, 200)))
                                 {
                                     for (int i = 0; i < element->debugcall.blacklist_words.size(); i++)
                                     {
                                         const std::string& word = element->debugcall.blacklist_words[i];
                                         bool selected = (element->debugcall.item_current == i);

                                         if (ImGui::Selectable(word.c_str(), selected))
                                             element->debugcall.item_current = i;
                                     }
                                     ImGui::EndListBox();
                                 }

                                 if (element->debugcall.item_current >= 0 && element->debugcall.item_current < element->debugcall.blacklist_words.size())
                                 {
                                     if (ImGui::Button(ICON_FA_TRASH " Remove", ImVec2(-FLT_MIN, 30)))
                                     {
                                         element->debugcall.blacklist_words.erase(element->debugcall.blacklist_words.begin() + element->debugcall.item_current);
                                         element->debugcall.item_current = -1;
                                     }
                                 }

                                 ImGui::Separator();

                                 ImGui::Text(ICON_FA_SERVER " Selected Event Details");
                                 if (element->debugcall.item_current >= 0 && element->debugcall.item_current < element->debugcall.events_list.size())
                                 {
                                     auto& selected_event = element->debugcall.events_list[element->debugcall.item_current];


                                     ImGui::TextColored(ImVec4(0.25f, 0.5f, 1.0f, 1.0f), "Event Name:");
                                     ImGui::SameLine();
                                     ImGui::TextWrapped("%s", selected_event.event_name.c_str());





                                     ImVec4 status_color = selected_event.is_blocked
                                         ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f)
                                         : ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
                                     ImGui::TextColored(status_color, "Status:");
                                     ImGui::SameLine();
                                     ImGui::Text("%s", selected_event.is_blocked ? "Blocked" : "Allowed");


                                     std::string button_label = selected_event.is_blocked ? ICON_FA_UNLOCK " Unblock" : ICON_FA_LOCK " Block";
                                     if (ImGui::Button(button_label.c_str(), ImVec2(-FLT_MIN, 35)))
                                     {
                                         selected_event.is_blocked = !selected_event.is_blocked;
                                     }
                                 }
                                 else
                                 {
                                     ImGui::TextDisabled("No event selected.");
                                 }

                             }
                             gui.end_group_box();






                         }
                             break;
                           
                    case 10:




                        gui.group_box(ICON_FA_GEARS " Packet List", ImVec2(720, 622));
                        {
                            const auto& packets = packet->GetPackets();
                            static size_t lastPacketCount = 0;
                            static std::vector<size_t> displayedPackets;
                            static char filterBuffer[32] = "";

                            
                            ImGui::BeginGroup();
                            {
                                ImGui::AlignTextToFramePadding();
                                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), ICON_FA_MAGNIFYING_GLASS " Filter:");
                                ImGui::SameLine();

                                ImGui::PushItemWidth(180);
                                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.25f, 1.0f));
                                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.25f, 0.25f, 0.3f, 1.0f));
                                ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_CharsDecimal;
                                ImGui::InputTextWithHint("Package Filter", "Enter Packet ID...", filterBuffer, sizeof(filterBuffer), inputFlags);
                                ImGui::PopStyleColor(2);
                                ImGui::PopItemWidth();

                                
                                ImGui::SameLine();
                                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 320);
                                ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "Total: %zu packets", packets.size());

                                
                                ImGui::SameLine();
                                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 420);

                                
                                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.4f, 0.2f, 0.9f));
                                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.5f, 0.3f, 1.0f));
                                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.3f, 0.1f, 1.0f));
                                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);


                                
                                if (strlen(filterBuffer) > 0)
                                {
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 50);
                                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 0.8f));
                                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));
                                    if (ImGui::Button(ICON_FA_XMARK "##clearFilter", ImVec2(25, 0)))
                                    {
                                        filterBuffer[0] = '\0';
                                    }
                                    ImGui::PopStyleColor(2);
                                    if (ImGui::IsItemHovered())
                                    {
                                        ImGui::SetTooltip("Clear filter");
                                    }
                                }
                            }
                            ImGui::EndGroup();

                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::Spacing();

                            
                            if (packets.size() > lastPacketCount)
                            {
                                for (size_t i = lastPacketCount; i < packets.size(); i++)
                                {
                                    displayedPackets.push_back(i);
                                }
                                lastPacketCount = packets.size();
                            }

                            
                            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.12f, 1.0f));
                            ImGui::BeginChild("PacketScrollRegion", ImVec2(0, -1), true, ImGuiWindowFlags_HorizontalScrollbar);

                            bool hasVisiblePackets = false;
                            size_t visibleCount = 0;

                            for (size_t index : displayedPackets)
                            {
                                if (index >= packets.size()) continue;

                                const auto& m_packet = packets[index];

                                
                                bool matchesFilter = true;
                                if (strlen(filterBuffer) > 0)
                                {
                                    std::string packetIdStr = std::to_string(m_packet.packetId);
                                    matchesFilter = (packetIdStr.find(filterBuffer) != std::string::npos);
                                }

                                if (!matchesFilter) continue;

                                hasVisiblePackets = true;
                                visibleCount++;

                                
                                if (visibleCount > 1)
                                {
                                    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4f, 0.4f, 0.45f, 0.6f));
                                    ImGui::Separator();
                                    ImGui::PopStyleColor();
                                    ImGui::Spacing();
                                }

                                
                                ImGui::PushID((int)index);
                                ImGui::BeginGroup();
                                {
                                    
                                    ImVec2 itemStart = ImGui::GetCursorScreenPos();
                                    ImVec2 itemSize = ImVec2(ImGui::GetContentRegionAvail().x, 35);

                                    
                                    bool isHovered = ImGui::IsMouseHoveringRect(itemStart, ImVec2(itemStart.x + itemSize.x, itemStart.y + itemSize.y));
                                    if (isHovered)
                                    {
                                        ImGui::GetWindowDrawList()->AddRectFilled(
                                            itemStart,
                                            ImVec2(itemStart.x + itemSize.x, itemStart.y + itemSize.y),
                                            IM_COL32(255, 255, 255, 8)
                                        );
                                    }

                                    ImGui::AlignTextToFramePadding();

                                    
                                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_FA_CUBE);
                                    ImGui::SameLine();
                                    ImGui::Text("Packet ID:");
                                    ImGui::SameLine();

                                    
                                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.8f, 1.0f, 1.0f));
                                    ImGui::PushFont(nullptr); 
                                    ImGui::Text("%d", m_packet.packetId);
                                    ImGui::PopFont();
                                    ImGui::PopStyleColor();

                                    ImGui::SameLine();

                                    
                                    float buttonWidth = 80.0f;
                                    float availableWidth = ImGui::GetContentRegionAvail().x;
                                    if (availableWidth > buttonWidth + 10.0f)
                                    {
                                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availableWidth - buttonWidth);
                                    }

                                    
                                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.6f, 0.15f, 1.0f));
                                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.75f, 0.2f, 1.0f));
                                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));
                                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

                                    std::string buttonText = ICON_FA_PLAY " Resend";
                                    if (ImGui::Button(buttonText.c_str(), ImVec2(buttonWidth, 28)))
                                    {
                                        packet->resend(index, 1);
                                    }

                                    ImGui::PopStyleVar();
                                    ImGui::PopStyleColor(3);

                                    
                                    if (ImGui::IsItemHovered())
                                    {
                                        ImGui::SetTooltip("Resend packet %d", m_packet.packetId);
                                    }
                                }
                                ImGui::EndGroup();
                                ImGui::PopID();

                                ImGui::Spacing();
                            }

                            
                            if (!hasVisiblePackets)
                            {
                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
                                ImVec2 windowSize = ImGui::GetWindowSize();

                                if (strlen(filterBuffer) > 0)
                                {
                                    ImGui::SetCursorPosX((windowSize.x - 300) * 0.5f);
                                    ImGui::BeginGroup();
                                    ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.2f, 1.0f), ICON_FA_TRIANGLE_EXCLAMATION);
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No packets found matching filter:");
                                    ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize(filterBuffer).x) * 0.5f);
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "\"%s\"", filterBuffer);
                                    ImGui::EndGroup();
                                }
                                else if (displayedPackets.empty())
                                {
                                    ImGui::SetCursorPosX((windowSize.x - 200) * 0.5f);
                                    ImGui::BeginGroup();
                                    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), ICON_FA_INBOX);
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No packets available");
                                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Packets will appear here when captured");
                                    ImGui::EndGroup();
                                }
                            }
                            else
                            {
                                
                                if (strlen(filterBuffer) > 0)
                                {
                                    ImGui::Separator();
                                    ImGui::Spacing();
                                    ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f),
                                        ICON_FA_CIRCLE_INFO " Showing %zu packet(s) matching filter", visibleCount);
                                }
                            }

                            ImGui::EndChild();
                            ImGui::PopStyleColor();

                        } gui.end_group_box();

                        break;

                         case 11:
                             if (gui.m_spoofer_subtab == 0) {

                                 gui.group_box(ICON_FA_USER_SECRET " Hardware Spoofer", ImVec2(720, 622));
                                 {
                                     
                                     ImGui::BeginGroup();
                                     {
                                         
                                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.2f, 1.0f));
                                         ImGui::Text(ICON_FA_SHIELD_HALVED " System Identification Spoofer");
                                         ImGui::PopStyleColor();

                                         ImGui::SameLine();
                                         ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 120);

                                         
                                         if (element->spoofer.isSerialChanged)
                                         {
                                             ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 0.2f));
                                             ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 1.0f, 0.2f, 1.0f));
                                             ImGui::Button(ICON_FA_CHECK " ACTIVE", ImVec2(80, 20));
                                             ImGui::PopStyleColor(2);
                                         }
                                         else
                                         {
                                             ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.6f, 0.6f, 0.2f));
                                             ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                                             ImGui::Button(ICON_FA_MINUS " INACTIVE", ImVec2(80, 20));
                                             ImGui::PopStyleColor(2);
                                         }
                                     }
                                     ImGui::EndGroup();

                                     ImGui::Spacing();
                                     ImGui::Separator();
                                     ImGui::Spacing();

                                     
                                     ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.15f, 0.25f, 0.6f));
                                     ImGui::BeginChild("InfoPanel", ImVec2(0, 80), true, ImGuiWindowFlags_NoScrollbar);
                                     {
                                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.9f, 1.0f, 1.0f));
                                         ImGui::Text(ICON_FA_CIRCLE_INFO " Hardware spoofing changes your system's unique identifiers.");
                                         ImGui::TextWrapped("This can help bypass hardware-based bans but may affect system stability. Use with caution and ensure you have backup identifiers.");
                                         ImGui::PopStyleColor();
                                     }
                                     ImGui::EndChild();
                                     ImGui::PopStyleColor();

                                     ImGui::Spacing();

                                     
                                     ImGui::BeginGroup();
                                     {
                                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                                         ImGui::Text(ICON_FA_MICROCHIP " Hardware Status:");
                                         ImGui::PopStyleColor();

                                         ImGui::Indent(20.0f);

                                         if (element->spoofer.isSerialChanged)
                                         {
                                             
                                             ImGui::BeginGroup();
                                             {
                                                 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                                                 ImGui::Text("Original Serial:");
                                                 ImGui::PopStyleColor();
                                                 ImGui::SameLine();
                                                 ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s", element->spoofer.szCurrentSerial);

                                                 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                                                 ImGui::Text("Spoofed Serial:");
                                                 ImGui::PopStyleColor();
                                                 ImGui::SameLine();
                                                 ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%s", element->spoofer.szLastSpoofedSerial);

                                                 
                                                 ImGui::Spacing();
                                                 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                                                 ImGui::Text(ICON_FA_ARROW_RIGHT " System is currently using spoofed identifiers");
                                                 ImGui::PopStyleColor();
                                             }
                                             ImGui::EndGroup();
                                         }
                                         else
                                         {
                                             
                                             ImGui::BeginGroup();
                                             {
                                                 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                                                 ImGui::Text("Current Serial:");
                                                 ImGui::PopStyleColor();
                                                 ImGui::SameLine();
                                                 ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", element->spoofer.szCurrentSerial);

                                                 ImGui::Spacing();
                                                 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.2f, 1.0f));
                                                 ImGui::Text(ICON_FA_ARROW_RIGHT " System is using original identifiers");
                                                 ImGui::PopStyleColor();
                                             }
                                             ImGui::EndGroup();
                                         }

                                         ImGui::Unindent(20.0f);
                                     }
                                     ImGui::EndGroup();

                                     ImGui::Spacing();
                                     ImGui::Separator();
                                     ImGui::Spacing();

                                     
                                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                                     ImGui::Text(ICON_FA_GEARS " Configuration:");
                                     ImGui::PopStyleColor();

                                     ImGui::Spacing();
                                     ImGui::Indent(20.0f);

                                     
                                     ImGui::PushItemWidth(400);
                                     ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.2f, 1.0f));
                                     ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.2f, 0.25f, 1.0f));
                                     ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.3f, 1.0f));

                                     ImGui::InputTextWithHint("Custom Serial", "Enter players serial",
                                         element->spoofer.szCustomSerial, sizeof(element->spoofer.szCustomSerial));

                                     ImGui::PopStyleColor(3);
                                     ImGui::PopItemWidth();

                                     ImGui::SameLine();
                                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                                     ImGui::Text(ICON_FA_KEYBOARD " Custom Serial");
                                     ImGui::PopStyleColor();

                                     ImGui::Unindent(20.0f);

                                     ImGui::Spacing();
                                     ImGui::Separator();
                                     ImGui::Spacing();

                                     
                                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                                     ImGui::Text(ICON_FA_BOLT " Actions:");
                                     ImGui::PopStyleColor();

                                     ImGui::Spacing();
                                     ImGui::Indent(20.0f);

                                     
                                     ImGui::BeginGroup();
                                     {
                                         
                                         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.2f, 0.8f));
                                         ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.4f, 0.3f, 1.0f));
                                         ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.2f, 0.1f, 1.0f));
                                         ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

                                         std::string spoofButtonText = element->spoofer.isSerialChanged ?
                                             ICON_FA_ROTATE_LEFT " Re-Spoof Custom" : ICON_FA_USER_SECRET " Spoof Custom";

                                         if (ImGui::Button(spoofButtonText.c_str(), ImVec2(140, 35)))
                                         {
                                             Spoofer::ChangeSerial(1);
                                         }

                                         ImGui::PopStyleVar();
                                         ImGui::PopStyleColor(3);

                                         if (ImGui::IsItemHovered())
                                         {
                                             const char* tooltip = element->spoofer.isSerialChanged ?
                                                 "Re-apply custom serial from input field" :
                                                 "Apply custom serial from input field";
                                             ImGui::SetTooltip("%s", tooltip);
                                         }

                                         ImGui::SameLine();
                                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

                                         
                                         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.2f, 0.8f, 0.8f));
                                         ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.3f, 0.9f, 1.0f));
                                         ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.1f, 0.7f, 1.0f));
                                         ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

                                         if (ImGui::Button(ICON_FA_SHUFFLE " Random Spoof", ImVec2(140, 35)))
                                         {
                                             Spoofer::ChangeSerial(2);
                                         }

                                         ImGui::PopStyleVar();
                                         ImGui::PopStyleColor(3);

                                         if (ImGui::IsItemHovered())
                                         {
                                             ImGui::SetTooltip("Generate and apply a completely random serial");
                                         }

                                         ImGui::SameLine();
                                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

                                         
                                         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.8f, 0.8f));
                                         ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.9f, 1.0f));
                                         ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.7f, 1.0f));
                                         ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

                                         
                                         if (!element->spoofer.isSerialChanged)
                                         {
                                             ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                                             ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                                             ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                                             ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                                         }

                                         bool restoreClicked = ImGui::Button(ICON_FA_REPLY " Restore Original", ImVec2(140, 35));

                                         if (!element->spoofer.isSerialChanged)
                                         {
                                             ImGui::PopStyleColor(4);
                                         }

                                         if (restoreClicked)
                                         {
                                             Spoofer::ChangeSerial(0);
                                         }

                                         ImGui::PopStyleVar();
                                         ImGui::PopStyleColor(3);

                                         if (ImGui::IsItemHovered())
                                         {
                                             const char* tooltip = element->spoofer.isSerialChanged ?
                                                 "Restore original hardware identifiers" :
                                                 "No spoofed identifiers to restore";
                                             ImGui::SetTooltip("%s", tooltip);
                                         }
                                     }
                                     ImGui::EndGroup();

                                     ImGui::Unindent(20.0f);

                                     ImGui::Spacing();
                                     ImGui::Spacing();

                                     
                                     ImGui::Separator();
                                     ImGui::Spacing();

                                     ImGui::BeginGroup();
                                     {
                                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.6f, 0.1f, 1.0f));
                                         ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION " Warning:");
                                         ImGui::PopStyleColor();

                                         ImGui::SameLine();
                                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                                         ImGui::TextWrapped("Changes take effect immediately.Keep original serial backed up.");
                                         ImGui::PopStyleColor();
                                     }
                                     ImGui::EndGroup();

                                 } gui.end_group_box();
                        }
                        else if (gui.m_spoofer_subtab == 1) {
                            static const char* gpuNames[] = {
                                
                                "NVIDIA GeForce GTX 750 Ti",
                                "NVIDIA GeForce GTX 950",
                                "NVIDIA GeForce GTX 960",
                                "NVIDIA GeForce GTX 970",
                                "NVIDIA GeForce GTX 980",
                                "NVIDIA GeForce GTX 1050",
                                "NVIDIA GeForce GTX 1050 Ti",
                                "NVIDIA GeForce GTX 1060",
                                "NVIDIA GeForce GTX 1070",
                                "NVIDIA GeForce GTX 1070 Ti",
                                "NVIDIA GeForce GTX 1080",
                                "NVIDIA GeForce GTX 1080 Ti",

                                
                                "NVIDIA GeForce RTX 2060",
                                "NVIDIA GeForce RTX 2060 Super",
                                "NVIDIA GeForce RTX 2070",
                                "NVIDIA GeForce RTX 2070 Super",
                                "NVIDIA GeForce RTX 2080",
                                "NVIDIA GeForce RTX 2080 Super",
                                "NVIDIA GeForce RTX 2080 Ti",

                                
                                "NVIDIA GeForce RTX 3050",
                                "NVIDIA GeForce RTX 3060",
                                "NVIDIA GeForce RTX 3060 Ti",
                                "NVIDIA GeForce RTX 3070",
                                "NVIDIA GeForce RTX 3070 Ti",
                                "NVIDIA GeForce RTX 3080",
                                "NVIDIA GeForce RTX 3080 Ti",
                                "NVIDIA GeForce RTX 3090",
                                "NVIDIA GeForce RTX 3090 Ti",

                                
                                "NVIDIA GeForce RTX 4060",
                                "NVIDIA GeForce RTX 4060 Ti",
                                "NVIDIA GeForce RTX 4070",
                                "NVIDIA GeForce RTX 4070 Ti",
                                "NVIDIA GeForce RTX 4080",
                                "NVIDIA GeForce RTX 4090",

                                
                                "AMD Radeon RX 550",
                                "AMD Radeon RX 560",
                                "AMD Radeon RX 570",
                                "AMD Radeon RX 580",
                                "AMD Radeon RX 590",

                                
                                "AMD Radeon RX 5500 XT",
                                "AMD Radeon RX 5600 XT",
                                "AMD Radeon RX 5700",
                                "AMD Radeon RX 5700 XT",

                                
                                "AMD Radeon RX 6600",
                                "AMD Radeon RX 6600 XT",
                                "AMD Radeon RX 6700",
                                "AMD Radeon RX 6700 XT",
                                "AMD Radeon RX 6800",
                                "AMD Radeon RX 6800 XT",
                                "AMD Radeon RX 6900 XT",

                                
                                "AMD Radeon RX 7600",
                                "AMD Radeon RX 7700 XT",
                                "AMD Radeon RX 7800 XT",
                                "AMD Radeon RX 7900 XT",
                                "AMD Radeon RX 7900 XTX"
                            };

                            static int gpuRAMOptions[] = { 1024, 2048, 4096, 6144, 8192, 12288, 16384 };
                            static int freeOptions[] = { 256000, 512000, 1024000, 2048000, 4096000, 8192000 };
                            static int usedOptions[] = { 128000, 256000, 512000, 1024000, 2048000 };

                            
                            gui.group_box(ICON_FA_MICROCHIP " Device Spoofer", ImVec2(720, 622));
                            {
                                static int selectedGpuIndex = 0;
                                static int selectedRamIndex = 2; 
                                static int selectedFreeIndex = 0;
                                static int selectedUsedIndex = 0;
                                static bool bWindowed = false;
                                static bool bAllowScreenUpload = true;

                                
                                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 0.8f));
                                ImGui::BeginChild("HeaderSection", ImVec2(-1, 100), true, ImGuiWindowFlags_NoScrollbar);
                                {
                                    ImGui::SetCursorPosY(10);
                                    ImGui::TextColored(ImVec4(0.8f, 0.9f, 1.0f, 1.0f), ICON_FA_CIRCLE_INFO " Configure your system hardware spoofing parameters");

                                    
                                    ImGui::SetCursorPosY(35);
                                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Current: ");
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", gpuNames[selectedGpuIndex]);
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), " | ");
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%d MB VRAM", gpuRAMOptions[selectedRamIndex]);
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), " | ");
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%d MB Free", freeOptions[selectedFreeIndex] / 1000);

                                    ImGui::SetCursorPosY(55);
                                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Settings: ");
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Windowed: %s", bWindowed ? "ON" : "OFF");
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), " | ");
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Screen Upload: %s", bAllowScreenUpload ? "ON" : "OFF");

                                    
                                    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 130, 15));
                                    if (ImGui::Button(ICON_FA_REPEAT " Randomize All", ImVec2(120, 30)))
                                    {
                                        
                                        selectedGpuIndex = rand() % IM_ARRAYSIZE(gpuNames);
                                        selectedRamIndex = rand() % IM_ARRAYSIZE(gpuRAMOptions);
                                        selectedFreeIndex = rand() % IM_ARRAYSIZE(freeOptions);
                                        selectedUsedIndex = rand() % IM_ARRAYSIZE(usedOptions);
                                        bWindowed = rand() % 2;
                                        bAllowScreenUpload = rand() % 2;

                                        
                                        std::string randomizeMsg = "All values randomized!\n";
                                        randomizeMsg += "GPU: " + std::string(gpuNames[selectedGpuIndex]) + "\n";
                                        randomizeMsg += "VRAM: " + std::to_string(gpuRAMOptions[selectedRamIndex]) + " MB\n";
                                        randomizeMsg += "Free Memory: " + std::to_string(freeOptions[selectedFreeIndex] / 1000) + " MB\n";
                                        randomizeMsg += "Used Memory: " + std::to_string(usedOptions[selectedUsedIndex] / 1000) + " MB\n";
                                        randomizeMsg += "Windowed: " + std::string(bWindowed ? "Yes" : "No") + "\n";
                                        randomizeMsg += "Screen Upload: " + std::string(bAllowScreenUpload ? "Yes" : "No");

                                        ImGui::InsertNotification({
                                            ImGuiToastType::Info,
                                            5000,
                                            randomizeMsg.c_str()
                                            });
                                    }
                                }
                                ImGui::EndChild();
                                ImGui::PopStyleColor();

                                ImGui::Spacing();
                                ImGui::Separator();
                                ImGui::Spacing();

                                
                                ImGui::BeginChild("ConfigArea", ImVec2(-1, 480), false);
                                {
                                    
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), ICON_FA_DESKTOP  " Graphics Hardware");
                                    ImGui::Spacing();

                                    
                                    ImGui::Text("Video Card Model:");
                                    ImGui::SetNextItemWidth(450);
                                    ImGui::Combo("gpuName", &selectedGpuIndex, gpuNames, IM_ARRAYSIZE(gpuNames));

                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                    if (ImGui::Button(ICON_FA_REPEAT "randomGpu", ImVec2(30, 0)))
                                    {
                                        selectedGpuIndex = rand() % IM_ARRAYSIZE(gpuNames);

                                        std::string msg = "GPU randomized to: " + std::string(gpuNames[selectedGpuIndex]);
                                        ImGui::InsertNotification({
                                            ImGuiToastType::Info,
                                            3000,
                                            msg.c_str()
                                            });
                                    }

                                    ImGui::Spacing();

                                    
                                    ImGui::Text("Video Memory (MB):");
                                    ImGui::SetNextItemWidth(200);
                                    if (ImGui::BeginCombo("gpuRAM", (std::to_string(gpuRAMOptions[selectedRamIndex]) + " MB").c_str()))
                                    {
                                        for (int n = 0; n < IM_ARRAYSIZE(gpuRAMOptions); n++)
                                        {
                                            bool isSelected = (selectedRamIndex == n);
                                            std::string label = std::to_string(gpuRAMOptions[n]) + " MB";
                                            if (ImGui::Selectable(label.c_str(), isSelected))
                                                selectedRamIndex = n;
                                            if (isSelected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }

                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                    if (ImGui::Button(ICON_FA_REPEAT "randomRam", ImVec2(30, 0)))
                                    {
                                        selectedRamIndex = rand() % IM_ARRAYSIZE(gpuRAMOptions);

                                        std::string msg = "VRAM randomized to: " + std::to_string(gpuRAMOptions[selectedRamIndex]) + " MB";
                                        ImGui::InsertNotification({
                                            ImGuiToastType::Info,
                                            3000,
                                            msg.c_str()
                                            });
                                    }

                                    ImGui::Spacing();
                                    ImGui::Separator();
                                    ImGui::Spacing();

                                    
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), ICON_FA_MEMORY " Memory Management");
                                    ImGui::Spacing();

                                    
                                    ImGui::Text("Available Memory:");
                                    ImGui::SetNextItemWidth(200);
                                    if (ImGui::BeginCombo("freeForMTA", (std::to_string(freeOptions[selectedFreeIndex] / 1000) + " MB").c_str()))
                                    {
                                        for (int n = 0; n < IM_ARRAYSIZE(freeOptions); n++)
                                        {
                                            bool isSelected = (selectedFreeIndex == n);
                                            std::string label = std::to_string(freeOptions[n] / 1000) + " MB";
                                            if (ImGui::Selectable(label.c_str(), isSelected))
                                                selectedFreeIndex = n;
                                            if (isSelected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }

                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                    if (ImGui::Button(ICON_FA_REPEAT "randomFree", ImVec2(30, 0)))
                                    {
                                        selectedFreeIndex = rand() % IM_ARRAYSIZE(freeOptions);

                                        std::string msg = "Free Memory randomized to: " + std::to_string(freeOptions[selectedFreeIndex] / 1000) + " MB";
                                        ImGui::InsertNotification({
                                            ImGuiToastType::Info,
                                            3000,
                                            msg.c_str()
                                            });
                                    }

                                    ImGui::Spacing();

                                    
                                    ImGui::Text("Render Target Usage:");
                                    ImGui::SetNextItemWidth(200);
                                    if (ImGui::BeginCombo("usedByRenderTargets", (std::to_string(usedOptions[selectedUsedIndex] / 1000) + " MB").c_str()))
                                    {
                                        for (int n = 0; n < IM_ARRAYSIZE(usedOptions); n++)
                                        {
                                            bool isSelected = (selectedUsedIndex == n);
                                            std::string label = std::to_string(usedOptions[n] / 1000) + " MB";
                                            if (ImGui::Selectable(label.c_str(), isSelected))
                                                selectedUsedIndex = n;
                                            if (isSelected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }

                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                    if (ImGui::Button(ICON_FA_REPEAT "randomUsed", ImVec2(30, 0)))
                                    {
                                        selectedUsedIndex = rand() % IM_ARRAYSIZE(usedOptions);

                                        std::string msg = "Render Target Usage randomized to: " + std::to_string(usedOptions[selectedUsedIndex] / 1000) + " MB";
                                        ImGui::InsertNotification({
                                            ImGuiToastType::Info,
                                            3000,
                                            msg.c_str()
                                            });
                                    }

                                    ImGui::Spacing();
                                    ImGui::Separator();
                                    ImGui::Spacing();

                                    
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), ICON_FA_GEARS " Display Settings");
                                    ImGui::Spacing();

                                    
                                    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.4f, 0.8f, 1.0f, 1.0f));

                                    if (ImGui::Checkbox(ICON_FA_WINDOW_RESTORE " Windowed Mode", &bWindowed))
                                    {
                                        std::string msg = "Windowed Mode: " + std::string(bWindowed ? "Enabled" : "Disabled");
                                        ImGui::InsertNotification({
                                            ImGuiToastType::Info,
                                            2000,
                                            msg.c_str()
                                            });
                                    }

                                    ImGui::Spacing();

                                    if (ImGui::Checkbox(ICON_FA_UPLOAD " Allow Screen Upload", &bAllowScreenUpload))
                                    {
                                        std::string msg = "Screen Upload: " + std::string(bAllowScreenUpload ? "Enabled" : "Disabled");
                                        ImGui::InsertNotification({
                                            ImGuiToastType::Info,
                                            2000,
                                            msg.c_str()
                                            });
                                    }

                                    ImGui::PopStyleColor();
                                }
                                ImGui::EndChild();

                                ImGui::Spacing();
                                ImGui::Separator();
                                ImGui::Spacing();

                                
                                ImGui::BeginChild("ButtonSection", ImVec2(-1, 60), false);
                                {
                                    ImGui::SetCursorPosY(10);

                                    
                                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 0.8f));
                                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
                                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));

                                    if (ImGui::Button(ICON_FA_CHECK " Apply Configuration", ImVec2(180, 35)))
                                    {
                                        element->dxSpoof.bSystemSpoofer = true;

                                        
                                        strncpy(element->dxSpoof.szVideoCardName, gpuNames[selectedGpuIndex],
                                            sizeof(element->dxSpoof.szVideoCardName) - 1);
                                        element->dxSpoof.szVideoCardName[sizeof(element->dxSpoof.szVideoCardName) - 1] = '\0';

                                        
                                        element->dxSpoof.iVideoCardRAM = gpuRAMOptions[selectedRamIndex];

                                        
                                        element->dxSpoof.bWindowedMode = bWindowed;
                                        element->dxSpoof.bAllowScreenUpload = bAllowScreenUpload;

                                        
                                        element->dxSpoof.iFreeForMTA = freeOptions[selectedFreeIndex];
                                        element->dxSpoof.iUsedByRenderTargets = usedOptions[selectedUsedIndex];

                                        ImGui::InsertNotification({
                                            ImGuiToastType::Success,
                                            4000,
                                            "Hardware configuration applied successfully!"
                                            });
                                    }
                                    ImGui::PopStyleColor(3);

                                    ImGui::SameLine();

                                    
                                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.3f, 0.2f, 0.8f));
                                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.4f, 0.3f, 1.0f));
                                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.2f, 0.1f, 1.0f));

                                    if (ImGui::Button(ICON_FA_ROTATE_LEFT " Reset to Default", ImVec2(160, 35)))
                                    {
                                        selectedGpuIndex = 0;
                                        selectedRamIndex = 2;
                                        selectedFreeIndex = 0;
                                        selectedUsedIndex = 0;
                                        bWindowed = false;
                                        bAllowScreenUpload = true;

                                        ImGui::InsertNotification({
                                            ImGuiToastType::Info,
                                            3000,
                                            "Settings reset to default values"
                                            });
                                    }
                                    ImGui::PopStyleColor(3);

                                    
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150);
                                    ImGui::BeginGroup();
                                    {
                                        ImGui::Text("Status:");
                                        ImGui::SameLine();
                                        if (element->dxSpoof.bSystemSpoofer)
                                        {
                                            ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), ICON_FA_CIRCLE_CHECK " Active");
                                        }
                                        else
                                        {
                                            ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.2f, 1.0f), ICON_FA_CIRCLE_EXCLAMATION " Inactive");
                                        }
                                    }
                                    ImGui::EndGroup();
                                }
                                ImGui::EndChild();
                            }
                            gui.end_group_box();
                        }

                        break;

                         case 12:

                             gui.group_box(ICON_FA_SERVER " NexWare Settings", ImVec2(700, 510));
                             {
                                 ImGui::Spacing();
                                 ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                 ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Welcome to NexWare");

                                 ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                 ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Version 1.2");

                                 ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                 ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "discord.gg/nexware");

                                 ImGui::Spacing();
                                 ImGui::Separator();
                                 ImGui::Spacing();

                                 ImGui::BeginChild("version_info", ImVec2(0, 60), true, ImGuiWindowFlags_NoScrollbar);
                                 {
                                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                                     ImGui::Text(ICON_FA_WRENCH  " Build Information");
                                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
                                     ImGui::BulletText("Build Date: %s", __DATE__);
                                 }
                                 ImGui::EndChild();

                                 ImGui::Spacing();

                                 ImGui::BeginChild("key_binding", ImVec2(0, 120), true, ImGuiWindowFlags_NoScrollbar);
                                 {
                                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                                     ImGui::Text(ICON_FA_KEYBOARD " Key Bindings");
                                     ImGui::Spacing();

                                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                     ImGui::Text("Menu Toggle Key:");
                                     ImGui::Spacing();

                                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                     if (element->binds.is_waiting_for_key)
                                     {
                                         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
                                         ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.7f, 0.2f, 1.0f));
                                         ImGui::Button("Press any key...", ImVec2(200, 30));
                                         ImGui::PopStyleColor(2);

                                         for (int key = 0x01; key <= 0xFE; key++)
                                         {
                                             if (GetAsyncKeyState(key) & 0x8000)
                                             {
                                                 element->binds.menu_bind = key;
                                                 element->binds.is_waiting_for_key = false;
                                                 break;
                                             }
                                         }
                                     }
                                     else
                                     {
                                         std::string buttonLabel = ICON_FA_KEY " " + GetKeyName(element->binds.menu_bind);
                                         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.8f, 1.0f));
                                         ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.6f, 0.9f, 1.0f));
                                         if (ImGui::Button(buttonLabel.c_str(), ImVec2(200, 30)))
                                         {
                                             element->binds.is_waiting_for_key = true;
                                         }
                                         ImGui::PopStyleColor(2);
                                     }
                                 }
                                 ImGui::EndChild();

                                 ImGui::Spacing();


                                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                                     ImGui::Text(ICON_FA_PALETTE " Theme Selection");
   

                                     static int selected_theme = 0; 
                                     const char* theme_items[] = { "Main", "Purple" };

                                     if (ImGui::Combo("Theme Selector", &selected_theme, theme_items, IM_ARRAYSIZE(theme_items)))
                                     {
                                         element->info.whitetheme = (selected_theme == 1);
                                     }

                                     if (ImGui::IsItemHovered())
                                     {
                                         ImGui::SetTooltip("Select interface color theme");
                                     }
  
                                     ImGui::Separator();

                                 ImGui::Spacing();

                                 if (ImGui::Checkbox(ICON_FA_CAMERA " Anti-Screenshot Protection", &element->info.bypass_screenshot))


                                 if (ImGui::IsItemHovered())
                                 {
                                     ImGui::SetTooltip("Prevents screenshots from capturing the application");
                                 }

                                 ImGui::Spacing();

                                 if (ImGui::Checkbox(ICON_FA_USER_SHIELD " Administrator Detection", &element->info.Admin_alert))

                                 if (ImGui::IsItemHovered())
                                 {
                                     ImGui::SetTooltip("Alerts when administrator privileges are detected");
                                 }
                             }
                             gui.end_group_box();
            }                


            EndChild();
            PopStyleVar(2);
        } ImGui::End();
    }




    




    if (fovLoopEnabled) {
        SetFOV(FovValue); 
    };    
    
    if (aspectLoopEnabled) {
        SetAspectRatio(AspectValue); 
    };

    if (pCustomCrosshair) {
        pCustomCrosshair->DrawCrosshair();
    }

if (pSpinbot && pSpinbot->bEnabled) {
    Update();  // Her zaman çalışır, içeride kontrol var
}
if (pSpinbot && pSpinbot->bEnabled) {
    UpdateFakeRotation();
}




    ImGui::RenderNotifications();
    player->release();
    weapon->release();
    weathersa();
    speedxd();
    PullAllVehiclesTick();
    LoadMTASerial();
    aimbot->draw_player_aimbot();
    vehicle_speed_boost();
    visual->draw_vehicle_esp();       
    visual->draw_ped_esp();
    visual->draw_player_esp();
    VehicleRainLoop();
    VelocityMoviment();
    CheckAirbreakKey();
    NoRecoil();
    SuperBreak();    
    ApplyBoost();
    CheckHealthArmorKey();
    FastCrosshair();
    NoReloadXD();
    ImGuiIO& ku = ImGui::GetIO();
    ku.MouseDrawCursor = (var->gui.is_open || visual->overlayEnabled);

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}




void c_menu::shutdown(bool before)
{
    if (before)
    {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        return;
    }

    ImGui_ImplDX9_CreateDeviceObjects();
}



bool c_menu::release()
{
    var->winapi.mh_status = MH_CreateHook(&::SetCursorPos, &h_cursor, reinterpret_cast<LPVOID*>(&o_cursor));
    if (var->winapi.mh_status != MH_OK)
    {
        console->error("Failed to create Hook at Cursor: %d\n", var->winapi.mh_status);
        return false;
    }

    var->winapi.mh_status = MH_EnableHook(&::SetCursorPos);
    if (var->winapi.mh_status != MH_OK)
    {
        console->error("Failed to enable Hook at Cursor: %d\n", var->winapi.mh_status);
        return false;
    }

    /* ============ */

    void* dw_present = reinterpret_cast<void*>(utilities::c_device::get_address(17));

    var->winapi.mh_status = MH_CreateHook(dw_present, &h_present, reinterpret_cast<LPVOID*>(&o_present));
    if (var->winapi.mh_status != MH_OK)
    {
        console->error("Failed to make Hook at Present: %d\n", var->winapi.mh_status);
        return false;
    }

    var->winapi.mh_status = MH_EnableHook(dw_present);
    if (var->winapi.mh_status != MH_OK)
    {
        console->error("Failed to enable Hook at Present: %d\n", var->winapi.mh_status);
        return false;
    }

    /* ============ */

    void* dw_reset = reinterpret_cast<void*>(utilities::c_device::get_address(16));

    var->winapi.mh_status = MH_CreateHook(dw_reset, &h_reset, reinterpret_cast<LPVOID*>(&o_reset));
    if (var->winapi.mh_status != MH_OK)
    {
        console->error("Failed to make Hook at Reset: %d\n", var->winapi.mh_status);
        return false;
    }

    var->winapi.mh_status = MH_EnableHook(dw_reset);
    if (var->winapi.mh_status != MH_OK)
    {
        console->error("Failed to enable Hook at Reset: %d\n", var->winapi.mh_status);
        return false;
    }

    return true;
}