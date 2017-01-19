#include "msp430x22x4.h"
#include "stdint.h"
volatile uint16_t PW_time_us ;
volatile uint16_t PW_time_us_2 ;

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
   static uint16_t currEdgeStamp=10, left_sensor = 0, left_motor = 0, right_motor = 0; 
   static uint16_t prevEdgeStamp = 0; 
   static uint32_t pulse_width_count=10;
   
    case TAIV_TACCR1:                 // handle chnl 1 IRQ
                  
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
    
    PW_time_us = pulse_width_count*100/115;
	left_sensor = PW_time_us/58;        //distance from wall in cm
    prevEdgeStamp = currEdgeStamp;
    TACCTL1 &= ~0x01; 
    break;
    
    case TAIV_TACCR2:                 // chnl 2 IRQ
    case TAIV_TAIFG:                   // ignore TAR rollover IRQ
    default:                          // ignore everything else
  }
  
  
  while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
  UCA0TXBUF = 112;                  // Robot in 3/4 forward Spin state
  while ( !(IFG2 & UCA0TXIFG)) {};  //Confirm that Tx Buff is empty
  UCA0TXBUF = 236 ;                // Robot in 3/4 forward Spin stat
  
  if(left_sensor < 90)                           //Turn Right
{
 if(left_motor > 64 && left_motor < 127)
  {
    left_motor++;                      //INCREMENT LEFT MOTOR
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = left_motor;            
  }
 if(right_motor > 192 && right_motor < 255)
  {
    right_motor--;                      //DECREMENT right MOTOR
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = right_motor;          
  }

}
else if(left_sensor > 90)                      //Turn LEFT
{
 if(left_motor > 64 && left_motor < 127)
  {
    left_motor--;                      //DECREMENT LEFT MOTOR
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = left_motor;            
  }
 if(right_motor > 192 && right_motor < 255)
  {
    right_motor++;                      //INCREMENT right MOTOR
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = right_motor;          
  }
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
    
    PW_time_us_2 = pulse_width_count_2*100/115;
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
   
  TBCCR0 = 40250-1;               // 35 ms Set frequency of PWM (UP mode)
  TBCCR1 = 12;            // PW = 10us Set pulse wid of PWM (UP mode) initialize to 0 degree
   
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
