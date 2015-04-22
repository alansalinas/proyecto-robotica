
#include <stdio.h>
#include <xc.h>
#include "pin_definitions.h"
#include "CoreFunctions.h"
#include "motors.h"
#include "ultrasonic.h"

unsigned long t;

short errbuf[ORDER], med[5], sortbuf[5];



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


    //printf("uleft: %d, uright: %d\n", uLeft, uRight);

   if (seesBoth()){
    //printf("EFECTIVE error ROT:  %d\n", error);
     //t = 0;
    if (error < MAXROT && error > -MAXROT){
    if (error > ERR_ROT)
      rotateRight(MAX_ROT_VEL);//rotateRight(pid_rot_L(error));
     else if (error < -ERR_ROT)
     rotateLeft(MAX_ROT_VEL);//rotateLeft(pid_rot_R(error-(2*error)));
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
    //     t = 0;
    //if(uRight > 0 && uLeft > 0)
    //rotationControl(PING);
    error = setpoint - (uLeft+uRight) - absval(uLeft - uRight);
    readSensors();

    if (seesBoth() == 1){

    //printf("EFECTIVE pos control ping: %d\n", error);
    //Serial.print(uLeft);Serial.print("-");Serial.print(uRight);Serial.print("=pos=");Serial.println(error);
    if (error < MAXPOS+100 && error > -MAXPOS-100){
    if (error > ERR_POS)
      backward(MAX_VEL);//backward(pid_pos(error));
     else if (error < -ERR_POS)
     forward(MAX_VEL);//forward(pid_pos(error-(2*error)));
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
    }else
    {
        printf("\n CANT SEE HC rot\n");
        forward(140);
        __delay_ms(600);
        stopMotors();
    }

    lastreading=millis();

    }   // end if TS

  } // end while
}

void positionControlPieza(short setpoint)
{
   int error;

   // Comprobar si hay error, detener movimientos previos
   readSensors();
   error = setpoint - uRight;

  while((error > ERR_POS || error < -ERR_POS) && error < MAXPOS+100 && error > -MAXPOS-100){
     if (millis() - lastreading >= TS)
    {
      t = 0;
    //if(uRight > 0 && uLeft > 0)
    //rotationControl();
    error = filter(setpoint - uRight);
    readSensors();

    //printf("EFECTIVE PIEZA control: %d\n", error);
    //Serial.print(uLeft);Serial.print("-");Serial.print(uRight);Serial.print("=pos=");Serial.println(error);
    if (error < MAXPOS+100 && error > -MAXPOS-100){
    if (error > ERR_POS)
      backward(MAX_VEL);
     else if (error < -ERR_POS)
     forward(MAX_VEL);
    }

    lastreading = millis();

    }   // end if TS

  } // end while

}


void Control(char sensor_set)
{
    unsigned long ms;
    ms = 0;
    t=0;
    printf("\nCONTROLLING...\n");
    while (t < 10){
          rotationControl(sensor_set);

          if (sensor_set == HC){
            positionControlHC(500);}
          else{
              positionControlPing(500);}
          
            
           t++;
    }
    stopMotors();
    printf("CONTROLLED\n");
           
}
