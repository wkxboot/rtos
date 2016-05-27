/*
 * uxos.c
 *
 * Created: 2014/11/20 10:17:11
 *  Author: wkxboot
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_def.h"
#include "uxos.h"
#include "uxos_mem.h"
#include "uxos_timer.h"
#include "user_app.h"

extern  uint8_t tasks_count;
extern  uint8_t task_event[];
extern ptask task_list[];


void uxos_set_event(uint8_t task_id,uint8_t set_event)
{
 
 if (task_id<tasks_count)
 {
	 task_event[task_id]|=set_event;
 }
 	
}

static void uxos_event_poll()
{
 uint8_t temp_event;
 uint8_t task_id=0;
  do 
  {
	if (task_event[task_id])
	break;	
  } while (++task_id<tasks_count);
  
 if (task_id<tasks_count)
  {
	temp_event=task_event[task_id];
	task_event[task_id]=0;
	temp_event=task_list[task_id](temp_event);	
	task_event[task_id]|=temp_event;	
  }
	
}

static void uxos_update_sysclock()
{

	uint16_t temp_overflow_count;
	ENTER_CRITICAL_SECTION();
	temp_overflow_count=uxos_get_overflow();
	EXIT_CRITICAL_SECTION();
	if (temp_overflow_count>0)
	{
		ENTER_CRITICAL_SECTION();
		uxos_clear_overflow();
		EXIT_CRITICAL_SECTION();
		uxos_timer_update(temp_overflow_count);
	} 
		
}
void uxos_init()
{
	UXOS_DISABLE_GLOBLE_INT();
	uxos_mem_init();//clear ram
	uxos_tasks_init();//user to do
	uxos_timer_init();
	UXOS_ENABLE_GLOBLE_INT();
}
void uxos_start()
{	
	while (1)
	{
		uxos_event_poll();
		uxos_update_sysclock();
	}
}

