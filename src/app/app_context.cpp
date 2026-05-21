#include "app_context.hpp"

namespace PasswordGuard::App {

void AppContext::reset() {
    currentPage = PageId::Login;
    session.entries.clear();
    session.selectedSiteId = std::nullopt;
    session.isEditMode = false;
    session.isVaultUnlocked = false;
    ui.wipeAll();
    lastActivityTime = 0.0;
}

} // namespace PasswordGuard::App
