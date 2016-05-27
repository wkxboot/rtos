/*
 * user_app.c
 *
 * Created: 2014/11/20 22:36:36
 *  Author: asus
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_def.h"
#include "uxos.h"
#include "uxos_msg.h"
#include "uxos_mem.h"
#include "uxos_timer.h"
#include "user_app.h"
#include "hal_led_driver.h"
#include "hal_key_driver.h"

ptask task_list[]={process_led_event,process_key_event};
uint8_t tasks_count=sizeof(task_list)/sizeof(uint16_t);
uint8_t task_event[sizeof(task_list)/sizeof(uint16_t)];

uint8_t process_led_id=0;//task id
uint8_t process_key_id=0;//task id

extern led_control_t led_control[];
extern key_control_t key_control[];

void uxos_tasks_init()
{
  uint8_t task_id=0;
  leds_init(task_id++);
  keys_init(task_id++);	
}



void leds_init(uint8_t task_id)
{
	process_led_id=task_id;
    hal_led_register_for_task(process_led_id);//register perocess_led_event for led
	
	led_control[RED_LED].current_mode=LED_NORMAL_MODE;
	led_control[RED_LED].current_state=LED_OFF;
	led_control[RED_LED].bit_in_port=RED_LED_BIT;
	hal_led_turn_on_off(RED_LED,LED_ON);
    led_control[POWER_LED].current_mode=LED_NORMAL_MODE;
    led_control[POWER_LED].current_state=LED_OFF;
    led_control[POWER_LED].bit_in_port=POWER_LED_BIT;
    hal_led_turn_on_off(POWER_LED,LED_ON);
	led_control[BLUE_LED].current_mode=LED_NORMAL_MODE;
	led_control[BLUE_LED].current_state=LED_OFF;
	led_control[BLUE_LED].bit_in_port=BLUE_LED_BIT;
	hal_led_turn_on_off(BLUE_LED,LED_ON);
	led_control[GRENN_LED].current_mode=LED_NORMAL_MODE;
	led_control[GRENN_LED].current_state=LED_OFF;
	led_control[GRENN_LED].bit_in_port=GRENN_LED_BIT;
	hal_led_turn_on_off(GRENN_LED,LED_ON);
/*	
	hal_led_turn_on_off(RATE_LED,LED_ON);
	led_control[RATE_LED].current_mode=LED_NORMAL_MODE;
	led_control[RATE_LED].current_state=LED_OFF;
	led_control[RATE_LED].bit_in_port=RATE_LED_BIT;
	hal_led_turn_on_off(RATE_LED,LED_ON);
*/	
	hal_led_blink(RED_LED,300,40,100);
	hal_led_blink(POWER_LED,200,30,200);
	hal_led_blink(BLUE_LED,100,20,300);
	hal_led_blink(GRENN_LED,50,10,400);
	
	DDRB=0xff;
	PORTB=0xff;

	uxos_start_reload_timer(process_led_id,USER_EVENT_LED_OFF,10000);
}


void led_update()
{
	uint16_t wait_clock;
	uint16_t min_wait=0xffff;
	uint32_t current_clock;
for (uint8_t led_idx=0;led_idx<LEDS_MAX;led_idx++)
{  
	if (led_control[led_idx].current_mode==LED_BLINK_MODE)
	{
		if (led_control[led_idx].remain_changes>0)
		{
			ENTER_CRITICAL_SECTION();
			current_clock=uxos_get_sysclock();
			EXIT_CRITICAL_SECTION();
			if (current_clock>=led_control[led_idx].next)
			{
				led_control[led_idx].remain_changes--;
				if (led_control[led_idx].current_state==LED_ON)
				{
					hal_led_turn_on_off(led_idx,LED_OFF);
					led_control[led_idx].next=current_clock+led_control[led_idx].period-led_control[led_idx].on_time;
					wait_clock=led_control[led_idx].period-led_control[led_idx].on_time;
					if(wait_clock<=min_wait)
					min_wait=wait_clock;
				}
				else
				{
					hal_led_turn_on_off(led_idx,LED_ON);
					led_control[led_idx].next=current_clock+led_control[led_idx].on_time;
					wait_clock=led_control[led_idx].on_time;
					if(wait_clock<=min_wait)
					min_wait=wait_clock;
				}
			}
			else
			{
		        wait_clock=led_control[led_idx].next-current_clock;
				if(wait_clock<=min_wait)
				min_wait=wait_clock;
			}				
	 }
	else
	{
	 led_control[led_idx].current_mode=LED_NORMAL_MODE;
	 	
	}//end of 	if (led_control[led_idx].remain_changes>0)	
  }//end of if (led_control[led_idx].current_mode==LED_BLINK_MODE)
  	
 }//end of for(...)	
  if (min_wait!=0xffff)
  {
	 uxos_start_timer(process_led_id,USER_EVENT_LED_BLINK,min_wait);//reset the timer
  }	
}


uint8_t process_led_event(uint8_t event_flag)
{
	if (event_flag&SYS_EVENT_MSG)
	{
		
		return event_flag^SYS_EVENT_MSG;
	}
	if (event_flag&USER_EVENT_LED_BLINK)
	{
		led_update();
		return event_flag^USER_EVENT_LED_BLINK;
	}
	if (event_flag&USER_EVENT_LED_OFF)
	{	
		return event_flag^USER_EVENT_LED_OFF;
	}
 return event_flag;
}


void keys_init(uint8_t task_id)
{
	process_key_id=task_id;
	hal_key_register_for_task(process_key_id);
	
	KEY_DDR&=~(1<<KEY_FUNC_SET_BIT|1<<KEY_FUNC_ADD_BIT);
	KEY_PORT|=(1<<KEY_FUNC_SET_BIT)|(1<<KEY_FUNC_ADD_BIT);
	
	key_control[KEY_FUNC_SET].state=KEY_DOWN_UP;
	key_control[KEY_FUNC_SET].value=KEY_FUNC_SET_VALUE;
	
	key_control[KEY_FUNC_ADD].state=KEY_DOWN_UP;
	key_control[KEY_FUNC_ADD].value=KEY_FUNC_ADD_VALUE;
	
	uxos_start_reload_timer(process_key_id,USER_EVENT_KEY_CHECK,KEY_CHECK_PERIOD);
}

uint8_t process_key_event(uint8_t event_flag)
{	
	
	if (event_flag&SYS_EVENT_MSG) //最高优先级事件检测 SYS_EVENT_MSG
	{
		uxos_msg_t *recv_msg=uxos_search_msg(process_key_id);
		if (recv_msg)
		{
			switch(recv_msg->event_flag)
			{
				case KEY_PRESS:
				     PORTB^=(1<<5);
				break;
				case KEY_PRESS_HOLD:
				     PORTB^=(1<<4);
				break;
				default:
				break;			
			}
		uxos_delete_msg(recv_msg);	
		recv_msg=uxos_search_msg(process_key_id);//是否还有没读取的消息
		if (recv_msg)
		{
		  return event_flag;//事件标志位不清除 返回
		}
	  }
			
		return event_flag^SYS_EVENT_MSG;
	}
 if (event_flag&USER_EVENT_KEY_CHECK)
 {
	 hal_check_key_port();
	 return event_flag^USER_EVENT_KEY_CHECK;
 }
	
return event_flag;	
}

