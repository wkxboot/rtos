/*
 * uxos.h
 *
 * Created: 2014/11/20 22:09:16
 *  Author: asus
 */ 


#ifndef __UXOS_H__
#define __UXOS_H__

#define   ENTER_CRITICAL_SECTION()   cli()
#define   EXIT_CRITICAL_SECTION()    sei()

#define   UXOS_DISABLE_GLOBLE_INT()  cli()
#define   UXOS_ENABLE_GLOBLE_INT()   sei()

#define   SYS_EVENT_MSG               0x80
#define   SYS_EVENT_ADC_INT           0x40
#define   SYS_EVENT_RX_INT            0x20



typedef uint8_t (*ptask)(uint8_t task_event);
void uxos_set_event(uint8_t task_id,uint8_t set_event);
void uxos_init(void);
void uxos_start();

#endif /* __UXOS_H__ */