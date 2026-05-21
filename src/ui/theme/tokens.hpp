#pragma once
#include <imgui.h>

namespace PasswordGuard::UI::Tokens {

// ── Color Palette ──
constexpr ImVec4 Success       = {0.3f, 0.8f, 0.3f, 1.0f};
constexpr ImVec4 Danger        = {1.0f, 0.4f, 0.4f, 1.0f};
constexpr ImVec4 DangerBg      = {0.7f, 0.1f, 0.1f, 1.0f};
constexpr ImVec4 DangerHover   = {0.9f, 0.2f, 0.2f, 1.0f};
constexpr ImVec4 Info          = {0.3f, 0.7f, 0.9f, 1.0f};
constexpr ImVec4 WarningText   = {0.8f, 0.3f, 0.3f, 1.0f};

// ── Layout ──
constexpr float SidebarWidth       = 180.0f;
constexpr float CardHeight         = 80.0f;
constexpr float ButtonHeightSmall  = 30.0f;
constexpr float ButtonHeightMedium = 35.0f;
constexpr float ButtonHeightLarge  = 40.0f;
constexpr float IconRounding       = 20.0f;
constexpr float ProgressBarHeight  = 5.0f;
constexpr float SearchBarWidth     = 300.0f;

// ── Timing ──
constexpr float CopyFeedbackDuration = 2.0f;
constexpr double AutoLockTimeout     = 300.0;  // 5 分钟
constexpr double ClipboardTimeout    = 15.0;   // 15 秒

} // namespace PasswordGuard::UI::Tokens
