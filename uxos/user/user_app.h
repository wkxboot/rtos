/*
 * app.c
 *
 * Created: 2014/11/20 22:35:24
 *  Author: asus
 */ 


#ifndef __USER_APP_H__
#define __USER_APP_H__



void keys_init(uint8_t task_id);	
void leds_init(uint8_t task_id);
void uxos_tasks_init();
void led_blink(uint8_t led_idx,uint8_t num_blinks,uint8_t percent,uint16_t period);// period unit is ms
void led_turn_on_off(uint8_t led_idx,uint8_t set_state);
void led_update();
uint8_t process_led_event(uint8_t task_event);
uint8_t process_key_event(uint8_t task_event);
#endif /* APP_H_ */