#ifndef __RANDOM_H
#define __RANDOM_H

#include "stm32f10x.h"

/**
 * @brief 获取随机数
 * @param max 最大值
 * @param randomSeed 随机数种子
 * @return 随机数
 */
uint32_t Get_Random(uint32_t max, uint32_t randomSeed);

#endif