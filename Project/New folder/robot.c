#include "declaration.h"
volatile uint8_t i=0;
volatile uint8_t j;


 void Navigation(uint16_t front, uint16_t left){
      switch (j)
      {
      case 0:
        while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
        UCA0TXBUF = 0x00;                // robot in stopped state
        while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
        UCA0TXBUF = 0x00;
        i++;
        if(i==100)
        {
          j=1;
          i=0;
        }
        break;
      case 1:
        while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
        UCA0TXBUF = 0x7F ;               // Robot in Full Ahead state
        while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
        UCA0TXBUF = 0xFF ;               // Robot in Full Ahead state
        i++;
        if(i==100)
        {
          j=2;
          i=0;
        }
        break;
      case 2:
        while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
        UCA0TXBUF = 0x00;                // robot in stopped state
        // in stopped state
        i++;
        if(i==100)
        {
          j=3;
          i=0;
        }
        break;
      case 3:
        while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
        UCA0TXBUF = 0x01;                // Robot in Spin state
        while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
        UCA0TXBUF = 0xFF ;               // Robot in Full Ahead state
        i++;
        if(i==100)
        {
          j=4;
		  i=0;
         }
        break;
      case 4:
        while ( !(IFG2 & UCA0TXIFG)) {};      // Confirm that Tx Buff is empty
        UCA0TXBUF = 0x00;                // robot in stopped state

          break;
      }
  }
