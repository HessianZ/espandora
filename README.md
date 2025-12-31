# ESPandora

基于 ESP32-S3-BOX-Lite 开发板的智能语音交互系统，集成了传感器监测、Home Assistant 集成、Web 服务等功能。

## 项目概述

ESPandora 是一个功能丰富的 ESP32-S3 嵌入式应用，主要特性包括：

- **语音识别与交互**：支持中英文唤醒词和语音命令
- **环境监测**：集成 AHT20（温湿度）和 BMP280（气压）传感器
- **Home Assistant 集成**：通过 MQTT 协议与 Home Assistant 通信
- **图形用户界面**：基于 LVGL 的触摸屏界面
- **网络功能**：Wi-Fi 配网、HTTP 服务器、NTP 时间同步
- **GPIO 控制**：支持运动检测和开关控制

## 硬件平台

- **开发板**: ESP32-S3-BOX-Lite
- **芯片**: ESP32-S3
- **存储**: 16MB Flash, 支持 PSRAM
- **外设**: 
  - LCD 触摸屏
  - 麦克风（语音识别）
  - I2C 传感器接口（GPIO10:SCL, GPIO11:SDA）
  - GPIO 扩展（运动检测、开关控制）

## 项目结构

```
espandora/
├── components/                 # 自定义组件
│   ├── aht/                   # AHT20 温湿度传感器驱动
│   ├── bmp280/                # BMP280 气压传感器驱动
│   ├── bsp/                   # 板级支持包
│   │   ├── include/           # BSP 头文件
│   │   └── src/
│   │       ├── boards/        # 开发板配置（ESP32-S3-BOX, BOX-Lite）
│   │       └── storage/       # SD 卡支持
│   ├── esp_idf_lib_helpers/   # ESP-IDF 库辅助工具
│   └── i2cdev/                # I2C 设备抽象层
│
├── main/                      # 主应用程序
│   ├── app/                   # 应用层功能模块
│   │   ├── app_sensor.c/h     # 传感器数据采集与处理
│   │   ├── app_sntp.c/h       # SNTP 时间同步
│   │   ├── app_sr.c/h         # 语音识别引擎
│   │   ├── app_sr_handler.c/h # 语音命令处理
│   │   ├── app_wifi.c/h       # Wi-Fi 管理
│   │   ├── file_manager.c/h   # 文件系统管理
│   │   └── ha_client.c/h      # Home Assistant 客户端
│   │
│   ├── gui/                   # 图形用户界面
│   │   ├── font/              # 字体资源（中英文、图标字体）
│   │   ├── image/             # 图像资源
│   │   ├── ui_main.c/h        # 主界面
│   │   ├── ui_boot_animate.c/h # 启动动画
│   │   ├── ui_net_config.c/h  # 网络配置界面
│   │   ├── ui_sr.c/h          # 语音识别界面
│   │   ├── ui_ring.c/h        # 环形进度界面
│   │   ├── ui_hint.c/h        # 提示信息界面
│   │   └── ui_about_us.c/h    # 关于页面
│   │
│   ├── http/                  # HTTP 功能模块
│   │   ├── http.c/h           # HTTP 客户端
│   │   ├── http_server.c/h    # HTTP 服务器
│   │   ├── time_sync.c/h      # 时间同步
│   │   └── ca.pem             # CA 证书
│   │
│   ├── system/                # 系统级功能
│   │   ├── mqtt.c/h           # MQTT 客户端（Home Assistant 通信）
│   │   └── settings.c/h       # 系统设置（NVS 存储）
│   │
│   ├── espandora.c            # 主程序入口
│   ├── constants.h            # 全局常量定义
│   ├── util.c/h               # 工具函数
│   ├── CMakeLists.txt         # 构建配置
│   ├── Kconfig.projbuild      # 项目配置菜单
│   └── idf_component.yml      # 依赖管理
│
├── managed_components/        # ESP Component Manager 管理的依赖（自动生成）
│   ├── espressif__esp-box-lite/    # BOX-Lite BSP
│   ├── espressif__esp-sr/          # 语音识别库
│   ├── lvgl__lvgl/                 # LVGL 图形库
│   └── ...                         # 其他依赖组件
│
├── spiffs/                    # SPIFFS 文件系统数据
│   └── index.html             # Web 界面
│
├── CMakeLists.txt             # 顶层 CMake 配置
├── partitions.csv             # Flash 分区表
└── sdkconfig.defaults         # 默认配置

```

## 核心功能模块

### 1. 语音识别 (Speech Recognition)
- **引擎**: ESP-SR (Multinet5)
- **支持语言**: 中文/英文
- **唤醒词**: "Hi Lexin" (中文) / "Hi ESP" (英文)
- **命令管理**: 动态命令添加/删除机制
- **处理流程**: 唤醒检测 → 命令识别 → 命令执行

### 2. 传感器系统
- **AHT20**: 温度、湿度采集（I2C 接口）
- **BMP280**: 气压测量、海拔计算
- **数据上报**: 通过 MQTT 发送到 Home Assistant
- **采样周期**: 可配置的定时任务

### 3. Home Assistant 集成
- **协议**: MQTT
- **功能**: 
  - 设备自动发现
  - 传感器状态上报
  - 接收控制命令
  - 运动检测事件推送
- **实体类型**: 二进制传感器（运动）、开关、传感器（温湿度、气压）

### 4. 图形界面 (LVGL)
- **主界面**: 时间显示、传感器数据、Wi-Fi 状态
- **配置界面**: Wi-Fi 配网、系统设置
- **语音界面**: 语音识别状态、命令反馈
- **主题**: 自定义颜色方案和字体

### 5. 网络服务
- **Wi-Fi**: 
  - BLE Provisioning 配网
  - 自动重连
  - 凭证重置（长按 Boot 按钮）
- **HTTP 服务器**: Web 配置界面
- **SNTP**: 时间同步（CST-8 时区）

### 6. GPIO 控制
- **运动检测**: GPIO 输入，边沿触发中断
- **开关控制**: GPIO 输出，可通过 MQTT/语音控制

## 技术栈

- **框架**: ESP-IDF 5.5.1+ (从 5.1 升级)
- **语言**: C
- **构建系统**: CMake
- **GUI 框架**: LVGL 8.x
- **网络协议**: Wi-Fi, TCP/IP, MQTT, HTTP/HTTPS
- **存储**: NVS (非易失性存储), SPIFFS (文件系统)
- **依赖管理**: ESP Component Manager

## 构建与烧录

### 环境准备
```bash
# 安装 ESP-IDF 5.5.1 或更高版本
# 参考: https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32s3/get-started/

# 设置 IDF 环境
. $IDF_PATH/export.sh
```

### 配置项目
```bash
# 使用默认配置
idf.py set-target esp32s3

# 可选：自定义配置
idf.py menuconfig
```

### 编译与烧录
```bash
# 首次编译或配置变更时（完整刷写，约 120 秒）
idf.py build
idf.py -p /dev/ttyACM0 flash

# 日常代码修改时（快速刷写，约 30 秒，节省 75% 时间）
idf.py build  
idf.py -p /dev/ttyACM0 app-flash

# 高速刷写（推荐）
idf.py -p /dev/ttyACM0 -b 921600 app-flash

# 查看日志
idf.py -p /dev/ttyACM0 monitor
```

**何时使用 app-flash？**

✅ **可以使用**（节省 75% 时间）：
- 修改 C/H 源文件
- 调试代码、修复 bug
- 调整算法参数

❌ **不能使用**（必须用 flash）：
- 修改分区表 (`partitions.csv`)
- 修改系统配置 (`sdkconfig.defaults`)
- 修改 SPIFFS 文件 (`spiffs/`)
- 首次编译或清理后重新编译
- 语音识别模型损坏需重新写入

## 配置说明

### 主要配置项 (menuconfig)
- **Board Selection**: `CONFIG_BSP_BOARD_ESP32_S3_BOX_Lite`
- **Speech Recognition**: 
  - Wake Word: `CONFIG_SR_WN_WN9_HILEXIN_MULTI`
  - MultiNet: `CONFIG_SR_MN_CN_MULTINET5_RECOGNITION_QUANT8`
- **Wi-Fi**: 内存优化配置
- **LVGL**: 颜色格式、内存配置

### 运行时配置
- NTP 服务器: `ESPANDORA_NTP_SERVER` (默认: 0.cn.pool.ntp.org)
- MQTT 配置: 通过 NVS 存储，可在运行时修改

## 系统架构

### 启动流程
```
app_main()
  ├─> NVS 初始化
  ├─> SPIFFS 挂载
  ├─> I2C 初始化
  ├─> 显示屏初始化
  ├─> BSP 初始化
  ├─> GPIO 初始化
  ├─> GUI 启动 (ui_main_start)
  ├─> 语音识别启动 (app_sr_start)
  ├─> Wi-Fi 任务启动 (wifi_task)
  ├─> MQTT 任务启动 (mqtt_task)
  ├─> HTTP 服务器启动 (http_server_init)
  └─> 传感器任务启动 (app_sensor_task)
```

### 任务列表
| 任务名称 | 优先级 | 栈大小 | 核心 | 功能 |
|---------|-------|--------|-----|------|
| wifi_task | 1 | 6KB | 0 | Wi-Fi 管理 |
| mqtt_task | 3 | 4KB | - | MQTT 通信 |
| http_server_task | 3 | 4KB | - | HTTP 服务 |
| app_sensor_task | 3 | 4KB | - | 传感器采集 |
| gpio_task_example | 10 | 2KB | - | GPIO 中断处理 |

## 开发指南

### 添加新的语音命令
1. 在 `app_sr_handler.c` 中定义命令字符串
2. 添加到 `g_default_cmd_info` 数组
3. 实现命令处理函数并注册到 `sr_handler_install()`

### 添加新的 MQTT 实体
1. 在传感器任务中生成实体 ID (`ha_entity_id()`)
2. 使用 `ha_state_push()` 发送状态更新
3. 在 `mqtt.c` 中实现控制命令处理

### 自定义 GUI 页面
1. 在 `main/gui/` 下创建新的 UI 文件
2. 使用 LVGL API 构建界面
3. 在 `ui_main.c` 中注册页面

## 依赖项

自动管理的依赖（通过 `idf_component.yml`）：
- `espressif/esp-sr`: 语音识别引擎
- `espressif/esp-box-lite`: BSP 支持
- `lvgl/lvgl`: 图形库
- `chmorgan/esp-file-iterator`: 文件迭代器
- `espressif/json_generator`: JSON 生成
- `espressif/json_parser`: JSON 解析
- `espressif/qrcode`: 二维码生成

## 许可证

项目包含多个许可证：
- ESP-IDF 部分: Apache 2.0
- LVGL 部分: MIT
- 部分示例代码: Unlicense OR CC0-1.0

## 作者

- Hessian (HessianZ)

## 链接

- [ESP32-S3 技术文档](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32s3/)
- [ESP-BOX-Lite 硬件资料](https://github.com/espressif/esp-box)
- [LVGL 文档](https://docs.lvgl.io/)
