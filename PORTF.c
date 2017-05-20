#include "PORTF.h"

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
int sw1,sw2,sw1pressFlag,sw2pressFlag;

int isSw1Pressed(void){
	sw1pressFlag = 0;
	sw1=GPIO_PORTF_DATA_R &0x10;
	sw1pressFlag= (sw1 == 0x00 ? 1 : 0 );
	while(!sw1){sw1=GPIO_PORTF_DATA_R &0x10;}
	return sw1pressFlag;
}
int isSw2Pressed(void){
	sw2pressFlag = 0;
	sw2=GPIO_PORTF_DATA_R &0x01;
	sw2pressFlag= (sw2 == 0x00 ? 1 : 0 );
	while(!sw2){sw2=GPIO_PORTF_DATA_R &0x01;}
	return sw2pressFlag;
}
