#include "./BSP/USART/myusart.h"
#include "string.h"
#include "./SYSTEM/delay/delay.h"

UART_HandleTypeDef g_uart2_handle;  /* UART2��� */
/**
 * @brief       ����2��ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @note        ע��: ����������ȷ��ʱ��Դ, ���򴮿ڲ����ʾͻ������쳣.
 *              �����USART��ʱ��Դ��sys_stm32_clock_init()�������Ѿ����ù���.
 * @retval      ��
 */
void usart2_init(uint32_t baudrate)
{
    
    GPIO_InitTypeDef gpio_init_struct;
    USART2_TX_GPIO_CLK_ENABLE();                             /* ʹ�ܴ���TX��ʱ�� */
    USART2_RX_GPIO_CLK_ENABLE();                             /* ʹ�ܴ���RX��ʱ�� */
    USART2_UX_CLK_ENABLE();                                  /* ʹ�ܴ���ʱ�� */
    
    USART2_GPIO_REMAP();                                     /*USART2������ӳ��*/
    
    gpio_init_struct.Pin = USART2_TX_GPIO_PIN;               /* ���ڷ������ź� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO�ٶ�����Ϊ���� */
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpio_init_struct);
            
    gpio_init_struct.Pin = USART2_RX_GPIO_PIN;               /* ����RX�� ģʽ���� */
    gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;    
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &gpio_init_struct);   /* ����RX�� �������ó�����ģʽ */
    
     /*UART2 ��ʼ������*/
    g_uart2_handle.Instance = USART2_UX;                                       /* USART_UX */
    g_uart2_handle.Init.BaudRate = baudrate;                                  /* ������ */
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;                           /* һ��ֹͣλ */
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;                            /* ����żУ��λ */
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* ��Ӳ������ */
    g_uart2_handle.Init.Mode = UART_MODE_TX_RX;                               /* �շ�ģʽ */
    HAL_UART_Init(&g_uart2_handle);                                           /* HAL_UART_Init()��ʹ��UART2 */
    
    HAL_NVIC_EnableIRQ(USART2_UX_IRQn);                      /* ʹ��USART2�ж�ͨ�� */
    HAL_NVIC_SetPriority(USART2_UX_IRQn, 3, 4);              /* ��2��������ȼ�:��ռ���ȼ�3�������ȼ�3 */
}

static uint8_t rx_len=0;
static uint8_t rx_data;
static uint8_t usart2_rx_buf[200];

/**
 * @brief       ����2�жϷ�����
                ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
 * @param       ��
 * @retval      ��
 */

void USART2_UX_IRQHandler(void)
{
   HAL_UART_IRQHandler(&g_uart2_handle);
}
//����2�жϻص�����������ÿ�ν��յ����ݣ���������һ�ν��գ�
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    usart2_rx_buf[rx_len%200]=rx_data;
    rx_len++;
    HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)&rx_data, 1); 

}
/*����2�������պ���*/
void USART2_StartRx(void)
{
    /* �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ��������������� */
    HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)&rx_data, 1); 
}

/*USART2�������ݺ���*/
uint16_t USART2_Receive(uint8_t *pdata)
{
    memcpy(pdata,(uint8_t*)usart2_rx_buf,rx_len);
    return rx_len;

}

/*USART2�������ݺ���*/
void USART2_Transmit(uint8_t *pData,uint16_t Size,uint32_t Timeout)
{
    HAL_UART_Transmit(&g_uart2_handle,pData,Size,Timeout);
}

/*���usart2 buffer*/
void USART2_ClearBuf(void)
{
    rx_len=0;
    memset((uint8_t*)usart2_rx_buf,0,200);
}

//ESP8266����ָ���
uint8_t ESP8266_SendCommand(char *cmd,char *reply,uint32_t timeout)
{
    //1.ָ��Ԥ����
    char buf[256]={0};
    strcat(buf,cmd);
    //ָ���β��ӻ��з�
    if(strstr(buf,"\r\n")==0)
    {
        strcat(buf,"\r\n");
    }
    //2.����ǰ����յ�buffer
    USART2_ClearBuf();
    //3.����ָ��
    USART2_Transmit((uint8_t*)buf,strlen(buf),500);
    //4.��������
    memset(buf,0,256);
    while(timeout!=0)
    {
         if(USART2_Receive((uint8_t*)buf))
        {
            if(strstr(buf,reply))
            {
                printf("%s Send OK!\r\n",cmd);
                //��ӡip��ַ
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
//ESP8266�������ݺ���
uint8_t ESP8266_Send_TCP(char *data)
{
    char buf[256]={0};
    uint8_t str_len;
    str_len=strlen(data);
    
    sprintf(buf, "AT+CIPSEND=0,%d\r\n",str_len); //�Ѹ�ʽ��������д���ַ���
    if(ESP8266_SendCommand(buf,"OK",500)==0)
    {
        delay_ms(1000);
        //3.��������
        ESP8266_SendCommand(data,"SEND OK",500);
        return 0;
    }
     return 1;
   
}



