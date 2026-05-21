#pragma once
#include <string>

namespace PasswordGuard::App {

struct ClipboardState {
    bool hasPendingClear = false;
    double copyTimestamp = 0.0;
    std::string copiedContentHash;  // 不存原始密码，存 hash 用于比对
    
    static constexpr double kClearTimeoutSeconds = 15.0;
};

class ClipboardService {
public:
    static void copyToClipboard(const std::string& text, ClipboardState& state, double currentTime);
    static void updateAutoClean(ClipboardState& state, double currentTime);
};

} // namespace PasswordGuard::App
