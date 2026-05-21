#include "security.hpp"
#include <cctype>

namespace PasswordGuard::Core {

PasswordStrength get_password_strength(std::string_view pwd) {
    if (pwd.length() < 8) return PasswordStrength::Weak;

    bool has_lower = false, has_upper = false, has_digit = false, has_special = false;
    for (char ch : pwd) {
        unsigned char uc = static_cast<unsigned char>(ch);
        if (std::islower(uc)) has_lower = true;
        else if (std::isupper(uc)) has_upper = true;
        else if (std::isdigit(uc)) has_digit = true;
        else has_special = true;
    }

    if (pwd.length() >= 12 && has_lower && has_upper && has_digit && has_special) {
        return PasswordStrength::Strong;
    }

    if ((has_lower || has_upper) && has_digit) {
        return PasswordStrength::Fair;
    }

    return PasswordStrength::Weak;
}

bool is_strong_password(std::string_view pwd) {
    return get_password_strength(pwd) >= PasswordStrength::Fair;
}

} // namespace PasswordGuard::Core