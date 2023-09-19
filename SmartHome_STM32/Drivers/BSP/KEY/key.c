#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"


void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    
     gpio_init_struct.Mode=GPIO_MODE_IT_RISING;
     gpio_init_struct.Pin=GPIO_KEY_PIN;
     gpio_init_struct.Pull=GPIO_PULLDOWN;;
     gpio_init_struct.Speed=GPIO_SPEED_FREQ_LOW;
     HAL_GPIO_Init(GPIO_KEY_PORT, &gpio_init_struct);        /* WKUP配置为上升沿触发中断 */
    
     HAL_NVIC_SetPriority(KEY_INT_IRQn, 3, 2); /* 抢占 3，子优先级 2 */
     HAL_NVIC_EnableIRQ(KEY_INT_IRQn); /* 使能中断线 0 */

    
}


/**
* @brief key 外部中断服务程序
* @param 无
* @retval 无
*/
void KEY_INT_IRQHandler(void)
{
    /* 调用中断处理公用函数 清除 KEY 所在中断线 的中断标志位 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_KEY_PIN);
    /* HAL 库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_KEY_PIN);
}

uint8_t key_flag=0;

/**
* @brief 中断服务程序中需要做的事情
在 HAL 库中所有的外部中断服务函数都会调用此函数
* @param GPIO_Pin:中断引脚号
* @retval 无
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    
    if(GPIO_Pin==GPIO_KEY_PIN)
    {
        delay_ms(10); /* 消抖 */
        key_flag=1;
       
    }
}
/**
*查询按键标志
*/
uint8_t Get_Key_flag(void)
{
    if(key_flag==1)
    {
        key_flag=0;
        return 1;
    }
    return 0;
}


