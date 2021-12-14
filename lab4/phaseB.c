#include "xparameters.h"
#include "xgpio.h"
//#include "cache.h"
//====================================================

#define N 100000


int main (void) 
{

   XGpio dip, push, leds;
   int i=0, psb_check, dip_check, level = 1, flag, flag_floor=1;

   xil_printf("-- Start of the Program --\r\n");
 
   XGpio_Initialize(&leds, XPAR_LED_IP_DEVICE_ID); // Modify this
   XGpio_SetDataDirection(&leds, 1, 0x00000000);

   XGpio_Initialize(&dip, XPAR_SWITCHES_DEVICE_ID); // Modify this
   XGpio_SetDataDirection(&dip, 1, 0xffffffff);
	
   XGpio_Initialize(&push, XPAR_BUTTONS_DEVICE_ID); // Modify this
   XGpio_SetDataDirection(&push, 1, 0xffffffff);
	

   while (1)
   {
	  psb_check = XGpio_DiscreteRead(&push, 1);
	  xil_printf("Push Buttons Status %x\r\n", psb_check);
	  dip_check = XGpio_DiscreteRead(&dip, 1);
	  xil_printf("DIP Switch Status %x\r\n", dip_check);

	  //leds=2^2 --> 8(right end)=128

	  XGpio_DiscreteWrite(&leds, 1, 1);		//floor level -- 1

	  //DIP indicates floor (led)

	  if(dip_check == 1){
		  XGpio_DiscreteWrite(&leds, 1, 1);
	  	  flag=1;
	  }

	  if(dip_check == 2){
		  XGpio_DiscreteWrite(&leds, 1, 2);
	  	  flag=2;
	  }

	  if(dip_check == 4){
		  XGpio_DiscreteWrite(&leds, 1, 4);
  	  	  flag=3;
	  }

	  if(dip_check == 8){
		  XGpio_DiscreteWrite(&leds, 1, 8);
  	  	  flag=4;
	  }

	  if(dip_check == 16){
		  XGpio_DiscreteWrite(&leds, 1, 16);
  	      flag=5;
	  }

	  if(dip_check == 32){
		  XGpio_DiscreteWrite(&leds, 1, 32);
	  	  flag=6;
	  }

	  if(dip_check == 64){
		  XGpio_DiscreteWrite(&leds, 1, 64);
  	  	  flag=7;
	  }

	  if(dip_check == 128){
		  XGpio_DiscreteWrite(&leds, 1, 128);
	  	  flag=8;
	  }

	  //Button decides direction
	  //Button values are :  up =  16 | mid = 1 | down =  , according to psb_check output

	  //Move upwards
	  while(psb_check == 16){
		      level += level;
			  XGpio_DiscreteWrite(&leds, 1, level);
			  sleep(1);

			  if(level == dip_check){
				  break;
			  }
  	   }

	  //Stop elevator

	  //Move downwards
	  while(psb_check == 4){
		      level = level - (level/2);
			  XGpio_DiscreteWrite(&leds, 1, level);
			  sleep(1);

			  if(level == dip_check)
				  break;

   }
}

