#pragma once

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "color_t.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;

class c_gui {

public:

    float m_anim = 0.f;
    int m_tab = 0;

    int m_weapon_subtab = 0;    
    
    int m_vehicle_subtab = 0;    
    int m_luafunc_subtab = 0;        
    int m_localplayer_subtab = 0;        
    int m_misc_subtabs = 0;        
    int m_executor_subtab = 0;    
    int m_visual_subtab = 0;    
    int m_spoofer_subtab = 0;

    int m_rage_subtab = 0;
    vector < const char* > rage_subtabs = { "General", "Anti aim", "Subtab" };
    vector<const char*> weapon_subtabs = { "Main","Rage"};
    vector<const char*> vehicle_subtabs = { "Main", " Vehicle List" };      
    vector<const char*> spoofer_subtabs = { "Serial Changer", " Device Spoofer" };     
    vector<const char*> executor_subtabs = { "Runtime-Executor", " OverWrite-Executor" };        
    
    vector<const char*> luafunc_subtabs = { "Server Event", " Client Event" , " AddEvent" ,"AddEventHandler", "Mta Funcs"};
    vector<const char*> localplayer_subtabs = { "General", "Config"};
    vector<const char*> misc_subtabs = { "Main", "Radar"};

    vector<const char*> visual_subtabs = { "Player", " Vehicle", "Ped"};

    color_t accent_color = { 1.f, 1.f, 1.f, 0.5f };  // Beyaz, %50 opaklýk  
    color_t text = { 1.f, 1.f, 1.f, 1.f };           // Beyaz, %100 opak  
    color_t text_disabled = { 0.51f, 0.52f, 0.56f, 1.f };  // Gri, %100 opak  

    color_t border = { 0.f, 0.f, 0.f, 0.5f };  // Siyah, %50 opaklýk  

    color_t frame_inactive = { 0.102f, 0.102f, 0.102f, 0.5f };  // Koyu gri, %50 opaklýk  
    color_t frame_active = { 1.f, 1.f, 1.f, 0.5f };     // Beyaz, %50 opaklýk  

    color_t button = { 0.07f, 0.07f, 0.07f, 1.f };
    color_t button_hovered = { 0.12f, 0.12f, 0.12f, 1.f };
    color_t button_active = { 0.18f, 0.18f, 0.18f, 1.f };

    color_t group_box_bg = { 0.102f, 0.102f, 0.102f, 0.25f };  // Koyu gri, %25 opaklýk
    color_t glow = { 0.6f, 0.6f, 0.6f, 0.3f };  // Yumuþak gri glow


    void render_circle_for_horizontal_bar(ImVec2 pos, ImColor color, float alpha);

    inline void group_title(const char* name) {

        SetCursorPosX(GetCursorPosX() + 10);
        PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 0.5f));
        Text(name);
        PopStyleColor();
    }

    void group_box(const char* name, ImVec2 size_arg);
    void end_group_box();

    bool tab(const char* icon, const char* label, bool selected);
    bool subtab(const char* label, bool selected, int size, ImDrawFlags flags);

};

inline c_gui gui;
