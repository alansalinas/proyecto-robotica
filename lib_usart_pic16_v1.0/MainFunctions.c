
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

            //printf("%d, %d\n", uLeft, uRight);
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
        forward(45);
        __delay_ms(1000);
        backward(100);
        __delay_ms(200);
        stopMotors();
   
}


void rigs_pos_inicial()
{
    backward(50);
    __delay_ms(1000);
    forward(100);
    __delay_ms(200);
    
}

void buscaRig(char init, char sensor, char dir, unsigned short limit)
{
    char sees_sensor = !init;
    unsigned short reading;
    char actualSensor;
    unsigned long t, t_control;

    t = millis();
    times = 0;

    printf("EMPIEZA BUSCA RIG init: %d, seesleft: %d, SENSOR: %d\n",init,sees_sensor,sensor);
    if(dir == LEFT)
        left(70);
    else
        right(70);

    do{
        if(millis() - t > TS)
        {
            readSensors();
            t = millis();
        }

        if(sensor == RIGHT)
            reading = uRight;
        else
            reading = uLeft;
        
        // ajustar bandera para debounce
        if(reading > limit)
                actualSensor = FALSE;
            else
                actualSensor = TRUE;

            if(sees_sensor != actualSensor)
            {
                times++;
                printf("times update: %d, distancer: %u\n", times, reading);
            }else
            {
                times = 0;
                printf("times update: %d, distancer: %u\n", times, reading);
            }

            if(times >= DEBOUNCE_COUNT && actualSensor != sees_sensor)
                sees_sensor = actualSensor;

    }while(sees_sensor != init);

        stopMotors();
        printf("FOUND EDGE RIG \n");
        
}   // end busca oilrig


