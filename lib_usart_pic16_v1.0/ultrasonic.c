#include <xc.h>
#include <stdio.h>
//#include <stdlib.h>
#include "pin_definitions.h"
#include "CoreFunctions.h"



unsigned long time;
unsigned long sizeofpulse;
unsigned short distanceR[ORDER], distanceL[ORDER], bufR[5], sortR[5], bufL[5], sortL[5];






void setup_sensors(){
    WPUB = 0x00;
  ultraR_mode = OUTPUT;
  ultraR = HIGH ;// Trig pin is normally HIGH
  ultraL_mode = OUTPUT;
  ultraL = HIGH; // Trig pin is normally HIGH

  triggerL_mode = OUTPUT;
  echoL_mode = INPUT;

  triggerR_mode = OUTPUT;
  echoR_mode = INPUT;

}

static unsigned short sort5(unsigned short* d){
#define SWAP(x,y) if (d[y] < d[x]) { unsigned short tmp = d[x]; d[x] = d[y]; d[y] = tmp; }
    SWAP(0, 1);
    SWAP(2, 3);
    SWAP(0, 2);
    SWAP(1, 4);
    SWAP(0, 1);
    SWAP(2, 3);
    SWAP(1, 2);
    SWAP(3, 4);
    SWAP(2, 3);
#undef SWAP
}


//
// Sensor reading routine
void readSensors()
{
  ultraL_mode = OUTPUT; //return digital pin to OUTPUT mode after reading
  ultraL = LOW;
  delayMicroseconds(10);
  ultraL = HIGH; //Trig pin pulsed LOW for 25usec
  time = micros(); //record timer
  ultraL_mode = INPUT; //change pin to INPUT to read the echo pulse
  sizeofpulse = pulseInLeft(ultraL); //should be approx 150usec, timeout at 18msec

  if (sizeofpulse == 0 )
    sizeofpulse = 180000;

  time = micros() - time - sizeofpulse; // amount of time elapsed since we sent the trigger pulse and detect the echo pulse, then subtract the size of the echo pulse

  // Signal filtering
   for (int i = 1; i < ORDER; i++){
    distanceL[i-1] = distanceL[i];
  }
  distanceL[ORDER-1] = (time*340.29/2/1000)+4; // convert to distance in millimeters

  uLeft=0;
  //printf("distance L: %d,%d,%d,%d,%d\n",distanceL[0],distanceL[1],distanceL[2],distanceL[3],distanceL[4]);
  for(int i = 0; i < ORDER; i++){
   uLeft += distanceL[i];
  }

  uLeft = uLeft / ORDER;

  // set up median filter buffer
  for (int i = 1; i < ORDER; i++){
    bufL[i-1] = bufL[i];
  }
  bufL[4] = uLeft;

  //printf("SOrt5 ANTES: %d,%d,%d,%d,%d\n",bufL[0],bufL[1],bufL[2],bufL[3],bufL[4]);

  // copying and sorting
  for (int i = 0; i < ORDER; i++)
  {
      sortL[i] = bufL[i];
  }
  sort5(&sortL);

  //printf("SOrt5 DESPUES: %d,%d,%d,%d,%d\n",bufL[0],bufL[1],bufL[2],bufL[3],bufL[4]);
  uLeft = sortL[2];

  delayMicroseconds(7200);

  ultraR_mode = OUTPUT; //return digital pin to OUTPUT mode after reading
  ultraR = LOW;
  delayMicroseconds(10);
  ultraR = HIGH; //Trig pin pulsed LOW for 25usec
  time = micros(); //record timer
  ultraR_mode = INPUT; //change pin to INPUT to read the echo pulse
  sizeofpulse = pulseInRight(ultraR); //should be approx 150usec, timeout at 18msec

  if (sizeofpulse == 0 )
    sizeofpulse = 180000;

  time = micros() - time - sizeofpulse; // amount of time elapsed since we sent the trigger pulse and detect the echo pulse, then subtract the size of the echo pulse


  // Signal filtering
  for (int i = 1; i < ORDER; i++){
    distanceR[i-1] = distanceR[i];
  }
  distanceR[ORDER-1] = (time*340.29/2/1000)-3 + 7; // convert to distance in millimeters

  uRight=0;

  for(int i = 0; i < ORDER; i++){
   uRight += distanceR[i];
  }

  uRight = uRight / ORDER;

  // set up median filter buffer
  for (int i = 1; i < ORDER; i++){
    bufR[i-1] = bufR[i];
  }
  bufR[4] = uRight;

  // copying and sorting
  for (int i = 0; i < ORDER; i++)
  {
      sortR[i] = bufR[i];
  }
  sort5(&sortR);

  uRight = sortR[2];

  delayMicroseconds(6000); // !!!!

  //Serial.print(distanceL[ORDER-1]);Serial.print("-");;Serial.println(distanceR[ORDER-1]);

}  // end readSensors


void readHC(){

    //
    // Read Left sensor
    //
     delayMicroseconds(2000); // !!!!
     
    //time=0;
    triggerL = LOW;
    __delay_us(4);
    triggerL = 1;
    __delay_us(10);
    triggerL = LOW;
    time = pulseInHC_L();

   

  // Signal filtering
  for (int i = 1; i < ORDER; i++){
    distanceL[i-1] = distanceL[i];
  }
  distanceL[ORDER-1] = (time*340.29/2/1000)-4; // convert to distance in centimeters

  uRight=0;

  for(int i = 0; i < ORDER; i++){
   uLeft += distanceL[i];
  }

  uLeft = uLeft / ORDER;
 

  //uLeft = (time*340.29/2/1000);


    //
    // Read Right HRC sensor
    //
    delayMicroseconds(2000); // !!!!

    time=0;
    triggerR = LOW;
    __delay_us(4);
    triggerR = 1;
    __delay_us(10);
    triggerR = LOW;
    time = micros(); //record timer
    time = pulseInHC_R();


  // Signal filtering
  for (int i = 1; i < ORDER; i++){
    distanceR[i-1] = distanceR[i];
  }
  distanceR[ORDER-1] = (time*340.29/2/1000)-4; // convert to distance in centimeters

  uRight=0;

  for(int i = 0; i < ORDER; i++){
   uRight += distanceR[i];
  }

  uRight = uRight / ORDER;

 
}
