#ifndef __DRIVER_LED_KEY
#define __DRIVER_LED_KEY

#include "stm32f1xx_hal.h"


/**LED**/

#define LED0_PORT                 GPIOB
#define LED0_PIN                  GPIO_PIN_5
#define LED0(STATUS)              HAL_GPIO_WritePin(LED0_PORT,LED0_PIN,STATUS?GPIO_PIN_RESET:GPIO_PIN_SET)
#define LED0_TOGGLE()             HAL_GPIO_TogglePin(LED0_PORT,LED0_PIN)
#define LED0_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

int Driver_LED_Init(void);

int Driver_LED_WriteStatus(uint8_t status);

/**KEY**/

#define KEY0_PORT                 GPIOE
#define KEY0_PIN                  GPIO_PIN_4
#define KEY0_STATUS()              HAL_GPIO_ReadPin(KEY0_PORT,KEY0_PIN)
#define KEY0_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)

typedef struct{
    uint16_t num;
    uint16_t time;
}KeyEvent;

int Driver_KEY_Init(void);
/*启动函数startup_stm32f103xe.s中会调用中断服务函数*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void KeyShakeProcess_Callback(void);
int Driver_Key_Read(uint8_t *buf,uint16_t len);

#endif

