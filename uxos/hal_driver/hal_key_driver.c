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
		if (!(temp_key&key_control[key_idx].value))//当前按键按下
		{
			interval=current_clock-key_control[key_idx].time_stamp;//读取与上次状态之间的间隔时间差
			
			if (key_control[key_idx].state==KEY_DOWN_UP)//如果上一次检测的状态是DOWN_UP
			{
				key_control[key_idx].state=KEY_PRESS ;//那么此时状态为按键按下
				key_control[key_idx].time_stamp=current_clock;//更新时间戳
			}
			else if(key_control[key_idx].state==KEY_PRESS )//如果上一次检测的状态是KEY_PRESS按键按下
			{
				if (interval>=KEY_LONG_PRESS_TIME)//如果按键时间大于长按键时间
				{
					key_control[key_idx].state=KEY_PRESS_HOLD;//此时状态更新为KEY_PRESS_HOLD长按键
					key_control[key_idx].time_stamp=current_clock;//更新时间戳
				}
			}
			else if (key_control[key_idx].state==KEY_PRESS_HOLD)//如果上一次检测的状态是KEY_PRESS_HOLD长按键
			{
				if (interval>=KEY_LONG_PRESS_PERIOD)//如果时间大于长按键触发周期
				{
					key_control[key_idx].state=KEY_PRESS_HOLD;//保持长按键状态
					key_control[key_idx].time_stamp=current_clock;//更新时间戳
				}
			}
			if (key_control[key_idx].time_stamp==current_clock)//如果时间戳和当前时间相等 表示是新状态
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

