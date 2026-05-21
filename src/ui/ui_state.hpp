#pragma once
#include <string>
#include <unordered_set>
#include <sodium.h>

namespace PasswordGuard::UI {

struct LoginPageState {
    char passwordBuf[128] = {};
    char confirmBuf[128] = {};
    std::string errorMessage;
    
    void wipeSensitive() {
        sodium_memzero(passwordBuf, sizeof(passwordBuf));
        sodium_memzero(confirmBuf, sizeof(confirmBuf));
        errorMessage.clear();
    }
};

struct AddEntryPageState {
    char siteBuf[128] = {};
    char userBuf[128] = {};
    char passBuf[128] = {};
    char totpBuf[128] = {};
    std::string errorMsg;
    std::string verifyCode;
    
    void wipeSensitive() {
        sodium_memzero(passBuf, sizeof(passBuf));
        sodium_memzero(totpBuf, sizeof(totpBuf));
    }
    
    void clear() {
        wipeSensitive();
        sodium_memzero(siteBuf, sizeof(siteBuf));
        sodium_memzero(userBuf, sizeof(userBuf));
        errorMsg.clear();
        verifyCode.clear();
    }
};

struct DashboardPageState {
    char searchBuf[128] = {};
    std::unordered_set<std::string> revealedSites;
    std::string lastCopiedLabel;
    float copyFeedbackTimer = 0.0f;
    
    void clearRevealed() {
        revealedSites.clear();
    }
};

struct TOTPPageState {
};

struct UIState {
    LoginPageState login;
    AddEntryPageState addEntry;
    DashboardPageState dashboard;
    TOTPPageState totp;
    
    void wipeAll() {
        login.wipeSensitive();
        addEntry.clear();
        dashboard.clearRevealed();
        sodium_memzero(dashboard.searchBuf, sizeof(dashboard.searchBuf));
        dashboard.lastCopiedLabel.clear();
        dashboard.copyFeedbackTimer = 0.0f;
    }
};

} // namespace PasswordGuard::UI
