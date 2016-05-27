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
	uint8_t task_id;//������Ϣ����ID
	uint8_t event_flag;//��Ϣ�����е��¼�����
	uint8_t data_arry[4];//��Ϣ�����е�����
	void *next;//��һ����Ϣ
}uxos_msg_t;

void uxos_send_msg(uint8_t task_id,uxos_msg_t *pmsg,uint8_t sys_event);
void uxos_add_msg(uxos_msg_t *pmsg);
void uxos_delete_msg(uxos_msg_t *pmsg);
uxos_msg_t *uxos_search_msg(uint8_t task_id );

#endif /* __UXOS_MSG_H__ */