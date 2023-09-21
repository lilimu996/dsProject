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
    ��������Driver_Buffer_Init
    ���ܣ���ʼ��ָ���Ļ��λ�����
    ���������buffer->ָ��Ŀ�껺������size->��ʾ������������ڴ��С����λ���ֽ�
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
*/

int Driver_Buffer_Init(ptRingBuffer buffer,uint16_t size);
/**
    ��������Driver_Buffer_Write
    ���ܣ���ָ���Ļ��λ�����дһ���ֽ�
    ���������buffer->ָ��Ŀ�껺������data->��ʾҪд����ֽ�
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
*/
int Driver_Buffer_Write(ptRingBuffer buffer,const uint8_t data);
/**
    ��������Driver_Buffer_WriteBytes
    ���ܣ���ָ���Ļ��λ�����д�ֽ���
    ���������buffer->ָ��Ŀ�껺������data_stream->��ʾҪд����ֽ�����len->Ҫд����ֽ����ĳ���
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
*/
int Driver_Buffer_WriteBytes(ptRingBuffer buffer,const uint8_t *data_stream,uint8_t len);
/**
    ��������Driver_Buffer_Write
    ���ܣ���ָ���Ļ��λ�������ȡһ���ֽ�
    ���������buffer->ָ��Ŀ�껺������data->��ʾҪ��ȡ���ֽڣ�
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
*/
int Driver_Buffer_Read(ptRingBuffer buffer,uint8_t *data);
/**
    ��������Driver_Buffer_ReadBytes
    ���ܣ���ָ���Ļ��λ�������ȡ�ֽ���
    ���������buffer->ָ��Ŀ�껺������data_stream->��ʾҪ��ȡ���ֽ�����len->Ҫ��ȡ���ֽ����ĳ���
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
*/
int Driver_Buffer_ReadBytes(ptRingBuffer buffer,uint8_t *data_stream,uint8_t len);
/**
    ��������Driver_Buffer_Clean
    ���ܣ����ָ���Ļ��λ�����
    ���������buffer->ָ��Ŀ�껺������
    �����������
    ����ֵ��-1->��ʾ����0->��ʾ�ɹ�
*/
int Driver_Buffer_Clean(ptRingBuffer buffer);



#endif
