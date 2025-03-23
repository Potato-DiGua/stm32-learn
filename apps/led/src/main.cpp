#include "stm32f10x.h"
#include "Random.h"
#include "Delay.h"

// LED类 - 封装单个LED的操作
class LED
{
private:
    GPIO_TypeDef *port;
    uint16_t pin;

public:
    LED(GPIO_TypeDef *_port, uint16_t _pin) : port(_port), pin(_pin) {}

    void turnOn() const
    {
        GPIO_ResetBits(port, pin);
    }

    void turnOff() const
    {
        GPIO_SetBits(port, pin);
    }

    void toggle() const
    {
        if (GPIO_ReadOutputDataBit(port, pin))
        {
            turnOff();
        }
        else
        {
            turnOn();
        }
    }

    void flash(uint32_t duration) const
    {

        turnOn();
        Delay_ms(duration);
        turnOff();
    }
};

// LEDController类 - 管理多个LED的控制逻辑
class LEDController
{
private:
    LED *leds;
    int count;
    uint32_t randomSeed;

public:
    LEDController(LED *_leds, int _count) : leds(_leds), count(_count)
    {
        // 使用SysTick值初始化随机种子
        randomSeed = SysTick->VAL;
    }

    void initializeGPIO() const
    {
        // 启用GPIOA和GPIOB时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        // 配置GPIOA引脚
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        // 配置GPIOB引脚
        GPIO_InitTypeDef GPIO_InitStructure2;
        GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_10;
        GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure2);

        // 初始化所有输出为高电平（LED熄灭）
        GPIO_Write(GPIOA, ~0);
        GPIO_Write(GPIOB, ~0);
    }

    void runRandomPattern() const
    {
        // 随机选择LED
        int randomIdx = Get_Random(count - 1, randomSeed);

        // 随机延时时间 (100-500ms)
        uint32_t delayTime = 100 + Get_Random(400, randomSeed);

        // 闪烁选中的LED
        leds[randomIdx].flash(delayTime);

        // 短暂间隔
        Delay_ms(100);
    }
};

int main(void)
{
    // 创建LED对象数组
    LED leds[] = {
        LED(GPIOA, GPIO_Pin_0),
        LED(GPIOA, GPIO_Pin_1),
        LED(GPIOA, GPIO_Pin_2),
        LED(GPIOA, GPIO_Pin_3),
        LED(GPIOA, GPIO_Pin_5),
        LED(GPIOB, GPIO_Pin_0),
        LED(GPIOB, GPIO_Pin_10)};

    // 创建控制器并初始化
    LEDController controller(leds, sizeof(leds) / sizeof(leds[0]));
    controller.initializeGPIO();

    // 主循环
    while (1)
    {
        controller.runRandomPattern();
    }
}
