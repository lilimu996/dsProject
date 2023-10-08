/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "dev_io.h"
#include "stdio.h"

TaskHandle_t keyTaskHandle;


void KeyTask(void *patameter)
{
    
    ptIODev keyDev = IODev_GetDev(KEY);
    if(keyDev != NULL)
    {
        keyDev->Init(keyDev);
    }
    else 
    {
        printf("KEY Device not found. \r\n");
    }
    
    while(1)
    {
    
    }

}



void vStartKEYTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
    BaseType_t x = 0L;
    
    if(xTaskCreate(KeyTask,	                            /* The function that implements the task. */
            "KEY",                                      /* Just a text name for the task to aid debugging. */
            usTaskStackSize,                            /* The stack size is defined in FreeRTOSIPConfig.h. */
            (void *)x,                                  /* The task parameter, not used in this case. */
            uxTaskPriority,	                            /* The priority assigned to the task is defined in FreeRTOSConfig.h. */
            &keyTaskHandle) == pdPASS)                  /* The task handle is not used. */
    {
        printf("Create KEY Task success. \r\n");
    }
    else 
    {
        printf("Create KEY Task success. \r\n");
    }

    
}
/*-----------------------------------------------------------*/
