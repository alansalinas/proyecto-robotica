
#include <xc.h>
#include <stdio.h>

#include "pin_definitions.h"
#include "CoreFunctions.h"
#include "ultrasonic.h"
#include "motors.h"
#include "usart_pic16.h"
#include "control.h"

unsigned long t;
char times;

void goToPos1(){
    Control(PING, POS_SETPOINT_PING);
}

void goToPos2(){
    Control(PING, POS_SETPOINT_PING);
    right(120);
    Control(PING, POS_SETPOINT_PING);
    __delay_ms(2500);
    stopMotors();
}


void SwitchToPing()
{
    printf("\nBEGIN\n");
        printf("CONTROL HC\n");
        Control(HC, POS_SETPOINT_HC);

        printf("rotate LEFT\n");
        rotateLeft(SWITCH_SPEED);
        __delay_ms(ROTATION_DELAY);
        printf("STOP\n");
        stopMotors();

        printf("CONTROL PING\n");
        Control(PING, POS_SETPOINT_PING);

        printf("FINISH\n");
}


void SwitchToHC()
{
    printf("\nBEGIN\n");
        printf("CONTROL HC\n");
        Control(PING, POS_SETPOINT_PING);

        printf(" rotate right\n");
        rotateRight(SWITCH_SPEED);
        __delay_ms(ROTATION_DELAY);
        printf("STOP\n");
        stopMotors();

        printf("CONTROL PING\n");
        Control(HC, POS_SETPOINT_HC);

        printf("FINISH\n");
}

void goToPieza(){
    SwitchToHC();
    printf("GO TO PIEZA 1\n");
    left(50);
    __delay_ms(800);

    printf("FINISHED pos2, control de pieza->seeking...\n");

    positionControlPing_pieza(100);
    stopMotors();
    printf("FOUND LOAD ZONE\n");
    __delay_ms(500);
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
            Control(PING, POS_SETPOINT_PING);

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

void buscaRig(char init, char dir, unsigned short limit)
{
    char seesRight = !init;
    char actualRight;
    unsigned long t, t_control;

    t = millis();
    times = 0;

    printf("EMPIEZA BUSCA RIG init: %d, seesleft: %d\n",init,seesRight);
    if(dir == LEFT)
        left(77);
    else
        right(77);

    do{
        if(millis() - t > TS)
        {
            readSensors();
            t = millis();
        }
        
        // ajustar bandera para debounce
        if(uRight > limit)
                actualRight = FALSE;
            else
                actualRight = TRUE;

            if(seesRight != actualRight)
            {
                times++;
                printf("times update: %d, distancer: %u\n", times, uRight);
            }else
            {
                times = 0;
                printf("times update: %d, distancer: %u\n", times, uRight);
            }

            if(times >= DEBOUNCE_COUNT && actualRight != seesRight)
                seesRight = actualRight;

    }while(seesRight != init);

        stopMotors();
        printf("FOUND EDGE RIG \n");
        
}   // end busca oilrig


void grab_pieza1()
{
    printf("backwarding\n");
    backward(70);
    __delay_ms(100);
    right(100);
    __delay_ms(300);
    printf("PING SWITCHED\n");
    
    // SWITCH TO PING W/O HC CONTROL
    rotateLeft(SWITCH_SPEED);
        __delay_ms(ROTATION_DELAY);
        stopMotors();

        printf("CONTROL PING\n");
        Control(PING, POS_SETPOINT_PING);

    printf("aligning\n");
    forward(50);
    __delay_ms(1500);
    stopMotors();
    printf("backward\n");
    backward(100);
    __delay_ms(550);
    stopMotors();
    
    buscaRig(TRUE, RIGHT, PIEZA1_THRESHOLD);

    right(100);
    __delay_ms(PIEZA1_DELAY);
    stopMotors();

    Control(PING, PIEZA1_SETPOINT);

}
