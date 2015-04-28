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

/////////////////////////////////////
/*
            MAIN LOOP
 */
//////////////////////////////////////

int main(int argc, char** argv) {

    unsigned char pieza;
    unsigned char inputData;

    setup();

    //t = millis();

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
                Control(PING, POS_SETPOINT_PING);
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
                __delay_ms(800);
                positionControlPing_pieza(PIEZA1_LOADZONE_SETPOINT);
                stopMotors();
                printf("FOUNDLOADZONE\n");
                __delay_ms(500);
                break;

            case TOOL1_CMD:
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
                if (pieza != 1)
                    SwitchToPing();

		printf("START OIL RIG NAVIGATION (Rect)\n");
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
                printf("OIL RIGS ZONE OK\n");
                break;

            case GOTO_OILRIG_T_CMD:
		printf("START OIL RIG NAVIGATION (Triang)\n");
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
                __delay_ms(JUMP_DELAY);//__delay_ms(JUMP_DELAY);
                stopMotors();
                printf("OIL RIGS ZONE OK\n");
                break;

            case GOTO_OILRIG_C_CMD:
		printf("START OIL RIG NAVIGATION (Circ)\n");
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
                __delay_ms(JUMP_DELAY);//__delay_ms(JUMP_DELAY);
                stopMotors();
                printf("OIL RIGS ZONE OK\n");
                break;

            case PUSH_TOOL_CMD:
                //En el estado 0 trabaja la raspberry
                break;

            case RETURN_CMD:
                //En el estado 0 trabaja la raspberry
                break;
        }

        ack = HIGH;

        do {
            //Lee los datos del puerto para ver si ya cambiaron
            inputData = readPort();

            printf("Esperando clear...\n");
        } while (inputData != 0);

    }

    return (1);

}