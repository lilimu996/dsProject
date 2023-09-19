#include "./BSP/USART/myusart.h"
#include "string.h"
#include "./SYSTEM/delay/delay.h"

UART_HandleTypeDef g_uart2_handle;  /* UART2句柄 */
/**
 * @brief       串口2初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @note        注意: 必须设置正确的时钟源, 否则串口波特率就会设置异常.
 *              这里的USART的时钟源在sys_stm32_clock_init()函数中已经设置过了.
 * @retval      无
 */
void usart2_init(uint32_t baudrate)
{
    
    GPIO_InitTypeDef gpio_init_struct;
    USART2_TX_GPIO_CLK_ENABLE();                             /* 使能串口TX脚时钟 */
    USART2_RX_GPIO_CLK_ENABLE();                             /* 使能串口RX脚时钟 */
    USART2_UX_CLK_ENABLE();                                  /* 使能串口时钟 */
    
    USART2_GPIO_REMAP();                                     /*USART2引脚重映射*/
    
    gpio_init_struct.Pin = USART2_TX_GPIO_PIN;               /* 串口发送引脚号 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO速度设置为高速 */
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpio_init_struct);
            
    gpio_init_struct.Pin = USART2_RX_GPIO_PIN;               /* 串口RX脚 模式设置 */
    gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;    
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &gpio_init_struct);   /* 串口RX脚 必须设置成输入模式 */
    
     /*UART2 初始化设置*/
    g_uart2_handle.Instance = USART2_UX;                                       /* USART_UX */
    g_uart2_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    g_uart2_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&g_uart2_handle);                                           /* HAL_UART_Init()会使能UART2 */
    
    HAL_NVIC_EnableIRQ(USART2_UX_IRQn);                      /* 使能USART2中断通道 */
    HAL_NVIC_SetPriority(USART2_UX_IRQn, 3, 4);              /* 组2，最低优先级:抢占优先级3，子优先级3 */
}

static uint8_t rx_len=0;
static uint8_t rx_data;
static uint8_t usart2_rx_buf[200];

/**
 * @brief       串口2中断服务函数
                注意,读取USARTx->SR能避免莫名其妙的错误
 * @param       无
 * @retval      无
 */

void USART2_UX_IRQHandler(void)
{
   HAL_UART_IRQHandler(&g_uart2_handle);
}
//串口2中断回调函数（保存每次接收的数据，并启动下一次接收）
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    usart2_rx_buf[rx_len%200]=rx_data;
    rx_len++;
    HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)&rx_data, 1); 

}
/*串口2启动接收函数*/
void USART2_StartRx(void)
{
    /* 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)&rx_data, 1); 
}

/*USART2接收数据函数*/
uint16_t USART2_Receive(uint8_t *pdata)
{
    memcpy(pdata,(uint8_t*)usart2_rx_buf,rx_len);
    return rx_len;

}

/*USART2发送数据函数*/
void USART2_Transmit(uint8_t *pData,uint16_t Size,uint32_t Timeout)
{
    HAL_UART_Transmit(&g_uart2_handle,pData,Size,Timeout);
}

/*清空usart2 buffer*/
void USART2_ClearBuf(void)
{
    rx_len=0;
    memset((uint8_t*)usart2_rx_buf,0,200);
}

//ESP8266发送指令函数
uint8_t ESP8266_SendCommand(char *cmd,char *reply,uint32_t timeout)
{
    //1.指令预处理
    char buf[256]={0};
    strcat(buf,cmd);
    //指令结尾添加换行符
    if(strstr(buf,"\r\n")==0)
    {
        strcat(buf,"\r\n");
    }
    //2.清理前面接收的buffer
    USART2_ClearBuf();
    //3.发送指令
    USART2_Transmit((uint8_t*)buf,strlen(buf),500);
    //4.接收数据
    memset(buf,0,256);
    while(timeout!=0)
    {
         if(USART2_Receive((uint8_t*)buf))
        {
            if(strstr(buf,reply))
            {
                printf("%s Send OK!\r\n",cmd);
                //打印ip地址
                if(strstr(buf,"CIFSR"))
                {
                    printf("%s\r\n",buf);
                }
                if(strstr(buf,">"))
                {
                    printf("%s\r\n",buf);
                
                }
                return 0;
            }
            else
            {
                timeout--;
                HAL_Delay(1);
            }
        }    
    }
   printf("%s Send Error!\r\n",cmd);
   return 1;
}
//ESP8266发送数据函数
uint8_t ESP8266_Send_TCP(char *data)
{
    char buf[256]={0};
    uint8_t str_len;
    str_len=strlen(data);
    
    sprintf(buf, "AT+CIPSEND=0,%d\r\n",str_len); //把格式化的数据写入字符串
    if(ESP8266_SendCommand(buf,"OK",500)==0)
    {
        delay_ms(1000);
        //3.发送数据
        ESP8266_SendCommand(data,"SEND OK",500);
        return 0;
    }
     return 1;
   
}



