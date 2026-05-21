#pragma once
#include "app/app_context.hpp"
#include "app/services/vault_service.hpp"

namespace PasswordGuard::UI::Layout {

void RenderSidebar(App::AppContext& ctx, App::VaultService& vault);

} // namespace PasswordGuard::UI::Layout
