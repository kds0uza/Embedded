/*************************************************************
*
* The navigation algorithm for the Hallway Navigation Robot.
*
**************************************************************/
#include "declaration.h"

void Navigation(uint16_t front, uint16_t left)
//--------------------------------------------------------------------------
// Func:  Control's Robot's movement according to the respective distances
//				observed using the fron and the left sensor
// Args:  Distance from the neareset object in front (inches)
//				Distance from the nearest object on the right (inches)
// Retn:  None
//--------------------------------------------------------------------------
{
	if( j == 0 )                        // if the robot has just started
	{
		if(front > 40)
			StraightForward();
		else if ( front < 40)
			j = 3;                          // Robot has come across the first
		// obstacle
	}
	else if (j > 0)                     // Once the navigation starts
	{
		if( left < 30 )
		{
			if(front > 40)
				VerySlowRight();
			else if( front < 40 && front > 30)
				SlowRight();
			else if ( front < 30 && front > 20)
				FastRight();
			else if( front < 20 && front > 10)
				HardRight();
			else if (front < 10)
				Stop();
			j = 1;                          // Robot is less than 30 inches far from
																			// the object on left (zone 1)
		}
		else if( left > 35 && left < 35)
		{
			if(front > 40)
				VerySlowLeft();
			else if ( front < 40 && front > 30)
				SlowLeft();
			else if( front < 30 && front > 20)
				FastLeft();
			else if( front < 20 && front > 10)
				HardLeft();
			else if(front < 10)
				Stop();
			j = 2;                           // Robot is more than 35 inches far from
																			 // the object on left (zone 2)
		}
		else if( left > 30 && left < 35 )
		{
			if( j == 1)                   	// If robot has entered zone 3 from zone 1
			{
				if(front > 40)
					StraightForward();
				else if ( front < 40 && front > 30)
					VerySlowRight();
				else if( front < 30 && front > 20)
					SlowRight();
				else if( front < 20 && front > 10)
					FastRight();
				else if(front < 10)
					Stop();
			}
			else if (j == 2 )            		// If robot has entered zone 3 from zone 2
			{
				if(front > 40)
					StraightForward();
				else if ( front < 40 && front > 30)
					VerySlowLeft();
				else if( front < 30 && front > 20)
					SlowLeft();
				else if( front < 20 && front > 10)
					FastLeft();
				else if(front < 10)
					Stop();
			}
			else if ( j == 3)            		// If robot has been in zone 3 only
			{
				if(front > 40)
					StraightForward();
				else if ( front < 40 && front > 30)
					VerySlowRight();
				else if( front < 30 && front > 20)
					FastRight();
				else if( front < 20 && front > 10)
					FastRight();
				else if(front < 10)
					Stop();
			}
			j = 3;                          // Robot is in intermediate zone between
																			// 30 to 35 inches from
																			//the object on left (zone 3)
		}
		else if( left > 60)
			FastLeft();
		else if ( left < 20)
		{
			if(front < 20)
				Stop();
		}
	}
}

