# ESPandora 升级与优化开发计划

## 📋 项目背景

**当前版本**: ESP-IDF 5.1  
**目标版本**: ESP-IDF 5.5.1  
**优化目标**: 减少 app 数据大小，优化语音模型刷写策略

---

## 🎯 核心需求

### 1. ESP-IDF 版本升级（5.1 → 5.5.1）
升级 ESP-IDF 框架，处理可能的 API 变更，确保项目兼容性。

### 2. 优化语音模型刷写策略
将语音识别模型从 app 分区分离，避免每次 `app-flash` 时重复写入模型数据。

### 3. 智能刷写命令选择
建立规则让 Qoder 自动判断使用 `flash` 还是 `app-flash`，节省开发时间。

---

## 📊 当前架构分析

### 分区表结构（partitions.csv）
```
名称         类型        子类型      偏移      大小       说明
-----------------------------------------------------------------------
sec_cert    data        -          0xd000    12KB      安全证书
nvs         data        nvs        0x10000   24KB      非易失性存储
otadata     data        ota        auto      8KB       OTA 数据
phy_init    data        phy        auto      4KB       PHY 初始化数据
fctry       data        nvs        auto      24KB      出厂设置
ota_0       app         ota_0      auto      6MB       应用程序分区
storage     data        spiffs     auto      1MB       SPIFFS 文件系统
model       data        spiffs     auto      4847KB    语音识别模型 ⚠️
voice_data  data        fat        auto      3MB       语音数据
```

**当前问题**：
- `model` 分区类型为 `spiffs`，每次 `idf.py flash` 时会被写入
- 模型数据约 4.8MB，严重拖慢刷写速度
- `app-flash` 会跳过 data 分区，但模型初始化代码在 `app_sr.c:422` 使用 `esp_srmodel_init("model")`

### 语音识别模块架构
**文件**: `main/app/app_sr.c`

**模型加载流程**：
```c
// Line 422: 初始化模型
models = esp_srmodel_init("model");  // 从 "model" 分区加载

// Line 426: 选择唤醒词模型
afe_config.wakenet_model_name = esp_srmodel_filter(models, ESP_WN_PREFIX, NULL);

// Line 357-368: 设置多网模型（中英文）
char *mn_name = esp_srmodel_filter(models, ESP_MN_PREFIX, ...);
esp_mn_iface_t *multinet = esp_mn_handle_from_name(mn_name);
model_iface_data_t *model_data = multinet->create(mn_name, 5760);
```

**依赖组件**：
- `espressif/esp-sr: 1.3.*` (在 `main/idf_component.yml` 中声明)
- 唤醒词模型: WN9 (Hi Lexin / Hi ESP)
- 多网模型: Multinet5 (中英文命令识别)

---

## 🔧 任务分解与实施计划

### 阶段一：ESP-IDF 升级准备（预计 2 小时）

#### 任务 1.1：检查 API 兼容性 ⚠️ **高优先级**
**目标**: 识别 ESP-IDF 5.1 → 5.5.1 的 breaking changes

**检查项**：
1. **Wi-Fi API** (`app_wifi.c`)
   - `esp_wifi_*` 函数签名变更
   - `wifi_prov_mgr_*` 配网接口变更
   - 事件处理机制变更

2. **I2C 驱动** (`i2cdev.c`, `aht.c`, `bmp280.c`)
   - ⚠️ **已知风险**: ESP-IDF 5.x 引入 `driver_ng`，与旧版 `i2c_driver` 冲突
   - 检查是否需要从 legacy driver 迁移到 new driver
   - 参考文档: [I2C Driver Migration Guide](https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32s3/migration-guides/release-5.x/peripherals.html#i2c-driver)

3. **LVGL 集成** (`gui/*.c`)
   - ⚠️ **已知风险**: LVGL 返回类型可能从 `void` 改为 `lv_obj_t *`
   - 检查 `lvgl__lvgl` 组件版本是否兼容
   - 检查 `esp_lvgl_port` 组件是否需要更新

4. **MQTT 客户端** (`mqtt.c`)
   - `esp_mqtt_client_*` API 变更
   - TLS/SSL 配置变更

5. **SPIFFS/FAT 文件系统** (`file_manager.c`)
   - 挂载和分区 API 变更

**执行步骤**：
```bash
# 1. 备份当前配置
cp sdkconfig sdkconfig.backup
cp sdkconfig.defaults sdkconfig.defaults.backup

# 2. 切换到 ESP-IDF 5.5.1
cd $IDF_PATH
git checkout v5.5.1
git submodule update --init --recursive
./install.sh

# 3. 设置环境变量
. $IDF_PATH/export.sh

# 4. 清理旧配置
cd /home/hessian/workspace/github/HessianZ/espandora
idf.py fullclean

# 5. 尝试编译，记录错误
idf.py build 2>&1 | tee upgrade_errors.log
```

**风险点**：
- I2C 驱动冲突可能导致传感器无法工作
- LVGL 版本不兼容可能导致 GUI 崩溃
- Wi-Fi 配网流程可能需要重写

---

#### 任务 1.2：更新依赖版本
**文件**: `main/idf_component.yml`

**当前依赖**：
```yaml
dependencies:
  idf: ">=5.1"
  chmorgan/esp-file-iterator: "1.0.0"
  espressif/esp-sr: "1.3.*"
  espressif/json_generator: ^1
  espressif/json_parser: =1.0.0
  espressif/qrcode: ^0.1.0
```

**升级策略**：
```yaml
dependencies:
  idf: ">=5.5.1"  # 更新最低版本要求
  chmorgan/esp-file-iterator: "1.0.0"  # 检查是否有新版本
  espressif/esp-sr: "1.5.*"  # ⚠️ 检查 esp-sr 是否支持 IDF 5.5
  espressif/json_generator: ^1  # 保持
  espressif/json_parser: "^1.0.3"  # 使用更灵活的版本约束
  espressif/qrcode: ^0.1.0  # 保持
```

**验证步骤**：
```bash
# 清理 managed_components
rm -rf managed_components

# 重新下载依赖
idf.py reconfigure

# 检查组件是否兼容
idf.py build
```

---

#### 任务 1.3：更新 sdkconfig.defaults
**目标**: 适配新版本配置项

**检查项**：
1. 废弃的配置项（使用 `idf.py menuconfig` 查看警告）
2. 新增的必要配置项
3. Wi-Fi/BT 内存优化配置

**操作流程**：
```bash
# 1. 加载默认配置
idf.py menuconfig

# 2. 检查红色警告项（废弃配置）
# 3. 保存新配置
idf.py save-defconfig

# 4. 对比差异
diff sdkconfig.defaults.backup sdkconfig.defaults
```

---

### 阶段二：语音模型刷写优化（预计 3 小时）

#### 任务 2.1：理解当前模型存储机制 🔍

**现状分析**：
- 模型分区名称: `model`
- 分区类型: `data, spiffs`
- 大小: 4847KB (4.73 MB)
- 加载方式: `esp_srmodel_init("model")` 从 SPIFFS 分区读取

**问题**：
1. `model` 分区被定义为 `spiffs` 类型，会被 `idf.py flash` 写入
2. ESP-SR 期望从特定分区名称加载模型
3. 需要确认模型数据是否可以独立于 app 刷写

**调研任务**：
```bash
# 1. 查看 esp-sr 组件源码
cd managed_components/espressif__esp-sr
grep -r "esp_srmodel_init" .
grep -r "model_path" .

# 2. 确认模型分区挂载方式
# 检查是否可以使用自定义分区类型
```

---

#### 任务 2.2：重新设计分区表 🎯 **核心任务**

**优化方案 A: 使用独立的 data 分区（推荐）**

**新分区表**：
```csv
# Name,   Type, SubType, Offset,  Size, Flags
sec_cert, data, ,        0xd000,  0x3000,
nvs,      data, nvs,     0x10000, 0x6000,
otadata,  data, ota,     ,        0x2000,
phy_init, data, phy,     ,        0x1000,
fctry,    data, nvs,     ,        0x6000,
ota_0,    app,  ota_0,   ,        6M,
storage,  data, spiffs,  ,        1M,
# 🔥 关键修改: 将 model 分区类型改为 0x40 (自定义数据分区)
model,    data, 0x40,    ,        4847K,
voice_data, data,  fat,  ,        3M
```

**优势**：
- `idf.py flash` 仍会写入，但 `app-flash` 跳过
- 模型独立于应用程序，升级 app 不影响模型
- 支持后续 OTA 升级时保留模型

**实施步骤**：
1. 修改 `partitions.csv`
2. 确认 `esp_srmodel_init()` 仍能识别 `model` 分区
3. 测试模型加载是否正常

---

**优化方案 B: 模型嵌入固件（备选）**

如果 ESP-SR 支持从内存加载模型：
```c
// 将模型编译到 rodata 段
extern const uint8_t wakenet_model[] asm("_binary_wakenet_bin_start");
extern const uint8_t multinet_model[] asm("_binary_multinet_bin_start");

// 在 CMakeLists.txt 中嵌入二进制文件
target_add_binary_data(app_sr.c "models/wakenet.bin" BINARY)
```

**优势**：
- 模型随 app 一起刷写，`app-flash` 可用
- 无需单独分区

**劣势**：
- 增大 app 分区 ~5MB，可能超出 6MB 限制
- 需要确认 ESP-SR 是否支持此模式

---

#### 任务 2.3：验证优化效果 ✅

**测试场景**：

| 场景 | 命令 | 预期结果 | 验证点 |
|------|------|----------|--------|
| 首次刷写 | `idf.py flash` | 写入 app + model | 语音识别正常工作 |
| 修改代码后 | `idf.py app-flash` | 仅写入 app | 语音识别仍正常 |
| 模型损坏后 | `esptool.py write_flash 0x700000 model.bin` | 仅写入 model | 恢复正常 |

**性能对比**：
```bash
# 测试前（每次都写入模型）
time idf.py flash
# 预计: ~120 秒

# 测试后（跳过模型）
time idf.py app-flash
# 预计: ~30 秒

# 节省时间: ~90 秒 (75%)
```

---

### 阶段三：智能刷写命令选择（预计 1 小时）

#### 任务 3.1：定义刷写规则 📜

**决策矩阵**：

| 变更内容 | 使用命令 | 理由 |
|----------|----------|------|
| 仅修改 `main/` 下的 `.c/.h` 文件 | `app-flash` | 应用代码变更，无需写入分区表和模型 |
| 修改 `partitions.csv` | `flash` | 分区表变更，必须完整刷写 |
| 修改 `sdkconfig.defaults` | `flash` | 配置变更可能影响引导加载程序 |
| 修改 `components/` 中的驱动 | `app-flash` | 组件代码变更，无需完整刷写 |
| 修改 `spiffs/` 文件 | `flash` | SPIFFS 文件系统变更 |
| 首次编译或 `fullclean` 后 | `flash` | 确保所有分区正确初始化 |

**实现方式**：规则已添加到 `.qoder-rules` 文件中

---

#### 任务 3.2：完善刷写规则文件

**状态**: ✅ 已完成

刷写规则已添加到 `.qoder-rules` 文件的「智能刷写策略」章节，包括：
- 自动选择命令的判断逻辑
- flash 和 app-flash 的使用场景
- 自动检测脚本
- 常见错误处理方案
- 性能优化建议（波特率设置）

---

#### 任务 3.3：添加开发规范记忆

**状态**: ✅ 已完成

已创建 `development_practice_specification` 类别的记忆：
- 标题：ESP-IDF 智能刷写命令自动选择规则
- 包含完整的判断逻辑和使用场景
- 确保后续开发自动遵守规范

---

### 阶段四：BSP 组件与 LVGL 升级（预计 6 小时）⚠️ **新增任务**

#### 任务 4.1：升级 esp-box-lite 组件（2.1.0）

**背景**：
- 当前版本：`espressif/esp-box-lite 1.0.2`
- 目标版本：`2.1.0` (最新版本)
- 升级原因：修复 ESP-IDF 5.5.1 新 I2C 驱动兼容性问题

**依赖变化**：
```yaml
# 1.0.2 依赖
espressif/esp_lvgl_port: ^1  # LVGL 8.x
lvgl/lvgl: ~8.4.0

# 2.1.0 依赖
espressif/esp_lvgl_port: ^2  # LVGL 9.x
lvgl/lvgl: ~9.4.0  # ⚠️ 重大 API 变更
```

**修改文件**：`components/bsp/idf_component.yml`
```yaml
dependencies:
  espressif/button: ^2
  espressif/esp-box-lite:
    version: "~2.1.0"  # 从 1.0.* 升级到 2.1.0
```

**操作步骤**：
```bash
# 1. 清理旧版本组件
rm -rf managed_components/espressif__esp-box-lite
rm -rf managed_components/espressif__esp_lvgl_port
rm -rf managed_components/lvgl__lvgl

# 2. 更新依赖
idf.py reconfigure

# 3. 验证新版本
grep "version:" managed_components/espressif__esp-box-lite/idf_component.yml
grep "version:" managed_components/lvgl__lvgl/library.json
```

---

#### 任务 4.2：移除 esp-box 依赖及相关代码

**当前状态**：
- `components/bsp/idf_component.yml` 中同时依赖 `esp-box` 和 `esp-box-lite`
- 项目实际使用的是 `esp-box-lite`（ESP32-S3-BOX-Lite 硬件）
- `esp-box` 依赖导致版本冲突（esp-box 2.4.* 需要 esp_lvgl_port ^1，与 2.1.0 冲突）

**需要删除**：
1. **依赖声明**（`components/bsp/idf_component.yml`）：
   ```yaml
   # 删除以下行
   espressif/esp-box:
     version: "2.4.*"
     require: "no"
   ```

2. **源文件**（`components/bsp/src/boards/`）：
   - 检查 `esp32_s3_box.c` 是否被引用
   - 如未使用则删除该文件

3. **构建配置**（`components/bsp/CMakeLists.txt`）：
   - 移除 `esp32_s3_box.c` 的编译引用

**验证步骤**：
```bash
# 1. 检查 esp-box 引用
grep -r "esp32_s3_box\.c" components/bsp/
grep -r "ESP_BOX[^_]" main/ components/

# 2. 确认删除安全性
# 如果没有引用，则安全删除
rm components/bsp/src/boards/esp32_s3_box.c

# 3. 重新配置
idf.py reconfigure
idf.py build
```

---

#### 任务 4.3：LVGL 9.x API 迁移 🔴 **高风险**

**重大 API 变更**：

1. **图像格式常量**：
   ```c
   // LVGL 8.x
   LV_IMG_CF_TRUE_COLOR
   LV_IMG_CF_TRUE_COLOR_ALPHA
   LV_IMG_PX_SIZE_ALPHA_BYTE
   LV_COLOR_SIZE
   
   // LVGL 9.x
   LV_COLOR_FORMAT_RGB565         // 替代 LV_IMG_CF_TRUE_COLOR
   LV_COLOR_FORMAT_RGB565A8       // 替代 LV_IMG_CF_TRUE_COLOR_ALPHA
   LV_COLOR_SIZE_RGB565_A8        // 替代 LV_IMG_PX_SIZE_ALPHA_BYTE
   LV_COLOR_DEPTH / 8             // 替代 LV_COLOR_SIZE
   ```

2. **图像头结构体**：
   ```c
   // LVGL 8.x
   typedef struct {
       uint16_t w;
       uint16_t h;
       uint32_t always_zero;
       uint32_t reserved;
       lv_img_cf_t cf;
   } lv_image_header_t;
   
   // LVGL 9.x
   typedef struct {
       uint16_t w;
       uint16_t h;
       lv_color_format_t cf;
       uint16_t stride;      // 新增字段
       uint32_t reserved_2;  // reserved 改名为 reserved_2
   } lv_image_header_t;
   ```

3. **返回类型变更**：
   ```c
   // LVGL 8.x
   void lv_obj_delete(lv_obj_t *obj);
   
   // LVGL 9.x  
   lv_obj_t * lv_obj_delete(lv_obj_t *obj);  // 返回父对象指针
   ```

**需要修改的文件**（预计 20+ 文件）：
- `main/gui/image/*.c` (12 个图像文件)
- `main/gui/ui_*.c` (7 个 UI 文件)
- 可能需要修改的字体文件

**迁移策略**：

**方案 A：自动批量替换（推荐）**
```bash
# 1. 替换图像格式常量
find main/gui -name "*.c" -exec sed -i 's/LV_IMG_CF_TRUE_COLOR_ALPHA/LV_COLOR_FORMAT_RGB565A8/g' {} \;
find main/gui -name "*.c" -exec sed -i 's/LV_IMG_CF_TRUE_COLOR/LV_COLOR_FORMAT_RGB565/g' {} \;
find main/gui -name "*.c" -exec sed -i 's/LV_IMG_PX_SIZE_ALPHA_BYTE/LV_COLOR_SIZE_RGB565_A8/g' {} \;
find main/gui -name "*.c" -exec sed -i 's/LV_COLOR_SIZE/(LV_COLOR_DEPTH \/ 8)/g' {} \;

# 2. 替换结构体字段
find main/gui -name "*.c" -exec sed -i 's/\.always_zero = 0/.stride = 0/g' {} \;
find main/gui -name "*.c" -exec sed -i 's/\.reserved = 0/.reserved_2 = 0/g' {} \;

# 3. 验证编译
idf.py build 2>&1 | grep -E "error:|warning:" | head -50
```

**方案 B：降级到 esp-box-lite 1.2.x（备选）**
- 如果存在 1.2.x 版本支持 LVGL 8.x 和 IDF 5.5.1
- 避免 GUI 大规模修改
- 需要确认该版本是否存在

---

#### 任务 4.4：编译验证与错误修复

**测试步骤**：
```bash
# 1. 完整编译
idf.py fullclean
idf.py build 2>&1 | tee upgrade_lvgl9_build.log

# 2. 检查错误类型
grep "error:" upgrade_lvgl9_build.log | sort | uniq -c

# 3. 逐个修复错误
# - 未定义的宏：检查 LVGL 9.x 新常量名
# - 结构体字段错误：参考新结构体定义
# - 类型不匹配：检查函数签名变更

# 4. 验证无警告
idf.py build 2>&1 | grep "warning:" | wc -l
# 预期输出: 0
```

**常见错误及解决方案**：

| 错误信息 | 原因 | 解决方案 |
|---------|------|----------|
| `'LV_IMG_CF_TRUE_COLOR' undeclared` | 常量被移除 | 替换为 `LV_COLOR_FORMAT_RGB565` |
| `'lv_image_header_t' has no member named 'always_zero'` | 字段被移除 | 替换为 `stride` |
| `'lv_image_header_t' has no member named 'reserved'` | 字段改名 | 替换为 `reserved_2` |
| `assignment of function call with void return` | 函数返回类型变更 | 移除赋值操作 |

---

#### 任务 4.5：运行时测试

**测试清单**：
- [ ] 显示初始化正常
- [ ] 启动动画正常播放
- [ ] 主界面显示正常
- [ ] 网络配置界面响应正常
- [ ] 语音识别界面显示正常
- [ ] 按钮交互正常
- [ ] 触摸响应正常（如有）
- [ ] 图标和图像显示清晰
- [ ] 字体渲染正常
- [ ] 无内存泄漏或崩溃

**性能对比**：
```bash
# 启动时间对比
echo "=== LVGL 8.x 启动时间 ==="
grep "GUI start" boot_before.log

echo "=== LVGL 9.x 启动时间 ==="  
grep "GUI start" boot_after.log

# 内存占用对比
echo "=== LVGL 8.x 内存 ==="
grep "Free heap" boot_before.log | tail -1

echo "=== LVGL 9.x 内存 ==="
grep "Free heap" boot_after.log | tail -1
```

---

### 阶段五：集成测试与文档更新（预计 2 小时）

#### 任务 5.1：完整测试流程 ✅

**测试清单**：

- [ ] ESP-IDF 5.5.1 编译通过
- [ ] 所有组件依赖正常下载
- [ ] 首次 `idf.py flash` 成功
- [ ] 语音识别功能正常（唤醒词 + 命令识别）
- [ ] 传感器数据采集正常（AHT20 + BMP280）
- [ ] Wi-Fi 连接和配网正常
- [ ] MQTT 通信正常
- [ ] LVGL GUI 显示正常
- [ ] HTTP 服务器响应正常
- [ ] 修改代码后 `app-flash` 成功
- [ ] `app-flash` 后语音识别仍正常
- [ ] SD 卡读写正常

**回归测试脚本**：
```bash
#!/bin/bash
# test_upgrade.sh

echo "=== ESP-IDF 版本检查 ==="
idf.py --version

echo "=== 完整编译 ==="
idf.py fullclean
idf.py build || exit 1

echo "=== 完整刷写 ==="
idf.py -p /dev/ttyUSB0 flash || exit 1

echo "=== 等待启动 ==="
sleep 5

echo "=== 检查日志（30秒） ==="
timeout 30 idf.py -p /dev/ttyUSB0 monitor | tee boot.log

echo "=== 检查关键模块 ==="
grep -E "(Speech Recognition|WiFi|MQTT|Sensor)" boot.log

echo "=== 测试 app-flash ==="
# 修改一个文件触发重新编译
touch main/espandora.c
idf.py build
idf.py -p /dev/ttyUSB0 app-flash || exit 1

echo "✅ 升级测试完成"
```

---

#### 任务 5.2：更新文档

**需要更新的文档**：

1. **README.md**
   - 更新 ESP-IDF 版本要求为 5.5.1
   - 添加刷写命令选择说明
   - 更新构建命令示例

2. **.qoder-rules**
   - 添加"智能刷写策略"章节（已在任务 3.2 中完成）

3. **DEVELOPMENT_PLAN.md**（本文档）
   - 标记已完成的任务
   - 记录遇到的问题和解决方案

**README.md 修改点**：
```markdown
## 环境要求

- **ESP-IDF**: 5.5.1 或更高版本 (从 5.1 升级)
- **Python**: 3.8+
- **CMake**: 3.16+

## 快速开始

### 编译与刷写

```bash
# 首次编译或配置变更时（完整刷写，约 120 秒）
idf.py build
idf.py -p PORT flash

# 日常代码修改时（快速刷写，约 30 秒）
idf.py build  
idf.py -p PORT app-flash

# 高速刷写（推荐）
idf.py -p PORT -b 921600 app-flash
```

### 何时使用 app-flash？

✅ **可以使用**（节省 75% 时间）：
- 修改 C/H 源文件
- 调试代码、修复 bug
- 调整算法参数

❌ **不能使用**（必须用 flash）：
- 修改分区表 (`partitions.csv`)
- 修改系统配置 (`sdkconfig.defaults`)
- 修改 SPIFFS 文件 (`spiffs/`)
- 首次编译或清理后重新编译
```

---

## 📅 时间估算

| 阶段 | 任务 | 预计时间 | 风险级别 |
|------|------|----------|----------|
| 一 | API 兼容性检查 | 1.5h | 🔴 高 |
| 一 | 依赖版本更新 | 0.5h | 🟡 中 |
| 二 | 模型存储机制调研 | 1h | 🟡 中 |
| 二 | 分区表重新设计 | 1h | 🔴 高 |
| 二 | 验证优化效果 | 1h | 🟢 低 |
| 三 | 刷写规则定义 | 0.5h | 🟢 低 |
| 三 | 更新 .qoder-rules | 0.5h | 🟢 低 |
| **四 (新增)** | **升级 esp-box-lite** | **2h** | **🔴 高** |
| **四 (新增)** | **移除 esp-box 依赖** | **0.5h** | **🟢 低** |
| **四 (新增)** | **LVGL 9.x API 迁移** | **3h** | **🔴 高** |
| **四 (新增)** | **编译验证与错误修复** | **0.5h** | **🟡 中** |
| 五 | 完整测试 | 1.5h | 🟡 中 |
| 五 | 文档更新 | 0.5h | 🟢 低 |
| **总计** | | **14 小时** | |

---

## 🚨 风险评估与缓解策略

### 高风险项

#### 风险 1: I2C 驱动冲突 🔴
**描述**: ESP-IDF 5.x 引入新 I2C 驱动，与旧驱动不兼容  
**影响**: 传感器（AHT20, BMP280）无法工作  
**概率**: 60%  
**缓解措施**:
- 提前查阅 [I2C Migration Guide](https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32s3/migration-guides/release-5.x/peripherals.html#i2c-driver)
- 准备回退到 ESP-IDF 5.1 的方案
- 考虑使用兼容层或重写 I2C 驱动

#### 风险 2: ESP-SR 组件不兼容 🔴
**描述**: esp-sr 1.3.x 可能不支持 ESP-IDF 5.5  
**影响**: 语音识别功能完全失效  
**概率**: 40%  
**缓解措施**:
- 提前检查 [esp-sr 组件注册表](https://components.espressif.com/components/espressif/esp-sr)
- 确认是否有 1.4+ 或 1.5+ 版本支持 IDF 5.5
- 准备降级到 IDF 5.3 的备选方案

#### 风险 3: 模型分区类型不被识别 🔴
**描述**: 修改分区类型后 `esp_srmodel_init()` 无法加载模型  
**影响**: 语音识别功能失效  
**概率**: 30%  
**缓解措施**:
- 保留原分区表作为备份
- 测试时先用小模型验证
- 查阅 esp-sr 源码确认分区加载逻辑

### 中风险项

#### 风险 4: LVGL 8.x → 9.x 重大 API 变更 🔴 **升级为高风险**
**描述**: esp-box-lite 2.1.0 强制依赖 LVGL 9.4.0，API 不向后兼容  
**影响**: 
- 所有 GUI 图像文件编译失败（12+ 文件）
- 图像头结构体字段变更
- 对象操作函数返回类型变更
- 可能导致运行时崩溃或显示异常
**概率**: 100% （已确认）  
**缓解措施**:
- 使用 sed 批量替换常量和结构体字段
- 参考 [LVGL 9.x Migration Guide](https://docs.lvgl.io/master/intro/migration/8-to-9.html)
- 准备回退到 esp-box-lite 1.x 的备选方案
- 逐文件验证编译，确保无遗漏

#### 风险 5: Wi-Fi Provisioning API 变更 🟡
**描述**: BLE 配网接口可能有变化  
**影响**: 无法通过手机配网  
**概率**: 20%  
**缓解措施**:
- 参考最新的 provisioning 示例代码
- 保留硬编码 Wi-Fi 凭证用于测试

---

## ✅ 验收标准

### 功能验收
- [ ] 所有原有功能正常工作（语音、传感器、网络、GUI）
- [ ] `idf.py flash` 和 `app-flash` 都能成功刷写
- [ ] `app-flash` 后语音识别仍正常（关键指标）

### 性能验收  
- [ ] `app-flash` 时间 < 40 秒（相比原先 120 秒）
- [ ] 内存占用无明显增加
- [ ] 启动时间无明显延迟

### 代码质量验收
- [ ] 无编译警告
- [ ] 无运行时错误日志
- [ ] 代码符合原有风格

---

## 📝 实施记录

### 完成日志
（在执行过程中填写）

**日期**: 2025-12-31  
**阶段**: 计划制定  
**完成人**: Qoder  
**状态**: ✅ 计划已完成，等待执行

---

**日期**: 2025-12-31  
**阶段**: 阶段一 - ESP-IDF 升级  
**问题**: I2C 驱动冲突导致 BSP 音频编解码器初始化失败  
**解决方案**: 发现 esp-box-lite 1.0.2 使用旧版 I2C 驱动，需升级到 2.1.0  
**状态**: ✅ 已识别问题，转入阶段四处理

---

**日期**: _________  
**阶段**: 阶段二 - 模型优化  
**问题**:  
**解决方案**:  
**状态**:

---

**日期**: _________  
**阶段**: 阶段三 - 刷写策略  
**问题**:  
**解决方案**:  
**状态**:

---

**日期**: _________  
**阶段**: 阶段四 - BSP 与 LVGL 升级  
**问题**:  
**解决方案**:  
**状态**:

---

**日期**: _________  
**阶段**: 阶段五 - 测试验收  
**问题**:  
**解决方案**:  
**状态**:

---

## 📚 参考资料

### 官方文档
- [ESP-IDF 5.5.1 Release Notes](https://github.com/espressif/esp-idf/releases/tag/v5.5.1)
- [ESP-IDF Migration Guide 5.x](https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32s3/migration-guides/release-5.x/index.html)
- [ESP-SR 组件文档](https://github.com/espressif/esp-sr)
- [Partition Tables](https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32s3/api-guides/partition-tables.html)

### 相关 Issues
- [ESP-SR 与 IDF 5.5 兼容性](https://github.com/espressif/esp-sr/issues)
- [I2C Driver Migration](https://github.com/espressif/esp-idf/issues?q=i2c+migration)
- [LVGL 9.x Migration Guide](https://docs.lvgl.io/master/intro/migration/8-to-9.html)
- [esp-box-lite Component Registry](https://components.espressif.com/components/espressif/esp-box-lite/versions/2.1.0~2)

---

## 🎯 下一步行动

1. **确认执行**: 等待开发者确认计划可行性
2. **环境准备**: 切换到 ESP-IDF 5.5.1
3. **开始执行**: 按阶段逐步实施
4. **持续沟通**: 遇到问题及时反馈

---

**计划制定时间**: 2025-12-31  
**预计完成时间**: 2025-12-31（1 工作日内）  
**计划版本**: v1.0  
**制定人**: Qoder AI Assistant
