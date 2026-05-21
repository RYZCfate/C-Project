#include "add_entry_page.hpp"
#include "ui/layout/sidebar.hpp"
#include "ui/components/password_field.hpp"
#include "ui/theme/tokens.hpp"
#include "core/totp/totp.hpp"
#include <imgui.h>
#include <string>

namespace PasswordGuard::UI::Pages {

void RenderAddEntryPage(App::AppContext& ctx, App::VaultService& vault) {
    ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->Pos);
    ImGui::SetNextWindowSize(vp->Size);

    ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    Layout::RenderSidebar(ctx, vault);

    ImGui::SameLine();
    ImGui::BeginChild("Content", ImVec2(0, 0), ImGuiChildFlags_None);

    auto& state = ctx.ui.addEntry;
    bool isEdit = ctx.session.isEditMode;

    ImGui::Text(isEdit ? "Edit Credential" : "Add New Credential");
    ImGui::Separator();
    ImGui::Spacing();

    // 如果是编辑模式且是初次进入该页面（即 siteBuf 为空），需要加载数据
    if (isEdit && state.siteBuf[0] == '\0' && ctx.session.selectedSiteId) {
        auto* entry = vault.findBySite(*ctx.session.selectedSiteId);
        if (entry) {
            strncpy(state.siteBuf, entry->site.c_str(), sizeof(state.siteBuf) - 1);
            strncpy(state.userBuf, entry->username.c_str(), sizeof(state.userBuf) - 1);
            strncpy(state.passBuf, entry->password.c_str(), sizeof(state.passBuf) - 1);
            strncpy(state.totpBuf, entry->totp_secret.c_str(), sizeof(state.totpBuf) - 1);
        }
    }

    ImGui::SetNextItemWidth(400);
    if (isEdit) {
        ImGui::InputText("Site Name", state.siteBuf, sizeof(state.siteBuf), ImGuiInputTextFlags_ReadOnly);
    } else {
        ImGui::InputText("Site Name", state.siteBuf, sizeof(state.siteBuf));
    }

    ImGui::Spacing();
    ImGui::SetNextItemWidth(400);
    ImGui::InputText("Username", state.userBuf, sizeof(state.userBuf));

    ImGui::Spacing();
    ImGui::SetNextItemWidth(400);
    Components::RenderPasswordField("Password", state.passBuf, sizeof(state.passBuf), true);

    ImGui::Spacing();
    ImGui::SetNextItemWidth(400);
    ImGui::InputTextWithHint("TOTP Secret (Base32)", "Optional", state.totpBuf, sizeof(state.totpBuf));
    
    // TOTP Verify button
    if (state.totpBuf[0] != '\0') {
        ImGui::SameLine();
        if (ImGui::Button("Verify Secret")) {
            try {
                state.verifyCode = Core::TOTP::generate_totp(state.totpBuf);
                state.errorMsg = "";
            } catch (const std::exception& e) {
                state.errorMsg = "Invalid TOTP Secret!";
                state.verifyCode = "";
            }
        }
        if (!state.verifyCode.empty()) {
            ImGui::TextColored(Tokens::Success, "Current Code: %s", state.verifyCode.c_str());
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Save", ImVec2(100, Tokens::ButtonHeightMedium))) {
        if (strlen(state.siteBuf) == 0) {
            state.errorMsg = "Site Name cannot be empty.";
        } else if (strlen(state.passBuf) == 0) {
            state.errorMsg = "Password cannot be empty.";
        } else {
            Core::Credential new_entry{
                std::string(state.siteBuf),
                std::string(state.userBuf),
                Core::SecureString(state.passBuf),
                Core::SecureString(state.totpBuf)
            };
            
            bool success = false;
            if (isEdit) {
                success = vault.updateCredential(new_entry.site, std::move(new_entry));
            } else {
                success = vault.addCredential(std::move(new_entry));
                if (!success) {
                    state.errorMsg = "Site already exists.";
                }
            }
            
            if (success) {
                state.clear();
                ctx.currentPage = App::PageId::Dashboard;
            } else if (state.errorMsg.empty()) {
                state.errorMsg = "Failed to save credential.";
            }
        }
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(100, Tokens::ButtonHeightMedium))) {
        state.clear();
        ctx.currentPage = App::PageId::Dashboard;
    }

    if (!state.errorMsg.empty()) {
        ImGui::Spacing();
        ImGui::TextColored(Tokens::Danger, "%s", state.errorMsg.c_str());
    }

    ImGui::EndChild();
    ImGui::End();
}

} // namespace PasswordGuard::UI::Pages
