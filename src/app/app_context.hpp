#pragma once
#include "app/session/session_context.hpp"
#include "app/services/clipboard_service.hpp"
#include "ui/ui_state.hpp"

namespace PasswordGuard::App {

enum class PageId {
    Login,
    Dashboard,
    AddEntry,
    TwoFactorAuth
};

struct AppContext {
    PageId currentPage = PageId::Login;
    SessionContext session;       // 会话状态（entries、选中项等）
    ClipboardState clipboard;     // 剪贴板追踪
    UI::UIState ui;               // 所有页面的 UI 状态
    double lastActivityTime = 0.0; // 用于自动锁定
    
    // 重置所有状态（锁库时调用）
    void reset();
};

} // namespace PasswordGuard::App
