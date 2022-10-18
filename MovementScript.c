#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


task main()
{
	// Initialize User Control using Infinite Loop
	while (1 == 1)
	{
		/* 
			Motor Power Values (-127 to 127 inclusive) are set to the values of the joysticks 
			in a tank configuration to control drive train
		*/
		motor[leftMotor] = vexRT[Ch3]; // Left Joystick Y value 
		motor[rightMotor] = vexRT[Ch2]; // Right Joystick Y value

		/*

		// Future Drive Train Code (Arcade Control)

		// Turn Left
		if (vexRT[Ch4] < 0) // Could flip, we dont know if left is negative or positive
		{
			motor[leftMotor] = vexRT[Ch4];
			motor[rightMotor] = vexRT[Ch4] * -1;
		}

		// Turn Right
		if (vexRT[Ch4] > 0) // Could flip, we dont know if right is negative or positive
		{
			motor[leftMotor] = vexRT[Ch4] * -1;
			motor[rightMotor] = vexRT[Ch4];
		}

		// Forward/Backward Control
		if (vexRT[Ch2]) 
		{
			motor[leftMotor] = vexRT[Ch2];
			motor[rightMotor] = vexRT[Ch2];
		}
		
		*/
	} 
}
