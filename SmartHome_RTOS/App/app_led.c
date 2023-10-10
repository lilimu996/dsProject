/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "dev_io.h"
#include "stdio.h"

TaskHandle_t ledTaskHandle;

void LedTask(void *patameter)
{
    uint32_t notify_value = 0;
    ptIODev ledDev = IODev_GetDev(LED);
    if(ledDev != NULL)
    {
        ledDev->Init(ledDev);
    }
    else 
    {
        printf("LED Device not found. \r\n");
    }
    
    while(1)
    {
        if(xTaskNotifyWait(0,0xFFFFFFFF, &notify_value, pdMS_TO_TICKS(10) == pdTRUE))
        {
            printf("LED Task notify value is %d \r\n",notify_value);
            ledDev->Write(ledDev, (unsigned char*)&notify_value, 1);
        }
        
    }

}


void vStartLEDTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
    BaseType_t x = 0L;
    if(xTaskCreate(LedTask,	                            /* The function that implements the task. */
            "LED",                                      /* Just a text name for the task to aid debugging. */
            usTaskStackSize,                            /* The stack size is defined in FreeRTOSIPConfig.h. */
            (void *)x,                                  /* The task parameter, not used in this case. */
            uxTaskPriority,                             /* The priority assigned to the task is defined in FreeRTOSConfig.h. */
            &ledTaskHandle) == pdPASS)                  /* The task handle is not used. */
    {
        printf("Create LED Task success. \r\n");            
    }
    else
    {
        printf("Create LED Task fail. \r\n");
    }
    
}
/*-----------------------------------------------------------*/
