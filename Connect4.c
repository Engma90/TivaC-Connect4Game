// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground


#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"
#include "PORTF.h"
#include "UART.h"
#include "Bitmaps.h"

#include "CheckWin.h"
#include <stdlib.h>



void draw(void);
void choose(void);
void identifyFirstTurn(void);
void turn(void);
void drop(void);
int random_number(int);
void WaitForInterrupt(void); 
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long TimerCount;
unsigned long Semaphore;



int arrowX = 18;
int chFlag=0;
int mode=1;
unsigned char r=0;
int player=1; //toggle with turn() function
int whoAmI=1; //Default value 1 may be change in multiplayer mode
int whoWon=0;
int Barr[6][7];
unsigned char RecVal;
int Rcol;

int main(void){
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Nokia5110_Init();
	PortF_Init();
	UART_Init();              // initialize UART
	EnableInterrupts();  // The grader uses interrupts
	Timer2_Init(1 * 10000000);  // Timer is used to seed rand()

	
  while(1){
		// Game doesn't start
	if(!chFlag){
	if(isSw1Pressed()){
			mode++;
				if(mode==3){
						mode=1;
				}
		}	
		else if(isSw2Pressed()){
			chFlag=1;
				//new
			srand(TimerCount);
			TIMER2_CTL_R = 0x00000000;    // stop seed timer
			if(mode==2){
			 identifyFirstTurn();
			}
		}
		if(!chFlag)
		choose();
	}
	// Someone has win 
	else if(whoWon != 0 && whoWon != 3 ){
			draw();

			Nokia5110_SetCursor(0,0);
			Nokia5110_OutString(whoWon == whoAmI ? "You Won" : "You lost");
		
			Delay100ms(10);
	
	}		//No one won & Bord is Full
		else if(whoWon == 3){
			draw();

			Nokia5110_SetCursor(0,0);
			Nokia5110_OutString("No one Won");
		
			Delay100ms(10);
	
	}
	
	else{ // Game in Progress
		
		if(player==whoAmI)
		{
			
				GPIO_PORTF_DATA_R = 0x08;
				if(isSw1Pressed()){
					if(arrowX==60){
					arrowX =11;
					}
					arrowX += 7;
				}	
				else if(isSw2Pressed()){
					drop();
				}
				
		}
		
		else {
			
			GPIO_PORTF_DATA_R = 0x00;
				if(mode==1){
				arrowX=18+(random_number(6) * 7);
				drop();
				}
				
				//new
				else{
					
					Timer2_Init(1 * 80000000);//start Time out Timer with 1 sec (15 sec in handler loop).
					RecVal=UART_InChar();
					TIMER2_CTL_R = 0x00000000; //if onother kit respond stop timer
					if(end_receve_flag){ //15 sec without response
						turn();
						whoWon=whoAmI;
						continue;
					}
					Rcol= RecVal - '0';
					if(Rcol>=0 && Rcol <=6){
						arrowX=18+(Rcol * 7);
						drop();
					}
				}
	}
		
		draw();

	}
		Delay100ms(2);
  }

}


void drop(){
	int eflag=0,result;
	int i=0,col=(arrowX-18)/7;
	for(i=0;i<6;i++){
		if(Barr[i][col]==0){
			Barr[i][col]=player;
			eflag=1;
			if(mode==2 && player==whoAmI){ //new
					UART_OutChar('0'+col);
			}
			result = winner(player,Barr,i,col);
			if(result==1)
				whoWon = player;
			else if(result==3)
				whoWon = result; //no one won
			break;
		}
	}
	if(eflag)
	turn();
	
}
void turn(){
	player=-player;
}


void choose(){
	
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(0,0);
	Nokia5110_OutString("Choose Mode:");
	Nokia5110_SetCursor(1,2);
	Nokia5110_OutString("Single");
	Nokia5110_SetCursor(1,3);
	Nokia5110_OutString("Multi");
	Nokia5110_SetCursor(0,mode+1);
	Nokia5110_OutChar('>');
		Delay100ms(10);
}


//new
void identifyFirstTurn(){
	unsigned char MyChar,OtherChar;
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(0,0);
	Nokia5110_OutString("Numbers");
	Nokia5110_SetCursor(1,2);
	Nokia5110_OutString("ME : ");
	

	MyChar=(char)random_number(255);

	UART_OutChar(MyChar);

	Nokia5110_OutUDec(MyChar);
		Nokia5110_SetCursor(1,3);
	Nokia5110_OutString("You: ");
	
	OtherChar=UART_InChar();
	Nokia5110_OutUDec(OtherChar);
	Delay100ms(500);
	
	if(MyChar > OtherChar){
		whoAmI=1;
	}
	else if(MyChar <= OtherChar){
		whoAmI=-1;
	}
	else{
		whoAmI=-1;
	}
	//must call here in case of whoAmI=-1 (main loop will not call draw())
	draw();
}


void draw(){
	int i;int j;
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(0, 47, Board, 0);
	
	
	
	for(i=0;i<6;i++){
		for(j=0;j<7;j++){
		if(Barr[i][j]==1){
			Nokia5110_PrintBMP((18+(7*(j))), (46-(7*i)), x, 0);
			
		}
		else if(Barr[i][j]==-1){
			Nokia5110_PrintBMP((18+(7*(j))), (46-(7*i)), o, 0);
			
		}
	}
	}
	
	
	Nokia5110_PrintBMP(arrowX, 4, arrow, 0);
	Nokia5110_DisplayBuffer();

}

int random_number(int max_num)
{
    int result = 0;
    result = (rand() % (max_num +1));
    return result;
}




// You can use this timer only if you learn how it works
void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}
void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCount++;
	if(TimerCount == 15 && chFlag){ //To prevent disable timer when determining seed 
  Semaphore = 1; // trigger
	end_receve_flag=1;
	GPIO_PORTF_DATA_R = 0x02;
	TIMER2_CTL_R = 0x00000000;    // 10) Disable timer2A //new
	}
}
void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 72724;
    while(time){
	  	time--;
    }
    count--;
  }
}
