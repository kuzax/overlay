#pragma once

#include <iostream>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <windows.h> // WinApi header 
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


#include "font_defines.h"
#include "imgui_settings.h"
#include <map>

static const char* current_popup_name = "123";

class custom_popup {
private:
    const char* name;
    bool is_enabled;
    bool should_close = false; // Глобальный флаг для закрытия
    float rounding = c::child::rounding;

    ImVec2 win_padding = ImVec2(12, 12);

    struct popup_info {
        float target_height;
        float current_height;
        float target_alpha;
        float current_alpha;
        bool is_closing; // Флаг для анимации закрытия

        bool once_set_pos;
    };

    std::map<ImGuiID, popup_info> anim;

  
public:

    custom_popup(const char* name) : name(name), is_enabled(false) {}


    struct button_state {
        ImVec4 bg_color{ 1.0f, 1.0f, 1.0f, 0.0f }; // Цвет фона кнопки
        ImVec4 text_color{ 1.0f, 1.0f, 1.0f, 0.6f }; // Цвет текста
        ImVec2 text_offset{ 0.0f, 0.0f }; // Смещение текста
    };

    bool animated_button(const char* label, ImDrawFlags flags = 0, float x_scale = 1.f) {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);
        const float button_width = label_size.x + 20.0f;
        const float button_height = ImGui::GetWindowHeight() - style.WindowPadding.y * 2.0f;
        ImVec2 button_size(button_width * x_scale, button_height);

        ImVec2 button_pos = window->DC.CursorPos;
        ImRect button_bb(button_pos, button_pos + button_size);

        ImGui::ItemSize(button_bb, style.FramePadding.y);
        ImGui::ItemAdd(button_bb, window->GetID(label));

        bool is_hovered, is_held;
        bool pressed = ImGui::ButtonBehavior(button_bb, window->GetID(label), &is_hovered, &is_held);

        ImGuiID id = window->GetID(label);
        static std::map<ImGuiID, button_state> anim;
        auto it_anim = anim.emplace(id, button_state()).first;

        ImVec4 current_col = bTheme ? ImColor(1.f, 1.f, 1.f, 1.f) : ImColor(0.f, 0.f, 0.f, 1.f);

        ImVec4 target_bg_color = is_held ? utils::GetColorWithAlpha(current_col, 0.15F * x_scale) : is_hovered ? utils::GetColorWithAlpha(current_col, 0.075F * x_scale) : utils::GetColorWithAlpha(current_col, 0.0F);
        ImVec4 target_text_color = is_held ? utils::GetColorWithAlpha(label::active, x_scale) : is_hovered ? utils::GetColorWithAlpha(label::hovered, x_scale) : utils::GetColorWithAlpha(label::default, x_scale);
        ImVec2 target_text_offset = ImVec2(0.0f, 0.0f);

        it_anim->second.bg_color = ImLerp(it_anim->second.bg_color, target_bg_color, g.IO.DeltaTime * 10.0f);
        it_anim->second.text_color = ImLerp(it_anim->second.text_color, target_text_color, g.IO.DeltaTime * 10.0f);
        it_anim->second.text_offset = ImLerp(it_anim->second.text_offset, target_text_offset, g.IO.DeltaTime * 12.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, x_scale);
        window->DrawList->AddRectFilled(button_bb.Min, button_bb.Max, ImGui::ColorConvertFloat4ToU32(it_anim->second.bg_color), c::child::rounding, flags);

        if (!(flags & ImDrawFlags_RoundCornersRight))
            window->DrawList->AddRectFilled(ImVec2(button_bb.Max.x - 1, button_bb.Min.y), ImVec2(button_bb.Max.x, button_bb.Max.y), ImGui::GetColorU32(c::stroke_color.Value), 0, 0);
        else
            window->DrawList->AddRectFilled(ImVec2(button_bb.Min.x, button_bb.Min.y), ImVec2(button_bb.Min.x + 1, button_bb.Max.y), ImGui::GetColorU32(c::stroke_color.Value), 0, 0);

        ImVec2 text_pos = button_pos + ImVec2(10.0f, (button_height - label_size.y) * 0.5f) + it_anim->second.text_offset;
        window->DrawList->AddText(text_pos, ImGui::GetColorU32(it_anim->second.text_color), label);

        ImGui::PopStyleVar();

        return pressed;
    }

    bool animated_button(const char* label, bool active = false, ImDrawFlags flags = 0) {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);
        const float button_width = label_size.x + 20.0f;
        const float button_height = ImGui::GetWindowHeight() - style.WindowPadding.y * 2.0f;
        const ImVec2 button_size(button_width, button_height);

        ImVec2 button_pos = window->DC.CursorPos;
        ImRect button_bb(button_pos, button_pos + button_size);

        ImGui::ItemSize(button_bb, style.FramePadding.y);
        ImGui::ItemAdd(button_bb, window->GetID(label));

        bool is_hovered, is_held;
        bool pressed = ImGui::ButtonBehavior(button_bb, window->GetID(label), &is_hovered, &is_held);

        ImGuiID id = window->GetID(label);
        static std::map<ImGuiID, button_state> anim;
        auto it_anim = anim.emplace(id, button_state()).first;

        // Анимация
        ImVec4 current_col = bTheme ? ImColor(1.f, 1.f, 1.f, 1.f) : ImColor(0.f, 0.f, 0.f, 1.f);

        ImVec4 target_bg_color = active ? utils::GetColorWithAlpha(current_col, 0.15F) : is_hovered ? utils::GetColorWithAlpha(current_col, 0.075F) : utils::GetColorWithAlpha(current_col, 0.0F);
        ImVec4 target_text_color = active ? label::active : is_hovered ? label::hovered : label::default;
        ImVec2 target_text_offset = ImVec2(0.0f, 0.0f);

        it_anim->second.bg_color = ImLerp(it_anim->second.bg_color, target_bg_color, g.IO.DeltaTime * 10.0f);
        it_anim->second.text_color = ImLerp(it_anim->second.text_color, target_text_color, g.IO.DeltaTime * 10.0f);
        it_anim->second.text_offset = ImLerp(it_anim->second.text_offset, target_text_offset, g.IO.DeltaTime * 12.0f);


        window->DrawList->AddRectFilled(button_bb.Min, button_bb.Max, ImGui::ColorConvertFloat4ToU32(it_anim->second.bg_color), c::child::rounding, flags);

        if (!(flags & ImDrawFlags_RoundCornersRight))
            window->DrawList->AddRectFilled(ImVec2(button_bb.Max.x - 1, button_bb.Min.y), ImVec2(button_bb.Max.x, button_bb.Max.y), ImGui::GetColorU32(c::stroke_color.Value), 0, 0);
        else
            window->DrawList->AddRectFilled(ImVec2(button_bb.Min.x, button_bb.Min.y), ImVec2(button_bb.Min.x + 1, button_bb.Max.y), ImGui::GetColorU32(c::stroke_color.Value), 0, 0);

        ImVec2 text_pos = button_pos + ImVec2(10.0f, (button_height - label_size.y) * 0.5f) + it_anim->second.text_offset;
        window->DrawList->AddText(text_pos, ImGui::ColorConvertFloat4ToU32(it_anim->second.text_color), label);

        return pressed;
    }
        
    bool is_closing() { return should_close; };

    void open() {
        
        if (!is_enabled) {
            current_popup_name = name;
            is_enabled = true;
            should_close = false;
            ImGui::SetNextWindowPos(ImGui::GetMousePos());

            ImGuiID id = ImGui::GetID(name);
            auto it_anim = anim.find(id);
            if (it_anim != anim.end()) {
                it_anim->second.is_closing = false;
                it_anim->second.target_height = 400.0f;
                it_anim->second.target_alpha = 1.0f;
                it_anim->second.current_alpha = 0.0f;   // <— вот эта строка
                it_anim->second.current_height = 10.0f;  // и, возможно, эта
            }
        }
    }

    void close() {
        should_close = true; // Устанавливаем флаг закрытия
    }

    bool begin(float x_size = 40.f) {
                
        
        if (!this->is_enabled) {

            return false;
        }

        ImGuiID id = ImGui::GetID(name);
        auto it_anim = anim.find(id);

        if (it_anim == anim.end()) {
            popup_info info;
            info.target_height = 400.0f;
            info.current_height = 10.0f;
            info.target_alpha = 1.0f;
            info.current_alpha = 0.0f;
            info.is_closing = false;
            anim.insert({ id, info });
            ImGui::SetNextWindowPos(ImGui::GetMousePos());
            it_anim = anim.find(id);
        }

        // Если флаг закрытия установлен, начинаем анимацию закрытия
        if (should_close) {
            it_anim->second.is_closing = true;
            it_anim->second.target_height = 10.0f;
            it_anim->second.target_alpha = 0.0f;
        }

        if (it_anim->second.is_closing) {
            
            it_anim->second.current_height = ImLerp(it_anim->second.current_height, it_anim->second.target_height, ImGui::GetIO().DeltaTime * 24.0f);
            it_anim->second.current_alpha = ImLerp(it_anim->second.current_alpha, 0.f, ImGui::GetIO().DeltaTime * 7.f);
            
            if (it_anim->second.current_height < 35.0f && it_anim->second.current_alpha < 0.1f) {
                is_enabled = false; // Полностью закрываем попап, если анимация завершена
                it_anim->second.is_closing = false;
                should_close = false; // Сбрасываем флаг закрытия
                it_anim->second.once_set_pos = false;
                return false;
            }
        }
        else {

            
            it_anim->second.current_height = ImLerp(it_anim->second.current_height, it_anim->second.target_height, ImGui::GetIO().DeltaTime * 24.0f);

            it_anim->second.current_alpha = ImLerp(it_anim->second.current_alpha, 1.f, ImGui::GetIO().DeltaTime * 7.f);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, rounding);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, win_padding);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, it_anim->second.current_alpha);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

        if (x_size > 0.f)
            ImGui::SetNextWindowSize(ImVec2(x_size, it_anim->second.current_height), ImGuiCond_Always);

        
        if (!it_anim->second.once_set_pos) {
            ImGui::SetNextWindowPos(ImGui::GetMousePos());
            it_anim->second.once_set_pos = true;
        }

        bool is_open = ImGui::Begin(name, &is_enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_AlwaysAutoResize);
        if (is_open) {
            ImGui::PushClipRect(ImVec2(0, 0), ImVec2(4000, 4000), false);
            //blur::add_blur(ImGui::GetWindowDrawList(), ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::GetStyle().Alpha, 1.5f);

            //draw_background_blur(ImGui::GetBackgroundDrawList(), g_pSwapChain, g_pd3dDevice, g_pd3dDeviceContext, ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), 1.f);

            ImGui::GetWindowDrawList()->AddShadowRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImColor(0.f, 0.f, 0.f, ImGui::GetStyle().Alpha), 230.f, ImVec2(0, 0), ImDrawFlags_RoundCornersAll | ImDrawFlags_ShadowCutOutShapeBackground, rounding);
            ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), utils::GetColorWithAlpha(c::child::background, ImGui::GetStyle().Alpha), rounding);


            ImGui::GetWindowDrawList()->AddRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::GetColorU32(c::child::stroke, c::child::stroke.w * ImGui::GetStyle().Alpha), rounding);
            ImGui::PopClipRect();

            it_anim->second.target_height = ImGui::GetCurrentWindow()->ContentSize.y + win_padding.y * 2;

            if (x_size == -1.f)
            {
                //ImGui::SetWindowSize(ImVec2(it_anim->second.current_height, ImGui::GetCurrentWindow()->ContentSize.y), ImGuiCond_Always);
            }

        }
        else
            it_anim->second.once_set_pos = false;

        return is_open;
    }

    void end() {
        ImGui::End();
        ImGui::PopStyleVar(4);
    }

    bool is_open(const char* label = "123") const {
        return is_enabled; 
    }
};

inline custom_popup features_popup("FeaturesList");
inline custom_popup settings_popup("SettingsList");
inline custom_popup vehicle_popup("VehiclePopup");