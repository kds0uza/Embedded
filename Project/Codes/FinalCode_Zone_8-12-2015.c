//**********************Initialization***********************//
//
// Func:  
// Args:  None
// Retn:  None
//------------------------------------------------------------------------
#include "msp430x22x4.h"
#include "stdint.h"
volatile uint16_t left_sensor;
volatile uint16_t front_sensor;
//volatile uint16_t front_sensor_diff;
volatile uint8_t i, j;
volatile uint8_t zone;
volatile uint8_t prev_z;
volatile uint8_t adj;
volatile uint8_t stop_count;
volatile prop = 1;
volatile uint8_t delay;
static uint8_t left_motor = 100;
static uint8_t right_motor = 228;
static uint8_t cal = 0;

#pragma vector=TIMERA1_VECTOR
__interrupt void IsrCntPulseTACC1 (void) 
//--------------------------------------------------------------------------
// Func:  At TACCR1 IRQ, increm pulse count & toggle built-in Red LED 
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{ 
  switch (__even_in_range(TAIV, 10))  // I.D. source of TA IRQ
  {   
   static uint16_t currEdgeStamp=10; 
   static uint16_t prevEdgeStamp = 0; 
   static uint32_t pulse_width_count=10;
   
   
    case TAIV_TACCR1:                 // handle chnl 1 IRQ
       cal++;
	
        
	if ((cal/4)==0)
	{
	//Capturing the front sensor value	
		
    currEdgeStamp = TACCR1;
    
    if ((P2IN&0x08)==0)
    {
     if (currEdgeStamp <= prevEdgeStamp)         // assuming period is smaller than FFFF
    {
        pulse_width_count = 0xFFFF - prevEdgeStamp + currEdgeStamp; 
    }
    else
        pulse_width_count =  - prevEdgeStamp + currEdgeStamp;
          
    }
    
        if(cal/3)
    front_sensor = pulse_width_count/58;    // in cm
    prevEdgeStamp = currEdgeStamp;
 
	
	}
	
	
	
	//Navigation logic
	
if(cal/4)
{

//Taking Input from both front and left sensors and defining zones and conditions
	
if(front_sensor <= 45 || front_sensor > 660 && left_sensor > 25)
	zone = 0;
else if((front_sensor < 45 || front_sensor > 660) && left_sensor <= 25)
	zone = 6;
else
{
if(left_sensor > 25)
	zone = 1;
if(left_sensor > 100)
	zone = 2;
if(left_sensor > 125)
	zone = 3;
if(left_sensor > 150)
	zone = 4;
if(left_sensor > 200)
	zone = 5;
}

//Navigation Logic

if(stop_count >= 25)				 //STOP bot if trying to navigate near end of the hallway  
{
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
        UCA0TXBUF = 000;
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    	UCA0TXBUF = 000;
}
else
{
if(zone == 0)						 //too close to obstacle, REVERSE & correct
{
	
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
        UCA0TXBUF = 000;
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    	UCA0TXBUF = 000;
	
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    	UCA0TXBUF = 32;					   //1/2 reverse
	
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    	UCA0TXBUF = 160;				   //1/2 reverse
	
		__delay_cycles(4000);
		
		//while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
        //UCA0TXBUF = 000;
		//while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    	//UCA0TXBUF = 000;
		
		if(left_sensor <= 60)				 // if closer to left wall TURN RIGHT
		{
			while(!(IFG2 & UCA0TXIFG)) {};
			UCA0TXBUF = 48;
			while(!(IFG2 & UCA0TXIFG)) {};
			UCA0TXBUF = 144;
			__delay_cycles(400);
			stop_count++;
		}
		else								 // if closer to right wall TURN LEFT
		{
			while(!(IFG2 & UCA0TXIFG)) {};
			UCA0TXBUF = 16;
			while(!(IFG2 & UCA0TXIFG)) {};
			UCA0TXBUF = 176;
			__delay_cycles(400);
			stop_count++;
		}
		
		
		
}
if(zone == 5)						 //new hallway detected, EXTREME LEFT
{
	  left_motor = 100;
	  right_motor = 228;
	  
	  //turn left
	  
	
	  
	  for(i=0;i<10;i++)
	  {
	  	  left_motor = 1;                      //INCREMENT LEFT MOTOR
	       while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
           UCA0TXBUF = left_motor;
		   
		   right_motor = 255;                      //INCREMENT Right MOTOR
	       while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
           UCA0TXBUF = right_motor;
		   
		  
	  }
	  __delay_cycles(400);
	  stop_count = 0;
   
}
if(zone == 6)						 //wall detected, REVERSE
{
	
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
        UCA0TXBUF = 000;
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    	UCA0TXBUF = 000;
	
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    	UCA0TXBUF = 32;					   //1/2 reverse
	
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    	UCA0TXBUF = 160;				   //1/2 reverse
	
		__delay_cycles(5000);
		stop_count++;
} 
if(front_sensor <= 150)				 //obstacle detected in Zones 2 or 3
{
		

	if(zone == 2)					 //if obstacle is detected in Zone 2
	{
		
	        adj =0;
			delay = 1;
	
			if(front_sensor <= 120)
				delay = 2;
			else if(front_sensor < 90)
				delay = 3;
			else if(front_sensor < 60)
				delay = 4;
			//else if(front_sensor < 40)
			//	delay = 5;
			//else if(front_sensor < 40)
			//	delay = 5;
	
			prop = adj + 5 + ((150 - front_sensor)/21);			  //(10-5)/(150-45);
	
	
    		for(j = 0;j <= delay; j++)
			{
				left_motor = 100;
	    		right_motor = 228;
	
				for(i=0;i <= prop; i++)					  //TURN LEFT
				{
	     			if(left_motor >= 75 && left_motor < 127)
      				{
         				left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	     				while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         				UCA0TXBUF = left_motor; 
     
      				}
         			if(right_motor > 203 && right_motor <= 255)
      				{
         				right_motor = right_motor++;                      //DECREMENT right MOTOR
         				while ( !(IFG2 & UCA0TXIFG)){};
						UCA0TXBUF = right_motor;
	  				}
    			}
			}
	
	}
	if(zone == 3)					 // if obstacle detected in Zone 3
	{
		
		adj =0;
		delay = 1;
	
		if(front_sensor <= 120)
			delay = 2;
		else if(front_sensor < 90)
			delay = 3;
		else if(front_sensor < 60)
			delay = 4;
		//else if(front_sensor < 40)
		//	delay = 5;
		//else if(front_sensor < 40)
		//	delay = 5;
	
		prop = adj + 5 + ((150 - front_sensor)/21);			  //(10-5)/(150-45);
	
	
    	for(j = 0;j <= delay; j++)
		{
			left_motor = 100;
	    	right_motor = 228;
	
			for(i=0;i <= prop; i++)					  //TURN LEFT
			{
	     		if(left_motor >= 75 && left_motor < 127)
      			{
         			left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	     			while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         			UCA0TXBUF = left_motor; 
      			}
         		if(right_motor > 203 && right_motor <= 255)
      			{
         			right_motor = right_motor++;                      //DECREMENT right MOTOR
         			while ( !(IFG2 & UCA0TXIFG)){};
		 			UCA0TXBUF = right_motor;
	  			}
    		}
		}
	
	}
}
else								 //if no obstacle detected in Zones 2 or 3
{
if(zone == 2)						 //Minor RIGHT
{
	adj =0;
	delay = 1;
	
	if(prev_z == 3)
		adj = 2;
	
	prop = adj + 2 + ((125 - left_sensor)/20);			  //(8-3)/(125-25);
	
	
    for(j = 0;j <= delay; j++)
	{
		left_motor = 100;
	    right_motor = 228;
	
	for(i=0;i <= prop; i++)					  //TURN RIGHT
	{
	     if(left_motor >= 75 && left_motor < 127)
      {
         left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = left_motor; 
     
      }
         if(right_motor > 203 && right_motor <= 255)
      {
         right_motor = right_motor--;                      //DECREMENT right MOTOR
         while ( !(IFG2 & UCA0TXIFG)){};
		 UCA0TXBUF = right_motor;
	  }
	  
    }
	}
	
	
	if(prev_z ==0)
	{
		stop_count++;
	}
	
	prev_z = 2;

}
if(zone == 3)						 //Minor LEFT
{
	adj =0;
	delay = 1;
	
//if(front_sensor <=150)					//obstacle in front or right of bot, TURN LEFT

	//__delay_cycles(100);

	
	if(prev_z == 2)
		adj = 2;
	
	
	for(j = 0; j <= delay; j++)
	{
		left_motor = 100;
	    right_motor = 228;
	
	prop = adj + 3 + ((left_sensor - 125)/20);			  //(8-3)/(125-25);
	
	for(i=0;i <= prop; i++)					  //TURN LEFT
	{
	     if(left_motor >= 75 && left_motor < 127)
      {
         left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = left_motor; 
     
      }
         if(right_motor > 203 && right_motor <= 255)
      {
         right_motor = right_motor++;                      //DECREMENT right MOTOR
         while ( !(IFG2 & UCA0TXIFG)){};
		 UCA0TXBUF = right_motor;
	  }
	  
    }
	}
	prev_z = 3;
	stop_count = 0;
}
}
if(zone == 1)                        //TURN HARD RIGHT
{
	adj = 0;
	
	left_motor = 100;
	right_motor = 228;
	
	if(prev_z = 2)
		adj = 5;
	
	prop = adj + 3 + ((100 - left_sensor)*10)/75;			  //(13-3)/(100-25);
	
	
	for(i=0;i <= prop; i++)					  //TURN RIGHT
	{
	     if(left_motor >= 75 && left_motor < 127)
      {
         left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = left_motor; 
     
      }
         if(right_motor > 203 && right_motor <= 255)
      {
         right_motor = right_motor--;                      //DECREMENT right MOTOR
         while ( !(IFG2 & UCA0TXIFG)){};
		 UCA0TXBUF = right_motor;
	  }
	   if (front_sensor < 50)
		   __delay_cycles(10);
    }
	
	if(prev_z == 0)
	stop_count++;
	
	prev_z = 1;
}
if(zone == 4)						 //TURN HARD LEFT
{
	adj = 0;
	
	left_motor = 100;
	right_motor = 228;
	
	if(prev_z == 3)
		adj = 5;
	
	prop = adj + 3 + ((left_sensor - 125)*10)/75;			  //(13-3)/(200-125);
	
	for(i=0;i <= prop; i++)					  //TURN LEFT
	{
	     if(left_motor >= 75 && left_motor < 127)
      {
         left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = left_motor; 
     
      }
         if(right_motor > 203 && right_motor <= 255)
      {
         right_motor = right_motor++;                      //DECREMENT right MOTOR
         while ( !(IFG2 & UCA0TXIFG)){};
		 UCA0TXBUF = right_motor;
	  }
	  
    }
	
	prev_z = 4;
	stop_count = 0;
}  

}
prev_z = zone;
cal=0;
}
   

    TACCTL1 &= ~0x01; 
    break;
    
    case TAIV_TACCR2:                 // chnl 2 IRQ
    case TAIV_TAIFG:                   // ignore TAR rollover IRQ
    default:                          // ignore everything else
  }

  //TACCTL1 &= ~0x01;                   //clear interrupt flag
  }

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{  
   static uint16_t currEdgeStamp_2=10; 
   static uint16_t prevEdgeStamp_2 = 0; 
   static uint32_t pulse_width_count_2=10; 
    
    //case TAIV_TACCR1:                 // handle chnl 1 IRQ
   
   if ((cal/3)==0)
	{
   currEdgeStamp_2 = TACCR0;
    
    if ((P2IN&0x04)==0)
    {
     if (currEdgeStamp_2 <= prevEdgeStamp_2)         // assuming period is smaller than FFFF
    {
        pulse_width_count_2 = 0xFFFF - prevEdgeStamp_2 + currEdgeStamp_2; 
    }
    else
        pulse_width_count_2 =  - prevEdgeStamp_2 + currEdgeStamp_2;
          
    }
    
    if(cal/2)
    left_sensor= pulse_width_count_2/58;  // in cm
    prevEdgeStamp_2 = currEdgeStamp_2;
        }
    TACCTL0 &= ~0x01;   
     
}


void InitPorts (void)
//------------------------------------------------------------------------
// Func:  Initialize the ports for I/O on TA1 Capture
// Args:  None
// Retn:  None
//------------------------------------------------------------------------
{
   P2DIR &= ~0x08;                      // P2.3 = Input mode
   P2SEL |= 0x08;                       // Timer A2 select
   P2DIR &= ~0x04;                      // P2.4 = Input mode
   P2SEL |= 0x04;                       // Timer A2 select
   P4DIR |= 0x10;                      // P4.4 = Output mode
   P4SEL |= 0x10;                      // P4.4 = TB1 = TB compare OUT1
   P3SEL = 0x30;                         // P3.4,5 = USCI_A0 TXD/RXD 
} 



void main( void )
//------------------------------------------------------------------------
// Func:  Initialize & start timer A to generate a PWM waveform & 
//        output it to P2.3/TA1 without further CPU intervention.
// Args:  None
// Retn:  None
//------------------------------------------------------------------------
{ WDTCTL = WDTPW + WDTHOLD;           // Stop WDT
  
   InitPorts();    
  
  BCSCTL1 = CALBC1_1MHZ;                // DCO = 1 MHz
  DCOCTL  = CALDCO_1MHZ;                // DCO = 1 MHz
  UCA0CTL1 |= UCSSEL_2;                 // UART use SMCLK
  
  UCA0MCTL = UCBRS0;                    // Map 1MHz -> 9600 (Tbl 15-4)
  UCA0BR0  = 104;                       // Map 1MHz -> 9600 (Tbl 15-4)
  UCA0BR1  = 0;                         // Map 1MHz -> 9600 (Tbl 15-4)

  UCA0CTL1 &= ~UCSWRST;                 // Enable USCI state mach
  
  while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
  UCA0TXBUF = left_motor;                     // Init robot to stopped state
  while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
  UCA0TXBUF = right_motor;                     // Init robot to stopped state
  
  TBCCR0 = 40250-1;               // 35 ms Set frequency of PWM (UP mode)
  TBCCR1 = 10;           // PW = 10us Set pulse wid of PWM (UP mode) initialize to 0 degree
   
  TBCTL   = TBSSEL_2 | ID_0 | MC_1 ;   // Sel. SMCLK | div by 1 | UP Mode | enable TAIE
  TBCCTL1 = CCIS0 | OUTMOD_6;         // Chnl-1 Inp=CCI1B | OUTMOD=Tgl/Rst
                                      // Note:  Compare mode is default
  TACTL   = TASSEL_2 | ID_0 | MC_2;          // SMCLK | Div by 1 | Contin Mode
  TACCTL1 = CM0 |CM1| CCIS0 | CAP | SCS |SCCI| CCIE;  // Ris Edge | inp = CCI1B |  
                                             // Capture | Sync Cap | Enab IRQ    CM1 = rising mode bit 01
  TACCTL0 = CM0 |CM1| CCIS0 | CAP | SCS |SCCI| CCIE; 

  _BIS_SR(LPM1_bits + GIE);           // Enter LPM1 w/ IRQs enab

  // The CPU is now asleep in LPM3 with general IRQs enabled (GIE=1).
  // The CPU never wakes up because all the work is done by peripheral
  // hardware (Timer A Compare Channel 1 signal OUT1).
}
