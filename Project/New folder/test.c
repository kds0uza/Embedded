
void turing(uint16_t front, uint16_t left)
{
	if(front>20)
	{
		if(left > 60)
		{
			for (i=1;i<300;i++)
				HardLeft();
			for (i=1;i<300;i++)
				StraightForward();
		}
	else
		StraightForward();
	}
}
