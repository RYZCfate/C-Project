#include "login_page.hpp"
#include "ui/components/password_field.hpp"
#include "ui/theme/tokens.hpp"
#include <imgui.h>
#include <sodium.h>
#include "core/security.hpp"

namespace PasswordGuard::UI::Pages {

void RenderLoginPage(App::AppContext& ctx, App::VaultService& vault) {
    static bool env_init = false;
    if (!env_init) {
        Core::init_crypto_env();
        env_init = true;
    }

    bool isNew = vault.isNewVault();
    const char* title = isNew ? "Create New Vault" : "Unlock Vault";

    ImGui::Begin(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    auto& state = ctx.ui.login;
    bool trigger_submit = false;

    if (isNew) {
        ImGui::Text("Welcome! Create a master password for your new vault.");
        ImGui::Spacing();
        
        Components::RenderPasswordField("Master Password", state.passwordBuf, sizeof(state.passwordBuf), true);
        
        if (ImGui::InputText("Confirm Password", state.confirmBuf, sizeof(state.confirmBuf), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue)) {
            trigger_submit = true;
        }
    } else {
        ImGui::Text("Please enter your master password to decrypt the vault.");
        ImGui::Spacing();
        
        if (ImGui::InputText("Master Password", state.passwordBuf, sizeof(state.passwordBuf), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue)) {
            trigger_submit = true;
        }
    }
    
    ImGui::Spacing();
    if (ImGui::Button(isNew ? "Create Vault" : "Unlock", ImVec2(120, Tokens::ButtonHeightMedium))) {
        trigger_submit = true;
    }

    if (!state.errorMessage.empty()) {
        ImGui::TextColored(Tokens::Danger, "%s", state.errorMessage.c_str());
    }

    if (trigger_submit) {
        if (isNew) {
            std::string_view p1(state.passwordBuf);
            std::string_view p2(state.confirmBuf);
            if (p1 != p2) {
                state.errorMessage = "Passwords do not match.";
            } else if (Core::get_password_strength(p1) == Core::PasswordStrength::Weak) {
                state.errorMessage = "Password is too weak. (Min 8 chars, needs letters & numbers)";
            } else {
                Core::SecureString masterPassword(state.passwordBuf);
                if (vault.unlock(masterPassword)) {
                    // Create new vault success
                    vault.save(); // Save to disk immediately to persist salt
                    ctx.currentPage = App::PageId::Dashboard;
                    state.wipeSensitive();
                } else {
                    state.errorMessage = "Failed to initialize vault.";
                }
            }
        } else {
            Core::SecureString masterPassword(state.passwordBuf);
            if (vault.unlock(masterPassword)) {
                ctx.currentPage = App::PageId::Dashboard;
                state.wipeSensitive();
            } else {
                state.errorMessage = "Failed to decrypt vault. Wrong password?";
            }
        }
    }

    ImGui::End();
}

} // namespace PasswordGuard::UI::Pages
