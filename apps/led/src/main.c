#include "stm32f10x.h"
#include "Random.h"
#include "Delay.h"

typedef struct
{
    uint16_t pin;
    GPIO_TypeDef *GPIOx;
} Pin;

static inline void initRCC() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    uint16_t pins = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = pins;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitTypeDef GPIO_InitStructure2;
    GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_10 | GPIO_Pin_12;
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure2);
}

int main(void)
{
    initRCC();
    Pin arr[] = {
        {GPIO_Pin_0, GPIOA},
        {GPIO_Pin_1, GPIOA},
        {GPIO_Pin_2, GPIOA},
        {GPIO_Pin_3, GPIOA},
        {GPIO_Pin_5, GPIOA},
        {GPIO_Pin_0, GPIOB},
        {GPIO_Pin_10, GPIOB},
    };

    const int len = sizeof(arr) / sizeof(arr[0]);

    GPIO_Write(GPIOA, ~0);
    GPIO_Write(GPIOB, ~0);

    // 使用SysTick值初始化随机种子
    uint32_t randomSeed = SysTick->VAL;

    while (1)
    {
        // 随机选择LED
        int randomIdx = Get_Random(len - 1, randomSeed);
        Pin pin = arr[randomIdx];
        
        // 随机延时时间 (100-500ms)
        uint32_t delayTime = 100 + Get_Random(400, randomSeed);
        
        GPIO_ResetBits(pin.GPIOx, pin.pin);
        Delay_ms(delayTime);
        GPIO_SetBits(pin.GPIOx, pin.pin);
        Delay_ms(100);  // 短暂间隔

        // GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        // Delay_ms(500);
        // GPIO_SetBits(GPIOB, GPIO_Pin_12);
        // Delay_ms(600);
    }
}
