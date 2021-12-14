/*
 * lab7_2.c
 *
 *  Created on: Dec 2, 2021
 *      Author: user
 */


#include "xparameters.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "ZedboardOLED.h"
#include "xgpio.h"

// Parameter definitions
#define INTC_DEVICE_ID 		XPAR_PS7_SCUGIC_0_DEVICE_ID
#define BTNS_DEVICE_ID		XPAR_AXI_GPIO_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define BTN_INT 			XGPIO_IR_CH1_MASK // This is the interrupt mask for channel one
#define DELAY 100000000

XGpio   BTNInst;
XScuGic INTCInst;
static int btn_value;
int cc=0;
int cd=0;
int cu=0;
int cr=0;
int cl=0;

char matrix[100];

//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------
static void BTN_Intr_Handler(void *baseaddr_p);
static int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
static int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr);


//----------------------------------------------------
//  INTERRUPT SERVICE ROUTINE(ISR)
//also know as : INTERRUPT HANDLER FUNCTION
// - called by the buttons interrupt, performs push buttons read
// - OLED displaying
//----------------------------------------------------

void BTN_Intr_Handler(void *InstancePtr)
{

	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) !=
			BTN_INT) {
			return;

	// Disable GPIO interrupts
	XGpio_InterruptDisable(&BTNInst, BTN_INT);
			 }
	clear_OLED(); // clear the OLED
	btn_value = XGpio_DiscreteRead(&BTNInst, 1);
	switch (btn_value){

				//Checking if BTNC was pressed
				case 1:
                    print_message("BTNC WAS PRESSED",1);
                    cc++;
				break;

				//Checking if BTND was pressed
				case 2:
					print_message("BTND WAS PRESSED",1);
					cd++;
				break;

				//Checking if BTNL was pressed
				case 4:
					print_message("BTNL WAS PRESSED",1);
					cl++;
				break;

				//Checking if BTNR was pressed
				case 8:
					print_message("BTNR WAS PRESSED",1);
					cr++;
				break;

				//Checking if BTNU was pressed
				case 16:
					print_message("BTNU WAS PRESSED",1);
					cu++;
				break;

				default:
				break;
					}

	// Acknowledge GPIO interrupts
    (void)XGpio_InterruptClear(&BTNInst, BTN_INT);
    // Enable GPIO interrupts
    XGpio_InterruptEnable(&BTNInst, BTN_INT);

}


//----------------------------------------------------
// MAIN FUNCTION
//----------------------------------------------------
int main(void)

{
  int status;
  unsigned int i;
  char c;
  int n;

  //----------------------------------------------------
  // INITIALIZE THE PERIPHERALS & SET DIRECTIONS OF GPIO
  //----------------------------------------------------
  // Initialize Push Buttons
  status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
  if(status != XST_SUCCESS) return XST_FAILURE;

  // Set all buttons direction to inputs
  XGpio_SetDataDirection(&BTNInst, 1, 0xFF);

  // Initialize interrupt controller
  status = IntcInitFunction(INTC_DEVICE_ID, &BTNInst);
  if(status != XST_SUCCESS) return XST_FAILURE;


  while(1){
	  for (i=0;i<=matrix ;i++){
		  matrix[i]=0;
	  }

      xil_printf("Push Button Center Pressed %d Times\r\n", cc);
      xil_printf("Push Buttons UP Pressed %d Times %d\r\n", cu);
      xil_printf("Push Buttons Down Pressed %d Times %d\r\n", cd);
      xil_printf("Push Buttons Right Pressed %d Times %d\r\n", cr);
      xil_printf("Push Buttons LeftPressed %d Times %d\r\n", cl);
      n=sprintf (matrix, "Push Button Center Pressed %d Times\r\n Push Buttons UP Pressed %d Times %d\r\n Push Buttons Down Pressed %d Times %d\r\n Push Buttons Right Pressed %d Times %d\r\n Push Buttons LeftPressed %d Times %d\r\n", cc,cu,cd,cr,cl);
      print_message(matrix,2);




       for (c = 'A'; c <= 'Z'; c++) {
         clear_OLED();
         print_char(c,0,7);

         for (i=0;i<=DELAY ;i++);
       }





  }
// Never reached on normal execution
  return (0);
}


//----------------------------------------------------
// INTERRUPT SETUP FUNCTIONS
//----------------------------------------------------


int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr)
{
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialization
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call interrupt setup function
	status = InterruptSystemSetup(&INTCInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Register GPIO interrupt handler
	status = XScuGic_Connect(&INTCInst,
					  	  	 INTC_GPIO_INTERRUPT_ID,
					  	  	 (Xil_ExceptionHandler)BTN_Intr_Handler,
					  	  	 (void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Enable GPIO interrupts
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_INTERRUPT_ID);

	return XST_SUCCESS;
}

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
	// Register GIC interrupt handler

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 	 	 	 	 	 XScuGicInstancePtr);
	Xil_ExceptionEnable();


	return XST_SUCCESS;

}


