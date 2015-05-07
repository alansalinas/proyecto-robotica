#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <pthread.h>

#include "defines.h"

#include "gpioLib.cpp"
#include "uartLib.cpp"
#include "servoLib.cpp"
#include "cameraLib.cpp"

#define MAIN_DEBUG 1
#define SIMULATE_ACK 0

#define IGNORE_CLEANUP		0
#define EXECUTE_CLEANUP		1

using namespace std;

struct CommandPins {
	string commandPin1;
	string commandPin2;
	string commandPin3;
	string commandPin4;
	string ackPin;
};

//Global variables
int uart_filestream;
//CvCapture* capture;
VideoCapture capture;
struct CommandPins cmdPins;

void configureCommandPins(struct CommandPins &cmdPins)
{
	cmdPins.commandPin1 = "5";
	cmdPins.commandPin2 = "6";
	cmdPins.commandPin3 = "13";
	cmdPins.commandPin4 = "19";
	cmdPins.ackPin = "17";
	
	exportGPIO(cmdPins.commandPin1);
	configureGPIO(cmdPins.commandPin1, "out");
	
	exportGPIO(cmdPins.commandPin2);
	configureGPIO(cmdPins.commandPin2, "out");
	
	exportGPIO(cmdPins.commandPin3);
	configureGPIO(cmdPins.commandPin3, "out");
	
	exportGPIO(cmdPins.commandPin4);
	configureGPIO(cmdPins.commandPin4, "out");
	
	exportGPIO(cmdPins.ackPin);
	configureGPIO(cmdPins.ackPin, "in");	//Pull down?
	
	#if MAIN_DEBUG
	cout << "Pins Configured" << endl;
	#endif
}

void sendCommand(struct CommandPins cmdPins, int command)
{	
	switch(command)
	{
		case POSITION1_CMD:
			setGPIOvalue(cmdPins.commandPin1, "1");
			setGPIOvalue(cmdPins.commandPin2, "0");
			setGPIOvalue(cmdPins.commandPin3, "0");
			setGPIOvalue(cmdPins.commandPin4, "0");
			
			#if MAIN_DEBUG
			cout << "POSITION1_CMD" << endl;
			#endif
			break;
			
		case POSITION2_CMD:
			setGPIOvalue(cmdPins.commandPin1, "0");
			setGPIOvalue(cmdPins.commandPin2, "1");
			setGPIOvalue(cmdPins.commandPin3, "0");
			setGPIOvalue(cmdPins.commandPin4, "0");
			
			#if MAIN_DEBUG
			cout << "POSITION2_CMD" << endl;
			#endif
			break;
			
		case GOTO_LOADZONE_CMD:
			setGPIOvalue(cmdPins.commandPin1, "1");
			setGPIOvalue(cmdPins.commandPin2, "1");
			setGPIOvalue(cmdPins.commandPin3, "0");
			setGPIOvalue(cmdPins.commandPin4, "0");
			
			#if MAIN_DEBUG
			cout << "GOTO_LOADZONE_CMD" << endl;
			#endif
			break;
		
		case TOOL1_CMD:
			setGPIOvalue(cmdPins.commandPin1, "0");
			setGPIOvalue(cmdPins.commandPin2, "0");
			setGPIOvalue(cmdPins.commandPin3, "1");
			setGPIOvalue(cmdPins.commandPin4, "0");
			
			#if MAIN_DEBUG
			cout << "TOOL1_CMD" << endl;
			#endif
			break;

		case TOOL2_CMD:
			setGPIOvalue(cmdPins.commandPin1, "1");
			setGPIOvalue(cmdPins.commandPin2, "0");
			setGPIOvalue(cmdPins.commandPin3, "1");
			setGPIOvalue(cmdPins.commandPin4, "0");
			
			#if MAIN_DEBUG
			cout << "TOOL2_CMD" << endl;
			#endif
			break;

		case TOOL3_CMD:
			setGPIOvalue(cmdPins.commandPin1, "0");
			setGPIOvalue(cmdPins.commandPin2, "1");
			setGPIOvalue(cmdPins.commandPin3, "1");
			setGPIOvalue(cmdPins.commandPin4, "0");
			
			#if MAIN_DEBUG
			cout << "TOOL3_CMD" << endl;
			#endif
			break;
			
		case GOTO_OILRIG_R_CMD:
			setGPIOvalue(cmdPins.commandPin1, "1");
			setGPIOvalue(cmdPins.commandPin2, "1");
			setGPIOvalue(cmdPins.commandPin3, "1");
			setGPIOvalue(cmdPins.commandPin4, "0");
			
			#if MAIN_DEBUG
			cout << "GOTO_OILRIG_R_CMD" << endl;
			#endif
			break;
			
		case GOTO_OILRIG_T_CMD:
			setGPIOvalue(cmdPins.commandPin1, "0");
			setGPIOvalue(cmdPins.commandPin2, "0");
			setGPIOvalue(cmdPins.commandPin3, "0");
			setGPIOvalue(cmdPins.commandPin4, "1");
			
			#if MAIN_DEBUG
			cout << "GOTO_OILRIG_T_CMD" << endl;
			#endif
			break;
			
		case GOTO_OILRIG_C_CMD:
			setGPIOvalue(cmdPins.commandPin1, "1");
			setGPIOvalue(cmdPins.commandPin2, "0");
			setGPIOvalue(cmdPins.commandPin3, "0");
			setGPIOvalue(cmdPins.commandPin4, "1");
			
			#if MAIN_DEBUG
			cout << "GOTO_OILRIG_C_CMD" << endl;
			#endif
			break;
			
		case MOVE_FORWARD_CMD:
			setGPIOvalue(cmdPins.commandPin1, "0");
			setGPIOvalue(cmdPins.commandPin2, "1");
			setGPIOvalue(cmdPins.commandPin3, "0");
			setGPIOvalue(cmdPins.commandPin4, "1");
			
			#if MAIN_DEBUG
			cout << "MOVE_FORWARD_CMD" << endl;
			#endif
			break;

		case MOVE_BACKWARD1_CMD:
			setGPIOvalue(cmdPins.commandPin1, "1");
			setGPIOvalue(cmdPins.commandPin2, "1");
			setGPIOvalue(cmdPins.commandPin3, "0");
			setGPIOvalue(cmdPins.commandPin4, "1");
			
			#if MAIN_DEBUG
			cout << "MOVE_BACKWARD1_CMD" << endl;
			#endif
			break;
		
		case MOVE_BACKWARD2_CMD:
			setGPIOvalue(cmdPins.commandPin1, "0");
			setGPIOvalue(cmdPins.commandPin2, "0");
			setGPIOvalue(cmdPins.commandPin3, "1");
			setGPIOvalue(cmdPins.commandPin4, "1");
			
			#if MAIN_DEBUG
			cout << "MOVE_BACKWARD2_CMD" << endl;
			#endif
			break;
			
		case FINAL_PUSH_CMD:
			setGPIOvalue(cmdPins.commandPin1, "1");
			setGPIOvalue(cmdPins.commandPin2, "0");
			setGPIOvalue(cmdPins.commandPin3, "1");
			setGPIOvalue(cmdPins.commandPin4, "1");
			
			#if MAIN_DEBUG
			cout << "FINAL_PUSH_CMD" << endl;
			#endif
			break;
			
		case RETURN_CMD:
			setGPIOvalue(cmdPins.commandPin1, "0");
			setGPIOvalue(cmdPins.commandPin2, "1");
			setGPIOvalue(cmdPins.commandPin3, "1");
			setGPIOvalue(cmdPins.commandPin4, "1");
			
			#if MAIN_DEBUG
			cout << "RETURN_CMD" << endl;
			#endif
			break;
			
		case CLEAR_CMD:
			setGPIOvalue(cmdPins.commandPin1, "0");
			setGPIOvalue(cmdPins.commandPin2, "0");
			setGPIOvalue(cmdPins.commandPin3, "0");
			setGPIOvalue(cmdPins.commandPin4, "0");
			
			#if MAIN_DEBUG
			cout << "CLEAR_CMD" << endl;
			#endif
			break;
	}
}

void waitForAcknowledge(struct CommandPins cmdPins)
{
	#if SIMULATE_ACK
	string dummy;
	cout << "Acknowledge? " << endl;
	getline(cin, dummy);
	
	#else
	string ackPinValue;
	
	#if MAIN_DEBUG
	cout << "Waiting for Ack" << endl;
	#endif

	do {
		getGPIOvalue(cmdPins.ackPin, ackPinValue);
		usleep(100000);
	} while(ackPinValue == "0");
	
	#if MAIN_DEBUG
	cout << "Ack recieved" << endl;
	#endif
	
	sendCommand(cmdPins, CLEAR_CMD);
	
	do {
		getGPIOvalue(cmdPins.ackPin, ackPinValue);
		usleep(100000);
	} while(ackPinValue == "1");
	
	#if MAIN_DEBUG
	cout << "Ack cleared" << endl;
	#endif
	
	#endif
}

void insertTool(int uart_filestream, int tool)
{
	switch(tool)
	{
		case RECTANGLE:
			//Open slightly
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, 720 * 4);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, 1900 * 4);
			sleep(1);
			
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, 1375 * 4);
			usleep(800000);
			
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_R);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_R);
			sleep(1);

			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1610 * 4);	//Low Body
			
			sendCommand(cmdPins, MOVE_FORWARD_CMD);
			waitForAcknowledge(cmdPins);
			
			//Open
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
			sleep(1);

			sendCommand(cmdPins, MOVE_BACKWARD1_CMD);
			waitForAcknowledge(cmdPins);

			//Insert
			for(int i = 0; i < 5; i++)
			{
				sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1440 * 4);
				sleep(1);
				sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1600 * 4);
				sleep(1);
			}
			
			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1400 * 4);
			usleep(500000);

			sendCommand(cmdPins, MOVE_BACKWARD2_CMD);
			waitForAcknowledge(cmdPins);
			
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, PUSH_GEAR_IN);

			//Final push
			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, GRIPPER_BODY_IN);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED);
			
			sendCommand(cmdPins, FINAL_PUSH_CMD);
			waitForAcknowledge(cmdPins);
			break;
		
		case TRIANGLE:
			//Open slightly
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, 850 * 4);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, 1580 * 4);
			sleep(1);
			
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, 1400 * 4);
			sleep(1);
			
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, PUSH_GEAR_IN);
			usleep(700000);
			
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_T);
			usleep(600000);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_T);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_T);
			sleep(1);
			
			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1660 * 4); //NOTE
			sleep(1);

			sendCommand(cmdPins, MOVE_FORWARD_CMD);
			waitForAcknowledge(cmdPins);

			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1670 * 4);	//Low Body
			sleep(2);

			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, PUSH_GEAR_OUT);			
			sleep(1);
			
			//Insert
			for(int i = 0; i < 2; i++)
			{
				sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1480 * 4);
				sleep(1);
				sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1650 * 4);
				sleep(1);
				
			}

			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1480 * 4);	//Low Body a little

			sendCommand(cmdPins, MOVE_BACKWARD1_CMD);
			waitForAcknowledge(cmdPins);
			
			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1350 * 4);	//Low Body a little

			sendCommand(cmdPins, MOVE_BACKWARD2_CMD);
			waitForAcknowledge(cmdPins);
			
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, PUSH_GEAR_IN);

			//Final push
			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, GRIPPER_BODY_IN);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED);
			
			sendCommand(cmdPins, FINAL_PUSH_CMD);
			waitForAcknowledge(cmdPins);
			break;
	
		case CIRCLE:
			//Open slightly
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, 700 * 4);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, 1730 * 4);
			sleep(1);
			
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, 1080 * 4);
			sleep(1);
			
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_C);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_C);
			sleep(1);
			
			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1640 * 4);	//Low Body

			sendCommand(cmdPins, MOVE_FORWARD_CMD);
			waitForAcknowledge(cmdPins);
			
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, PUSH_GEAR_OUT);	//Low Body

			sendCommand(cmdPins, MOVE_BACKWARD1_CMD);
			waitForAcknowledge(cmdPins);

			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, 1320 * 4);	//Low Body a little
			
			sendCommand(cmdPins, MOVE_BACKWARD2_CMD);
			waitForAcknowledge(cmdPins);
			
			sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, PUSH_GEAR_IN);

			//Final push
			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, GRIPPER_BODY_IN);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED);
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED);
			
			sendCommand(cmdPins, FINAL_PUSH_CMD);
			waitForAcknowledge(cmdPins);

			//Make sure
			sendCommand(cmdPins, MOVE_BACKWARD1_CMD);
			waitForAcknowledge(cmdPins);

			sendCommand(cmdPins, MOVE_BACKWARD2_CMD);
			waitForAcknowledge(cmdPins);

			sendCommand(cmdPins, FINAL_PUSH_CMD);
			waitForAcknowledge(cmdPins);

			break;
	}
	
}

void pickTool(int uart_filestream, int tool)
{
	sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, PUSH_GEAR_IN);
	sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
	sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
	sleep(1);
	
	sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, GRIPPER_BODY_OUT);
	#if MAIN_DEBUG
	cout << "Gripper body out" << endl;
	#endif
	sleep(1);

	switch(tool)
	{
		case RECTANGLE:
			#if MAIN_DEBUG
			cout << "Holding Rectangle" << endl;
			#endif
			
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_R);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_R);
			sleep(1);
			
			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, GRIPPER_BODY_IN);
			sleep(1);

			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
			sleep(1);
			
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_R);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_R);
			sleep(1);
			break;
			
		case TRIANGLE:
			#if MAIN_DEBUG
			cout << "Holding Triangle" << endl;
			#endif
			
			for(int i = 0; i < 2; i++)
			{
				sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED);
				sleep(1);
				sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
				sleep(1);

				sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED);
				sleep(1);
				sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
				sleep(1);
			}

			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_T);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_T);
			sleep(1);

			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, GRIPPER_BODY_IN);
			sleep(1);

			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
			sleep(1);
			
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_T);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_T);
			sleep(1);
			break;
			
		case CIRCLE:
			#if MAIN_DEBUG
			cout << "Holding Circle" << endl;
			#endif
			
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_C);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_C);
			sleep(1);

			sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, GRIPPER_BODY_IN);
			sleep(1);

			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
			sleep(1);
			
			sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_CLOSED_C);
			sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_CLOSED_C);
			sleep(1);
			break;
	}
}

void threadCleanup(void *args)
{
	sendCommand(cmdPins, CLEAR_CMD);
	
	releaseCapture(capture);
	closeUART(uart_filestream);
}

void signalHandler(int signal)
{
	switch(signal)
	{
		case SIGINT:
		releaseCapture(capture);
		cout << endl << "Signal catched!" << endl;
		abort();
			break;
	}
}

void *thread(void *argument)
{
	int state;
	
	int burningOilRig;
	int tool, toolNumber;
	
	//Configure parallel port to be used with PIC16
	configureCommandPins(cmdPins);

	uart_filestream = -1;
	openUART(uart_filestream, B9600);
	sendServoCommand(uart_filestream, CAMERA_SERVO, CAMERA_REST);             //Dummy command

	capture = initCapture();

	pthread_cleanup_push(threadCleanup, (void *) &uart_filestream);
	
	state = POSITION1;

	/*
	cout << "Tool? " << endl;
	cin >> tool;

	string x;
	getline(cin, x);

	toolNumber = 2;
	
	state = PICK_TOOL;
	*/

	cout << "Initial state?" << endl;
	cin >> state;

	while(state != STOP)
	{
		string dummy;
		cout << endl << "Press enter to continue... " << endl;
		getline(cin, dummy);

		switch(state)
		{
			case POSITION1:
				#if MAIN_DEBUG
				cout << "State: " << "POSITION1" << endl;
				#endif
				
				sendServoCommand(uart_filestream, CAMERA_SERVO, CAMERA_REST);
				sendServoCommand(uart_filestream, GRIPPER_BODY_SERVO, GRIPPER_BODY_IN);
				sendServoCommand(uart_filestream, GRIPPER_RIGHT_SERVO, GRIPPER_RIGHT_OPEN);
				sendServoCommand(uart_filestream, GRIPPER_LEFT_SERVO, GRIPPER_LEFT_OPEN);
				sendServoCommand(uart_filestream, PUSH_GEAR_SERVO, PUSH_GEAR_IN);

				sendCommand(cmdPins, POSITION1_CMD);
				waitForAcknowledge(cmdPins);
				
				sendServoCommand(uart_filestream, CAMERA_SERVO, CAMERA_DETECT_FLAME);

				burningOilRig = scanBurningOilRigP1(capture);
				//burningOilRig = UNDEFINED;
				
				#if MAIN_DEBUG
				cout << "Burning oil: " ;
				
				switch(burningOilRig)
				{
					case RECTANGLE:
						cout << "RECTANGLE" << endl;
						break;
						
					case TRIANGLE:
						cout << "TRIANGLE" << endl;
						break;
						
					case CIRCLE:
						cout << "CIRCLE" << endl;
						break;
						
					default:
						cout << "UNDEFINED" << endl;
						break;
				}
				#endif
				
				state = POSITION2;
				break;
			
			case POSITION2:
				#if MAIN_DEBUG
				cout << "State: " << "POSITION2" << endl;
				#endif
				
				sendServoCommand(uart_filestream, CAMERA_SERVO, CAMERA_REST);
				
				sendCommand(cmdPins, POSITION2_CMD);
				waitForAcknowledge(cmdPins);
				
				if(burningOilRig == UNDEFINED)
				{
					sendServoCommand(uart_filestream, CAMERA_SERVO, CAMERA_DETECT_FLAME);
					burningOilRig = scanBurningOilRigP2(capture);
				}
				
				#if MAIN_DEBUG
				cout << "Burning oil: " ;
				
				switch(burningOilRig)
				{
					case RECTANGLE:
						cout << "RECTANGLE" << endl;
						break;
						
					case TRIANGLE:
						cout << "TRIANGLE" << endl;
						break;
						
					case CIRCLE:
						cout << "CIRCLE" << endl;
						break;
						
					default:
						cout << "UNDEFINED" << endl;
						break;
				}
				#endif
				
				if(burningOilRig != UNDEFINED)
					state = GOTO_LOADZONE;
				else
					state = ERROR;
				
				break;
			
			case GOTO_LOADZONE:
				#if MAIN_DEBUG
				cout << "State: " << "GOTO_LOADZONE" << endl;
				#endif
				
				sendServoCommand(uart_filestream, CAMERA_SERVO,  CAMERA_REST);
				
				sendCommand(cmdPins, GOTO_LOADZONE_CMD);
				waitForAcknowledge(cmdPins);
				
				state = SCAN_TOOL;
				toolNumber = 1;
				
				break;
			
			case SCAN_TOOL:
				#if MAIN_DEBUG
				cout << "State: " << "SCAN_TOOL" << endl;
				cout << "Scanning tool " << toolNumber << endl;
				#endif

				sendServoCommand(uart_filestream, CAMERA_SERVO, CAMERA_DETECT_TOOL);
				
				tool = scanThisFigure(capture);

				#if MAIN_DEBUG
				cout << "Tool detected: " ;
				
				switch(tool)
				{
					case RECTANGLE:
						cout << "RECTANGLE" << endl;
						break;
						
					case TRIANGLE:
						cout << "TRIANGLE" << endl;
						break;
						
					case CIRCLE:
						cout << "CIRCLE" << endl;
						break;
						
					default:
						cout << "UNDEFINED" << endl;
						break;
				}
				#endif
				
				if(tool == burningOilRig || toolNumber >= 3)
					state = PICK_TOOL;
				else
				{
					toolNumber++;
					if(toolNumber == 2)
					{
						sendCommand(cmdPins, TOOL2_CMD);
						waitForAcknowledge(cmdPins);
					}
					else
					{
						sendCommand(cmdPins, TOOL3_CMD);
						waitForAcknowledge(cmdPins);
					}

					state = SCAN_TOOL;
				}
				break;
				
			case PICK_TOOL:
				#if MAIN_DEBUG
				cout << "State: " << "PICK_TOOL" << endl;
				#endif
				
				sendServoCommand(uart_filestream, CAMERA_SERVO, CAMERA_REST);
				
				if(toolNumber == 1)
				{
					sendCommand(cmdPins, TOOL1_CMD);
					waitForAcknowledge(cmdPins);
				}
				
				pickTool(uart_filestream, tool);
				
				state = GOTO_OILRIG;
				break;
				
			case GOTO_OILRIG:
				#if MAIN_DEBUG
				cout << "State: " << "GOTO_OILRIG" << endl;
				#endif
			
				switch(burningOilRig)
				{
					case RECTANGLE:
						sendCommand(cmdPins, GOTO_OILRIG_R_CMD);
						waitForAcknowledge(cmdPins);
						
						state = INSERT_TOOL;
						break;
						
					case TRIANGLE:
						sendCommand(cmdPins, GOTO_OILRIG_T_CMD);
						waitForAcknowledge(cmdPins);
						
						state = INSERT_TOOL;
						break;
						
					case CIRCLE:
						sendCommand(cmdPins, GOTO_OILRIG_C_CMD);
						waitForAcknowledge(cmdPins);
						
						state = INSERT_TOOL;
						break;
						
					default:
						sendCommand(cmdPins, GOTO_OILRIG_T_CMD);
						waitForAcknowledge(cmdPins);
						
						state = INSERT_TOOL;
						break;
				}
				break;
				
			case INSERT_TOOL:
				#if MAIN_DEBUG
				cout << "State: " << "INSERT_TOOL" << endl;
				#endif
				
				insertTool(uart_filestream, tool);
				
				state = RETURN;
				break;
				
			case RETURN:
				#if MAIN_DEBUG
				cout << "State: " << "RETURN" << endl;
				#endif
				
				sendCommand(cmdPins, RETURN_CMD);
				waitForAcknowledge(cmdPins);
				
				state = STOP;
				break;
				
			case ERROR:
				#if MAIN_DEBUG
				cout << "State: " << "ERROR" << endl;
				#endif
				
				state = STOP;
				break;
		}
	}

	pthread_cleanup_pop(EXECUTE_CLEANUP);	//Pop and execute cleanup routine

	#if MAIN_DEBUG
	cout << "Thread finished" << endl;
	#endif

   	return NULL;
	
}

int main(void)
{
	pthread_t threadPtr;

	string inputValue;
	string pinNumber = "21";
	
	signal(SIGINT, signalHandler);

	exportGPIO(pinNumber);
	configureGPIO(pinNumber, "in");
	
	while(true)
	{
		do {
			getGPIOvalue(pinNumber, inputValue);
			usleep(500000);  // wait for 0.5 seconds
		} while(inputValue == "0");
		
		#if MAIN_DEBUG
		cout << "Thread started" << endl << endl;
		#endif
		
		pthread_create(&threadPtr, NULL, thread, NULL);

		do {
			getGPIOvalue(pinNumber, inputValue);
			usleep(500000);  // wait for 0.5 seconds
		} while(inputValue == "1");
		
		pthread_cancel(threadPtr);
		
		#if MAIN_DEBUG
		cout << "Thread interrupted" << endl << endl;
		#endif
		
	}
	
	unexportGPIO(pinNumber);

	return 0;

}
