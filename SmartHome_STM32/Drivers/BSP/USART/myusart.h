#ifndef __MYUSART_H
#define __MYUSART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

#define USART2_TX_GPIO_PORT                  GPIOD
#define USART2_TX_GPIO_PIN                   GPIO_PIN_5
#define USART2_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD��ʱ��ʹ�� */

#define USART2_RX_GPIO_PORT                  GPIOD
#define USART2_RX_GPIO_PIN                   GPIO_PIN_6
#define USART2_RX_GPIO_CLK_ENABLE()           do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD��ʱ��ʹ�� */

#define USART2_UX                            USART2
#define USART2_UX_IRQn                       USART2_IRQn
#define USART2_UX_IRQHandler                 USART2_IRQHandler
#define USART2_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 ʱ��ʹ�� */

/*��ӳ��usart2������  USART2_TX--->PD5      USART2_RX--->PD6    */
#define USART2_GPIO_REMAP() do{__HAL_RCC_AFIO_CLK_ENABLE();__HAL_AFIO_REMAP_USART2_ENABLE();}while(0);

void usart2_init(uint32_t baudrate);           /* ����2��ʼ������ */

void USART2_StartRx(void);                      /*����2�������պ���*/
uint16_t USART2_Receive(uint8_t *pdata);         /*USART2�������ݺ���*/
void USART2_Transmit(uint8_t *pData,uint16_t Size,uint32_t Timeout);    /*USART2�������ݺ���*/
void USART2_ClearBuf(void);                      /*���usart2 buffer*/

uint8_t ESP8266_SendCommand(char *cmd,char *reply,uint32_t timeout);/*ESP8266����ATָ���*/
uint8_t ESP8266_Send_TCP(char *data);               /*ESP8266�������ݺ���*/



#endif



