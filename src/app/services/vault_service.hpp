#pragma once
#include <string>
#include "app/session/session_context.hpp"
#include "core/crypto/crypto.hpp"

namespace PasswordGuard::App {

// 封装所有 Vault 业务操作，UI 层不直接接触 Core API
class VaultService {
public:
    explicit VaultService(SessionContext& session);
    
    bool isNewVault() const;           // 检测 secrets.enc 是否存在
    bool unlock(Core::SecureString& password);
    void lock();
    
    bool addCredential(Core::Credential entry);
    bool updateCredential(const std::string& site, Core::Credential updated);
    bool deleteCredential(const std::string& site);
    Core::Credential* findBySite(const std::string& site);
    
    bool save();                       // 加密写盘
    
private:
    SessionContext& session_;
};

} // namespace PasswordGuard::App
