#pragma once

#include <array>
#include <string>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_settings.h"

#include <vector>
#include <algorithm>
#include "imgui.h"

#ifndef SCALE
#define SCALE(x) (x)
#endif

struct item_state
{
    std::string      name;
    ImVec4           color;
    ImVec4           color_two;
    bool             double_col;
    int              font_type;
    int              position;
    bool             allowed_positions[4];
    bool             active = false;
    bool             swapped = false;
    ImVec2           location = ImVec2(0, 0);
    ImRect           rect;
};

inline float esp_bar_size = 6.f;

struct add_item_state
{
    bool window_opened = false;
    float window_alpha = 0.f;
    bool window_hovered = false;
    float height{ 0 };
};

enum item_position
{
    position_top,
    position_bottom,
    position_left,
    position_right
};

struct preview_box
{
    ImVec2 position;
    ImVec2 size;
};

enum preview_area
{
    area_top,
    area_bottom,
    area_left,
    area_right,
    area_none
};

struct
{
    ImVec2 pos{ 0, 0 };
    ImVec2 window_size{ 320, 520 };
    float m_padding{ 20 };
    ImVec2 size{ 95, 210 };
    ImVec2 padding{ 10, 10 };
    ImVec2 spacing{ 10, 10 };
    float rounding{ 2 };
    float settings_width{ 220 };
    float window_padding{ 5 };
    std::vector<std::string> fonts{ "Regular", "Bold" };

    bool menu_opened{ false };
    float menu_height{ 0 };
    float menu_offset{ 0 };
    float menu_alpha{ 0 };
    ImVec2 menu_padding{ 20, 20 };
    ImVec2 menu_spacing{ 20, 20 };
} esp_info;

class esp_preview
{
public:

    template <typename T>
    T* anim_container(T** state_ptr, ImGuiID id)
    {
        T* state = static_cast<T*>(GetStateStorage()->GetVoidPtr(id));
        if (!state)
            GetStateStorage()->SetVoidPtr(id, state = new T());

        *state_ptr = state;
        return state;
    }

    std::vector<item_state> text =
    {
        { "Username", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), true, 0, position_top },

        { "Distance", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_left, { false, false, true, true } },
        { "Flashed", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_left, { false, false, true, true } },
        { "Money", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_left, { false, false, true, true } },
        { "Zoom", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_left, { false, false, true, true } },
        { "Hit", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_left, { false, false, true, true } },

        { "Defusing", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_right, { false, false, true, true } },
        { "Scoped", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_right, { false, false, true, true } },
        { "Bomb", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_right, { false, false, true, true } },
        { "LC", ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), false, 0, position_right, { false, false, true, true } },

    };

    std::vector<item_state> bar =
    {
        { "Health", ImVec4(0.9f, 0.25f, 0.25f, 1.f), ImVec4(0.9f, 0.25f, 0.25f, 1.f), false, 0,  position_left, { false, false, true, true } },
        { "Armor", ImVec4(0.25f, 0.55f, 0.9f, 1.f), ImVec4(0.25f, 0.55f, 0.9f, 1.f), false, 0, position_right, { false, false, true, true } },
    };

    int box_padding = 5;
    int texts_spacing = 20;
    int bars_spacing = 10;

private:


    int texts_buffer[4] = { 0, 0, 0, 0 };
    int bars_buffer[4] = { 0, 0, 0, 0 };
    int hovered_area = area_none;
    float area_size = 80.f;
    float anim_speed = 12.f;

public:

    enum
    {
        position_top = 0,
        position_bottom,
        position_left,
        position_right,
        area_none
    };



    void initialize_preview(const ImVec2& pos, const ImVec2& size)
    {
        fill_box(pos, size);
        fill_areas();
        fill_buffers();
    }

    void render_box()
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImVec2 box_pos = box.position;
        ImVec2 box_sz = box.size;
        ImGui::GetWindowDrawList()->AddRect(window->Pos + box_pos + ImVec2(2,2), window->Pos + box_pos + box_sz - -ImVec2(2, 2), IM_COL32(0, 0, 0, 150), 2.f);
        ImGui::GetWindowDrawList()->AddRect(window->Pos + box_pos - ImVec2(2,2), window->Pos + box_pos + box_sz + -ImVec2(2, 2), IM_COL32(0, 0, 0, 150), 2.f);
        ImGui::GetWindowDrawList()->AddRect(window->Pos + box_pos, window->Pos + box_pos + box_sz, IM_COL32(200, 200, 200, 255), 2.f, 0, 2.f);

    }

    void esp_preview::render_text(item_state& state)
    {
        // animation state
        add_item_state* anim = anim_container(&anim, ImGui::GetID(state.name.c_str()));

        // compute text size
        const ImVec2 text_size = ImGui::CalcTextSize(state.name.c_str());
        // top-left of box in window space
        const ImVec2 box_pos = box.position;
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        // offset based on how many texts/bars already on this side
        ImVec2 pos_offset[] = {
            ImVec2(0.f, texts_buffer[state.position] * -texts_spacing
                        - bars_buffer[state.position] * bars_spacing),
            ImVec2(0.f, texts_buffer[state.position] * texts_spacing
                        + bars_buffer[state.position] * bars_spacing),
            ImVec2(bars_buffer[state.position] * -bars_spacing,
                   texts_buffer[state.position] * texts_spacing),
            ImVec2(bars_buffer[state.position] * bars_spacing,
                   texts_buffer[state.position] * texts_spacing),
        };
        texts_buffer[state.position]++;

        // four candidate positions around the box
        ImVec2 positions[] = {
            // top
            ImVec2(box_pos.x + box.size.x / 2 - text_size.x / 2,
                   box_pos.y - text_size.y - box_padding)
                + pos_offset[position_top],
                // bottom
                ImVec2(box_pos.x + box.size.x / 2 - text_size.x / 2,
                       box_pos.y + box.size.y + box_padding)
                    + pos_offset[position_bottom],
                // left
                ImVec2(box_pos.x - text_size.x - box_padding,
                       box_pos.y)
                    + pos_offset[position_left],
                // right
                ImVec2(box_pos.x + box.size.x + box_padding,
                       box_pos.y)
                    + pos_offset[position_right],
        };

        // animate toward the target slot
        state.location = ImLerp(
            state.location,
            positions[state.position],
            ImGui::GetIO().DeltaTime * anim_speed);

        // clear the swapped flag when close enough
        if (state.swapped)
        {
            if (fabs(state.location.x - positions[state.position].x) < 1.f &&
                fabs(state.location.y - positions[state.position].y) < 1.f)
                state.swapped = false;
        }

        // compute the item's rectangle
        ImRect rect(
            window->Pos + state.location,
            window->Pos + state.location + text_size);
        state.rect = rect;

        // start dragging?
        if (ImGui::IsMouseHoveringRect(rect.Min, rect.Max) &&
            ImGui::IsMouseClicked(0) &&
            ImGui::IsWindowHovered())
        {
            state.active = true;
        }

        // if active, draw a floating preview at the cursor
        if (state.active)
        {
            ImVec2 mouse = ImGui::GetMousePos() - ImVec2(text_size.x / 2, text_size.y / 2);
            auto drawList = ImGui::GetWindowDrawList();
            ImU32 outline_col = IM_COL32(0, 0, 0, 255);
            const ImVec2 outline_offsets[4] = {
                ImVec2(-1,  0),
                ImVec2(1,  0),
                ImVec2(0, -1),
                ImVec2(0,  1),
            };
            if (!state.double_col)
            {
                // draw outline
                for (int i = 0; i < 4; i++)
                    drawList->AddText(
                        ImGui::GetFont(), ImGui::GetFontSize(),
                        mouse + outline_offsets[i],
                        outline_col,
                        state.name.c_str());
                // draw filled text
                drawList->AddText(
                    ImGui::GetFont(), ImGui::GetFontSize(),
                    mouse,
                    ImGui::GetColorU32(state.color),
                    state.name.c_str());
            }
            else
            {
                // outline for gradient text
                for (int i = 0; i < 4; i++)
                    drawList->AddText(
                        ImGui::GetFont(), ImGui::GetFontSize(),
                        mouse + outline_offsets[i],
                        outline_col,
                        state.name.c_str());
                // draw gradient text
                int vtx_start = drawList->VtxBuffer.Size;
                drawList->AddText(
                    ImGui::GetFont(), ImGui::GetFontSize(),
                    mouse,
                    IM_COL32_WHITE,
                    state.name.c_str());
                int vtx_end = drawList->VtxBuffer.Size;
                ShadeVertsLinearColorGradientKeepAlpha(
                    drawList,
                    vtx_start, vtx_end,
                    mouse,
                    mouse + ImVec2(text_size.x, -text_size.y),
                    ImGui::GetColorU32(state.color),
                    ImGui::GetColorU32(state.color_two));
            }

            // manual swap-on-hover logic
            for (auto& stored : text)
            {
                if (stored.name == state.name ||
                    stored.position != state.position ||
                    stored.swapped)
                    continue;

                if (ImGui::IsMouseHoveringRect(stored.rect.Min, stored.rect.Max))
                {
                    std::swap(state, stored);
                    state.active = false;
                    state.swapped = true;
                    stored.active = true;
                    stored.swapped = true;
                }
            }

            // change side if hovering a new area
            if (hovered_area != area_none &&
                state.allowed_positions[hovered_area])
            {
                state.position = hovered_area;
            }

            // finish drag
            if (ImGui::IsMouseReleased(0))
                state.active = false;
        }

        // right-click opens settings popup
        if (!anim->window_opened) {
            if ((ImGui::IsMouseHoveringRect(rect.Min, rect.Max) && ImGui::IsMouseClicked(1)) || (anim->window_opened && (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)))) {
                anim->window_opened = true;
            }
        }

        // animate popup alpha
        anim->window_alpha = ImClamp(
            anim->window_alpha +
            (ImGui::GetIO().DeltaTime * anim_speed *
                (anim->window_opened ? 1.f : -1.f)),
            0.f, 1.f);

        if (anim->window_alpha > 0.01f)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim->window_alpha);
            ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, esp_info.rounding);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, esp_info.spacing);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, esp_info.padding);
            ImGui::PushStyleColor(ImGuiCol_PopupBg,
                utils::ImColorToImVec4(c::background_color));

            ImVec2 wpos = ImVec2(rect.Min.x,
                rect.Max.y + esp_info.window_padding);
            wpos = ImClamp(wpos,
                ImVec2(0, 0),
                ImGui::GetIO().DisplaySize -
                ImVec2(esp_info.settings_width,
                    anim->height));
            ImGui::SetNextWindowPos(wpos);
            ImGui::SetNextWindowSize(
                ImVec2(esp_info.settings_width, anim->height));

            ImGui::Begin("ESP", nullptr,
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoNav |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse |
                ImGuiWindowFlags_Tooltip |
                ImGuiWindowFlags_AlwaysAutoResize);

            ImColor color_one = ImColor{ state.color.x, state.color.y, state.color.z, state.color.w };

            
            DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;


            custom::ColorEdit4(state.name.c_str(), (float*)&color_one, picker_flags);

            if (custom::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 35)))
                anim->window_opened = !anim->window_opened;

            state.color = utils::ImColorToImVec4(color_one);

            


            anim->window_hovered = ImGui::GetCurrentContext()->HoveredWindow &&
                strstr(ImGui::GetCurrentContext()->HoveredWindow->Name,
                    "ESP");

            float c1[4] = {
                state.color.x, state.color.y,
                state.color.z, state.color.w
            };
            float c2[4] = {
                state.color_two.x, state.color_two.y,
                state.color_two.z, state.color_two.w
            };

            // … здесь можно добавить ваши цветовые контролы …

            anim->height =
                ImGui::GetCurrentWindow()->ContentSize.y +
                ImGui::GetStyle().WindowPadding.y * 2;
            ImGui::End();

            ImGui::PopStyleColor();
            ImGui::PopStyleVar(5);
        }

        // finally draw the static text in place with outline
        {
            auto drawList = ImGui::GetWindowDrawList();
            ImVec2 pos = window->Pos + state.location;
            ImU32 outline_col = IM_COL32(0, 0, 0, 255);
            const ImVec2 outline_offsets[4] = {
                ImVec2(-1,  0),
                ImVec2(1,  0),
                ImVec2(0, -1),
                ImVec2(0,  1),
            };
            // outline
            for (int i = 0; i < 4; i++)
                drawList->AddText(
                    ImGui::GetFont(), ImGui::GetFontSize(),
                    pos + outline_offsets[i],
                    outline_col,
                    state.name.c_str());
            // actual text
            drawList->AddText(
                pos,
                ImGui::GetColorU32(state.color),
                state.name.c_str());
        }
    }



    void render_bar(item_state& state)
    {
        add_item_state* anim = anim_container(&anim, ImGui::GetID(state.name.c_str()));

        const ImVec2 box_pos = box.position;
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        float bar_thickness = esp_bar_size; // или просто bar_size, если bar_size == толщина полоски


        // Смещение без SCALE
        ImVec2 pos_offset[] = {
            ImVec2(0.f, bars_buffer[state.position] * -bars_spacing),
            ImVec2(0.f, bars_buffer[state.position] * bars_spacing),
            ImVec2(bars_buffer[state.position] * -bars_spacing, 0.f),
            ImVec2(bars_buffer[state.position] * bars_spacing, 0.f),
        };

        bars_buffer[state.position]++;

        // Позиции без SCALE
        ImVec2 positions[] = {
            // top
            ImVec2(box_pos.x, box_pos.y - box_padding - bar_thickness) + pos_offset[position_top],
            // bottom
            ImVec2(box_pos.x, box_pos.y + box.size.y + box_padding) + pos_offset[position_bottom],
            // left
            ImVec2(box_pos.x - box_padding - bar_thickness, box_pos.y) + pos_offset[position_left],
            // right
            ImVec2(box_pos.x + box.size.x + box_padding, box_pos.y) + pos_offset[position_right],
        };

        // Размеры полосок без SCALE
        ImVec2 bar_sizes[] = {
            ImVec2(box.size.x,            bar_thickness), // top
            ImVec2(box.size.x,            bar_thickness), // bottom
            ImVec2(bar_thickness,         box.size.y),    // left
            ImVec2(bar_thickness,         box.size.y),    // right
        };

        state.location = ImLerp(state.location, positions[state.position], ImGui::GetIO().DeltaTime * 12.f);

        if (state.swapped) {
            if (abs(state.location.x - positions[state.position].x) < 1.f && abs(state.location.y - positions[state.position].y) < 1.f) {
                state.swapped = false;
            }
        }

        ImRect rect = ImRect(ImGui::GetWindowPos() + state.location, ImGui::GetWindowPos() + state.location + bar_sizes[state.position]);
        state.rect = rect;

        if (ImGui::IsMouseHoveringRect(rect.Min, rect.Max) && ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
            state.active = true;

        if (state.active)
        {
            if (!state.double_col)
                ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetMousePos() - rect.GetSize() / 2, ImGui::GetMousePos() + rect.GetSize() / 2, GetColorU32(state.color_two));
            else
            {
                const int vtx_start = GetForegroundDrawList()->VtxBuffer.Size;
                //draw->fade_rect_filled(GetForegroundDrawList(), ImGui::GetMousePos() - rect.GetSize() / 2, ImGui::GetMousePos() + rect.GetSize() / 2, ImColor(1.f, 1.f, 1.f, 1.f), ImColor(1.f, 1.f, 1.f, 1.f), horizontally);
                const int vtx_end = GetForegroundDrawList()->VtxBuffer.Size;
                if (state.position == position_left || state.position == position_right)
                    ShadeVertsLinearColorGradientKeepAlpha(GetForegroundDrawList(), vtx_start, vtx_end, GetMousePos() - ImVec2(0, rect.GetHeight() / 2), GetMousePos() + ImVec2(0, rect.GetHeight() / 2), GetColorU32(state.color), GetColorU32(state.color_two));
                else
                    ShadeVertsLinearColorGradientKeepAlpha(GetForegroundDrawList(), vtx_start, vtx_end, GetMousePos() - ImVec2(rect.GetWidth() / 2, 0), GetMousePos() + ImVec2(rect.GetWidth() / 2, 0), GetColorU32(state.color), GetColorU32(state.color_two));
            }

            for (auto& stored : bar) {
                if (stored.name == state.name || stored.position != state.position || stored.swapped)
                    continue;

                if (ImGui::IsMouseHoveringRect(stored.rect.Min, stored.rect.Max))
                {
                    std::swap(state, stored);

                    state.active = false;
                    state.swapped = true;

                    stored.active = true;
                    stored.swapped = true;
                }

            }

            if (hovered_area != area_none && state.allowed_positions[hovered_area])
            {
                state.position = hovered_area;
            }

            if (ImGui::IsMouseReleased(0))
            {
                state.active = false;
            }
        }

        if ((ImGui::IsMouseHoveringRect(rect.Min, rect.Max) && ImGui::IsMouseClicked(1) || anim->window_opened && (ImGui::IsMouseClicked(1) || ImGui::IsMouseClicked(0)) && !anim->window_hovered) && ImGui::IsWindowHovered())
            anim->window_opened = !anim->window_opened;

        anim->window_alpha = ImClamp(anim->window_alpha + (ImGui::GetIO().DeltaTime * 12.f * (anim->window_opened ? 1.f : -1.f)), 0.f, 1.f);

        if (anim->window_alpha > 0.01f)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim->window_alpha);
            ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, esp_info.rounding);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, esp_info.spacing);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, esp_info.padding);
            ImGui::PushStyleColor(ImGuiCol_PopupBg, utils::ImColorToImVec4(c::background_color));
            ImVec2 wpos = ImVec2(rect.Min.x, rect.GetCenter().y);
            wpos = ImClamp(wpos, ImVec2(0, 0), GetIO().DisplaySize - ImVec2(SCALE(esp_info.settings_width), anim->height));
            ImGui::SetNextWindowPos(wpos);
            ImGui::SetNextWindowSize(ImVec2(esp_info.settings_width, anim->height));
            ImGui::Begin("ESP", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize);
            {
                if (anim->window_opened && IsMouseHoveringRect(GetWindowPos(), GetWindowPos() + GetWindowSize()))
                    SetWindowFocus();

                anim->window_hovered = GetCurrentContext()->HoveredWindow && strstr(GetCurrentContext()->HoveredWindow->Name, "ESP");

                float color_one[4] = { state.color.x, state.color.y, state.color.z, state.color.w };
                float color_two[4] = { state.color_two.x, state.color_two.y, state.color_two.z, state.color_two.w };

                custom::Checkbox("Gradient", &state.double_col);

                if (!state.double_col)
                {
                    //gui->color_edit("Color", color_one, false);
                    state.color = ImVec4(color_one[0], color_one[1], color_one[2], color_one[3]);
                }
                else
                {
                    //gui->double_color_edit("Gradient color", color_one, color_two, false);
                    state.color = ImVec4(color_one[0], color_one[1], color_one[2], color_one[3]);
                    state.color_two = ImVec4(color_two[0], color_two[1], color_two[2], color_two[3]);
                }

                anim->height = GetCurrentWindow()->ContentSize.y + GetStyle().WindowPadding.y * 2;
            }
            ImGui::End();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar(5);
        }

        if (!state.double_col)
            ImGui::GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(state.color));
        else
        {
            const int vtx_start = window->DrawList->VtxBuffer.Size;
            ImGui::GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, ImColor{ 1.f, 1.f, 1.f, 1.f });
            const int vtx_end = window->DrawList->VtxBuffer.Size;
            if (state.position == position_left || state.position == position_right)
                ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vtx_start, vtx_end, rect.Min, ImVec2(rect.Min.x, rect.Max.y), GetColorU32(state.color), GetColorU32(state.color_two));
            else
                ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vtx_start, vtx_end, rect.Min, ImVec2(rect.Max.x, rect.Min.y), GetColorU32(state.color), GetColorU32(state.color_two));
        }

    }


private:
    struct preview_box { ImVec2 position; ImVec2 size; } box;
    ImRect      area_rect[4];

    void fill_box(const ImVec2& pos, const ImVec2& size)
    {
        box.position = pos;
        box.size = size;
    }

    void fill_areas()
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        area_rect[position_top] = ImRect(window->Pos + box.position - ImVec2(0, SCALE(area_size)), window->Pos + box.position + ImVec2(box.size.x, 0));
        area_rect[position_bottom] = ImRect(window->Pos + box.position + ImVec2(0, box.size.y), window->Pos + box.position + box.size + ImVec2(0, SCALE(area_size)));
        area_rect[position_left] = ImRect(window->Pos + box.position - ImVec2(SCALE(area_size), 0), window->Pos + box.position + ImVec2(0, box.size.y));
        area_rect[position_right] = ImRect(window->Pos + box.position + ImVec2(box.size.x, 0), window->Pos + box.position + box.size + ImVec2(SCALE(area_size), 0));
        hovered_area = area_none;
        for (int i = 0; i < 4; i++)
            if (ImGui::IsMouseHoveringRect(area_rect[i].Min, area_rect[i].Max))
                hovered_area = i;
    }

    void fill_buffers()
    {
        std::fill(std::begin(texts_buffer), std::end(texts_buffer), 0);
        std::fill(std::begin(bars_buffer), std::end(bars_buffer), 0);
    }
};

inline esp_preview* esp_p = new esp_preview();
