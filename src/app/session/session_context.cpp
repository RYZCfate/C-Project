#include "session_context.hpp"
#include <algorithm>

namespace PasswordGuard::App {

Core::Credential* SessionContext::getSelectedCredential() {
    if (!selectedSiteId) return nullptr;
    auto it = std::ranges::find_if(entries, [this](const Core::Credential& c) {
        return c.site == *selectedSiteId;
    });
    if (it != entries.end()) return &(*it);
    return nullptr;
}

const Core::Credential* SessionContext::getSelectedCredential() const {
    if (!selectedSiteId) return nullptr;
    auto it = std::ranges::find_if(entries, [this](const Core::Credential& c) {
        return c.site == *selectedSiteId;
    });
    if (it != entries.end()) return &(*it);
    return nullptr;
}

} // namespace PasswordGuard::App
