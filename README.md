# HTTP-server
一个用C++编写的轻量级HTTP服务器，支持基本的Web服务功能，适合学习网络编程和HTTP协议。
A lightweight HTTP server written in C++ that supports basic web service functionalities, making it ideal for learning network programming and the HTTP protocol.
# Simple HTTP Server in C++

一个用C++编写的轻量级HTTP服务器，支持基本的Web服务功能，适合学习网络编程和HTTP协议。

## ✨ 功能特性

- ✅ 支持GET和POST请求处理
- ✅ 静态文件服务（HTML、CSS等）
- ✅ 简单的登录验证系统
- ✅ 基本表单数据处理
- ✅ 加法计算API示例
- ✅ 404错误页面处理
- ✅ 多线程并发连接处理

## 🛠️ 技术栈

- **语言**: C++
- **构建工具**: CMake 2.8+
- **网络编程**: Socket API
- **并发模型**: POSIX线程
- **系统支持**: Linux/macOS/Windows (需支持POSIX)

## 📦 快速开始

### 构建项目

```bash
mkdir -p Cmake
cd Cmake
cmake ..
make
```

### 运行服务器

```bash
# 默认使用80端口
./thttpd.out

# 或者指定端口
./thttpd.out 8080
```

### 访问服务

在浏览器中输入以下地址访问：
- 主页: `http://localhost` (默认端口)
- 或者: `http://localhost:8080` (指定端口)

## 📁 项目结构

```
├── main.cpp             # 程序入口
├── http.cpp/h           # HTTP协议核心处理
├── custom_handle.cpp/h  # 自定义业务逻辑处理
├── myhead.h             # 通用头文件
├── CMakeLists.txt       # CMake构建配置
└── wwwroot/             # 静态资源目录
    ├── index.html       # 主页
    ├── login.html       # 登录页面
    ├── post.html        # POST请求测试页
    ├── 404.html         # 404错误页面
    └── 样式表.css        # 样式文件
```

## 🔧 核心实现

### 请求处理流程

1. **Socket初始化与监听**：创建TCP socket并监听指定端口
2. **客户端连接处理**：使用多线程并发处理客户端请求
3. **HTTP请求解析**：解析请求方法、路径和参数
4. **请求分发**：根据请求类型和路径分发给不同的处理函数
5. **响应生成**：生成HTTP响应并发送给客户端

### 关键模块

- **HTTP模块**：实现HTTP协议解析和响应生成
- **自定义处理器**：处理登录验证和计算等业务逻辑
- **文件服务**：提供静态文件的读取和发送功能

## 📖 学习资源

这个项目可以帮助你学习：
- 网络编程基础（Socket API）
- HTTP协议原理
- 多线程编程
- C++项目构建与管理
- Web服务器基本架构

## ⚠️ 注意事项

- **学习用途**：本项目仅用于学习目的，不建议在生产环境中使用
- **安全限制**：缺乏完善的安全防护措施（如输入验证、认证授权等）
- **协议支持**：仅实现了HTTP协议的部分功能

## 📄 许可证

[MIT License](LICENSE)

## 🤝 贡献

欢迎提交Issue和Pull Request来改进这个项目！

---

**用C++构建自己的Web服务器，从这里开始！** 🚀
