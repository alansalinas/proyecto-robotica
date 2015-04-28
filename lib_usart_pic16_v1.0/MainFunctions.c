
#include <xc.h>
#include <stdio.h>

#include "pin_definitions.h"
#include "CoreFunctions.h"
#include "ultrasonic.h"
#include "motors.h"
#include "usart_pic16.h"
#include "control.h"

//unsigned long t;
char times;

void SwitchToPing()
{
    Control(HC,POS_SETPOINT_HC);
    rotateLeft(SWITCH_SPEED);
    __delay_ms(ROTATION_DELAY);
    stopMotors();
    Control(PING,POS_SETPOINT_PING);
}

void SwitchToHC()
{
    Control(PING,POS_SETPOINT_PING);
    rotateRight(SWITCH_SPEED);
    __delay_ms(ROTATION_DELAY);
    stopMotors();
    Control(HC, POS_SETPOINT_HC);
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


void rigs_pos_inicial()
{
    backward(50);
    __delay_ms(1000);
    forward(100);
    __delay_ms(200);
    
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


