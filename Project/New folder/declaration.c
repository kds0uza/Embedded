																					 /*************************************************************
 *
 * Declaration of all the variables and functions.
 *
 **************************************************************/
volatile uint8_t i;												// Counter Variable for various loops
volatile uint8_t j; 											// Variable to keep track of the
																					//previous position


volatile static uint16_t distanceL, distanceF;// Global distances

volatile static uint16_t temp;
volatile static uint16_t periodF, periodL;    // Periods
volatile static uint32_t currRiseStmpF,  currRiseStmpL;  // Current rise stamps
volatile static uint32_t prevRiseStmpF ,prevRiseStmpL;  // Previous rise stamps
volatile static uint32_t currFallStmpF, currFallStmpL;  // Current fall stamps
volatile static uint32_t pulsewidthF, pulsewidthL;    // Pulsewidths

volatile static uint16_t array_front[6], array_left[6];  // Arrays of Distances



void StraightForward()
//--------------------------------------------------------------------------
// Func:  Move Robot Forward at full speed in a straight line.
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 127;		      					// drive motor 1 at full speed
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 255;		     						// drive motor 2 at full speed
}

void VerySlowRight()
//--------------------------------------------------------------------------
// Func:  Move Robot slowly to the right side.
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 110;		      					// drive motor 1 at half speed
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 255;		      					// drive motor 2 at full speed
}

void SlowRight()
//--------------------------------------------------------------------------
// Func:  Move Robot slowly to the right side.
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 95;		     							// drive motor 1 at half speed
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 255;		      					// drive motor 2 at full speed
}

void FastRight()
//--------------------------------------------------------------------------
// Func:  Move Robot slowly to the right side.
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 80;		      						// drive motor 1 at half speed
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 255;		      					// drive motor 2 at full speed
}
void VerySlowLeft()
//--------------------------------------------------------------------------
// Func:  Move Robot very slowly to the left side.
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 127;		      					// drive motor 1 at full speed
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 240;		      					// drive motor 2 at half speed
}
void SlowLeft()
//--------------------------------------------------------------------------
// Func:  Move Robot slowly to the left side.
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 127;		      					// drive motor 1 at full speed
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 225;		      					// drive motor 2 at half speed
}

	void FastLeft()
//--------------------------------------------------------------------------
// Func:  Move Robot slowly to the left side.
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 127;		      					// drive motor 1 at full speed
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 215;		      					// drive motor 2 at half speed
}
void Stop()
//--------------------------------------------------------------------------
// Func:  Stop robot completely
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 00;		      						// stop motor 1
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 00;		      						// stop motor 2
}

void Stop2()
//--------------------------------------------------------------------------
// Func:  Stop robot completely
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 64;		      						// stop motor 1
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 192;		      					// stop motor 2
}
void Reverse()
//--------------------------------------------------------------------------
// Func:  Move Robot in the reverse direction.
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 001;		      					// drive motor 1 at full reverse
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 128;		      					// drive motor 2 at half reverse
}

void HardRight()
//--------------------------------------------------------------------------
// Func:  Move Robot hardly to the right side ( rotate clockwise ).
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 001;		      					// drive motor 1 at full reverse
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 255;		      					// drive motor 2 at full speed
}

void HardLeft()
//--------------------------------------------------------------------------
// Func:  Move Robot hardly to the left side ( rotate anti-clockwise ).
// Args:  None
// Retn:  None
//--------------------------------------------------------------------------
{
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 127;		      					// drive motor 1 at full speed
  while ( !(IFG2 & UCA0TXIFG)) {};    // Confirm that Tx Buff is empty
  UCA0TXBUF = 128;		      					// drive motor 2 at Stop
}

