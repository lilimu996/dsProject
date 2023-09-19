#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"


void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    
     gpio_init_struct.Mode=GPIO_MODE_IT_RISING;
     gpio_init_struct.Pin=GPIO_KEY_PIN;
     gpio_init_struct.Pull=GPIO_PULLDOWN;;
     gpio_init_struct.Speed=GPIO_SPEED_FREQ_LOW;
     HAL_GPIO_Init(GPIO_KEY_PORT, &gpio_init_struct);        /* WKUP����Ϊ�����ش����ж� */
    
     HAL_NVIC_SetPriority(KEY_INT_IRQn, 3, 2); /* ��ռ 3�������ȼ� 2 */
     HAL_NVIC_EnableIRQ(KEY_INT_IRQn); /* ʹ���ж��� 0 */

    
}


/**
* @brief key �ⲿ�жϷ������
* @param ��
* @retval ��
*/
void KEY_INT_IRQHandler(void)
{
    /* �����жϴ����ú��� ��� KEY �����ж��� ���жϱ�־λ */
    HAL_GPIO_EXTI_IRQHandler(GPIO_KEY_PIN);
    /* HAL ��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_KEY_PIN);
}

uint8_t key_flag=0;

/**
* @brief �жϷ����������Ҫ��������
�� HAL �������е��ⲿ�жϷ�����������ô˺���
* @param GPIO_Pin:�ж����ź�
* @retval ��
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    
    if(GPIO_Pin==GPIO_KEY_PIN)
    {
        delay_ms(10); /* ���� */
        key_flag=1;
       
    }
}
/**
*��ѯ������־
*/
uint8_t Get_Key_flag(void)
{
    if(key_flag==1)
    {
        key_flag=0;
        return 1;
    }
    return 0;
}


