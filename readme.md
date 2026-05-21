# PasswordGuard (oro)

PasswordGuard 是一个基于 C++ 和 ImGui 构建的本地密码管理器，致力于提供安全、高效且跨平台的凭据管理体验。项目经过多次重构，目前采用了清晰的分层架构设计。

## 1. 核心架构与目录说明

项目源码位于 `src/` 目录下，主要分为以下几个层次：

### 1.1 `src/core/` (核心业务逻辑层)
负责底层的数据安全与加密计算，不依赖于任何 UI 框架。
- **`crypto/`**: 基于 `libsodium` 的加解密模块。包含主密钥派生 (Argon2id)、XChaCha20-Poly1305 加密存储、内存安全清除等功能。
- **`storage/`**: 数据持久化模块。负责将加密数据进行读写、JSON 序列化，以及数据防损机制（临时文件覆盖）。
- **`totp/`**: 2FA/TOTP 模块。基于 `OpenSSL` 和 `ZXing` / `OpenCV`，支持手动输入密钥、屏幕截图扫码和摄像头扫码导入，以及实时验证码生成与校验。
- **`security.cpp` & `password_input.cpp`**: 负责密码强度的检测与安全的输入处理。

### 1.2 `src/app/` (应用服务层)
负责协调核心逻辑与 UI 的交互，维护应用的生命周期和全局状态。
- **`app_context` / `session_context`**: 维护当前登录会话的全局状态。
- **`vault_service`**: 封装核心加密与存储 API，提供对上层友好的“密码库”管理服务（如加载、增删改查）。
- **`clipboard_service`**: 提供跨平台的安全剪贴板操作，支持复制密码后自动清除。

### 1.3 `src/ui/` (用户交互层)
基于 `HelloImGui` 和 `ImGui` 构建的现代图形界面。
- **`pages/`**: 具体的页面级视图，包含登录页 (`login_page`)、主控制台 (`dashboard_page`)、添加条目页 (`add_entry_page`) 和 两步验证页 (`totp_page`)。
- **`components/`**: 可复用的 UI 组件，如凭据卡片 (`credential_card`)、搜索栏 (`search_bar`)、隐藏式密码输入框 (`password_field`)。
- **`layout/`**: 全局布局组件，如侧边栏 (`sidebar`)。
- **`theme/`**: 全局主题管理与设计令牌 (Tokens)，控制应用程序的视觉风格和配色。

### 1.4 入口文件
- **`main.cpp`**: 程序的启动入口，负责应用初始化和环境检查。
- **`gui.cpp`**: 负责初始化 ImGui 环境、字体加载和渲染主循环。

## 2. 外部依赖项

本项目通过 `vcpkg` 和 `CMake` 管理依赖：
- **[libsodium](https://doc.libsodium.org/)**: 用于所有底层的加密操作（Argon2id, XChaCha20-Poly1305）。
- **[Hello ImGui](https://github.com/pthom/hello_imgui) & [Dear ImGui](https://github.com/ocornut/imgui)**: 用于快速构建现代化跨平台图形界面。
- **[nlohmann/json](https://github.com/nlohmann/json)**: 用于凭据数据的内存结构序列化与反序列化。
- **[OpenSSL](https://www.openssl.org/)**: 协助计算 HMAC-SHA1，用于 TOTP 代码生成。
- **[ZXing-C++](https://github.com/zxing-cpp/zxing-cpp) & [OpenCV](https://opencv.org/)**: 用于 2FA 的二维码扫描（支持屏幕读取与摄像头捕捉）。

## 3. 开发与构建

项目采用 `CMake` 作为构建系统，且强依赖 C++ 20 标准。
构建之前，请确保已正确安装上述依赖项的 `vcpkg` 版本，并通过 `CMAKE_TOOLCHAIN_FILE` 指定到你的 vcpkg 环境中。

主要构建环境示例：
- **编译器**: 支持 MSVC 或 Clang
- **标准**: C++ 20 

## 4. UI 交互流程示例（对于二次开发）
当前的 UI 已经被重构并拆分为不同的 Page。在进行 UI 层级的新特性开发时：
1. 请勿直接在 UI 中调用 `core` 层的加密函数。
2. 应优先调用 `src/app/` 层的 `VaultService` 和 `SessionContext`。
3. 如果需新增页面，请在 `src/ui/pages/` 目录下新建类，并通过主窗口路由进行切换。
