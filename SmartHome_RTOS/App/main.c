#include "main.h"
#include "sys.h"
#include "driver_led_key.h"
#include "driver_dbg.h"
#include "stdio.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    Driver_KEY_Init();
    Driver_LED_Init();
    Driver_DBG_Init();
    
    
    KeyEvent nKeyEvent={0};
    uint8_t count = 0;
    printf("UART DEBUG!!\n\r");
    while(1)
    {
        if(Driver_Key_Read((uint8_t*)&nKeyEvent.num,sizeof(nKeyEvent)) == 0)
        {
            printf("the key press:%d\r\n",count++);
            LED0_TOGGLE();
        }
        
    }


}

