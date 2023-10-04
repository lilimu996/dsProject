#ifndef __DEV_IO_H
#define __DEV_IO_H

#include "stm32f1xx_hal.h"

typedef enum{
    LED = (0),
    KEY = (1),
    DBGOUT = (2),
}IODevType;

typedef struct IODev 
{
    IODevType Type;
    void(*Init)(struct IODev *dev);
    int(*Write)(struct IODev *dev,uint8_t *buf,uint16_t len);
    int(*Read)(struct IODev *dev,uint8_t *buf,uint16_t len);
}IODev,*ptIODev;

 typedef struct{
    uint16_t num;
    uint16_t time;
}KeyEvent;



static void IODev_Init(struct IODev *dev);
static int IODev_Write(struct IODev *dev,uint8_t *buf,uint16_t len);
static int IODev_Read(struct IODev *dev,uint8_t *buf,uint16_t len);
ptIODev IODev_GetDev(IODevType type);







































#endif
