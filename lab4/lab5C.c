#include "xparameters.h"
#include "xgpio.h"
//====================================================

int main (void) 
{

   XGpio dip, push, leds;
   int i=0, psb_check, dip_check, level = 1, flag, flag_floor=0;

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

	  if(dip_check == 0){
		  XGpio_DiscreteWrite(&leds, 1, 1);
	  	  flag=0;
	  }

	  if(dip_check == 1){
		  XGpio_DiscreteWrite(&leds, 1, 2);
	  	  flag=1;
	  }

	  if(dip_check == 2){
		  XGpio_DiscreteWrite(&leds, 1, 4);
  	  	  flag=2;
	  }

	  if(dip_check == 3){
		  XGpio_DiscreteWrite(&leds, 1, 8);
  	  	  flag=3;
	  }

	  if(dip_check == 4){
		  XGpio_DiscreteWrite(&leds, 1, 16);
  	      flag=4;
	  }

	  if(dip_check == 5){
		  XGpio_DiscreteWrite(&leds, 1, 32);
	  	  flag=5;
	  }

	  if(dip_check == 6){
		  XGpio_DiscreteWrite(&leds, 1, 64);
  	  	  flag=6;
	  }

	  if(dip_check == 7){
		  XGpio_DiscreteWrite(&leds, 1, 128);
	  	  flag=7;
	  }

	  //Button decides direction
	  //Button values are :  up =  10 | mid = 1 | down = 2 , according to psb_check output

	  //Move upwards
	  if(psb_check == 10 ){
		  while(i<7 && flag_floor != flag){			//flag automatically stops elevator
			  flag_floor++;
			  level += level;
			  XGpio_DiscreteWrite(&leds, 1, level);
			  sleep(1);
			  i++;
		  }
	  }

	  //Stop elevator
	  if(psb_check == 1 ){
			  level = level;
			  XGpio_DiscreteWrite(&leds, 1, level);
			  sleep(1);
	  }

	  //Move downwards
	  if(psb_check == 2 ){
		  while(i<7 && flag_floor != flag){
			  flag_floor--;
			  level = level - (level/2);
			  XGpio_DiscreteWrite(&leds, 1, level);
			  sleep(1);
			  i++;
		  }
	  }

	  for (i=0; i<9999999; i++);
   }
}
