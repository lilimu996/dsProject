#include "driver_buffer.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

/**
    ��������Driver_Buffer_Init
    ���ܣ���ʼ��ָ���Ļ��λ�����
    ���������buffer->ָ��Ŀ�껺������size->��ʾ������������ڴ��С����λ���ֽ�
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
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
    ��������Driver_Buffer_Write
    ���ܣ���ָ���Ļ��λ�����дһ���ֽ�
    ���������buffer->ָ��Ŀ�껺������data->��ʾҪд����ֽ�
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
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
    if(i!=buffer->pr)                       //pw == pr �������Ѿ������߻�����Ϊ��  (pw+1)/size ���������pw�ڱ�ͷ��pw�ڱ�β
    {
        buffer->fifo[buffer->pw]=data;
        buffer->pw=i;
        return 0;
    }
    //printf("buffer write error!!\r\n");
    return -1;
}

/**
    ��������Driver_Buffer_WriteBytes
    ���ܣ���ָ���Ļ��λ�����д�ֽ���
    ���������buffer->ָ��Ŀ�껺������data_stream->��ʾҪд����ֽ�����len->Ҫд����ֽ����ĳ���
    �����������
    ����ֵ��-1->��ʾ����len->��ʾ�ɹ�
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
    ��������Driver_Buffer_Write
    ���ܣ���ָ���Ļ��λ�������ȡһ���ֽ�
    ���������buffer->ָ��Ŀ�껺������data->����Ҫ��ȡ���ֽڣ�
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
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
    ��������Driver_Buffer_ReadBytes
    ���ܣ���ָ���Ļ��λ�������ȡ�ֽ���
    ���������buffer->ָ��Ŀ�껺������data_stream->��ʾҪ��ȡ���ֽ�����len->Ҫ��ȡ���ֽ����ĳ���
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
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
    ��������Driver_Buffer_Clean
    ���ܣ����ָ���Ļ��λ�����
    ���������buffer->ָ��Ŀ�껺������
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
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








