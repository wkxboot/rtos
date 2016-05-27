/*
 * hal_key_driver.c
 *
 * Created: 2014/11/24 16:11:01
 *  Author: wkxboot
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_def.h"
#include "hal_key_driver.h"
#include "uxos.h"
#include "uxos_timer.h"
#include "uxos_mem.h"
#include "uxos_msg.h"

uint8_t hal_key_register_id=0xff;
key_control_t key_control[KEY_MAX];

void hal_key_register_for_task(uint8_t task_id)
{
	if (hal_key_register_id==0xff)
	{
		hal_key_register_id=task_id;
	}
}


static uint8_t hal_read_key_port()
{
	uint8_t temp_key;
	temp_key=KEY_PIN;
	
	return temp_key;
	
}

void hal_check_key_port(void)
{
	uint32_t current_clock;
	uint32_t interval=0;
	uint8_t temp_key;
	temp_key=hal_read_key_port();
	
	ENTER_CRITICAL_SECTION();
	current_clock=uxos_get_sysclock();
	EXIT_CRITICAL_SECTION();
	
	for (uint8_t key_idx=0;key_idx<KEY_MAX;key_idx++)
	{
		if (!(temp_key&key_control[key_idx].value))//��ǰ��������
		{
			interval=current_clock-key_control[key_idx].time_stamp;//��ȡ���ϴ�״̬֮��ļ��ʱ���
			
			if (key_control[key_idx].state==KEY_DOWN_UP)//�����һ�μ���״̬��DOWN_UP
			{
				key_control[key_idx].state=KEY_PRESS ;//��ô��ʱ״̬Ϊ��������
				key_control[key_idx].time_stamp=current_clock;//����ʱ���
			}
			else if(key_control[key_idx].state==KEY_PRESS )//�����һ�μ���״̬��KEY_PRESS��������
			{
				if (interval>=KEY_LONG_PRESS_TIME)//�������ʱ����ڳ�����ʱ��
				{
					key_control[key_idx].state=KEY_PRESS_HOLD;//��ʱ״̬����ΪKEY_PRESS_HOLD������
					key_control[key_idx].time_stamp=current_clock;//����ʱ���
				}
			}
			else if (key_control[key_idx].state==KEY_PRESS_HOLD)//�����һ�μ���״̬��KEY_PRESS_HOLD������
			{
				if (interval>=KEY_LONG_PRESS_PERIOD)//���ʱ����ڳ�������������
				{
					key_control[key_idx].state=KEY_PRESS_HOLD;//���ֳ�����״̬
					key_control[key_idx].time_stamp=current_clock;//����ʱ���
				}
			}
			if (key_control[key_idx].time_stamp==current_clock)//���ʱ����͵�ǰʱ����� ��ʾ����״̬
			{
				uxos_msg_t *pkey_hold_msg=uxos_mem_alloc(sizeof(uxos_msg_t));
				if (pkey_hold_msg )//send the key_hold_msg to process task
				{
					pkey_hold_msg->event_flag=key_control[key_idx].state;
					pkey_hold_msg->task_id=hal_key_register_id;
					uxos_send_msg(hal_key_register_id,pkey_hold_msg,SYS_EVENT_MSG);
				}
			}
		}//end of if (!(temp_key&key_control[key_idx].value))
		else
		{
			key_control[key_idx].state=KEY_DOWN_UP;
		}
	}//end of for(....)
}

