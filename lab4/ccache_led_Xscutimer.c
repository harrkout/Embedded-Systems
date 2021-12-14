#include "xparameters.h"
#include "xgpio.h"
#include "xscutimer.h"

#define N 10000000

XScuTimer Timer;		

void vectoradd(int arrA[N],int arrB[N],int arrC[N]);
void XScuTimer_LoadTimer(InstancePtr, Value);

int main(){

	void Xil_DCacheDisable(void);
	void Xil_ICacheDisable(void);


	int i;
        //XScuTimer *InstancePtr = &Timer;
        //u32 XScuTimer_GetCounterValue(XScuTimer *InstancePtr)
	XScuTimer_ReadReg((InstancePtr)->Config.BaseAddr, XSCUTIMER_COUNTER_OFFSET);




	   XGpio leds;

	   xil_printf("-- Start of the Program --\r\n");

	   XGpio_Initialize(&leds, XPAR_LED_IP_DEVICE_ID);
	   XGpio_SetDataDirection(&leds, 1, 0x00000000);

		for(i=0;i<N;i++){
			XGpio_DiscreteWrite(&leds, 1, 8);
			for(i=0;i<N;i++){
				vectoradd;
			}
			for(i=0;i<N;i++){
				XGpio_DiscreteWrite(&leds, 1, 1);
				vectoradd;
			}
			for(i=0;i<N;i++){
				XGpio_DiscreteWrite(&leds, 1, 8);
				vectoradd;

			}
		}
}

void vectoradd(int arrA[N],int arrB[N],int arrC[N]){

	int i;
    arrC[i] = arrA[i] + arrB[i];
}

//1000000  optimization_00   23504	   1200	  22584	  47288	   b8b8	lab4.elf

//1000     optimization_00   23504	   1200	  22584	  47288	   b8b8	lab4.elf
//1000     optimization_03   23252	   1200	  22584	  47036	   b7bc	lab4.elf

//Cache disabled
//1000000  optimization_03   23252	   1200	  22584	  47036	   b7bc	lab4.elf
//1000000  optimization_03   23504	   1200	  22584	  47288	   b8b8	lab4.elf

