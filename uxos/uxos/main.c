/*
 * main.c
 *
 * Created: 2014/11/20 14:51:53
 *  Author: wkxboot
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_def.h"
#include "uxos.h"
#include "uxos_msg.h"
#include "uxos_mem.h"
#include "uxos_timer.h"

int main()
{

uxos_init();
uxos_start();

 return 0;
}