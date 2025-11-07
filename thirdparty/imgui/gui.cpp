#include "gui.hpp"
using namespace ImGui;

void c_gui::render_circle_for_horizontal_bar(ImVec2 pos, ImColor color, float alpha) {

    auto draw = GetWindowDrawList();
    draw->AddCircleFilled(pos, 6, ImColor(color.Value.x, color.Value.y, color.Value.z, alpha * GetStyle().Alpha));
}

bool c_gui::tab(const char* icon, const char* label, bool selected) {
    auto window = GetCurrentWindow();
    auto id = window->GetID(label);

    auto pos = window->DC.CursorPos;
    auto draw = window->DrawList;
    float width = GetWindowWidth();
    float height = 30.f;

    ImRect bb(pos, pos + ImVec2(width, height));
    ItemAdd(bb, id);
    ItemSize(bb, GetStyle().FramePadding.y);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Yazýlar için pozisyon ve renk
    auto icon_size = CalcTextSize(icon);
    auto label_size = CalcTextSize(label, 0, 1);

    // Yazý rengi: seçili ve hover’da biraz açýlýr
    float text_alpha = selected ? 1.f : (hovered ? 0.85f : 0.7f);
    ImU32 text_color = ImColor(1.f, 1.f, 1.f, text_alpha);

    // Yazýlar
    draw->AddText(ImVec2(bb.Min.x + 10, bb.GetCenter().y - icon_size.y / 2), gui.accent_color.to_im_color(), icon);
    draw->AddText(ImVec2(bb.Min.x + 35, bb.GetCenter().y - label_size.y / 2), text_color, label);

    // Alt çizgi animasyonu ve glow efekti
    if (selected || hovered) {
        float time = ImGui::GetTime() * 80.f; // Kayma hýzý
        float base_line_height = 2.5f;
        float line_height = hovered ? base_line_height + 1.5f : base_line_height;
        float line_width = 100.f;
        float x_start = bb.Min.x + fmod(time, width + line_width) - line_width;

        // Daha yumuþak ve glowlu degrade renkler
        float base_alpha = selected ? 0.85f : 0.5f;
        ImU32 col_start = ImColor(1.f, 1.f, 1.f, base_alpha * 0.0f);
        ImU32 col_mid_start = ImColor(1.f, 1.f, 1.f, base_alpha * 0.6f);
        ImU32 col_mid = ImColor(1.f, 1.f, 1.f, base_alpha);
        ImU32 col_mid_end = ImColor(1.f, 1.f, 1.f, base_alpha * 0.6f);
        ImU32 col_end = ImColor(1.f, 1.f, 1.f, base_alpha * 0.0f);

        // Kayar degrade çizgiyi 5 parçaya bölelim daha yumuþak geçiþ için
        float segment = line_width / 5.f;

        draw->AddRectFilledMultiColor(
            ImVec2(x_start, bb.Max.y - line_height),
            ImVec2(x_start + segment, bb.Max.y),
            col_start, col_mid_start, col_mid_start, col_start
        );
        draw->AddRectFilledMultiColor(
            ImVec2(x_start + segment, bb.Max.y - line_height),
            ImVec2(x_start + 2 * segment, bb.Max.y),
            col_mid_start, col_mid, col_mid, col_mid_start
        );
        draw->AddRectFilledMultiColor(
            ImVec2(x_start + 2 * segment, bb.Max.y - line_height),
            ImVec2(x_start + 3 * segment, bb.Max.y),
            col_mid, col_mid, col_mid, col_mid
        );
        draw->AddRectFilledMultiColor(
            ImVec2(x_start + 3 * segment, bb.Max.y - line_height),
            ImVec2(x_start + 4 * segment, bb.Max.y),
            col_mid, col_mid_end, col_mid_end, col_mid
        );
        draw->AddRectFilledMultiColor(
            ImVec2(x_start + 4 * segment, bb.Max.y - line_height),
            ImVec2(x_start + 5 * segment, bb.Max.y),
            col_mid_end, col_end, col_end, col_mid_end
        );

        // Hover veya seçili durumda alt tabýn tamamýna hafif glow (blur taklit)
        if (hovered || selected) {
            ImU32 glow_col = ImColor(1.f, 1.f, 1.f, (selected ? 0.15f : 0.07f));
            draw->AddRectFilled(bb.Min, bb.Max, glow_col, 4.f);
        }
    }

    return pressed;
}



bool c_gui::subtab(const char* label, bool selected, int size, ImDrawFlags flags) {

    auto window = GetCurrentWindow();
    auto id = window->GetID(label);

    auto label_size = CalcTextSize(label, 0, 1);

    auto pos = window->DC.CursorPos;
    auto draw = window->DrawList;

    ImRect bb(pos, pos + ImVec2(GetWindowWidth() / size, GetWindowHeight()));
    ItemAdd(bb, id);
    ItemSize(bb, GetStyle().FramePadding.y);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find(id);
    if (value == values.end()) {

        values.insert({ id, 0.f });
        value = values.find(id);
    }

    value->second = ImLerp(value->second, (selected ? 1.f : 0.f), 0.05f);

    draw->AddRectFilled(bb.Min, bb.Max, gui.frame_active.to_im_color(0.8f * value->second), 4, flags);

    draw->AddText(bb.GetCenter() - label_size / 2, selected ? gui.text.to_im_color() : gui.text_disabled.to_im_color(), label);

    return pressed;
}

void c_gui::group_box(const char* name, ImVec2 size_arg) {

    auto window = GetCurrentWindow();
    auto pos = window->DC.CursorPos;

    BeginChild(std::string(name).append(".main").c_str(), size_arg, false, ImGuiWindowFlags_NoScrollbar);

    // Group box'un arka planý ve kenar çizgisi
    GetWindowDrawList()->AddRectFilled(pos + ImVec2(0, 20), pos + size_arg, gui.group_box_bg.to_im_color(), 6);

    // Çok çok daha koyu gri kenar çizgisi için renk
    ImColor borderColor = ImColor(0.05f, 0.05f, 0.05f, 1.f); // Neredeyse siyah, çok koyu gri
    GetWindowDrawList()->AddRect(pos + ImVec2(0, 20), pos + size_arg, borderColor, 6);

    // Baþlýk için yazý ve glow efekti
    GetWindowDrawList()->AddText(pos + ImVec2(12, 0), GetColorU32(ImGuiCol_Text, 0.5f), name);
    auto title_pos = pos + ImVec2(12, 0);
    auto glow_col = ImColor(0.6f, 0.6f, 0.6f, 0.2f);  // Gri glow efekti
    for (int i = 0; i < 2; ++i)
        GetWindowDrawList()->AddText(title_pos + ImVec2(i, i), glow_col, name);

    SetCursorPos(ImVec2(12, 21));
    PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 10 });
    BeginChild(name, { size_arg.x - 24, size_arg.y - 21 }, 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

    BeginGroup();

    PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8, 10 });
    PushStyleVar(ImGuiStyleVar_Alpha, gui.m_anim);
}
void draw_glow_text(ImDrawList* draw, ImFont* font, float size, ImVec2 pos, ImColor color, const char* text) {
    for (int i = 0; i < 3; ++i)
        draw->AddText(font, size, pos + ImVec2(i - 1, i - 1), color, text);
}

void c_gui::end_group_box() {

    PopStyleVar(3);
    EndGroup();
    EndChild();
    EndChild();
}
