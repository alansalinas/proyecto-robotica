#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
using namespace std;

int exportGPIO(string gpioNum)
{
	//Export GPIO
	string exportString = "/sys/class/gpio/export";
	ofstream exportFile(exportString.c_str());	// Open "export" file.
    	
	if (exportFile < 0){
        	cout << " OPERATION FAILED: Unable to export GPIO"<< gpioNum << endl;
        	return -1;
    	}
	
	exportFile << gpioNum;	//Write GPIO number to export
	exportFile.close(); //Close export file
	
	return 0;
}

int exportGPIO(string gpioNum, string mode)
{
	//Export GPIO
	string exportString = "/sys/class/gpio/export";
	ofstream exportFile(exportString.c_str());	// Open "export" file.
    	
	if (exportFile < 0){
        	cout << " OPERATION FAILED: Unable to export GPIO"<< gpioNum << endl;
        	return -1;
    	}
	
	exportFile << gpioNum;	//Write GPIO number to export
	exportFile << " " << mode;
	exportFile.close(); //Close export file
	
	return 0;
}

int unexportGPIO(string gpioNum)
{
	//Unexport GPIO
	string unexportString = "/sys/class/gpio/unexport";
	ofstream unexportFile(unexportString.c_str());	// Open "unexport" file.
    	
	if (unexportFile < 0){
        	cout << " OPERATION FAILED: Unable to unexport GPIO"<< gpioNum << endl;
        	return -1;
    	}
	
	unexportFile << gpioNum;	//Write GPIO number to export
	unexportFile.close(); 		//Close unexport file
	
	return 0;
}

int configureGPIO(string gpioNum, string direction)
{	
	//Set GPIO Direction
    	string directionString ="/sys/class/gpio/gpio" + gpioNum + "/direction";
    	ofstream directionFile(directionString.c_str());	//Open direction file for GPIO
	
	if (directionFile < 0){
		cout << " OPERATION FAILED: Unable to set direction of GPIO"<< gpioNum << endl;
		return -1;
	}

	directionFile << direction;	//Write direction to direction file
	directionFile.close();			//Close direction file
	
	return 0;
}

int setGPIOvalue(string gpioNum, string value)
{
	string valueString = "/sys/class/gpio/gpio" + gpioNum + "/value";
	ofstream valueFile(valueString.c_str()); // open value file for GPIO
	if (valueFile < 0){
		cout << " OPERATION FAILED: Unable to set the value of GPIO"<< gpioNum << endl;
		return -1;
	}
 
	valueFile << value;//write value to value file
	valueFile.close();// close value file 
	return 0;
}

int getGPIOvalue(string gpioNum, string &value)
{
	string valueString = "/sys/class/gpio/gpio" + gpioNum + "/value";
	ifstream valueFile(valueString.c_str());	//Open value file for GPIO
	if (valueFile < 0) {
		cout << " OPERATION FAILED: Unable to get value of GPIO"<< gpioNum <<" ."<< endl;
		return -1;
	}
	
	valueFile >> value ;  //Read GPIO value
 
	if(value != "0")
		value = "1";
	
	//cout << "X" << endl;

	valueFile.close(); //Close the value file
	return 0;
}