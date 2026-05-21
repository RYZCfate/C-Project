#include "dashboard_page.hpp"
#include "ui/layout/sidebar.hpp"
#include "ui/components/search_bar.hpp"
#include "ui/components/credential_card.hpp"
#include "ui/theme/tokens.hpp"
#include "app/services/clipboard_service.hpp"
#include <imgui.h>
#include <algorithm>
#include <string>

namespace PasswordGuard::UI::Pages {

void RenderDashboardPage(App::AppContext& ctx, App::VaultService& vault) {
    ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->Pos);
    ImGui::SetNextWindowSize(vp->Size);

    ImGui::Begin("MainWindow", nullptr, 
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    Layout::RenderSidebar(ctx, vault);

    ImGui::SameLine();

    ImGui::BeginChild("Content", ImVec2(0, 0), ImGuiChildFlags_None);

    ImGui::Text("Vault Management");
    ImGui::Separator();
    ImGui::Spacing();

    auto& state = ctx.ui.dashboard;

    // Search Bar
    Components::RenderSearchBar(state.searchBuf, sizeof(state.searchBuf), Tokens::SearchBarWidth);

    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 120);
    if (ImGui::Button("Add Credential", ImVec2(120, Tokens::ButtonHeightSmall))) {
        ctx.currentPage = App::PageId::AddEntry;
        ctx.session.isEditMode = false;
        ctx.session.selectedSiteId = std::nullopt;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (state.copyFeedbackTimer > 0) {
        ImGui::TextColored(Tokens::Success, "Copied %s to clipboard!", state.lastCopiedLabel.c_str());
        state.copyFeedbackTimer -= ImGui::GetIO().DeltaTime;
    } else {
        ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
    }

    ImGui::BeginChild("ScrollArea");
    std::string filter = state.searchBuf;
    std::transform(filter.begin(), filter.end(), filter.begin(), ::tolower);

    for (size_t i = 0; i < ctx.session.entries.size(); ++i) {
        const auto& entry = ctx.session.entries[i];
        
        std::string site_lower = entry.site;
        std::transform(site_lower.begin(), site_lower.end(), site_lower.begin(), ::tolower);
        if (!filter.empty() && site_lower.find(filter) == std::string::npos) continue;

        bool isRevealed = state.revealedSites.count(entry.site) > 0;
        
        auto action = Components::RenderCredentialCard(entry, i, isRevealed);
        
        if (action.type == Components::CredentialCardAction::ToggleReveal) {
            if (isRevealed) {
                state.revealedSites.erase(entry.site);
            } else {
                state.revealedSites.insert(entry.site);
            }
        } else if (action.type == Components::CredentialCardAction::Copy) {
            // 根据 reveal 状态复制密码或显示内容（这里默认复制密码）
            // 注意：entry.password 是 SecureString，转换为 std::string 传给 clipboard service
            App::ClipboardService::copyToClipboard(
                std::string(entry.password.data(), entry.password.size()), 
                ctx.clipboard, 
                ImGui::GetTime()
            );
            state.lastCopiedLabel = "password";
            state.copyFeedbackTimer = Tokens::CopyFeedbackDuration;
        } else if (action.type == Components::CredentialCardAction::Edit) {
            ctx.session.selectedSiteId = entry.site;
            ctx.session.isEditMode = true;
            ctx.currentPage = App::PageId::AddEntry;
        } else if (action.type == Components::CredentialCardAction::View2FA) {
            ctx.session.selectedSiteId = entry.site;
            ctx.currentPage = App::PageId::TwoFactorAuth;
        } else if (action.type == Components::CredentialCardAction::Delete) {
            ImGui::OpenPopup(("Delete Confirmation##" + entry.site).c_str());
        }
        
        if (ImGui::BeginPopupModal(("Delete Confirmation##" + entry.site).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete '%s'?", entry.site.c_str());
            ImGui::Spacing();
            if (ImGui::Button("Yes, Delete", ImVec2(120, Tokens::ButtonHeightSmall))) {
                vault.deleteCredential(entry.site);
                if (ctx.session.selectedSiteId == entry.site) {
                    ctx.session.selectedSiteId = std::nullopt;
                }
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                break; // Break loop because entries vector changed
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, Tokens::ButtonHeightSmall))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        
        ImGui::Spacing();
    }
    ImGui::EndChild(); // ScrollArea

    ImGui::EndChild(); // Content

    ImGui::End();
}

} // namespace PasswordGuard::UI::Pages