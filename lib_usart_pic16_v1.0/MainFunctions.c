
#include <xc.h>
#include <stdio.h>

#include "pin_definitions.h"
#include "CoreFunctions.h"
#include "ultrasonic.h"
#include "motors.h"
#include "usart_pic16.h"
#include "control.h"

unsigned long t;

void goToPos1(){
    Control(PING);
}

void goToPos2(){
    Control(PING);
    right(120);
    __delay_ms(2500);
    stopMotors();
}



void SwitchToPing()
{
    printf("\nBEGIN\n");
        printf("CONTROL HC\n");
        Control(HC);

        printf("rotate LEFT\n");
        rotateLeft(140);
        __delay_ms(ROTATION_DELAY);
        printf("STOP\n");
        stopMotors();

        printf("CONTROL PING\n");
        Control(PING);

        printf("FINISH\n");
}


void SwitchToHC()
{
    printf("\nBEGIN\n");
        printf("CONTROL HC\n");
        Control(PING);

        printf(" rotate right\n");
        rotateRight(140);
        __delay_ms(ROTATION_DELAY);
        printf("STOP\n");
        stopMotors();

        printf("CONTROL PING\n");
        Control(HC);

        printf("FINISH\n");
}

void goToPieza(){
    SwitchToHC();

}

void sweep(char direction){
     //if(millis() - t > 1500){
     //       Control(PING);
     //       t = millis();
     //}
     
     //printf("seesone: %d\n", seesOne());

     if(seesOne()){
     
            if (direction == LEFT)
                left(SWEEP_SPEED);
            else
                right(SWEEP_SPEED);
     }else
         stopMotors();

            //printf("SWEEP");
}


void step(char direction)
{
    switch(direction)
    {
        case LEFT:
            left(STEP_VEL);
            break;

        case RIGHT:
            right(STEP_VEL);
            break;

        case FORW:
            forward(STEP_VEL);
            break;

        case BACKW:
            backward(STEP_VEL);
            break;

        default:
            break;
    }
    
    __delay_ms(STEP_SIZE);
}

void buscaHoyo(char initial_direction){

    unsigned long t, t_control;
    char dire;
    dire = initial_direction;

    do{
        if(millis() - t_control > CONTROL_TIME)
        {
            Control(PING);

            printf("%d, %d\n", uLeft, uRight);
            t_control = millis();
        }

        if(millis() - t > TS){

          readLimits();
          if(limitR < RIGHT_LIMIT)
              dire=LEFT;

          if(limitL < LEFT_LIMIT)
              dire=RIGHT;

          if(dire==LEFT)
              left(SWEEP_SPEED);
          else
              right(SWEEP_SPEED);
          //printf("right limit: %u, left limit: %u\n", limitR,limitL);

          readSensors();
          t=millis();
        }
        }while (seesOne());
        
        // continue motion for a small delay to center robot
        __delay_ms(100);
        stopMotors();
   
}

void goto_oilrigs()
{
    printf("START OIL RIG NAVIGATION\n");
    printf("FIRST BUMP\n");

    buscaHoyo(LEFT);
    forward(JUMP_SPEED);
    __delay_ms(JUMP_DELAY);
        stopMotors();
    printf("SECOND BUMP\n");

        buscaHoyo(LEFT);
    forward(JUMP_SPEED);
    __delay_ms(JUMP_DELAY);
        stopMotors();

    printf("THIRD BUMP\n");

      buscaHoyo(LEFT);
    forward(JUMP_SPEED);
    __delay_ms(JUMP_DELAY);
        stopMotors();

    printf("OIL RIGS ZONE OK\n");
}

void buscaRig()
{
    char dire = RIGHT;
    unsigned long t, t_control;

    t = millis();

    do{
        if(millis() - t_control > CONTROL_TIME)
        {
            Control(PING);

            printf("%d, %d\n", uLeft, uRight);
            t_control = millis();
        }
    
    if(millis() - t > TS){

          readLimits();
          if(limitR < RIGHT_LIMIT)
              dire=LEFT;

          if(limitL < LEFT_LIMIT)
              dire=RIGHT;

          if(dire==LEFT)
              left(SWEEP_SPEED);
          else
              right(SWEEP_SPEED);
          //printf("right limit: %u, left limit: %u\n", limitR,limitL);

          readSensors();
          t=millis();
        }

        }while (!seesBoth());

        stopMotors();
        printf("FOUND FIRST RIG\n");
        
}   // end busca oilrig

