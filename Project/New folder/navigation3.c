/*************************************************************
*
* The navigation algorithm for the Hallway Navigation Robot.
*
**************************************************************/
#include "declaration.h"

void Navigation(uint16_t front, uint16_t right)
{
	if(right < 150)
	{
		if (front > 150)
		{
			if (right < 40)
				VerySlowLeft ();
			else if (right > 40)
				VerySlowRight();
		}
		else if(front > 100 && front < 150)
		{
			if(right < 40)
				VerySlowLeft();
			else if(right > 40)
				VerySlowRight();
		}
		else if( front > 50 && front < 100)
		{
			if(right < 40)
			{
				//Reverse();
				//for(i=0;i < 8; i++){}
				SlowLeft();
			}
			else if (right > 40)
			{
				//Reverse();
				//for(i=0;i<8;i++){}
				VerySlowRight();
			}
		}
		else if (front < 50)
		{
			if(right < 20)
				Stop();
			else if (right > 20)
			{
				Reverse();
				//for(i=0;i<8;i++)
				SlowLeft();
			}
		}
	}
	else
	{

		HardRight();
		//for(i=0;i<4;i++) {}
		//StraightForward();
		//for(i=0;i<8;i++){}

	}
}



