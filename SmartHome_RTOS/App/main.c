#include "main.h"
#include "sys.h"
#include "stdio.h"
#include "dev_io.h"
#include "dev_net.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"

extern void vStartMQTTTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority);
extern void vStartLEDTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority);
extern void vStartKEYTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority);
static ptIODev dbgoutDev = NULL;




int main(void)
{
  
    
    HAL_Init();
    SystemClock_Config();
  
    dbgoutDev = IODev_GetDev(DBGOUT);
    if(dbgoutDev != NULL)
    {
        dbgoutDev->Init(dbgoutDev);
    }
    
    vStartMQTTTasks(512,10);
    vStartLEDTasks(128, 1);
    vStartKEYTasks(128,2);
    vTaskStartScheduler();
    while(1)
    {
        
    }
}










