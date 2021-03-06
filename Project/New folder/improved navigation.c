																					   /*************************************************************
*
* The navigation algorithm for the Hallway Navigation Robot.
*
**************************************************************/
#include "declaration.h"

void Navigation(uint16_t front, uint16_t right)
{


	if( j==0 )
	{
		if(front > 40)
			StraightForward();
		else if ( front < 40)
			j=3;
	}
	else if (j > 0)
	{
		if( right < 30 && right > 20 )
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
			j=1;
		}
		else if( right > 35 && right < 60)
		{
			if(front > 40)
				VerySlowLeft();
			else if ( front < 40 && front > 30)
				SlowLeft();
			else if( front < 30 && front > 20)
				FastLeft();
			else if( front < 20 && front > 10)
				HardRight();
			else if(front < 10)
				Stop();
			j=2;
		}
		else if( right < 30 && right > 35 )
		{
			if( j==1)
			{
				for(i=0; i < 150; i ++)
					VerySlowLeft();
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
			else if (j==2 )
			{
				for(i=0; i < 150; i ++)
					VerySlowRight();
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
			else if ( j== 3)
			{
				for( i=0; i< 100; i++)
					VerySlowLeft();
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
			j=3;
		}
		else if ( right < 20)
		{
			if(front < 20)
				Stop();
		}
		else if( right > 60)
		{
			FastLeft();

		}
	}


}

