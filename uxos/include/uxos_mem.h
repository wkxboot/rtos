/*
 * uxos_mem.h
 *
 * Created: 2014/11/20 10:20:11
 *  Author: wkxboot
 */ 

#ifndef __UXOS_MEM_H__
#define __UXOS_MEM_H__

#define UXOS_MEM_SIZE         800u  // bytes
#define UXOS_MEM_START_ADDR   0xc0  //0x0c-0x60=0x60=96


void uxos_mem_init();
void *uxos_mem_alloc(uint16_t mem_size);
void uxos_mem_free(void *paddr,uint16_t len);
void uxos_mem_set(uint8_t set_value,void *paddr,uint16_t len);

#endif
