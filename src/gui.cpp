#include "hello_imgui/hello_imgui.h"
#include "ui/theme/theme.hpp"
#include "ui/pages/login_page.hpp"
#include "ui/pages/dashboard_page.hpp"
#include "ui/pages/add_entry_page.hpp"
#include "ui/pages/totp_page.hpp"
#include "app/app_context.hpp"
#include "app/services/vault_service.hpp"
#include "app/services/clipboard_service.hpp"
#include "ui/theme/tokens.hpp"
#include <imgui.h>

using namespace PasswordGuard;

// 静态实例，供事件循环使用
static App::AppContext g_ctx;
// VaultService 需传入 session 引用
static App::VaultService g_vault(g_ctx.session);

void RenderUI()
{
    double currentTime = ImGui::GetTime();
    
    // 剪贴板自动清理
    App::ClipboardService::updateAutoClean(g_ctx.clipboard, currentTime);
    
    // 自动锁定逻辑（仅在非登录页执行）
    if (g_ctx.currentPage != App::PageId::Login) {
        // 简单检测是否有交互：鼠标按键、滚轮、键盘等
        if (ImGui::IsAnyItemActive() || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemFocused() || 
            ImGui::IsAnyMouseDown() || ImGui::GetIO().MouseWheel != 0.0f) {
            g_ctx.lastActivityTime = currentTime;
        } else {
            // 如果鼠标移动了也算 activity
            static ImVec2 lastMousePos = ImGui::GetMousePos();
            ImVec2 currentMousePos = ImGui::GetMousePos();
            if (currentMousePos.x != lastMousePos.x || currentMousePos.y != lastMousePos.y) {
                g_ctx.lastActivityTime = currentTime;
                lastMousePos = currentMousePos;
            }
        }
        
        if (currentTime - g_ctx.lastActivityTime > UI::Tokens::AutoLockTimeout) {
            g_vault.lock();
            g_ctx.reset();
        }
    } else {
        // 在登录页时，保持 activity time 为当前时间
        g_ctx.lastActivityTime = currentTime;
    }

    switch (g_ctx.currentPage)
    {
    case App::PageId::Login:
        UI::Pages::RenderLoginPage(g_ctx, g_vault);
        break;

    case App::PageId::Dashboard:
        UI::Pages::RenderDashboardPage(g_ctx, g_vault);
        break;

    case App::PageId::AddEntry:
        UI::Pages::RenderAddEntryPage(g_ctx, g_vault);
        break;

    case App::PageId::TwoFactorAuth:
        UI::Pages::RenderTOTPPage(g_ctx, g_vault);
        break;
    }
}

int run_passwordguard_gui()
{
    HelloImGui::RunnerParams params;

    params.appWindowParams.windowGeometry.size = {1400, 900};
    params.appWindowParams.windowTitle = "PasswordGuard";

    params.callbacks.SetupImGuiStyle = []() {
        UI::Theme::Apply();
    };

    params.callbacks.ShowGui = []() {
        RenderUI();
    };

    HelloImGui::Run(params);

    return 0;
}