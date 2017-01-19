#include "msp430x22x4.h"
#include "stdint.h"
#include "navigation.h"

#pragma vector=TIMERA1_VECTOR
__interrupt void IsrCntPulseTACC1 (void)
//--------------------------------------------------------------------------
// Func:  At TACCR1 IRQ, calculate pulse width and the corresponding distance
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
	switch (__even_in_range(TAIV, 10))    	// I.D. source of TA IRQ
	{
	case TAIV_TACCR1:
		// handle chnl 1 IRQ

		if((P2IN & 0x08) == 0x08)             // if the interrupt is on rising edge
		{
			prevRiseStmpF = currRiseStmpF;			// store TxR of previous IRQ
			currRiseStmpF = TACCR1;							// store TxR of current IRQ
			if(currRiseStmpF <= prevRiseStmpF)
				periodF = ( (65535 - prevRiseStmpF) + currRiseStmpF );
			// period with Timer rollover
			else
				periodF = (currRiseStmpF - prevRiseStmpF);
			// period with Timer rollover
		}
		else                                  // if interrupt is on falling edge
		{
			currFallStmpF = TACCR1;
			if(currFallStmpF <= currRiseStmpF)
			{
				pulsewidthF =((65535- currRiseStmpF + currFallStmpF ));
			}
			else
			{
				pulsewidthF = ((currFallStmpF - currRiseStmpF));
			}
		}
		distanceF = pulsewidthF/148;					// Convert Pulse width
																					//into distance in inches

		TACCTL1&=~0x08;	                			// clear IRQ

		//PushToStack(array_front, distanceF);
		for(i = 5; i >= 1; i--)          			// Shifting all values in array to right
			array_front[i] =  array_front[i-1];
		array_front[0] = distanceF;      			// First element is the current distance

		temp = 0;
		for(i = 0; i < 6; i++)
			temp = temp+array_front[i];
		distanceF = temp / 6;             		// Average of 6 values


		Navigation(distanceF, distanceL);    	// Implement navigation algorithm

		break;

	case TAIV_TACCR2:                     	// ignore chnl 2 IRQ
	case TAIV_TAIFG:                      	// ignore TAR rollover IRQ
	default:                              	// ignore everything else
	}
}


#pragma vector=TIMERB1_VECTOR
__interrupt void IsrCntPulseTBCC1 (void)
//--------------------------------------------------------------------------
// Func:  At TACCR1 IRQ, calculate pulse width and the corresponding distance
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
	switch (__even_in_range(TBIV, 10))    	// I.D. source of TB IRQ
	{
	case TBIV_TBCCR1:                     	// handle chnl 1 IRQ

		if((P4IN & 0x10) == 0x10)             // if the interrupt is on rising edge
		{
			prevRiseStmpL = currRiseStmpL;			// store TBR of previous IRQ
			currRiseStmpL = TBCCR1;							// store TBR of current IRQ
			if(currRiseStmpL <= prevRiseStmpL)
				periodL = ( (65535 - prevRiseStmpL) + currRiseStmpL );
																					// period with Timer rollover
			else
				periodL = (currRiseStmpL - prevRiseStmpL);
																					// period with Timer rollover
		}
		else                                  // if interrupt is on falling edge
		{
			currFallStmpL = TBCCR1;
			if(currFallStmpL <= currRiseStmpL)
			{
				pulsewidthL =((65535- currRiseStmpL + currFallStmpL ));
			}
			else
			{
				pulsewidthL = ((currFallStmpL - currRiseStmpL));
			}
		}
		distanceL = pulsewidthL/148;  				// Convert Pulse width
																					//into distance in inches

		TBCCTL1&=~0x08;	                			// clear IRQ

																					//PushToStack(array_left, distanceL)
		for(i = 5; i >= 1; i--)        				// Shifting values in array to right
			array_left[i] =  array_left[i-1];
		array_left[0] = distanceL;     				// First element is the current value

		temp = 0;
		for(i = 0; i < 6; i++)
			temp = temp+array_left[i];
		distanceL = temp/6;             			// Average of 6 values

		Navigation(distanceF, distanceL);  		// Implement the navigation algorithm
		break;

	case TBIV_TBCCR2:                     	// ignore chnl 2 IRQ
	case TBIV_TBIFG:                      	// ignore TAR rollover IRQ
	default:                              	// ignore everything else

	}
}

void InitPorts (void)
//------------------------------------------------------------------------
// Func:  Initialize the ports for I/O on TA1 Capture
// Args:  None
// Retn:  None
//------------------------------------------------------------------------
{
	P1DIR |= 0x01;														// Config P1.0 as Input (Red LED)

	P1DIR |= 0x02;														// Config P1.1 as Input (Green LED)

	P2DIR &= ~0x04;     											// Config P2.2 as Input
	P2SEL |= 0x04;														// P2.2 = TA0 (5) echo

	P2DIR &= ~0x08;     											// Config P2.3 as Input
	P2SEL |= 0x08;														// P2.3 = TA1 (6) echo

	P4DIR &= ~0x10;     											// Set pin P4.5 to Input
	P4SEL |= 0x10;      											// p4.5 = TB2 (10) echo

	P4DIR |= 0x20;      											// Config P4.4 as Output
	P4SEL |= 0x20;														// P4.4 = TB1 (9) Trigger

	P3SEL = 0x30;       											// P3.4,5 = USCI_A0 TXD/RXD
}


void main(void)
//------------------------------------------------------------------------
// Func:  Init I/O ports & IRQs, enter LoPwr Mode
// Args:  None
// Retn:  None
//------------------------------------------------------------------------
{
	WDTCTL = WDTPW | WDTHOLD;                   //Stop Watchdog Timer

	InitPorts();                                //  Configure I/O Pins
	distanceL = 0, distanceF = 0 ; 							// initialize variables to zero
	uint32_t array_front[6]={0,0,0,0,0,0};
	uint32_t array_left[6]={0,0,0,0,0,0};


	// Timer A initialization (for Echo pulses)

	TACTL  = TASSEL_2 | ID_0 | MC_2;             // SMCLK | Div by 1 | Contin Mode

	TACCTL1 = CM0 | CM1 | CCIS0 | CAP | SCS | CCIE;
																							// Both Edge | inp = CCI1B |
																							// Capture | Sync Cap | Enab IRQ

	// Timer B initialization (for Trigger pulses)

	TBCTL  = TBSSEL_2 | ID_0 | MC_1;            // SMCLK | Div by 1 | up Mode

	TBCCTL1 = CM0 | CM1 | CCIS0 | CAP | SCS | CCIE;
																							// Both Edge | inp = CCI1B |
																							// Capture | Sync Cap | Enab IRQ
	TBCCTL2 = OUTMOD_6;                         // TOGGLE/SET MODE

	TBCCR0 = 48000;                             // Period of 40 ms
	TBCCR2 = 18;                                // pulsewidth of 15 micro sec

	// Config. UART Clock & Baud Rate

	BCSCTL1 = CALBC1_1MHZ;                			// DCO = 1 MHz
	DCOCTL  = CALDCO_1MHZ;                			// DCO = 1 MHz
	UCA0CTL1 |= UCSSEL_2;                 			// UART use SMCLK

	UCA0MCTL = UCBRS0;                    			// Map 1MHz -> 9600 (Tbl 15-4)
	UCA0BR0  = 104;                       			// Map 1MHz -> 9600 (Tbl 15-4)
	UCA0BR1  = 0;                         			// Map 1MHz -> 9600 (Tbl 15-4)

	UCA0CTL1 &= ~UCSWRST;                 			// Enable USCI state mach

	j=0;			                                  // Starting from rest state
	Stop();   			                            // Initially in Stop state


	_BIS_SR(LPM1_bits + GIE);		               // Enter LPM w/ IRQs enabe
}
