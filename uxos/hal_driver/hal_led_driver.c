/*
 * hal_led_driver.c
 *
 * Created: 2014/11/24 14:08:07
 *  Author: wkxboot
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_def.h"
#include "hal_led_driver.h"
#include "uxos.h"
#include "uxos_timer.h"


led_control_t led_control[LEDS_MAX];
uint8_t hal_led_register_id=0xff;

void hal_led_register_for_task(uint8_t task_id) //register only once
{
	if (hal_led_register_id==0xff)
	{
		hal_led_register_id=task_id;
	}
}


void hal_led_turn_on_off(uint8_t led_idx,uint8_t set_state)
{
	uint8_t led_bit;
	if (led_idx<LEDS_MAX)
	{
		led_control[led_idx].current_state=set_state;
		led_bit=led_control[led_idx].bit_in_port;
		if (set_state==LED_ON)
		{
			LED_TURN_ON(led_bit);
		}
		else
		{
			LED_TURN_OFF(led_bit);
		}		
	}	
}

void hal_led_blink(uint8_t led_idx,uint16_t num_blinks,uint8_t percent,uint16_t period)// period unit is ms
{
	uint32_t current_clock;
	if (led_idx>LEDS_MAX)
	{
		return ;
	}
	if (period && percent)
	{
		if (percent<100)
		{
			led_control[led_idx].current_mode=LED_BLINK_MODE;
			led_control[led_idx].remain_changes=(num_blinks<<1);
			led_control[led_idx].on_time=percent*period/100;
			led_control[led_idx].period=period;
			ENTER_CRITICAL_SECTION();
			current_clock=uxos_get_sysclock();
			EXIT_CRITICAL_SECTION();
			if (led_control[led_idx].current_state==LED_ON)
			{
				led_control[led_idx].next=current_clock+led_control[led_idx].on_time;
			}
			else
			{
				led_control[led_idx].next=current_clock+period-led_control[led_idx].on_time;
			}
		}
		else
		{
			hal_led_turn_on_off(led_idx,LED_ON);
		}
	}
	else
	{
		hal_led_turn_on_off(led_idx,LED_OFF);
	}
	uxos_stop_timer(hal_led_register_id,USER_EVENT_LED_BLINK);
	uxos_set_event(hal_led_register_id,USER_EVENT_LED_BLINK);

}
