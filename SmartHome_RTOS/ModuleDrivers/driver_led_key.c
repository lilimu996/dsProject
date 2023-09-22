#include "driver_led_key.h"
#include "driver_buffer.h"

volatile static uint32_t KeyTrigerTime = 0;
static RingBuffer KeyBuffer;

int Driver_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    LED0_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin              = LED0_PIN;
    GPIO_InitStruct.Pull             = GPIO_PULLUP;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(LED0_PORT,&GPIO_InitStruct);
    return 0;

}

int Driver_LED_WriteStatus(uint8_t status)
{
    LED0(status);
    return 0;
}

int Driver_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    KEY0_GPIO_CLK_ENABLE();
    
    Driver_Buffer_Init(&KeyBuffer,sizeof(KeyEvent)<<4);
    
    GPIO_InitStruct.Mode             = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pin              = KEY0_PIN;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;

    HAL_GPIO_Init(KEY0_PORT,&GPIO_InitStruct);
    
    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    
    return 0;

}

/*启动函数startup_stm32f103xe.s中会调用中断服务函数*/
int EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_PIN);
    return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == KEY0_PIN)
    {
        KeyTrigerTime=HAL_GetTick()+50;
    }
}

void KeyShakeProcess_Callback(void)
{
    KeyEvent nKeyEvent = {0};
    static uint32_t press_time = 0;
    static uint32_t release_time = 0;
    uint32_t tick=HAL_GetTick();
    if(tick == KeyTrigerTime)
    {
        if( KEY0_STATUS() == 0)
        {
           press_time = tick;
        }
        else
        {
            release_time = tick;
        }
    }
    if(press_time != 0 && release_time != 0)
    {
        nKeyEvent.num = 1;
        nKeyEvent.time = press_time - release_time;
        press_time = 0;
        release_time = 0;
        Driver_Buffer_WriteBytes(&KeyBuffer,(uint8_t*)&nKeyEvent.num,sizeof(KeyEvent));
    }

}

int Driver_Key_Read(uint8_t *buf,uint16_t len)
{
    if(len == 0 || len<sizeof(KeyEvent) || (len % sizeof(KeyEvent))!= 0)
    {
        return -1;
    }
    if(buf == NULL)
    {
        return -1;
    }
    if(Driver_Buffer_ReadBytes(&KeyBuffer,buf,len) == len)
    {
        return 0;
    }
    return -1;
}






