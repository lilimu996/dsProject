/**
 ******************************************************************************
 * @file     main.c
 * @author   ����ԭ���Ŷ�(ALIENTEK)
 * @version  V1.0
 * @date     2020-08-20
 * @brief    �½�����ʵ��-HAL��汾 ʵ��
 * @license  Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ******************************************************************************
 * @attention
 * 
 * ʵ��ƽ̨:����ԭ�� STM32F103 ������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    led_init();                         /* LED��ʼ�� */
    key_init();                         /* KEY��ʼ�� */
    usart_init(115200);                 /*���ڳ�ʼ��*/
    usart2_init(115200);           /* ����2��ʼ������ */
    
    USART2_StartRx();                   /*����2������������*/
    
    
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
        //��������
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
                //��LED
                LED(0);
            }else if(strstr((char*)rx_pdata,"{\"room\":\"saloon\",\"dev\":\"led\",\"status\":\"0\"}"))
            {
                //�ر�LED;
                LED(1);
            }
            USART2_ClearBuf();
            memset(rx_pdata,0,200);
            data_len=0;
        }
        
        
    }
}


