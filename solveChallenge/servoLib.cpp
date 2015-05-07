#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void sendServoCommand(int &uart_filestream, unsigned int servoID, unsigned int command)
{
	unsigned char outputBuffer[10];

	outputBuffer[0]= 0x84;						//Header
	outputBuffer[1]= (unsigned char) servoID;			//ServoID
	outputBuffer [2] = (unsigned char) command & 0x7F;		//Command Part 1
	outputBuffer [3] = (unsigned char) (command >> 7) & 0x7F;	//Command Part 2
	
	transmitUART(uart_filestream, outputBuffer, 4);
	//printf("Sent: %d\n", command);
}