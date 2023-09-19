#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"


#define LED_GPIO_PORT GPIOB
#define LED_GPIO_PIN  GPIO_PIN_5


#define LED(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)     

#define LED_TOGGLE()   do{ HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN); }while(0)        /* ·­×ªLED0 */

void led_init(void);

#endif
