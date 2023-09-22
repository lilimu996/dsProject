#include "driver_dbg.h"
#include "stm32f1xx_hal_uart.h"
#include <stdio.h>


UART_HandleTypeDef huart1 = {0};
int Driver_DBG_Init(void)
{


    huart1.Instance             = USART1;
    huart1.Init.BaudRate        = 115200;
    huart1.Init.WordLength      = UART_WORDLENGTH_8B;
    huart1.Init.StopBits        = UART_STOPBITS_1;
    huart1.Init.Parity          = UART_PARITY_NONE;
    huart1.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    huart1.Init.Mode            = UART_MODE_TX_RX;
    
    HAL_UART1_MspInit(&huart1);
    
    if(HAL_UART_Init(&huart1) != HAL_OK)
    {
        return -1;
    }
    return 0;
}

void HAL_UART1_MspInit(UART_HandleTypeDef *huart)
{
    UART1_TX_GPIO_CLK_ENABLE();
    UART1_RX_GPIO_CLK_ENABLE();
    UART1_RCC_CLK_ENABLE();

    GPIO_InitTypeDef    GPIO_Initstruct={0};
    
    GPIO_Initstruct.Mode            = GPIO_MODE_AF_PP;
    GPIO_Initstruct.Pin             = UART1_TX_PIN;
    GPIO_Initstruct.Pull            = GPIO_PULLUP;
    GPIO_Initstruct.Speed           = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(UART1_TX_PORT, &GPIO_Initstruct);
    
    GPIO_Initstruct.Pin             = UART1_RX_PIN;
    GPIO_Initstruct.Mode            = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(UART1_RX_PORT, &GPIO_Initstruct);
    
}


struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
/* FILE is typedef¡¯d in stdio.h. */
FILE __stdout;
int fputc(int ch, FILE *f)
{
    /* Your implementation of fputc(). */
    HAL_UART_Transmit(&huart1,(uint8_t*)&ch, 1 , 10 );
    return ch;
}



