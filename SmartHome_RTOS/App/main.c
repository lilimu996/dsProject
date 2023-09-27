#include "main.h"
#include "sys.h"
#include "driver_led_key.h"
#include "driver_dbg.h"
#include "stdio.h"
#include "driver_net.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"


/* Task to be created. */
void vTaskCode( void * pvParameters )
{
   uint32_t count = 0;

    for( ;; )
    {
        printf("This is a test task -> %d\r\n", count ++);
        vTaskDelay(500);
    }
}

int main(void)
{
    uint8_t buf[5] = {0};
    
    HAL_Init();
    SystemClock_Config();
    Driver_KEY_Init();
    Driver_LED_Init();
    Driver_DBG_Init();
    Driver_Net_Init();
    
    KeyEvent nKeyEvent={0};
    uint8_t count = 0;
    
    
    
    TaskHandle_t xHandle = NULL;
    BaseType_t xReturned;
    /* Create the task, storing the handle. */
    xReturned=xTaskCreate(
                vTaskCode,       /* Function that implements the task. */
                "Test Task",     /* Text name for the task. */
                128,             /* Stack size in words, not bytes. */
                NULL,            /* Parameter passed into the task. */
                1,               /* Priority at which the task is created. */
                &xHandle );      /* Used to pass out the created task's handle. */

    if( xReturned == pdPASS )
    {
        /* The task was created.   */
        printf("the task was created!\r\n");
    }
    else
    {
        printf("the task creat error!\r\n");
    }
    vTaskStartScheduler();
    
    printf("HELLO WORLD!\r\n");
    
    if(Driver_Net_ConnectWiFi("GSW24","li.120500" ,500) == 0)
    {
        printf("wifi connect success !!\r\n");
    }
    else
    {
        printf("wifi connect error !!\r\n");
    }
    /*等待建立WiFi连接*/
    HAL_Delay(2000);
    if(Driver_Net_ConnectTCP("192.168.1.116",8896,500) == 0)
    {
        printf("connct TCP success!!\r\n");
    }
    else
    {
        printf("connect TCP error!!\r\n");
    }
    while(1)
    {
        if(Driver_Net_TransmitSorcket("Hello World!\r\n",strlen("Hello World!\r\n"), 500) == 0)
        {
            printf("Send OK!\r\n");
        }
        else
        {
             printf("Send Error!\r\n");
        }
        if(Driver_Net_RecvSocket((char*)buf,5,300) == 0)
        {
             printf("receive buf-->%s\r\n",buf);
             memset(buf,0,5);
        }
        if(Driver_Key_Read((uint8_t*)&nKeyEvent.num,sizeof(nKeyEvent)) == 0)
        {
            printf("the key press:%d\r\n",count++);
            LED0_TOGGLE();
        }
    }
}










