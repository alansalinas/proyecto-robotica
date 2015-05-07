/*
 * File:   robotmain.c
 * Author: alansalinas
 *
 * Created on March 29, 2015, 12:10 AM
 */


#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include "CoreFunctions.h"

#include "pin_definitions.h"
#include "ultrasonic.h"
#include "usart_pic16.h"
#include "MainFunctions.h"
#include "motors.h"
#include "control.h"
#include "comm.h"

// Configuration Bits
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config LVP = OFF


/*
Global program variables
*/

char rig;
unsigned long alinea_t;

//unsigned long t;

void setup() {

    OSCCON = 0; // clear osc control bits
    ANSELH = 0x00; // digital outs on PORTB
    TRISC = 0; // PORTC is for output
    TRISCbits.TRISC4 = 0;

    setup_timer0(); // timer0 - timekeeping services
    setup_timer1(); // timer1 - pulsein measurements
    setup_timer2(); // timer2 - pwm services

    setup_sensors(); // ultrasonic setup
    setup_motors(); // hbrdige controller setup
    setup_port();
    setup_adc();

    USARTInit(9600);

}

void alinear()
{
    if(millis() - alinea_t > 2500)
    {
        backward(45);
        __delay_ms(200);
        alinea_t = millis();
    }
}

void mete_pos()
{
    backward(45);
    __delay_ms(600);

    positionControlPing_pieza(METE_SETPOINT_1, FALSE);

   if(rig == RIG_RECTANGULO)
        left(100);
    else
     right(100);

     __delay_ms(CENTRA_RIG);
     stopMotors();
}
/////////////////////////////////////
/*
            MAIN LOOP
 */
//////////////////////////////////////

int main(int argc, char** argv) {

    unsigned char pieza;
    unsigned char inputData;
    unsigned long t;
    char correction_dir;

    setup();

    t = millis();
 
    printf("START ROBOT\n");

    // Loop forever
    while (1) {

        /*
        Main Program Loop, sequence here
        */

        ack = LOW;

        //Lee los datos del puerto para ver si ya cambiaron
        do {
            inputData = readPort();
            __delay_ms(100);
        } while (inputData != readPort() || readPort() == 0);

        printf("Input Data: %x\n", inputData);

        switch (inputData) {
            case POSITION1_CMD:
                sees_debounce();

                if (!seesleft || !seesright)
                {
                    if(!seesleft)
                    {
                        //printf("not sees left\n");
                        correction_dir = LEFT;
                        buscaRig(TRUE, LEFT, RIGHT, MAX_DISTANCE);
                        right(80);
                        __delay_ms(112);
                        stopMotors();
                        Control(PING, POS_SETPOINT_PING);


                    }else if (!seesright)
                    {
                        //printf("not sees right\n");
                        correction_dir = RIGHT;
                        buscaRig(TRUE, RIGHT, LEFT, MAX_DISTANCE);
                        left(80);
                        __delay_ms(112);
                        stopMotors();
                        Control(PING, POS_SETPOINT_PING);
                    }

                    if(correction_dir == LEFT)
                    {
                        left(100);
                        __delay_ms(1000);
                    }else
                    {
                        right(100);
                        __delay_ms(1000);
                    }

                    stopMotors();

                }
                else{
                    printf("uleft: %d, uright: %d\n",uLeft,uRight);
                    Control(PING, POS_SETPOINT_PING);
                }
                break;

            case POSITION2_CMD:
                Control(PING, POS_SETPOINT_PING);
                right(120);
                __delay_ms(POS2_DELAY);
                Control(PING, POS_SETPOINT_PING);
                stopMotors();
                break;

            case GOTO_LOADZONE_CMD:
                SwitchToHC();
                left(50);
                __delay_ms(DELAY_CMD_3);
                positionControlPing_pieza(PIEZA1_LOADZONE_SETPOINT, TRUE);
                stopMotors();
                //printf("FOUNDLOADZONE\n");
                break;

            case TOOL1_CMD:
               // printf("backwarding\n");
                backward(70);
                __delay_ms(100);
                right(100);
                __delay_ms(300);
                //printf("PING SWITCHED\n");

                // SWITCH TO PING W/O HC CONTROL
                rotateLeft(SWITCH_SPEED);
                __delay_ms(ROTATION_DELAY);
                stopMotors();

               // printf("CONTROL PING\n");
                Control(PING, POS_SETPOINT_PING);

                //printf("aligning\n");
                forward(50);
                __delay_ms(1500);
                stopMotors();
                //printf("backward\n");
                backward(100);
                __delay_ms(550);
                stopMotors();

                buscaRig(TRUE, RIGHT, RIGHT, PIEZA1_THRESHOLD);

                right(100);
                __delay_ms(PIEZA1_DELAY);
                stopMotors();

                Control(PING, PIEZA1_SETPOINT);
                pieza = 1;
                break;

            case TOOL2_CMD:
                ControlPieza(POS_2);
                pieza = 2;
                break;

            case TOOL3_CMD:
                ControlPieza(POS_3);
                pieza = 3;
                break;

            case GOTO_OILRIG_R_CMD:
                rig = RIG_RECTANGULO;
                if (pieza != 1)
                    SwitchToPing();

		//printf("START OIL RIG NAVIGATION (Rect)\n");
                //printf("FIRST BUMP\n");
                buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                    stopMotors();
                //printf("SECOND BUMP\n");

                buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                stopMotors();

               // printf("THIRD BUMP\n");

                 buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);//__delay_ms(JUMP_DELAY);
                stopMotors();
               // printf("OIL RIGS ZONE OK\n");

                // encontrar borde izquierdo
                do{
                    readLimits();
                    alinear();
                    left(80);
                }while(limitL > LEFT_LIMIT);

                buscaRig(TRUE, LEFT, RIGHT, 800);
                mete_pos();
                break;

            case GOTO_OILRIG_T_CMD:
		//printf("START OIL RIG NAVIGATION (Triang)\n");
                rig = RIG_TRIANGULO;
                if (pieza != 1)
                    SwitchToPing();

                //printf("FIRST BUMP\n");
                buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                    stopMotors();
                //printf("SECOND BUMP\n");

                buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                stopMotors();

               // printf("THIRD BUMP\n");

                 buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                stopMotors();
                //printf("OIL RIGS ZONE OK\n");

                // encontrar borde derecho
                do{
                    readLimits();
                    alinear();
                    right(80);
                }while(limitR > RIGHT_LIMIT);

                buscaRig(TRUE, RIGHT, LEFT, 800);    // primer oil rig circulo
                left(80);
                __delay_ms(1500);
                backward(45);
                __delay_ms(250);
                buscaRig(TRUE, RIGHT, LEFT, 800);    // segundo rig triangulo
                mete_pos();
                break;

            case GOTO_OILRIG_C_CMD:
		//printf("START OIL RIG NAVIGATION (Circ)\n");
                rig = RIG_CIRCULO;
                if (pieza != 1)
                    SwitchToPing();
                //printf("FIRST BUMP\n");
                buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                    stopMotors();
                //printf("SECOND BUMP\n");

                buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                stopMotors();

               // printf("THIRD BUMP\n");

                 buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                stopMotors();
                //printf("OIL RIGS ZONE OK\n");

                backward(45);
                __delay_ms(300);

                // encontrar borde derecho
                do{
                    readLimits();
                    alinear();
                    right(80);
                }while(limitR > RIGHT_LIMIT);

                buscaRig(TRUE, RIGHT, LEFT, 800);
                mete_pos();
                break;

            case PUSH_TOOL_1:
                forward(60);
                __delay_ms(DELAY_PUSH_1);
                stopMotors();
                break;

            case PUSH_TOOL_2:
                backward(60);
                __delay_ms(DELAY_PUSH_2);
                stopMotors();
                break;

            case PUSH_TOOL_3:
                backward(60);
                __delay_ms(500);
                stopMotors();
                break;

            case PUSH_TOOL_4:
                forward(60);
                __delay_ms(DELAY_PUSH_4);
                stopMotors();
                break;

            case RETURN_CMD:
                backward(100);
                __delay_ms(200);
                stopMotors();
                rotateLeft(200);
                __delay_ms(2900);
                stopMotors();

                buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                    stopMotors();
                //printf("SECOND BUMP\n");

                buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                stopMotors();

               // printf("THIRD BUMP\n");

                 buscaHoyo(LEFT);
                forward(JUMP_SPEED);
                __delay_ms(JUMP_DELAY);
                stopMotors();

                // encontrar borde derecho
                do{
                    readLimits();
                    alinear();
                    right(80);
                }while(limitR > RIGHT_LIMIT);

                stopMotors();
                break;
        }

        ack = HIGH;

        do {
            //Lee los datos del puerto para ver si ya cambiaron
            inputData = readPort();

            printf("Esperando clear...\n");
        } while (inputData != 0);
        
          

    }   // end while

    return (1);

}