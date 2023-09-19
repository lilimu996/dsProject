#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"


#define GPIO_KEY_PORT GPIOA
#define GPIO_KEY_PIN  GPIO_PIN_0

#define KEY_INT_IRQn EXTI0_IRQn
#define KEY_INT_IRQHandler EXTI0_IRQHandler

void key_init(void);
uint8_t Get_Key_flag(void);



#endif
