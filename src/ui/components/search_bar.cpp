#include "search_bar.hpp"
#include <imgui.h>

namespace PasswordGuard::UI::Components {

bool RenderSearchBar(char* buf, size_t bufSize, float width) {
    bool changed = false;
    ImGui::SetNextItemWidth(width);
    if (ImGui::InputTextWithHint("##Search", "Search sites...", buf, bufSize)) {
        changed = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        buf[0] = '\0';
        changed = true;
    }
    return changed;
}

}
