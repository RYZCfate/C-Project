#pragma once
#include <string>
#include <string_view>

namespace PasswordGuard::Core {

enum class PasswordStrength {
    Weak,
    Fair,
    Strong
};

PasswordStrength get_password_strength(std::string_view pwd);

// 主密码强度校验接口
bool is_strong_password(std::string_view pwd);

} // namespace PasswordGuard::Core
