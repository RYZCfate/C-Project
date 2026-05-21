#include "password_field.hpp"
#include "core/security.hpp"
#include "ui/theme/tokens.hpp"
#include <imgui.h>
#include <string_view>

namespace PasswordGuard::UI::Components {

bool RenderPasswordField(const char* label, char* buf, size_t bufSize, bool showStrengthBar) {
    bool changed = ImGui::InputText(
        label, buf, bufSize,
        ImGuiInputTextFlags_Password
    );
    
    if (showStrengthBar && buf[0] != '\0') {
        Core::PasswordStrength strength = Core::get_password_strength(std::string_view(buf));
        ImVec4 color;
        float fraction = 0.0f;
        
        switch (strength) {
            case Core::PasswordStrength::Strong:
                color = Tokens::Success;
                fraction = 1.0f;
                break;
            case Core::PasswordStrength::Fair:
                color = Tokens::Info;
                fraction = 0.6f;
                break;
            case Core::PasswordStrength::Weak:
            default:
                color = Tokens::Danger;
                fraction = 0.3f;
                break;
        }
        
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
        ImGui::ProgressBar(fraction, ImVec2(-1.0f, Tokens::ProgressBarHeight), "");
        ImGui::PopStyleColor();
    }
    
    return changed;
}

} // namespace PasswordGuard::UI::Components
