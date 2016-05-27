/*
 * hal_led_driver.h
 *
 * Created: 2014/11/24 14:10:10
 *  Author: wkxboot
 */ 


#ifndef __HAL_LED_DRIVER_H__
#define __HAL_LED_DRIVER_H__

/*
* ************************ user configure  start ***************************
*/

#define   LED_PORT                     PORTB  //PORTB or PORTC or PORTX
#ifndef   BIT_IN_PORT_X_X
#define   BIT_IN_PORT(x_bit)                x_bit
#endif

#define   LEDS_MAX                     4 //max led nums
#define   RED_LED                      0 //led NO.0
#define   POWER_LED                    1 //led NO.1
#define   BLUE_LED                     2 //led NO.2
#define   GRENN_LED                    3 //led NO.3
#define   RATE_LED                     4 //led NO.4

#define   RED_LED_BIT                  BIT_IN_PORT(0) //bit in LED_PORT
#define   POWER_LED_BIT                BIT_IN_PORT(1) //bit in LED_PORT
#define   BLUE_LED_BIT                 BIT_IN_PORT(2) //bit in LED_PORT
#define   GRENN_LED_BIT                BIT_IN_PORT(3) //bit in LED_PORT
#define   RATE_LED_BIT                 BIT_IN_PORT(4) //bit in LED_PORT

/*
* ************************ user configure end  ***************************
*/
#define   LED_TURN_ON(x_bit)           LED_PORT&=~(1<<x_bit)
#define   LED_TURN_OFF(x_bit)          LED_PORT|=(1<<x_bit)

#define   LED_BLINK_MODE               1
#define   LED_NORMAL_MODE              2


#define   LED_ON                       1
#define   LED_OFF                      2

/*
* private task event flag;
*/
#define   USER_EVENT_LED_ON            0x01
#define   USER_EVENT_LED_OFF           0x02
#define   USER_EVENT_LED_BLINK         0x04



typedef struct
{
	uint8_t bit_in_port;//bit in portb or portx
	uint8_t current_mode;//blink or normal
	uint8_t current_state;//led on or off
	uint16_t remain_changes;//change times remain
	uint16_t on_time;// led on time
	uint16_t period;// one cycle time -ms
	uint32_t next;//next time to change state -ms
}led_control_t;

void hal_led_register_for_task(uint8_t task_id); //register only once;
void hal_led_turn_on_off(uint8_t led_idx,uint8_t set_state);
void hal_led_blink(uint8_t led_idx,uint16_t num_blinks,uint8_t percent,uint16_t period);// period unit is ms
#endif /* HAL_LED_DRIVER_H_ */