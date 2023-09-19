/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2020-08-20
 * @brief    新建工程实验-HAL库版本 实验
 * @license  Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ******************************************************************************
 * @attention
 * 
 * 实验平台:正点原子 STM32F103 开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/USART/myusart.h"
#include "string.h"

int main(void)
{
    
//    uint8_t tx_pdata[200]="AT+RST\n\r";
      uint8_t rx_pdata[200]={0};
      uint16_t data_len;
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    led_init();                         /* LED初始化 */
    key_init();                         /* KEY初始化 */
    usart_init(115200);                 /*串口初始化*/
    usart2_init(115200);           /* 串口2初始化函数 */
    
    USART2_StartRx();                   /*串口2启动接收数据*/
    
    
    ESP8266_SendCommand("AT+RST","OK",500);
    delay_ms(1000);
    ESP8266_SendCommand("AT+CWMODE=1","OK",500);
    delay_ms(10);
    ESP8266_SendCommand("AT+CWJAP=\"GSW24\",\"li.120500\"","OK",5000);
    ESP8266_SendCommand("AT+CIFSR","OK",500);
    ESP8266_SendCommand("AT+CIPMUX=1","OK",500);
    ESP8266_SendCommand("AT+CIPSERVER=1,9999","OK",500);
    
    delay_ms(1000);
    
    while(1)
    { 
        //发送数据
        if(Get_Key_flag()==1)
        {
            ESP8266_Send_TCP("{\"data\":\"doorbell\",\"status\":\"1\"}");
        }
        data_len=USART2_Receive(rx_pdata);
        delay_ms(200);
        if(data_len!=0)
        {
            printf("%s",rx_pdata);
            if(strstr((char*)rx_pdata,"{\"room\":\"saloon\",\"dev\":\"led\",\"status\":\"1\"}"))
            {
                //打开LED
                LED(0);
            }else if(strstr((char*)rx_pdata,"{\"room\":\"saloon\",\"dev\":\"led\",\"status\":\"0\"}"))
            {
                //关闭LED;
                LED(1);
            }
            USART2_ClearBuf();
            memset(rx_pdata,0,200);
            data_len=0;
        }
        
        
    }
}


