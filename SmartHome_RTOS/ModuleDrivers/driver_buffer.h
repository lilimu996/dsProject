#ifndef __DRIVER_BUFFER_H
#define __DRIVER_BUFFER_H

#include "stm32f1xx_hal.h"

typedef struct{
	uint8_t *fifo;
	uint16_t pw;
	uint16_t pr;
	uint16_t buf_size;
}RingBuffer,*ptRingBuffer;

/**
    函数名：Driver_Buffer_Init
    功能：初始化指定的环形缓冲区
    输入参数：buffer->指向目标缓冲区；size->表示缓冲区分配的内存大小，单位是字节
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/

int Driver_Buffer_Init(ptRingBuffer buffer,uint16_t size);
/**
    函数名：Driver_Buffer_Write
    功能：向指定的环形缓冲区写一个字节
    输入参数：buffer->指向目标缓冲区；data->表示要写入的字节
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_Write(ptRingBuffer buffer,const uint8_t data);
/**
    函数名：Driver_Buffer_WriteBytes
    功能：向指定的环形缓冲区写字节流
    输入参数：buffer->指向目标缓冲区；data_stream->表示要写入的字节流；len->要写入的字节流的长度
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_WriteBytes(ptRingBuffer buffer,const uint8_t *data_stream,uint8_t len);
/**
    函数名：Driver_Buffer_Write
    功能：向指定的环形缓冲区读取一个字节
    输入参数：buffer->指向目标缓冲区；data->表示要读取的字节；
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_Read(ptRingBuffer buffer,uint8_t *data);
/**
    函数名：Driver_Buffer_ReadBytes
    功能：向指定的环形缓冲区读取字节流
    输入参数：buffer->指向目标缓冲区；data_stream->表示要读取的字节流；len->要读取的字节流的长度
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_ReadBytes(ptRingBuffer buffer,uint8_t *data_stream,uint8_t len);
/**
    函数名：Driver_Buffer_Clean
    功能：清空指定的环形缓冲区
    输入参数：buffer->指向目标缓冲区；
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_Clean(ptRingBuffer buffer);



#endif
