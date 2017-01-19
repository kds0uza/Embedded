/*************************************************************
*
* The navigation algorithm for the Hallway Navigation Robot.
*
**************************************************************/
#include "declaration.h"

void Navigation(uint16_t front, uint16_t right)
{
	if(front > 60)
	{
		if( right < 40)
			VerySlowRight();
		else if ( right > 42 && right < 45)
			VerySlowLeft();
		else if ( right > 60)
		{
		Reverse();
		for(i=0;i<4000;i++){} ;
			HardLeft();
		for(i=0;i<5000;i++){};
		}
	}
	else if( front < 60 && front > 50)
	{
		if( right < 40)
			SlowRight();
		else if(right > 42 && right < 45)
			SlowLeft();
		else if( right > 60)
		{
			Reverse();
			for(i=0;i<4000;i++){} ;
			HardLeft();
			for(i=0;i< 5000;i++){};
		}
	}
	else if(front < 50 && front > 30)
	{
		if( right < 40)
			FastRight();
		else if(right > 42 && right < 45)
			FastLeft();
		else if( right > 60)
		{
			Reverse();
			for(i=0;i<4000;i++){};
			HardLeft();
			for(i=0;i< 10000;i++){} ;
		}
	}
	else if(front < 30 && front > 20)
	{
		Reverse();
		for(i=0;i<1000;i++){};
			if( right < 40)
				FastRight();
			else if(right > 42 && right < 45)
				FastLeft();
			else if( right > 60)
			{
				Reverse();

				HardRight();
				for(i=0;i< 5000;i++){} ;
			}
	}
	else if( front < 10)
	{
		if( right > 60)
		{
			Reverse();
			HardLeft();
		}
		else if( right < 10)
		{
			Stop();
		}
	}
}


