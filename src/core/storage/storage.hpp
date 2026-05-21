#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "crypto/crypto.hpp"

namespace PasswordGuard::Core {

// 密码凭据数据结构：一个站点对应一条记录
struct Credential {
    std::string site;
    std::string username;
    SecureString password;
    SecureString totp_secret;
};

void to_json(nlohmann::json& j, const Credential& p);
void from_json(const nlohmann::json& j, Credential& p);

// 从文件中提前提取 Salt（供初始化密钥时使用）
std::vector<unsigned char> extract_salt_from_db();

// 读取并解密所有条目，失败返回 false
bool load_entries(std::vector<Credential>& entries);
// 加密并保存所有条目，失败返回 false
bool save_entries(const std::vector<Credential>& entries);
// 按站点查找条目，找不到返回 nullptr
Credential* find_entry(std::vector<Credential>& entries, const std::string& site);
// 删除指定站点条目，返回是否删除成功
bool delete_entry(std::vector<Credential>& entries, const std::string& site);

} // namespace PasswordGuard::Core