#include "msp430x22x4.h"
#include "stdint.h"
volatile uint16_t left_sensor;
volatile uint16_t front_sensor;
volatile uint8_t i;
static uint8_t left_motor = 100;
static uint8_t right_motor = 228;


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
    
    front_sensor = pulse_width_count/58;    // in cm
    prevEdgeStamp = currEdgeStamp;
 
	
	
	
	
	//Navigation logic
	
if(front_sensor > 70 && left_sensor > 40 && left_sensor < 100)   //moving forward when there is no obstacle or wall ahead
{
	left_motor = 100;
	right_motor = 228;
	
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty
	UCA0TXBUF = left_motor;
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty
	UCA0TXBUF = right_motor;
	
	
}	
else if(front_sensor <= 70 && left_sensor <= 40)			// if obstacle ahead and bot is too close to left wall
{
	//turn right
	left_motor = 100;
	right_motor = 228;
	
	for(i=0;i<10;i++)
	{
		
	if(left_motor >= 85 && left_motor < 127)
   {
	   
		   
           left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	       while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
           UCA0TXBUF = left_motor; 
	   
    }
   if(right_motor > 213 && right_motor <= 255)
   {
	     
            right_motor = right_motor--;                      //DECREMENT right MOTOR
            while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
            UCA0TXBUF = right_motor;
		 
   }
	}
   //__delay_cycles(1000);

   /*
   //readjustment of speed after turn
    //right_motor = 255 - (127 - left_motor);
   left_motor = 127 - (255 - right_motor);
   while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
   UCA0TXBUF = right_motor;
   while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
   UCA0TXBUF = left_motor; 
    
   __delay_cycles(300);
	*/
}
else if(front_sensor <= 70 && left_sensor >= 100)		  //if obtstacle ahead and bot too close to right wall
{
	//turn left
	left_motor = 100;
	right_motor = 228;
	for(i=0;i<10;i++)
	{
		
	   if(left_motor >= 85 && left_motor < 127)
       {
	   
		   
           left_motor = left_motor--;                      //DECREMENT LEFT MOTOR
	       while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
           UCA0TXBUF = left_motor; 
        }
       if(right_motor > 213 && right_motor <= 255)
       {
	     
            right_motor = right_motor++;                      //INCREMENT right MOTOR
            while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
            UCA0TXBUF = right_motor;
		 
        }
	}
   //__delay_cycles(1000);

   /*
   //readjustment of speed after turn
    //right_motor = 255 - (127 - left_motor);
   left_motor = 127 - (255 - right_motor);
   while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
   UCA0TXBUF = right_motor;
   while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
   UCA0TXBUF = left_motor; 
    
   __delay_cycles(300);
	*/
}
else if(front_sensor < 70 && left_sensor > 40 && left_sensor < 100) // if obstacle ahead
{
  left_motor = 100;
	right_motor = 228;	
	
 if(left_sensor < 100)                           //if closer to left wall then Turn Right
 {
	for(i=0;i<10;i++)
	{
		
       if(left_motor >= 85 && left_motor < 127)
       {
          left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	      while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
          UCA0TXBUF = left_motor; 
       }
       
	   if(right_motor > 213 && right_motor <= 255)
       {
          right_motor = right_motor--;                      //DECREMENT right MOTOR
          while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
          UCA0TXBUF = right_motor;
       }
    }
   /*
   __delay_cycles(1000);

   
   //readjustment of speed after turn
    //right_motor = 255 - (127 - left_motor);
   left_motor = 127 - (255 - right_motor);
   while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
   UCA0TXBUF = right_motor;
   while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
   UCA0TXBUF = left_motor; 
    
   __delay_cycles(300);

         while ( !(IFG2 & UCA0TXIFG)) {}; // Confirm that Tx Buff is empty                   
         UCA0TXBUF = 96;                     // Robot in 1/2 forward state
         while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
         UCA0TXBUF = 224 ;                   // Robot in 1/2 forward state
	 */
}


  if(left_sensor >= 100)                      //if closer to right wall then Turn Left
  {
    	for(i=0;i<10;i++)
	{
		
       if(left_motor >= 85 && left_motor < 127)
       {
          left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	      while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
          UCA0TXBUF = left_motor; 
       }
       
	   if(right_motor > 213 && right_motor <= 255)
       {
          right_motor = right_motor--;                      //DECREMENT right MOTOR
          while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
          UCA0TXBUF = right_motor;
       }
    }
	/*
right_motor = 255 - (127 - left_motor);
//left_motor = 127 - (255 - right_motor);
while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
UCA0TXBUF = left_motor;
while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
UCA0TXBUF = right_motor;

__delay_cycles(300);
 */
}


} 
else if(front_sensor > 70 && left_sensor < 40)			   //if all clear ahead but bot is close to left wall
{
	//turn right
	left_motor = 100;
	right_motor = 228;
	
	  if(left_motor >= 85 && left_motor < 127)
   {
     left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	 while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
     UCA0TXBUF = left_motor; 
     
    }
   if(right_motor > 213 && right_motor <= 255)
   {
     right_motor = right_motor--;                      //DECREMENT right MOTOR
     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
     UCA0TXBUF = right_motor;
    
   }
   //__delay_cycles(10);
   
   //go straight
     if(left_motor >= 85 && left_motor < 127)
   {
     left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	 while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
     UCA0TXBUF = left_motor; 
     
    }
   if(right_motor > 213 && right_motor <= 255)
   {
     right_motor = right_motor++;                      //DECREMENT right MOTOR
     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
     UCA0TXBUF = right_motor;
    
   }
   //__delay_cycles(10);
}
else if(front_sensor > 70 && left_sensor > 100)			   //if all clear ahead but bot is close to right wall OR new pathway on the left 
{
	left_motor = 100;
	  right_motor = 228;
	 if(left_sensor > 180)					  //if new pathway detected on the left
   {
	  
	  for(i=0;i<10;i++)
	  {
	  	  left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	       while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
           UCA0TXBUF = left_motor;
		   
		   right_motor = right_motor++;                      //INCREMENT Right MOTOR
	       while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
           UCA0TXBUF = right_motor;
	  }
	  
   }
	
	
	//turn left
	  if(left_motor >= 85 && left_motor < 127)
   {
     left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	 while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
     UCA0TXBUF = left_motor; 
     
    }
   if(right_motor > 213 && right_motor <= 255)
   {
     right_motor = right_motor++;                      //DECREMENT right MOTOR
     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
     UCA0TXBUF = right_motor;
    
   }
   //__delay_cycles(10);
   
   //go straight
     if(left_motor >= 85 && left_motor < 127)
   {
     left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	 while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
     UCA0TXBUF = left_motor; 
     
    }
   if(right_motor > 213 && right_motor <= 255)
   {
     right_motor = right_motor--;                      //DECREMENT right MOTOR
     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
     UCA0TXBUF = right_motor;
    
   }
   //__delay_cycles(10);
   
   
   
  
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
    
    left_sensor= pulse_width_count_2/58;  // in cm
    prevEdgeStamp_2 = currEdgeStamp_2;
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
