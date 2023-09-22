#ifndef __DRIVER_DBG_H
#define __DRIVER_DBG_H

#include "stm32f1xx_hal.h"


/**UART1**/

/**TX**/
#define UART1_TX_PORT                GPIOA
#define UART1_TX_PIN                 GPIO_PIN_9
#define UART1_TX_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)


/**RX**/
#define UART1_RX_PORT                 GPIOA
#define UART1_RX_PIN                  GPIO_PIN_10
#define UART1_RX_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define UART1_RCC_CLK_ENABLE()    do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)


int Driver_DBG_Init(void);
void HAL_UART1_MspInit(UART_HandleTypeDef *huart);





#endif






