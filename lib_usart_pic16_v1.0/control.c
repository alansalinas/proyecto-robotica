
#include <stdio.h>
#include <xc.h>
#include "pin_definitions.h"
#include "CoreFunctions.h"
#include "motors.h"
#include "ultrasonic.h"

unsigned long t;
unsigned char times;


void rotationControl(char sensor_set)
{
  short error;

  if(sensor_set == PING)
  readSensors();
  else
      readHC();

  error = (uLeft - uRight);
  //printf("\n enter rot control: %d\n",error);

  while((error > ERR_ROT || error < -ERR_ROT) && error < MAXROT && error > -MAXROT){
   if (millis() - lastreading >= TS)
    {
    
    error = (uLeft - uRight);
      if(sensor_set == PING)
        readSensors();
        else
        readHC();


   if (seesBoth()){
    //printf("EFECTIVE error ROT:  %d\n", error);
     //t = 0;
    if (error < MAXROT && error > -MAXROT){
    if (error > ERR_ROT)
      rotateRight(MAX_ROT_VEL);
     else if (error < -ERR_ROT)
     rotateLeft(MAX_ROT_VEL);
    }

    }

     lastreading = millis();
  } // end if TS

  } // end while
  stopMotors();
}

void positionControlPing(short setpoint)
{
   int error;
   // Comprobar si hay error, detener movimientos previos
   readSensors();
   error = setpoint - (uLeft+uRight) - absval(uLeft - uRight);

  while((error > ERR_POS || error < -ERR_POS) && error < MAXPOS+100 && error > -MAXPOS-100){
     if (millis() - lastreading >= TS)
    {
     //t = 0;

    error = setpoint - (uLeft+uRight) - absval(uLeft - uRight);
    readSensors();

    if (seesBoth() == 1){

    //printf("EFECTIVE pos control ping: %d\n", error);
    if (error < MAXPOS+100 && error > -MAXPOS-100){
    if (error > ERR_POS)
      backward(MAX_VEL);
     else if (error < -ERR_POS)
     forward(MAX_VEL);
    }

    lastreading = millis();

    }   // end if TS
    
     }// end if seesBoth
  } // end while
   stopMotors();
}


void positionControlHC(short setpoint)
{
   int error;
   // Comprobar si hay error, detener movimientos previos
   readHC();
   error = setpoint - (uLeft+uRight) - absval(uLeft - uRight);

  while((error > ERR_POS || error < -ERR_POS) && error < MAXPOS+100 && error > -MAXPOS-100){
     if (millis() - lastreading >= TS)
    {
         t = 0;

    error = setpoint - (uLeft+uRight) - absval(uLeft - uRight);
    readHC();

    if (seesBoth() == 1){
    //printf("EFECTIVE pos control HC: %d\n", error);
    
    if (error < MAXPOS+100 && error > -MAXPOS-100){
    if (error > ERR_POS)
      right(MAX_VEL);
     else if (error < -ERR_POS)
     left(MAX_VEL);
    }
    }

    lastreading=millis();

    }   // end if TS

  } // end while

   stopMotors();
}

void positionControlHC_pieza(short setpoint)
{
   int error;
   // Comprobar si hay error, detener movimientos previos
   readHC();
   error = setpoint - uRight;

  while((error > ERR_POS || error < -ERR_POS) && error < MAXPOS+100 && error > -MAXPOS-100){
     if (millis() - lastreading >= TS)
    {
         times = 0;

    error = setpoint - uRight;
    readHC();

    if (seesBoth() == 1){
    printf("EFECTIVE pos control HC pieza: %d\n", uRight);

    if (error < MAXPOS+100 && error > -MAXPOS-100){
    if (error > ERR_POS)
      right(60);
     else if (error < -ERR_POS)
     left(60);
    }
    }

    lastreading=millis();

    }   // end if TS

  } // end while
   times++;
   printf("TIMES UPDATE: %u\n",times);
   stopMotors();
}

void positionControlPing_pieza(short setpoint)
{
   int error;
   unsigned long t_2 = millis();
   times = 0;

   while (times < 8){

   // Comprobar si hay error, detener movimientos previos
   readSensors();
   error = setpoint - uRight;

  while((error > ERR_POS || error < -ERR_POS)){ // && error < MAXPOS+100 && error > -MAXPOS-100
     if (millis() - lastreading >= TS)
    {
         times = 0;
    error = setpoint - uRight;
    readSensors();

    //printf("EFECTIVE PIEZA control: %d\n", error);
    //if (error < MAXPOS+100 && error > -MAXPOS-100){
    if (error > ERR_POS)
      backward(MAX_VEL);
     else if (error < -ERR_POS)
     forward(MAX_VEL);
    //}

    lastreading = millis();

    }   // end if TS


     if(millis() - t_2 > 2000)
     {
            left(50);
            __delay_ms(100);
            t_2 = millis();

     }

  } // end while
  stopMotors();
  times++;
  printf("TIMES UPDATE: %u\n",times);
   }// end main while (times)
}


void Control(char sensor_set)
{
    t=0;
    printf("\nCONTROLLING...\n");
    while (t < 8){
          rotationControl(sensor_set);

          if (sensor_set == HC){
            positionControlHC(POS_SETPOINT_HC);}
          else{
              positionControlPing(POS_SETPOINT_PING);}
          
            
           t++;
    }
    stopMotors();
    printf("CONTROLLED\n");
           
}

void ControlPieza()
{
    times = 0;
    printf("ENTER CONTROL LOOP\n");
    while (times < 8)
    {
        positionControlHC_pieza(POS_3);
    }
    printf("EXIT CONTROL LOOP\n");

}
