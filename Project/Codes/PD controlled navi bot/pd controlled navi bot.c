				 					
#include "msp430x22x4.h"
#include "stdint.h"
volatile uint16_t left_sensor;
volatile uint16_t front_sensor;
//volatile uint16_t front_sensor_diff;
volatile uint8_t i;
volatile uint16_t pd = 1;
volatile uint16_t l_err;
volatile int16_t l_err_d;
volatile uint16_t r_err;
volatile int16_t r_err_d;
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
    //front_sensor_diff = front_sensor - (pulse_width_count/58);
    front_sensor = pulse_width_count/58;    // in cm
    prevEdgeStamp = currEdgeStamp;
 
	
	}
	
	
	
	//Navigation logic
	
if(cal/4)
{
   if(left_sensor < 100)
   {
	   
	   left_motor = 100;
	   right_motor = 228;
	   
	   
	   
	   l_err_d = 100 - left_sensor - l_err;

       l_err = 100 - left_sensor;

       pd = (l_err/2) + (l_err_d*2);
	   
	   if(pd>25)
	   {
		   pd = 25;
	   }
	   
	   for(i=0; i<= pd; i++)
	   {
		 left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = left_motor;   
		 
		 left_motor = right_motor--;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = right_motor; 
		   
	   }
	   //__delay_cycles(pd*10);
	   
	   
   }
   if(left_sensor > 100)
   {
	   
	   left_motor = 100;
	   right_motor = 228;
	   
	   
	   
	   r_err_d = left_sensor - 100 - r_err;

       r_err = left_sensor - 100;

       pd = (r_err/2) + (r_err_d*4);
	   
	   if(pd>25)
	   {
		   pd = 25;
	   }
	   
	   for(i=0; i<= pd; i++)
	   {
		 left_motor = right_motor++;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = right_motor;   
		 
		 left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = left_motor; 
		   
	   }
	   //__delay_cycles(pd*10);
	   
	   
   }	
	

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
