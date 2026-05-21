#include "sidebar.hpp"
#include "ui/theme/tokens.hpp"
#include <imgui.h>

namespace PasswordGuard::UI::Layout {

void RenderSidebar(App::AppContext& ctx, App::VaultService& vault) {
    ImGui::BeginChild("Sidebar", ImVec2(Tokens::SidebarWidth, 0), true);

    ImGui::Spacing();
    ImGui::Text("PasswordGuard");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // 导航
    if (ImGui::Selectable("Dashboard", ctx.currentPage == App::PageId::Dashboard)) {
        if (ctx.currentPage == App::PageId::AddEntry) {
            ctx.ui.addEntry.wipeSensitive();
        }
        ctx.currentPage = App::PageId::Dashboard;
    }

    if (ImGui::Selectable("Add Credential", ctx.currentPage == App::PageId::AddEntry && !ctx.session.isEditMode)) {
        if (ctx.currentPage == App::PageId::AddEntry) {
            ctx.ui.addEntry.wipeSensitive();
        }
        ctx.currentPage = App::PageId::AddEntry;
        ctx.session.isEditMode = false;
        ctx.session.selectedSiteId = std::nullopt;
    }
    
    // 占据剩余高度，将 Lock 按钮推到底部
    ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - 40));
    
    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Text, Tokens::Danger);
    if (ImGui::Selectable("Lock Vault")) {
        vault.lock();
        ctx.reset();
    }
    ImGui::PopStyleColor();

    ImGui::EndChild();
}

} // namespace PasswordGuard::UI::Layout
