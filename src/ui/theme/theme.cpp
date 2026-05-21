#include "theme.hpp"
#include "tokens.hpp"
#include <imgui.h>

namespace PasswordGuard::UI::Theme {
    void Apply() {
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 4.0f;
        style.WindowRounding = 6.0f;
        style.PopupRounding = 4.0f;
        style.ScrollbarRounding = 4.0f;
        style.TabRounding = 4.0f;
    }
}
