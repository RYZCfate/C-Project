#pragma once
#include "core/storage/storage.hpp"

namespace PasswordGuard::UI::Components {

struct CredentialCardAction {
    enum Type { None, Copy, Edit, Delete, View2FA, ToggleReveal };
    Type type = None;
};

// 渲染单条凭据卡片，返回用户执行的操作
CredentialCardAction RenderCredentialCard(
    const Core::Credential& entry,
    int index,
    bool isRevealed
);

} // namespace PasswordGuard::UI::Components
