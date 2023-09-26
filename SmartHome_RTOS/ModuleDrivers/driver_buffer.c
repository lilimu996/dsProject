#include "driver_buffer.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

/**
    函数名：Driver_Buffer_Init
    功能：初始化指定的环形缓冲区
    输入参数：buffer->指向目标缓冲区；size->表示缓冲区分配的内存大小，单位是字节
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/

int Driver_Buffer_Init(ptRingBuffer buffer,uint16_t size)
{
    if(buffer==NULL||size==0)
    {
        return -1;
    }
    if(buffer->fifo==NULL)
    {
        buffer->fifo=(uint8_t*)malloc(size);
    }
    buffer->pw=buffer->pr=0;
    buffer->buf_size=size;

    return 0;
}
/**
    函数名：Driver_Buffer_Write
    功能：向指定的环形缓冲区写一个字节
    输入参数：buffer->指向目标缓冲区；data->表示要写入的字节
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_Write(ptRingBuffer buffer,const uint8_t data)
{
    //printf("buffer wirte-------------------------------->!!\r\n");
    if(buffer==NULL || buffer->fifo == NULL)
    {
        //printf("ERROR!! buffer == NULL\r\n");
        return -1;
    }   
    int i = (buffer->pw+1)%(buffer->buf_size);           
    //printf("buffer->pw+1 == %d    buffer->buf_size ==  %d   i == %d \r\n",buffer->pw+1,buffer->buf_size,i);
    if(i!=buffer->pr)                       //pw == pr 缓冲区已经满或者缓冲区为空  (pw+1)/size 则可以区分pw在表头和pw在表尾
    {
        buffer->fifo[buffer->pw]=data;
        buffer->pw=i;
        return 0;
    }
    //printf("buffer write error!!\r\n");
    return -1;
}

/**
    函数名：Driver_Buffer_WriteBytes
    功能：向指定的环形缓冲区写字节流
    输入参数：buffer->指向目标缓冲区；data_stream->表示要写入的字节流；len->要写入的字节流的长度
    输出参数：无
    返回值：-1->表示错误；len->表示成功
*/
int Driver_Buffer_WriteBytes(ptRingBuffer buffer,const uint8_t *data_stream,uint8_t len)
{	
    if(buffer==NULL || buffer->fifo == NULL)
    {
        return -1;
    }
    if(data_stream == NULL)
    {
        return -1;
    }
    if(len == 0)
    {
        return -1;
    }
    int i=0;
    for(i=0;i<len;i++)
    {
        if(Driver_Buffer_Write(buffer,data_stream[i]) != 0)
        {
            break;
        }
    }
    return i;
}
/**
    函数名：Driver_Buffer_Write
    功能：向指定的环形缓冲区读取一个字节
    输入参数：buffer->指向目标缓冲区；data->保存要读取的字节；
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_Read(ptRingBuffer buffer,uint8_t *data)
{
	if(buffer==NULL || buffer->fifo == NULL)
	{
		return -1;
	}
	if(data == NULL)
	{
		return -1;
	}
	if(buffer->pr == buffer->pw)
	{
		return -1;
	}
	*data=buffer->fifo[buffer->pr];
	buffer->pr = (buffer->pr + 1) % buffer->buf_size;
	
	return 0;
}

/**
    函数名：Driver_Buffer_ReadBytes
    功能：向指定的环形缓冲区读取字节流
    输入参数：buffer->指向目标缓冲区；data_stream->表示要读取的字节流；len->要读取的字节流的长度
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_ReadBytes(ptRingBuffer buffer,uint8_t *data_stream,uint8_t len)
{
	if(buffer==NULL || buffer->fifo == NULL)
	{
		return -1;
	}
	if(data_stream == NULL)
	{
		return -1;
	}
	if(len == 0)
	{
		return -1;
	}
	int i=0;
	for(i=0;i<len;i++)
	{
		if(Driver_Buffer_Read(buffer,&data_stream[i]) != 0)
		{
			break;
		}
	}
	return i;
}
/**
    函数名：Driver_Buffer_Clean
    功能：清空指定的环形缓冲区
    输入参数：buffer->指向目标缓冲区；
    输出参数：无
    返回值：-1->表示错误；0->表示成功
*/
int Driver_Buffer_Clean(ptRingBuffer buffer)
{
	if(buffer == NULL)
	{
		return -1;
	}
	memset(buffer->fifo,0,buffer->buf_size);
	buffer->pw=buffer->pr=0;
	return 0;
}








