#include "main.h"
#include "sys.h"
#include "driver_led_key.h"

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	Driver_KEY_Init();
	Driver_LED_Init();
	
	
	return 0;

}

