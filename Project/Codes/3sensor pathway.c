if((front_sensor <= 25 || front_sensor > 1200) && left_sensor <= 35 && (right_sensor <= 35 || right_sensor > 1100))	  // if wall ahead
{
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = 000; 				   // stop
    while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = 000; 				   // stop
	__delay_cycles(100);
	
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = 32; 				   // LM = 1/2 reverse
    while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = 160; 				   // RM = 1/2 reverse
	__delay_cycles(500);	
		
    while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = 96; 				   // LM = 1/2 forward
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = 160; 				   // RM = 1/2 reverse
	__delay_cycles(100);
	
	
	/*
	stop_flag++;
	
	while(stop_flag == 5)
	{
		while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
        UCA0TXBUF = 000; 				   // stop
        while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
        UCA0TXBUF = 000; 				   // stop
	}
	*/
}
/*else if(front_sensor < 150 && (left_sensor > 100 && left_sensor < 145) && right_sensor < 150 && right_sensor > 105 ) //if obstacle in middle of hallway
{
	if(left_sensor <= 125)          // if bot is slightly
	
	
	
	
}*/
else if(front_sensor > 25 && left_sensor >= 250)    //new hallway detected on left
{
	  left_motor = 100;
	  right_motor = 228;
	  
	  //turn left
	  
	
	  
	  for(i=0;i<25;i++)
	  {
	  	  left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	       while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
           UCA0TXBUF = left_motor;
		   
		   right_motor = right_motor++;                      //INCREMENT Right MOTOR
	       while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
           UCA0TXBUF = right_motor;
		   
		  
	  }
	  __delay_cycles(400);
}
else if(front_sensor > 150 && left_sensor > 100 && left_sensor < 145 && right_sensor < 150 && right_sensor > 65 )	  //when bot is in center of hallway
{
	left_motor = 100;
	right_motor = 228;
	
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = left_motor; 				   // stop
	while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
    UCA0TXBUF = right_motor; 				   // stop
	
	if(left_sensor <= 125)
		zone = 2;
	else
		zone = 3;
}
else if(front_sensor > 150 && left_sensor > 25 && left_sensor < 100 && right_sensor > 100)           // when bot is in zone 1, near left wall
{
	zone = 1;
	
	left_motor = 100;
	right_motor = 228;
	
	prop = 15 + ((100 - left_sensor)*2)/15;			  //(25-15)/(100-25);
	
	for(i=0;i <= prop; i++)					  //TURN LEFT
	{
	     if(left_motor >= 75 && left_motor < 127)
      {
         left_motor = left_motor++;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = left_motor; 
     
      }
         if(right_motor > 203 && right_motor <= 255)
      {
         right_motor = right_motor--;                      //DECREMENT right MOTOR
         while ( !(IFG2 & UCA0TXIFG)){};
		 UCA0TXBUF = right_motor;
	  }
	  
    }
	
	
}
else if(front_sensor > 150 && right_sensor > 25 && right_sensor < 100 && left_sensor > 100)			 //when bot is in zone 4, near right wall
{
	zone = 4;
	
	left_motor = 100;
	right_motor = 228;
	
	prop = 15 + ((100 - right_sensor)*2)/15;			  //(25-15)/(100-25);
	
	for(i=0;i <= prop; i++)					  //TURN LEFT
	{
	     if(left_motor >= 75 && left_motor < 127)
      {
         left_motor = left_motor--;                      //INCREMENT LEFT MOTOR
	     while ( !(IFG2 & UCA0TXIFG)) {};   // Confirm that Tx Buff is empty                      
         UCA0TXBUF = left_motor; 
     
      }
         if(right_motor > 203 && right_motor <= 255)
      {
         right_motor = right_motor++;                      //DECREMENT right MOTOR
         while ( !(IFG2 & UCA0TXIFG)){};
		 UCA0TXBUF = right_motor;
	  }
	  
    }
	
	
}
