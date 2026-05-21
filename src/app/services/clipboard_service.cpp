#include "clipboard_service.hpp"
#include <imgui.h>
#include <functional>

namespace PasswordGuard::App {

void ClipboardService::copyToClipboard(const std::string& text, ClipboardState& state, double currentTime) {
    ImGui::SetClipboardText(text.c_str());
    state.hasPendingClear = true;
    state.copyTimestamp = currentTime;
    state.copiedContentHash = std::to_string(std::hash<std::string>{}(text));
}

void ClipboardService::updateAutoClean(ClipboardState& state, double currentTime) {
    if (!state.hasPendingClear) return;
    
    if (currentTime - state.copyTimestamp >= ClipboardState::kClearTimeoutSeconds) {
        const char* currentClipboard = ImGui::GetClipboardText();
        if (currentClipboard) {
            std::string currentHash = std::to_string(std::hash<std::string>{}(currentClipboard));
            if (currentHash == state.copiedContentHash) {
                ImGui::SetClipboardText("");
            }
        }
        
        state.hasPendingClear = false;
        state.copiedContentHash.clear();
    }
}

} // namespace PasswordGuard::App
