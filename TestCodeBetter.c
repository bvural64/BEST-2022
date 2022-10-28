#pragma config(UART_Usage, UART1, uartUserControl, baudRate9600, IOPins, None, None)
#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           baseArmMotor,  tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port5,           jointArmMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port6,           clawServo1,    tmotorServoStandard, openLoop)
#pragma config(Motor,  port7,           clawServo2,    tmotorServoStandard, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Define statements for IR sensor (peripheral memory map)
#define PERIPH_BASE ((unsigned long)0x40000000)
#define APB1PERIPH_BASE PERIPH_BASE
#define USART2_BASE (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE (APB1PERIPH_BASE + 0x4800)
#define USART2 ((USART_TypeDef*) USART2_BASE)
#define USART3 ((USART_TypeDef*) USART3_BASE)

// Typedefs for IR Sensor code
typedef unsigned long uint32_t;
typedef unsigned short uint16_t;

typedef struct
{
	uint16_t SR;
	uint16_t RESERVED0;
	uint16_t DR;
	uint16_t RESERVED1;
	uint16_t BRR;
	uint16_t RESERVED2;
	uint16_t CR1;
	uint16_t RESERVED3;
	uint16_t CR2;
	uint16_t RESERVED4;
	uint16_t CR3;
	uint16_t RESERVED5;
	uint16_t GTPR;
	uint16_t RESERVED6;
} USART_TypeDef;

// Set Baud Rate for IR Sensor
void setBaud(const TUARTs nPort, int baudRate)
{
	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;

	// 36 MHz
	apbclock = 36000000;

	// Determine int part
	integerdivider = ((0x19 * apbclock) / (0x04 * (baudRate)));
	tmpreg = (integerdivider / 0x64) << 0x04;

	// Determine fraction part
	fractionaldivider = integerdivider - (0x64 * (tmpreg >> 0x04));
	tmpreg = ((((fractionaldivider * 0x10) + 0x32) / 0x64)) &0x0F;

	// Write to USART BRR
	USART_TypeDef *uart = USART2;
	if (nPort == UART2)
	{
		uart = USART3;
	}
	uart->BRR = (uint16_t)tmpreg;
}

// Entry point
task main()
{
	 bool hasSentReset = false;
	 bool hasSentRotate = false;
	 bool hasSentDrive = false;
	 bool hasSentLift = false;
	// Set Baud
	setBaud(UART1, 600);

	int neutralClawPos = 0; // Variable for neutral claw position
	motor[clawServo1] = neutralClawPos; // Set claw to default before user control

	// Initialize User Control using Infinite Loop
	while (1 == 1)
	{
		// Drive Train Code (Split Right Arcade Control); Input from both joysticks

		//Checks if both joysticks are being used, then runs split arcade control
		if(vexRT[Ch2] != 0 && vexRT[Ch4] != 0)
    	{
			motor[leftMotor]  = (-vexRT[Ch2] - vexRT[Ch4])/2;  // (y + x)/2
    		motor[rightMotor] = (-vexRT[Ch2] + vexRT[Ch4])/2;  // (y - x)/2
  		}
  		/*
  		Because values are averaged, when only one stick is used,
  		the maximum speeds of the motors are half of the sticks' maximum inputs.
  		This statement counteracts this issue by only applying the
  		average when it is turning, and only forward and backward input when it is not.
  		*/
  		else
  		{
  			motor[leftMotor] = -vexRT[Ch2];
  			motor[rightMotor] = -vexRT[Ch2];
  		}

  		// Main arm motor control

  		// Inputs 65 to base motor when 5U button is pressed
  		if(vexRT[Btn5U] == 1)
  		{
  			motor[baseArmMotor] = 65;
  		}

  		// Inputs -65 to base motor when 5D button is pressed
  		else if(vexRT[Btn5D] == 1)
  		{
  			motor[baseArmMotor] = -65;
  		}
  		else
  		{
  			motor[baseArmMotor] = 0;
  		}

  		// Inputs 65 to joint motor when 6U button is pressed
  		if(vexRT[Btn6U] == 1)
  		{
  			motor[jointArmMotor] = 65;
  		}

  		// Inputs -65 to joint motor when 6D button is pressed
  		else if(vexRT[Btn6D] == 1)
  		{
  			motor[jointArmMotor] = -65;
  		}
  		else
  		{
  			motor[jointArmMotor] = 0;
  		}

  		// Code for claw open and close

		if (vexRT[Btn8U] == 1)
		{
			motor[clawServo1] = 127;
			neutralClawPos = motor[clawServo1];
			wait1Msec(10);
		}

		else if (vexRT[Btn8D] == 1)
		{
			motor[clawServo1] = -127;
			neutralClawPos = motor[clawServo1];
			wait1Msec(10);
		}

		else
		{
			motor[clawServo1] = neutralClawPos;
		}

		// IR Sensor

	//	sendChar(UART1, 0xf0); // test send

		if(vexRT[Btn7U] == 1 && hasSentReset == false)
		{
			sendChar(UART1, 0xAA);
			hasSentReset = true;
			hasSentRotate = false;
	  	hasSentDrive = false;
	 		hasSentLift = false;
		}
		else if(vexRT[Btn7D] == 1 && hasSentDrive == false)
		{
  		sendChar(UART1, 0x66);
  		hasSentReset = false;
			hasSentRotate = false;
	 		hasSentDrive = true;
	 		hasSentLift = false;
		}
		else if(vexRT[Btn7L] == 1 && hasSentLift == false)
		{
	  	sendChar(UART1, 0x55);
	  	hasSentReset = false;
			hasSentRotate = false;
		  hasSentDrive = false;
		  hasSentLift = true;
		}
		else if(vexRT[Btn7R] == 1 && hasSentRotate == false)
		{
		  sendChar(UART1, 0x33);
		  hasSentReset = false;
			hasSentRotate = true;
			hasSentDrive = false;
			hasSentLift = false;
		}
	}
}
