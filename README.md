# STM32 项目

这是一个基于STM32F10x系列微控制器的项目，使用CMake构建系统和ARM GCC编译器。

## 在Windows下编译

### 前提条件

1. 安装 [CMake](https://cmake.org/download/) (3.18或更高版本)
2. 安装 [ARM GNU工具链](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) (arm-none-eabi-gcc)
3. 将上述工具添加到系统PATH环境变量中

### 安装ARM工具链

1. 从ARM官方网站下载工具链：https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
2. 运行安装程序，并确保选择"添加到PATH"选项
3. 安装完成后，打开新的命令提示符并验证安装：

```
arm-none-eabi-gcc --version
```

### 编译步骤

1. 创建构建目录并进入：

```
mkdir build
cd build
```

2. 配置CMake项目：

```
cmake -G "MinGW Makefiles" ..
```

3. 编译项目：

```
cmake --build .
```

4. 编译完成后，你将在build目录中看到以下文件：
   - mystm32.elf - ELF格式的可执行文件
   - mystm32.bin - 二进制格式，可直接烧录
   - mystm32.hex - Intel HEX格式，适用于大多数烧录工具

### 烧录到设备

可以使用ST-Link工具、OpenOCD或其他兼容的烧录工具将生成的.bin或.hex文件烧录到您的STM32设备。

例如，使用OpenOCD：

```
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program mystm32.bin 0x08000000 verify reset exit"
```

## 项目结构

- `src/user/` - 用户代码
- `src/start/` - 启动代码和系统初始化
- `CMakeLists.txt` - CMake构建配置
- `stm32f10x_flash.ld` - 链接器脚本

## 常见问题解决方案

### 目标STM32设备未定义错误

如果编译时遇到以下错误：
```
#error "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
```

解决方法：
1. 确保在包含`stm32f10x.h`之前定义了目标设备宏，例如在代码中添加：
   ```c
   #define STM32F10X_MD  /* 中密度设备 */
   #include "stm32f10x.h"
   ```

2. 或者在CMakeLists.txt中使用全局定义：
   ```cmake
   add_definitions(-DSTM32F10X_MD)
   ```

可用的设备宏定义：
- `STM32F10X_LD` - 低密度设备
- `STM32F10X_MD` - 中密度设备
- `STM32F10X_HD` - 高密度设备
- `STM32F10X_XL` - 超高密度设备
- `STM32F10X_CL` - 互联型设备
- `STM32F10X_LD_VL` - 低密度Value Line设备
- `STM32F10X_MD_VL` - 中密度Value Line设备
- `STM32F10X_HD_VL` - 高密度Value Line设备

选择与您的STM32F10x芯片型号相匹配的定义。 