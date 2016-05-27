/*
 * uxos_msg.c
 *
 * Created: 2014/11/20 10:19:33
 *  Author: wkxboot
 */ 
#include <avr/io.h>
#include "std_def.h"
#include "uxos.h"
#include "uxos_msg.h"
#include "uxos_mem.h"
#include "uxos_timer.h"


uxos_msg_t *pmsg_head=null_ptr;


void uxos_send_msg(uint8_t task_id,uxos_msg_t *pmsg,uint8_t sys_event)
{
    uxos_add_msg(pmsg);
    uxos_set_event(task_id,sys_event);
}

void uxos_add_msg(uxos_msg_t *pmsg)
{
	uxos_msg_t *psrch_msg;
	if (pmsg_head==null_ptr)
	{
		pmsg_head=pmsg;
	}
	else
	{
		psrch_msg=pmsg_head;
		while(psrch_msg->next)
		psrch_msg=psrch_msg->next;
		psrch_msg->next=pmsg;
	}
}
uxos_msg_t *uxos_search_msg(uint8_t task_id )
{
	uxos_msg_t *psrch_msg;
	psrch_msg=pmsg_head;
	
	while(psrch_msg)
	{
		if (psrch_msg->task_id==task_id)
		{
			break;
		}
	 psrch_msg=psrch_msg->next;
	}	
	 return psrch_msg;
}

void uxos_delete_msg(uxos_msg_t *pmsg)
{
  uxos_msg_t *pprev_msg=null_ptr;
  uxos_msg_t *psrch_msg=pmsg_head;

	while(psrch_msg)
	{	
	if (psrch_msg==pmsg)
	{
		if (pprev_msg==null_ptr)
		{
			pmsg_head=pmsg->next;
			psrch_msg=pmsg_head;
		}
		else
		{
	    pprev_msg->next=pmsg->next;
		}
	    uxos_mem_free(pmsg,sizeof(uxos_msg_t));
	 }
	 else
	 {
		pprev_msg=psrch_msg;
		psrch_msg=psrch_msg->next; 
     } 
	 
  }
  
}