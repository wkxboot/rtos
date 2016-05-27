/*
 * uxos_mem.c
 *
 * Created: 2014/11/20 10:19:47
 *  Author: wkxboot
 */ 

#include <avr//io.h>
#include "std_def.h"
#include "uxos_mem.h"


uint8_t *pmem_head=null_ptr;

void uxos_mem_init()
{
	uxos_mem_set(0,(uint8_t*)UXOS_MEM_START_ADDR,UXOS_MEM_SIZE);
}

void *uxos_mem_alloc(uint16_t mem_size)
{
	
	uint16_t offset=0;
	uint16_t bytes_count=0;
	
	if (mem_size <=0 ||mem_size >= UXOS_MEM_SIZE)
	{
		return null_ptr;
	}
	if (pmem_head==null_ptr)
	{
	  pmem_head=(uint8_t*)UXOS_MEM_START_ADDR;
	}

	while (offset< UXOS_MEM_SIZE)
	{		
	 if(*(pmem_head+offset)==0)
	 {
		 bytes_count++;
		 offset++;
	  if (bytes_count==mem_size+1)//one more  byte indicate bytes count;
	   {
		 *(pmem_head+offset-bytes_count)=mem_size;
		 return (pmem_head+offset-mem_size);
		}
	     							
	  }
	  else
	  {
		bytes_count=0;
		offset+=*(pmem_head+offset)+1;
	  }	
	}
	 return null_ptr;
   }
 
void uxos_mem_free(void *paddr,uint16_t len)
{
	uint8_t *pdata=(uint8_t*)paddr;
	uxos_mem_set(0,pdata-1,len+1);
}
void uxos_mem_set(uint8_t set_value,void *paddr,uint16_t len)
{
	uint8_t *pdata=(uint8_t*)paddr;
    while(len--)
	{
		*pdata++=set_value;
	}
}