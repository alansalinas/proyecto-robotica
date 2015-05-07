#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>

#include <stdio.h>
#include <unistd.h>		//Used for UART
#include <fcntl.h>		//Used for UART
#include <termios.h>		//Used for UART
 
using namespace std;

void openUART(int &uart0_filestream, int baudrateFlags)
{
	// UART
	//	Access modes:
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables non blocking mode.
	//
	//	O_NOCTTY - When set, open() shall not cause the terminal device to become the controlling terminal for the process.
	
	//Open the UART filestream
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//Print error
		printf("Error - Unable to open UART.\n");
	}
	
	// Flags:
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	
	//Configure the UART
	struct termios options;
	tcgetattr(uart0_filestream, &options);	//Get current flags
	//Modify flags
	options.c_cflag = CS8 | CLOCAL | CREAD | baudrateFlags;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);	//Flush received data
	tcsetattr(uart0_filestream, TCSANOW, &options);	//Set new flags
}

void closeUART(int &uart0_filestream)
{
	//Close the UART filestream
	close(uart0_filestream);
}

void transmitUART(int &uart0_filestream, unsigned char *ptrTxBuffer, int nBytes)
{
	if ( uart0_filestream != -1)
	{
		int count = write(uart0_filestream, ptrTxBuffer, nBytes);		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			printf("UART TX error\n");
		}
	}
}

void receiveUART(int &uart0_filestream, unsigned char *ptrRxBuffer, int maxBytes, int &nBytes)
{
	if (uart0_filestream != -1)
	{
		// Read up to 255 characters from the port if they are there
		nBytes = read(uart0_filestream, ptrRxBuffer, maxBytes);		//Filestream, buffer to store in, number of bytes to read (max)
		if (nBytes < 0)
		{
			//An error occured (will occur if there are no bytes)
			cout << "No bytes received" << endl;
		}
		else if (nBytes == 0)
		{
			//No data waiting
		}
	}
}