/*
 * lab61.c
 *
 *  Created on: Dec 5, 2015
 *      Author: kornaros
 */


#include "xparameters.h"
#include "xgpio.h"
#include "led_ip.h"
// Include scutimer header file
#include "xscutimer.h"
//====================================================
XScuTimer Timer;		/* Cortex A9 SCU Private Timer Instance */

#define ONE_TENTH 32500000 // half of the CPU clock speed/10

int main (void)
{

   XGpio dip, push;
   int psb_check, dip_check, dip_check_prev, count, Status;

   // PS Timer related definitions
 //  XScuTimer_Config *ConfigPtr;
 //  XScuTimer *TimerInstancePtr = &Timer;

   xil_printf("-- A1: hello ! --\r\n");
   unsigned int *shared_v = 0x40000000;
   *shared_v = 0;

   return ;
}
