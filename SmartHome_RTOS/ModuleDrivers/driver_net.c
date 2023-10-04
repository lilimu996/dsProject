#include "driver_net.h"
#include "string.h"
#include "stdio.h"


UART_HandleTypeDef huart2 = {0};
RingBuffer CmdRetBuffer = {0};
RingBuffer NetDataBuffer = {0};

void HAL_UART2_MspInit(UART_HandleTypeDef *huart);
static int Driver_Net_TransmitCmd(const char *cmd,const char *reply,uint16_t timeout);
/**
*
*    串口2驱动初始化
*/
static int Driver_Net_UART_Init(void)
{
    huart2.Instance             = USART2;
    huart2.Init.BaudRate        = 115200;
    huart2.Init.WordLength      = UART_WORDLENGTH_8B;
    huart2.Init.StopBits        = UART_STOPBITS_1;
    huart2.Init.Parity          = UART_PARITY_NONE;
    huart2.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    huart2.Init.Mode            = UART_MODE_TX_RX;
    
    HAL_UART2_MspInit(&huart2);
    
    if(HAL_UART_Init(&huart2) != HAL_OK)
    {
        return -1;
    }
    __HAL_USART_ENABLE_IT(&huart2,USART_IT_RXNE);
    return 0;
}

void HAL_UART2_MspInit(UART_HandleTypeDef *huart)
{
    UART2_TX_GPIO_CLK_ENABLE();
    UART2_RX_GPIO_CLK_ENABLE();
    UART2_RCC_CLK_ENABLE();
    USART2_GPIO_REMAP();
    
    GPIO_InitTypeDef    GPIO_Initstruct = {0};
    
    GPIO_Initstruct.Mode            = GPIO_MODE_AF_PP;
    GPIO_Initstruct.Pin             = UART2_TX_PIN;
    GPIO_Initstruct.Pull            = GPIO_PULLUP;
    GPIO_Initstruct.Speed           = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(UART2_TX_PORT, &GPIO_Initstruct);
    
    GPIO_Initstruct.Pin             = UART2_RX_PIN;
    GPIO_Initstruct.Mode            = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(UART2_RX_PORT, &GPIO_Initstruct);
    
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    
}
/**
*
*    串口2接收消息中断
*/
void USART2_IRQHandler(void)
{
    uint8_t rx_data =0;
    if(__HAL_USART_GET_FLAG(&huart2,USART_FLAG_RXNE) == SET)
    {
        __HAL_USART_CLEAR_FLAG(&huart2,USART_FLAG_RXNE);
        rx_data = USART2->DR;
        Driver_Buffer_Write(&CmdRetBuffer,rx_data);
        NetDataProcess_Callback(rx_data);
    }
}
/**
*
*    网络驱动初始化
*/
int Driver_Net_Init(void)
{
    if(Driver_Net_UART_Init() == -1)
    {
        return -1;
    }
    Driver_Buffer_Init(&CmdRetBuffer,128);
    Driver_Buffer_Init(&NetDataBuffer,1024);
    //Driver_Net_TransmitCmd("AT","OK",5000);
    HAL_Delay(20);
    Driver_Net_TransmitCmd("AT+RST","OK",500);
    HAL_Delay(500);
    Driver_Net_TransmitCmd("AT+CWMODE=1","OK",500);
    return 0;
}
/**
*
*    发送指令
*    返回值:0 发送成功 -1 发送失败
*/
static int Driver_Net_TransmitCmd(const char *cmd,const char *reply,uint16_t timeout)
{
    char buf[128] = {0};
    strcat(buf,cmd);
    uint8_t i = 0;
    if(strstr(buf,"\r\n") == NULL)
    {
        strcat(buf,"\r\n");
    }
    Driver_Buffer_Clean(&CmdRetBuffer);
    if(strstr(buf,"AT+CWJAP") != 0)
    {
        /*等待wifi建立连接*/
        HAL_UART_Transmit(&huart2,(uint8_t*)buf,strlen(buf),500);
        HAL_Delay(2000);
    }
    else
    {
        HAL_UART_Transmit(&huart2,(uint8_t*)buf,strlen(buf),500);
    }
    memset(buf,0,128);
    while(timeout != 0)
    {
        if(Driver_Buffer_Read(&CmdRetBuffer,(uint8_t*)&buf[i]) == 0)
        {
            i = (i+1)%128;
            if(strstr(buf,reply) != 0)
            {
                return 0;
                
            }
        }
        else
        {
            timeout--;
            HAL_Delay(1);
        }
    }
    return -1;
}
/**
*
*    建立WiFi连接
*/
int Driver_Net_ConnectWiFi(const char *ssid ,const char *pwd ,int timeout)
{
    int connect_res=-1;
    char buf[50] = "AT+CWJAP=\"";
    strcat(buf ,ssid);
    strcat(buf ,"\",\"");
    strcat(buf ,pwd);
    strcat(buf ,"\"");
    
    connect_res=Driver_Net_TransmitCmd(buf,"WIFI GOT IP",timeout);
    
    return connect_res;
}
/**
*
*    建立UDP连接
*/
int Driver_Net_ConnectUDP(const char *ip ,int port ,int timeout)
{
    char buf[128] = "AT+CIPSTART=\"UDP\",\"";
    sprintf(&buf[19],"%s\",%d",ip,port);
    return Driver_Net_TransmitCmd(buf,"OK",timeout);
}
/**
*
*    建立TCP连接
*/
int Driver_Net_ConnectTCP(const char *ip ,int port ,int timeout)
{
    char buf[128] = "AT+CIPSTART=\"TCP\",\"";
    sprintf(&buf[19],"%s\",%d",ip,port);
    return Driver_Net_TransmitCmd(buf,"OK",timeout);
}

/**
*
*    断开WiFi连接
*/
int Driver_Net_DisconnectWiFi(void)
{
    return Driver_Net_TransmitCmd("AT+CWQAP","OK",500);
}
/**
*
*    断开TCP,UDP连接
*/
int Driver_Net_Disconnect_TCP_UDP(void)
{
    return Driver_Net_TransmitCmd("AT+CIPCLOSE","OK",500);
}

/**
*
*    发送数据包
*/
int Driver_Net_TransmitSorcket(const char *socket,int len, int timeout)
{
    int i = 0;
    char buf[64] = {0};
    char cmd[16] = {0};
    sprintf(cmd,"AT+CIPSEND=%d",len);
    if(Driver_Net_TransmitCmd(cmd,"OK",timeout) == 0)
    Driver_Buffer_Clean(&CmdRetBuffer);
    HAL_UART_Transmit(&huart2,(uint8_t*)socket,len,500);
    memset(buf,0,64);
    /*等待接收wifi模块回传*/
    HAL_Delay(1000);
    while(timeout != 0)
    {
        if(Driver_Buffer_Read(&CmdRetBuffer,(uint8_t*)&buf[i]) == 0)
        {
            i = (i+1)%64;
            if(strstr(buf,"OK")!= 0)
            {
                return 0;
            }
        }
        else
        {
            timeout--;
        }
    }
    return -1;
}
/**
*
*    调试，读取环形缓冲区的数据
*/
void Debug_Read_Buffer(uint16_t timeout)
{
    char buf[64] = {0};
    int i = 0;
    while(timeout != 0)
    {
        if(Driver_Buffer_Read(&NetDataBuffer,(uint8_t*)&buf[i]) == 0)
        {
            i = (i+1)%64;
        }
        else
        {
            timeout--;
        }
    }
    if(timeout == 0)
    {
        printf("this buf----->%s+++\r\n",buf);
    }
}

enum AT_STATUS{
    INIT_STATUS,
    LEN_STATUS,
    DATA_STATUS
};
static uint8_t g_DataBuffer[256] = {0};
/**
*
*    接收网络数据
*/
void NetDataProcess_Callback(uint8_t data)
{
    uint8_t *buf = g_DataBuffer;
    static enum AT_STATUS g_status = INIT_STATUS;
    static int g_DataBufferIndex = 0;
    int i = g_DataBufferIndex;
    static int g_DataLen = 0;
    int m = 0;
    buf[i] = data;
    g_DataBufferIndex++;
    switch(g_status)
    {
        case INIT_STATUS:
        {
            if(buf[0] != '+')
            {
                g_DataBufferIndex = 0;
            }
            else if(i == 4)
            {
                if(strncmp((char*)buf,"+IPD,",5) == 0)
                {
                    g_status = LEN_STATUS;
                }
                g_DataBufferIndex = 0;
            }
            break;
        }
        case LEN_STATUS:
        {
            if(buf[i] == ':')
            {
                for(m = 0; m<i; m++)
                {
                    g_DataLen = g_DataLen *10 + buf[m] - '0';
                }
                g_status = DATA_STATUS;
                g_DataBufferIndex = 0;
            }
            else if(i >=9) /* ESP8266数据大小是2920， 4位数 ： +IPD,YYYY:xxxxxx*/
            {
                /*都没有接收到':'*/
                g_status = INIT_STATUS;
                g_DataBufferIndex = 0;
            }
            break;
        }
        case DATA_STATUS:
        {
            if(g_DataBufferIndex == g_DataLen)
            {
                /*接收完数据 */
                Driver_Buffer_WriteBytes(&NetDataBuffer,buf,g_DataLen);
                g_status = INIT_STATUS;
            }
            break;
        }   
        default:break;
    }

}
/**
*
*    读取网络数据
*/
int Driver_Net_RecvSocket(char *buf, int len, int timeout)
{
    int tmp = 0;
    while(timeout != 0)
    {
        tmp += Driver_Buffer_ReadBytes(&NetDataBuffer, (uint8_t*)&buf[tmp],len);
        if(tmp == len)
        {
            return 0;
        }
        timeout--;
        HAL_Delay(1);
    }
    return 1;
}







