#ifndef __MYUSART_H
#define __MYUSART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

#define USART2_TX_GPIO_PORT                  GPIOD
#define USART2_TX_GPIO_PIN                   GPIO_PIN_5
#define USART2_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

#define USART2_RX_GPIO_PORT                  GPIOD
#define USART2_RX_GPIO_PIN                   GPIO_PIN_6
#define USART2_RX_GPIO_CLK_ENABLE()           do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

#define USART2_UX                            USART2
#define USART2_UX_IRQn                       USART2_IRQn
#define USART2_UX_IRQHandler                 USART2_IRQHandler
#define USART2_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 时钟使能 */

/*重映射usart2的引脚  USART2_TX--->PD5      USART2_RX--->PD6    */
#define USART2_GPIO_REMAP() do{__HAL_RCC_AFIO_CLK_ENABLE();__HAL_AFIO_REMAP_USART2_ENABLE();}while(0);

void usart2_init(uint32_t baudrate);           /* 串口2初始化函数 */

void USART2_StartRx(void);                      /*串口2启动接收函数*/
uint16_t USART2_Receive(uint8_t *pdata);         /*USART2接收数据函数*/
void USART2_Transmit(uint8_t *pData,uint16_t Size,uint32_t Timeout);    /*USART2发送数据函数*/
void USART2_ClearBuf(void);                      /*清空usart2 buffer*/

uint8_t ESP8266_SendCommand(char *cmd,char *reply,uint32_t timeout);/*ESP8266发送AT指令函数*/
uint8_t ESP8266_Send_TCP(char *data);               /*ESP8266发送数据函数*/



#endif



