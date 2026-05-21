#pragma once
#include "app/app_context.hpp"
#include "app/services/vault_service.hpp"

namespace PasswordGuard::UI::Pages {

void RenderTOTPPage(App::AppContext& ctx, App::VaultService& vault);

} // namespace PasswordGuard::UI::Pages
