#pragma once
#include <vector>
#include <string>
#include <optional>
#include "core/storage/storage.hpp"

namespace PasswordGuard::App {

struct SessionContext {
    std::vector<PasswordGuard::Core::Credential> entries;
    std::optional<std::string> selectedSiteId;  // 替代 Credential* 裸指针
    bool isEditMode = false;
    bool isVaultUnlocked = false;
    
    // 根据 selectedSiteId 查找 Credential，返回指针（仅在当前帧使用）
    Core::Credential* getSelectedCredential();
    const Core::Credential* getSelectedCredential() const;
};

} // namespace PasswordGuard::App
