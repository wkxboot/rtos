/*
 * uxos_timer.h
 *
 * Created: 2014/11/20 10:18:19
 *  Author: wkxboot
 */ 


#ifndef __UXOS_TIMER_H__
#define __UXOS_TIMER_H__

#define    SUCCESS                  0
#define    NO_TIMER_AVAIL           1
#define    INVALID_EVENT_ID         2


#define MEGA8_TIMER0_DIV_STOP       (0<<0)
#define MEGA8_TIMER0_DIV_1          (1<<0)
#define MEGA8_TIMER0_DIV_8          (1<<1)
#define MEGA8_TIMER0_DIV_64         (1<<1)|(1<<0)
#define MEGA8_TIMER0_DIV_256        (1<<2)
#define MEGA8_TIMER0_DIV_1024       (1<<2)|(1<<0)
#define MEGA8_TIMER0_UP_DOWN        (1<<2)|(1<<1)
#define MEGA8_TIMER0_DOWN_UP        (1<<2)|(1<<1)|(1<<0)
/*     MEGA8 TIMSK    */
#define MEGA8_TIMER0_I_ENABLE       (1<<0)
#define MEGA8_TIMER0_I_DISABLE      (0)


/*div==256 cpu_f==8000000hz time set value*/

#define   TIMER_COUNT_100_us         0xFD //100us
#define   TIMER_COUNT_200_us         0xFA //200us
#define   TIMER_COUNT_400_us         0xF4 //400us
#define   TIMER_COUNT_500_us         0xF1 //500s
#define   TIMER_COUNT_800_us         0xE7 //800s
#define   TIMER_COUNT_1000_us        0xE0 //1000us

/*div==1024 cpu_f==8000000hz time set value*/
#define TIMER_COUNT_1_MS             0xF8 //1ms
#define TIMER_COUNT_5_MS             0xD9 //5ms
#define TIMER_COUNT_10_MS            0xB1 //10ms
#define TIMER_COUNT_20_MS            0x64 //20ms

#define TIMER_DIV_XX                 MEGA8_TIMER0_DIV_256
#define TIMER_COUNT_XX               TIMER_COUNT_1000_us
#define TIMER_I_XX                   MEGA8_TIMER0_I_ENABLE


typedef struct 
{
void *next;
uint8_t event_flag;
uint8_t task_id;
uint16_t timeout;
uint16_t reload_timeout;	
}uxos_timer_t;

uint16_t uxos_get_overflow(void);
uint32_t uxos_get_sysclock(void);
void uxos_clear_overflow(void);
void uxos_timer_init();
uxos_timer_t *uxos_add_timer( uint8_t task_id, uint8_t event_flag, uint16_t timeout );
uxos_timer_t *uxos_find_timer( uint8_t task_id, uint8_t event_flag );
uint8_t uxos_start_timer( uint8_t taskID, uint8_t event_flag, uint16_t timeout );
uint8_t uxos_start_reload_timer( uint8_t taskID, uint8_t event_flag, uint16_t timeout);
uint8_t uxos_stop_timer( uint8_t task_id, uint8_t event_flag );
uint16_t uxos_get_timeout( uint8_t task_id, uint8_t event_flag );
void uxos_timer_update( uint16_t updateTime );




#endif /* UXOS_TIMER_H_ */