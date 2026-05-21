#include "vault_service.hpp"
#include <filesystem>
#include <sodium.h>

namespace PasswordGuard::App {

VaultService::VaultService(SessionContext& session) : session_(session) {}

bool VaultService::isNewVault() const {
    return !std::filesystem::exists("secrets.enc");
}

bool VaultService::unlock(Core::SecureString& password) {
    auto salt = Core::extract_salt_from_db();
    if (!Core::init_master_key(password, salt)) {
        return false;
    }
    
    if (!Core::load_entries(session_.entries)) {
        Core::clear_master_key();
        return false;
    }
    
    session_.isVaultUnlocked = true;
    return true;
}

void VaultService::lock() {
    Core::clear_master_key();
    session_.entries.clear();
    session_.selectedSiteId = std::nullopt;
    session_.isEditMode = false;
    session_.isVaultUnlocked = false;
}

bool VaultService::addCredential(Core::Credential entry) {
    if (findBySite(entry.site)) {
        return false; // Already exists
    }
    session_.entries.push_back(std::move(entry));
    return save();
}

bool VaultService::updateCredential(const std::string& site, Core::Credential updated) {
    auto* existing = findBySite(site);
    if (!existing) return false;
    
    *existing = std::move(updated);
    return save();
}

bool VaultService::deleteCredential(const std::string& site) {
    if (Core::delete_entry(session_.entries, site)) {
        return save();
    }
    return false;
}

Core::Credential* VaultService::findBySite(const std::string& site) {
    return Core::find_entry(session_.entries, site);
}

bool VaultService::save() {
    return Core::save_entries(session_.entries);
}

} // namespace PasswordGuard::App
