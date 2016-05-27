/*
 * uxos_msg.h
 *
 * Created: 2014/11/20 10:20:31
 *  Author: wkxboot
 */ 


#ifndef __UXOS_MSG_H__
#define __UXOS_MSG_H__

typedef struct
{
	uint8_t task_id;//接收消息任务ID
	uint8_t event_flag;//消息内容中的事件类型
	uint8_t data_arry[4];//消息内容中的数据
	void *next;//下一个消息
}uxos_msg_t;

void uxos_send_msg(uint8_t task_id,uxos_msg_t *pmsg,uint8_t sys_event);
void uxos_add_msg(uxos_msg_t *pmsg);
void uxos_delete_msg(uxos_msg_t *pmsg);
uxos_msg_t *uxos_search_msg(uint8_t task_id );

#endif /* __UXOS_MSG_H__ */