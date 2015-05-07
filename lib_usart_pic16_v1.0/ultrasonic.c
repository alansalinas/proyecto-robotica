#include <xc.h>
#include <stdio.h>
//#include <stdlib.h>
#include "pin_definitions.h"
#include "CoreFunctions.h"



unsigned short time;
char seesleft,seesright;


void setup_sensors(){
    //ANSEL = 0;
    ANSELH = 0;
  WPUB = 0x00; // disable pullup resistors con portB
  ultraR_mode = OUTPUT;
  ultraR = HIGH ;// Trig pin is normally HIGH
  ultraL_mode = OUTPUT;
  ultraL = HIGH; // Trig pin is normally HIGH

  triggerL_mode = OUTPUT;
  echoL_mode = INPUT;

  triggerR_mode = OUTPUT;
  echoR_mode = INPUT;


}


//
// Sensor reading routine
void readSensors()
{
  delay(6);  // delay for echo
  ultraL_mode = OUTPUT; //return digital pin to OUTPUT mode after reading
  ultraL = LOW;
  delayMicroseconds(10);

  ultraL = HIGH; //Trig pin pulsed LOW for 25usec
  ultraL_mode = INPUT; //change pin to INPUT to read the echo pulse
  time = pulseInLeft(); //should be approx 150usec, timeout at 18msec

  uLeft = (time*340.29/2/1000)+4;   // time to distance conversion

  //
  // RIGHT SENSOR
  //
    delay(8);  // delay for echo
  ultraR_mode = OUTPUT; //return digital pin to OUTPUT mode after reading
  ultraR = LOW;
  delayMicroseconds(10);

  ultraR = HIGH; //Trig pin pulsed LOW for 25usec
  ultraR_mode = INPUT; //change pin to INPUT to read the echo pulse
  time = pulseInRight(); //should be approx 150usec, timeout at 18msec

  uRight = (time*340.29/2/1000)-3 + 7; // convert to distance in millimeters

}  // end readSensors


void readHC(){
    //
    // Read Left sensor
    //
     delayMicroseconds(2000); // !!!!
    triggerL = LOW;
    __delay_us(4);
    triggerL = 1;
    __delay_us(10);
    triggerL = LOW;
    time = pulseInHC_L();

    uLeft = (time*340.29/2/1000)-4; // convert to distance in millimeters

    //
    // Read Right HRC sensor
    //
    delayMicroseconds(3000); // !!!!

    triggerR = LOW;
    __delay_us(4);
    triggerR = 1;
    __delay_us(10);
    triggerR = LOW;
    time = pulseInHC_R();

    uRight = (time*340.29/2/1000)-4; // convert to distance in millimmeters
}


char seesOne(){
    if (uLeft < MAX_DISTANCE || uRight < MAX_DISTANCE)
    {
        return 1;
    }
    return 0;
}

char seesBoth(){
    if (uLeft < MAX_DISTANCE && uRight < MAX_DISTANCE)
    {
        return 1;
    }
    return 0;
}

void sees_debounce()
{
    char times = 0;
    unsigned char seesleftcount= 0;
    unsigned char seesrightcount = 0;

    while(times < 7)
    {
        readSensors();
        __delay_ms(10);
        if(uLeft > MAX_DISTANCE)
            seesleftcount++;
        else
            seesleftcount--;

        if(uRight > MAX_DISTANCE)
            seesrightcount++;
        else
            seesrightcount--;

        //printf("uleft: %d, uright: %d\n",uLeft,uRight);

        times++;
    }

    if (seesleftcount < 200)
        seesleft = FALSE;
    else
        seesleft = TRUE;

    if (seesrightcount < 200)
        seesright = FALSE;
    else
        seesright = TRUE;

    printf("SeesL: %d, Lcount: %u, SeesR: %d, Rcount: %u\n",seesleft,seesleftcount,seesright,seesrightcount);


}