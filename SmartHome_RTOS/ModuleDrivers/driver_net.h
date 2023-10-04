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
/*重映射usart2的引脚  USART2_TX--->PD5      USART2_RX--->PD6    */
#define USART2_GPIO_REMAP() do{__HAL_RCC_AFIO_CLK_ENABLE();__HAL_AFIO_REMAP_USART2_ENABLE();}while(0);


/**
*
*    网络驱动初始化
*/
int Driver_Net_Init(void);

/**
*
*    发送指令
*/
static int Driver_Net_TransmitCmd(const char *cmd,const char *reply,uint16_t timeout);
/**
*
*    建立WiFi连接
*/
int Driver_Net_ConnectWiFi(const char *ssid ,const char *pwd ,int timeout);
/**
*
*    建立UDP连接
*/
int Driver_Net_ConnectUDP(const char *ip ,int port ,int timeout);
/**
*
*    建立TCP连接
*/
int Driver_Net_ConnectTCP(const char *ip ,int port ,int timeout);
/**
*
*    断开WiFi连接
*/
int Driver_Net_DisconnectWiFi(void);
/**
*
*    断开TCP,UDP连接
*/
int Driver_Net_Disconnect_TCP_UDP(void);
/**
*
*    发送数据包
*/
int Driver_Net_TransmitSorcket(const char *socket,int len, int timeout);
/**
*
*    接收网络数据
*/
void NetDataProcess_Callback(uint8_t data);
/**
*
*    读取网络数据
*/
int Driver_Net_RecvSocket(char *buf, int len, int timeout);
/**
*
*    调试，读取环形缓冲区的数据
*/
void Debug_Read_Buffer(uint16_t timeout);


#endif


