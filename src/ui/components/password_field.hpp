#pragma once
#include <cstddef>

namespace PasswordGuard::UI::Components {

// 渲染带强度指示器的密码输入框
bool RenderPasswordField(
    const char* label, 
    char* buf, 
    size_t bufSize,
    bool showStrengthBar = true
);

} // namespace PasswordGuard::UI::Components
