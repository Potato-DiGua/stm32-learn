#!/bin/bash

# 检查命令行参数
if [ $# -eq 0 ]; then
  echo "错误: 未提供bin文件路径"
  echo "用法: $0 <bin文件路径>"
  exit 1
fi

binPath=$1

# 检查文件是否存在
if [ ! -f "$binPath" ]; then
  echo "错误: bin文件 '$binPath' 不存在"
  exit 1
fi

sudo openocd -f "./stm32.cfg" \
-c init \
-c "reset halt; wait_halt; flash write_image erase $binPath 0x08000000" \
-c reset -c shutdown