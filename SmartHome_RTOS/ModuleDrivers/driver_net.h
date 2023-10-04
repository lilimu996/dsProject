#ifndef __DRIVER_NET_H
#define __DRIVER_NET_H

#include "stm32f1xx_hal.h"
#include "driver_buffer.h"


/**UART2**/

/**TX**/

#define UART2_TX_PORT                  GPIOD
#define UART2_TX_PIN                   GPIO_PIN_5
#define UART2_TX_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)


/**RX**/

#define UART2_RX_PORT                  GPIOD
#define UART2_RX_PIN                   GPIO_PIN_6
#define UART2_RX_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

#define UART2_RCC_CLK_ENABLE()    do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)
/*��ӳ��usart2������  USART2_TX--->PD5      USART2_RX--->PD6    */
#define USART2_GPIO_REMAP() do{__HAL_RCC_AFIO_CLK_ENABLE();__HAL_AFIO_REMAP_USART2_ENABLE();}while(0);


/**
*
*    ����������ʼ��
*/
int Driver_Net_Init(void);

/**
*
*    ����ָ��
*/
static int Driver_Net_TransmitCmd(const char *cmd,const char *reply,uint16_t timeout);
/**
*
*    ����WiFi����
*/
int Driver_Net_ConnectWiFi(const char *ssid ,const char *pwd ,int timeout);
/**
*
*    ����UDP����
*/
int Driver_Net_ConnectUDP(const char *ip ,int port ,int timeout);
/**
*
*    ����TCP����
*/
int Driver_Net_ConnectTCP(const char *ip ,int port ,int timeout);
/**
*
*    �Ͽ�WiFi����
*/
int Driver_Net_DisconnectWiFi(void);
/**
*
*    �Ͽ�TCP,UDP����
*/
int Driver_Net_Disconnect_TCP_UDP(void);
/**
*
*    �������ݰ�
*/
int Driver_Net_TransmitSorcket(const char *socket,int len, int timeout);
/**
*
*    ������������
*/
void NetDataProcess_Callback(uint8_t data);
/**
*
*    ��ȡ��������
*/
int Driver_Net_RecvSocket(char *buf, int len, int timeout);
/**
*
*    ���ԣ���ȡ���λ�����������
*/
void Debug_Read_Buffer(uint16_t timeout);


#endif


