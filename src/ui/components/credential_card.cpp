#include "credential_card.hpp"
#include "ui/theme/tokens.hpp"
#include <imgui.h>
#include <string>

namespace PasswordGuard::UI::Components {

CredentialCardAction RenderCredentialCard(
    const Core::Credential& entry,
    int index,
    bool isRevealed
) {
    CredentialCardAction result;
    
    ImGui::PushID(entry.site.c_str());
    ImGui::BeginChild("EntryCard", ImVec2(0, Tokens::CardHeight), true);
    
    // Site Info
    ImGui::SetCursorPos(ImVec2(10, 15));
    ImGui::BeginGroup();
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Tokens::IconRounding);
    ImGui::Button(entry.site.substr(0,1).c_str(), ImVec2(50, 50));
    ImGui::PopStyleVar();
    ImGui::SameLine(70);
    
    ImGui::BeginGroup();
    ImGui::Text("%s", entry.site.c_str());
    ImGui::TextDisabled("%s", entry.username.c_str());
    ImGui::EndGroup();
    ImGui::EndGroup();

    // Actions
    float available_x = ImGui::GetContentRegionAvail().x;
    ImGui::SameLine(available_x - 300); // 调整以容纳新按钮
    
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(25);
    
    if (ImGui::Button(isRevealed ? "Hide" : "Show", ImVec2(55, Tokens::ButtonHeightSmall))) {
        result.type = CredentialCardAction::ToggleReveal;
    }
    ImGui::SameLine();
    if (ImGui::Button("Copy", ImVec2(55, Tokens::ButtonHeightSmall))) {
        result.type = CredentialCardAction::Copy;
    }
    ImGui::SameLine();
    if (ImGui::Button("Edit", ImVec2(55, Tokens::ButtonHeightSmall))) {
        result.type = CredentialCardAction::Edit;
    }
    ImGui::SameLine();
    if (ImGui::Button("2FA", ImVec2(55, Tokens::ButtonHeightSmall))) {
        result.type = CredentialCardAction::View2FA;
    }
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, Tokens::Danger);
    if (ImGui::Button("Del", ImVec2(55, Tokens::ButtonHeightSmall))) {
        result.type = CredentialCardAction::Delete;
    }
    ImGui::PopStyleColor();
    
    ImGui::EndGroup();

    ImGui::EndChild();
    ImGui::PopID();
    
    return result;
}

} // namespace PasswordGuard::UI::Components
