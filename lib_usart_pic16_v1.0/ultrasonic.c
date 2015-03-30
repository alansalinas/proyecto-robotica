#include <xc.h>
#include "pin_definitions.h"
#include "CoreFunctions.h"


unsigned long time;
unsigned long sizeofpulse;
unsigned int distanceR[ORDER], distanceL[ORDER];





void setup_sensors(){
    WPUB = 0x00;
  ultraR_mode = OUTPUT;
  ultraR = HIGH ;// Trig pin is normally HIGH
  ultraL_mode = OUTPUT;
  ultraL = HIGH; // Trig pin is normally HIGH

  trigger_mode = OUTPUT;
  echo_mode = INPUT;

}


//
// Sensor reading routine
void readSensors()
{
  ultraL = OUTPUT; //return digital pin to OUTPUT mode after reading
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
  distanceL[ORDER-1] = (time*340.29/2/1000)-3; // convert to distance in centimeters

  uLeft=0;

  for(int i = 0; i < ORDER; i++){
   uLeft += distanceL[i];
  }

  uLeft = uLeft / ORDER;

  delayMicroseconds(3000);

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

  //a = a/58.82;

  // Signal filtering
  for (int i = 1; i < ORDER; i++){
    distanceR[i-1] = distanceR[i];
  }
  distanceR[ORDER-1] = (time*340.29/2/1000)-3 + 7; // convert to distance in centimeters

  uRight=0;

  for(int i = 0; i < ORDER; i++){
   uRight += distanceR[i];
  }

  uRight = uRight / ORDER;

  delayMicroseconds(3000); // !!!!

  //Serial.print(distanceL[ORDER-1]);Serial.print("-");;Serial.println(distanceR[ORDER-1]);

}  // end readSensors


void readHC(){
    sizeofpulse=0;
    trigger = LOW;
    __delay_us(2);
    trigger = HIGH;
    __delay_us(10);
    trigger = LOW;
    time = micros(); //record timer
    sizeofpulse = pulseInHC();

    time = sizeofpulse; // amount of time elapsed since we sent the trigger pulse and detect the echo pulse, then subtract the size of the echo pulse


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

  delayMicroseconds(2000); // !!!!
}

