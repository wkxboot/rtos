/*
 * uxos_timer.c
 *
 * Created: 2014/11/20 10:17:57
 *  Author: wkxboot
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_def.h"
#include "uxos.h"
#include "uxos_mem.h"
#include "uxos_timer.h"

 uxos_timer_t *ptimer_head=null_ptr;
 uint8_t  timer0_count=0;
 uint16_t timer_overflow_count=0;
 uint32_t ugos_system_clock=0;

static void mega8_timer0_init(uint8_t clock_mode,uint8_t t_count,uint8_t tint_enable)
{
	TCCR0=(TCCR0&(~0x07))|clock_mode;
	TIMSK=(TIMSK&(~0x01))|tint_enable;
	TCNT0=t_count;
	timer0_count=t_count;
	
}

ISR(TIMER0_OVF_vect)
{
	
	TCNT0=timer0_count;//重装定时器 8MHZ -div256-1ms one tick
	timer_overflow_count++;//每一个时钟tick 溢出+1
	ugos_system_clock++;

}


void uxos_timer_init()
{
	mega8_timer0_init(TIMER_DIV_XX,TIMER_COUNT_XX,TIMER_I_XX);
}

void uxos_clear_overflow(void)
{
	timer_overflow_count=0;
}
uint16_t uxos_get_overflow(void)
{
	return timer_overflow_count;
}	

uint32_t uxos_get_sysclock(void)
{
	return ugos_system_clock;
}
/*********************************************************************
 * @fn      uxos_add_timer
 *
 * @brief   Add a timer to the timer list.
 *          Ints must be disabled.
 *
 * @param   task_id
 * @param   event_flag
 * @param   timeout
 *
 * @return  uxos_timer_t * - pointer to newly created timer
 */
uxos_timer_t *uxos_add_timer( uint8_t task_id, uint8_t event_flag, uint16_t timeout )
{
  uxos_timer_t *new_timer;
  uxos_timer_t *srch_timer;

  // Look for an existing timer first
  new_timer = uxos_find_timer( task_id, event_flag );
  if ( new_timer )
  {
    // Timer is found - update it.
    new_timer->timeout = timeout;

    return ( new_timer );
  }
  else
  {
    // New Timer
    new_timer =(uxos_timer_t *) uxos_mem_alloc( sizeof( uxos_timer_t ));

    if ( new_timer )
    {
      // Fill in new timer
      new_timer->task_id = task_id;
      new_timer->event_flag = event_flag;
      new_timer->timeout = timeout;
      new_timer->next = null_ptr;
      new_timer->reload_timeout = 0;

      // Does the timer list already exist
      if ( ptimer_head == null_ptr )
      {
        // Start task list
        ptimer_head = new_timer;
      }
      else
      {
        // Add it to the end of the timer list
        srch_timer = ptimer_head;

        // Stop at the last record
        while ( srch_timer->next )
          srch_timer = srch_timer->next;

        // Add to the list
        srch_timer->next = new_timer;
      }

      return ( new_timer );
    }
    else
      return ( (uxos_timer_t *)null_ptr );
  }
  return ( (uxos_timer_t *)null_ptr );//消除编译警告
}

/*********************************************************************
 * @fn      uxos_find_timer
 *
 * @brief   Find a timer in a timer list.
 *          Ints must be disabled.
 *
 * @param   task_id
 * @param   event_flag
 *
 * @return  uxos_timer_t *
 */
uxos_timer_t *uxos_find_timer( uint8_t task_id, uint8_t event_flag )
{
  uxos_timer_t *srch_timer;

  // Head of the timer list
  srch_timer = ptimer_head;

  // Stop when found or at the end
  while ( srch_timer )
  {
    if ( srch_timer->event_flag == event_flag &&
         srch_timer->task_id == task_id )
      break;

    // Not this one, check another
    srch_timer = srch_timer->next;
  }

  return ( srch_timer );
}

/*********************************************************************
 * @fn      uxos_delete_timer
 *
 * @brief   Delete a timer from a timer list.
 *
 * @param   table
 * @param   pdelete_timer
 *
 * @return  none
 */
void uxos_delete_timer( uxos_timer_t *pdelete_timer )
{
  // Does the timer list really exist
  if ( pdelete_timer )
  {
    // Clear the event flag and uxos_timer_update() will delete 
    // the timer from the list.
    pdelete_timer->event_flag = 0;
  }
}

/*********************************************************************
 * @fn      uxos_start_timer
 *
 * @brief
 *
 *   This function is called to start a timer to expire in n mSecs.
 *   When the timer expires, the calling task will get the specified event.
 *
 * @param   uint8_t task_id - task id to set timer for
 * @param   uint16_t event_flag - event to be notified with
 * @param   uint16 timeout - in milliseconds.
 *
 * @return  SUCCESS, or NO_TIMER_AVAIL.
 */
uint8_t uxos_start_timer( uint8_t task_id, uint8_t event_flag, uint16_t timeout )
{
  uxos_timer_t *new_timer;

  ENTER_CRITICAL_SECTION();  // Hold off interrupts.

  // Add timer
  new_timer = uxos_add_timer( task_id, event_flag, timeout );

  EXIT_CRITICAL_SECTION();   // Re-enable interrupts.

  return ( (new_timer != null_ptr) ? SUCCESS : NO_TIMER_AVAIL );
}

/*********************************************************************
 * @fn      uxos_start_reload_timer
 *
 * @brief
 *
 *   This function is called to start a timer to expire in n mSecs.
 *   When the timer expires, the calling task will get the specified event
 *   and the timer will be reloaded with the timeout value.
 *
 * @param   uint8_t task_id - task id to set timer for
 * @param   uint16_t event_flag - event to be notified with
 * @param   UNINT16 timeout - in milliseconds.
 *
 * @return  SUCCESS, or NO_TIMER_AVAIL.
 */
uint8_t uxos_start_reload_timer( uint8_t task_id, uint8_t event_flag, uint16_t timeout )
{
  uxos_timer_t *new_timer;

  ENTER_CRITICAL_SECTION();  // Hold off interrupts.

  // Add timer
  new_timer = uxos_add_timer( task_id, event_flag, timeout );
  if ( new_timer )
  {
    // Load the reload timeout value
    new_timer->reload_timeout = timeout;
  }

  EXIT_CRITICAL_SECTION();   // Re-enable interrupts.

  return ( (new_timer != null_ptr) ? SUCCESS : NO_TIMER_AVAIL );
}

/*********************************************************************
 * @fn      uxos_stop_timer
 *
 * @brief
 *
 *   This function is called to stop a timer that has already been started.
 *   If ZSUCCESS, the function will cancel the timer and prevent the event
 *   associated with the timer from being set for the calling task.
 *
 * @param   uint8_t task_id - task id of timer to stop
 * @param   uint16_t event_flag - identifier of the timer that is to be stopped
 *
 * @return  SUCCESS or INVALID_EVENT_ID
 */
uint8_t uxos_stop_timer( uint8_t task_id, uint8_t event_flag )
{
  uxos_timer_t *foundTimer;

  ENTER_CRITICAL_SECTION();  // Hold off interrupts.

  // Find the timer to stop
  foundTimer = uxos_find_timer( task_id, event_flag );
  if ( foundTimer )
  {
    uxos_delete_timer( foundTimer );
  }

  EXIT_CRITICAL_SECTION();   // Re-enable interrupts.

  return ( (foundTimer != null_ptr) ? SUCCESS : INVALID_EVENT_ID );
}

/*********************************************************************
 * @fn      uxos_get_timeout
 *
 * @brief
 *
 * @param   uint8_t task_id - task id of timer to check
 * @param   uint16_t event_flag - identifier of timer to be checked
 *
 * @return  Return the timer's tick count if found, zero otherwise.
 */
uint16_t uxos_get_timeout( uint8_t task_id, uint8_t event_flag )
{
  uint16_t rtrn = 0;
  uxos_timer_t *tmr;

  ENTER_CRITICAL_SECTION();  // Hold off interrupts.

  tmr = uxos_find_timer( task_id, event_flag );

  if ( tmr )
  {
    rtrn = tmr->timeout;
  }

  EXIT_CRITICAL_SECTION();   // Re-enable interrupts.

  return rtrn;
}

/*********************************************************************
 * @fn      uxos_timer_update
 *
 * @brief   Update the timer structures for a timer tick.
 *
 * @param   none
 *
 * @return  none
 *********************************************************************/
void uxos_timer_update( uint16_t update_time )
{
  uxos_timer_t *srch_timer;
  uxos_timer_t *prev_timer;
  uxos_timer_t *free_timer;
/*
  ENTER_CRITICAL_SECTION();  // Hold off interrupts.
  // Update the system time
  uxos_system_clock += update_time;
  EXIT_CRITICAL_SECTION();   // Re-enable interrupts.
*/
  // Look for open timer slot
  if ( ptimer_head != null_ptr )
  {
    // Add it to the end of the timer list
    srch_timer = ptimer_head;
    prev_timer = (void *)null_ptr;

    // Look for open timer slot
    while ( srch_timer )
    {
      free_timer = null_ptr;
     
      ENTER_CRITICAL_SECTION();  // Hold off interrupts.
      
      if (srch_timer->timeout <= update_time)
      {
        srch_timer->timeout = 0;
      }
      else
      {
        srch_timer->timeout = srch_timer->timeout - update_time;
      }
      
      // Check for reloading
      if ( (srch_timer->timeout == 0) && (srch_timer->reload_timeout) && (srch_timer->event_flag) )
      {
        // Notify the task of a timeout
        uxos_set_event( srch_timer->task_id, srch_timer->event_flag );

        // Reload the timer timeout value
        srch_timer->timeout = srch_timer->reload_timeout;
      }
      
      // When timeout or delete (event_flag == 0)
      if ( srch_timer->timeout == 0 || srch_timer->event_flag == 0 )
      {
        // Take out of list
        if ( prev_timer == null_ptr )
          ptimer_head = srch_timer->next;
        else
          prev_timer->next = srch_timer->next;

        // Setup to free memory
        free_timer = srch_timer;

        // Next
        srch_timer = srch_timer->next;
      }
      else
      {
        // Get next
        prev_timer = srch_timer;
        srch_timer = srch_timer->next;
      }
      
      EXIT_CRITICAL_SECTION();   // Re-enable interrupts.
      
      if ( free_timer )
      {
        if ( free_timer->timeout == 0 )
        {
          uxos_set_event( free_timer->task_id, free_timer->event_flag );
        }
        uxos_mem_free((uint8_t*) free_timer,sizeof(uxos_timer_t));
      }
    }
  }
}

