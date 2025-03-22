#include "Random.h"

// 静态种子变量
static uint32_t s_randomSeed = 0;

uint32_t Get_Random(uint32_t max, uint32_t randomSeed) {
    // 初始化静态种子（首次调用时）
    if (s_randomSeed == 0) {
        s_randomSeed = randomSeed;
    }
    
    // 使用线性同余法生成伪随机数
    s_randomSeed = s_randomSeed * 1664525 + 1013904223;
    
    // 使用SysTick计数器增加随机性
    s_randomSeed ^= SysTick->VAL;
    
    return s_randomSeed % (max + 1);
}
