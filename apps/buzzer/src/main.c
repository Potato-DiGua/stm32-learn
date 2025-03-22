#include "stm32f10x.h"
#include "Delay.h"

static inline void initRCC()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure2;
    GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure2);
}

int main(void)
{
    initRCC();
    while (1)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        Delay_ms(500);
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        Delay_ms(600);
    }
}
