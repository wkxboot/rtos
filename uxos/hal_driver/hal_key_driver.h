/*
 * hal_key_driver.h
 *
 * Created: 2014/11/24 16:11:21
 *  Author: wkxboot
 */ 


#ifndef __HAL_KEY_DRIVER_H__
#define __HAL_KEY_DRIVER_H__

#ifndef   BIT_IN_PORT_X_X  
#define   BIT_IN_PORT(x_bit)                x_bit
#endif

#define   KEY_MAX                   2

#define   KEY_PIN                   PINB
#define   KEY_PORT                  PORTB
#define   KEY_DDR                   DDRB

#define   KEY_FUNC_SET              0//key NO.0
#define   KEY_FUNC_ADD              1//key NO.1

#define   KEY_FUNC_SET_BIT          BIT_IN_PORT(6)//key bit in port
#define   KEY_FUNC_ADD_BIT          BIT_IN_PORT(7)//key bit in port

#define   KEY_FUNC_SET_VALUE          (1<<KEY_FUNC_SET_BIT)
#define   KEY_FUNC_ADD_VALUE          (1<<KEY_FUNC_ADD_BIT)

#define   KEY_CHECK_PERIOD            50   // ����������� 50ms 20 HZ/s
#define   KEY_LONG_PRESS_TIME         2000 // press��������2000ms ��ʾ����
#define   KEY_LONG_PRESS_PERIOD       250 // ������ʱ�������� 4Hz/s
/*
* �����������μ��״ֵ̬
*/
#define   KEY_UP_DOWN                    1 //�����½���
#define   KEY_DOWN_UP                    2 //����������
#define   KEY_PRESS                      3 //��������״̬
#define   KEY_PRESS_HOLD                 4 //�������ְ���״̬


#define   USER_EVENT_KEY_PRESS         0x01
#define   USER_EVENT_KEY_HOLD          0x02
#define   USER_EVENT_KEY_CHECK         0x04



typedef struct
{
uint8_t value;//key value	
uint8_t state;//key state up or down or hold
uint32_t time_stamp;//
	
}key_control_t;

void hal_check_key_port(void);
void hal_key_register_for_task(uint8_t task_id);

#endif /* HAL_KEY_DRIVER_H_ */