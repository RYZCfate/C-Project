#include "totp_page.hpp"
#include "ui/layout/sidebar.hpp"
#include "ui/theme/tokens.hpp"
#include "core/totp/totp.hpp"
#include "app/services/clipboard_service.hpp"
#include <imgui.h>
#include <chrono>

namespace PasswordGuard::UI::Pages {

void RenderTOTPPage(App::AppContext& ctx, App::VaultService& vault) {
    ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->Pos);
    ImGui::SetNextWindowSize(vp->Size);

    ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    Layout::RenderSidebar(ctx, vault);

    ImGui::SameLine();
    ImGui::BeginChild("Content", ImVec2(0, 0), ImGuiChildFlags_None);

    auto* entry = ctx.session.getSelectedCredential();

    if (!entry) {
        ImGui::TextColored(Tokens::Danger, "Error: No credential selected.");
        if (ImGui::Button("Back to Dashboard")) {
            ctx.currentPage = App::PageId::Dashboard;
        }
        ImGui::EndChild();
        ImGui::End();
        return;
    }

    ImGui::Text("Two-Factor Authentication for %s", entry->site.c_str());
    ImGui::Separator();
    ImGui::Spacing();

    std::string secret(entry->totp_secret.data(), entry->totp_secret.size());

    if (secret.empty()) {
        ImGui::TextDisabled("No TOTP secret configured for this site.");
    } else {
        try {
            std::string code = Core::TOTP::generate_totp(secret);

            ImGui::SetWindowFontScale(2.0f);
            ImGui::TextColored(Tokens::Info, "%s", code.c_str());
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Spacing();

            // Progress bar
            auto now = std::chrono::system_clock::now();
            uint64_t time_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
            float progress = 1.0f - static_cast<float>(time_since_epoch % 30) / 30.0f;
            ImGui::ProgressBar(progress, ImVec2(200.0f, 0.0f), "Time Remaining");

            ImGui::Spacing();
            if (ImGui::Button("Copy Code", ImVec2(120, Tokens::ButtonHeightMedium))) {
                App::ClipboardService::copyToClipboard(code, ctx.clipboard, ImGui::GetTime());
                ctx.ui.dashboard.lastCopiedLabel = "2FA code";
                ctx.ui.dashboard.copyFeedbackTimer = Tokens::CopyFeedbackDuration;
            }
        }
        catch (const std::exception& e) {
            ImGui::TextColored(Tokens::Danger, "Error generating TOTP: %s", e.what());
        }
    }
    
    // Clear the secret std::string explicitly, just in case
    sodium_memzero(secret.data(), secret.size());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Back", ImVec2(100, Tokens::ButtonHeightMedium))) {
        ctx.session.selectedSiteId = std::nullopt;
        ctx.currentPage = App::PageId::Dashboard;
    }

    ImGui::EndChild();
    ImGui::End();
}

} // namespace PasswordGuard::UI::Pages
