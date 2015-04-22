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
 * Global program variables
 */

unsigned long t,mse;


void setup()
{

   OSCCON = 0; // clear osc control bits
   ANSELH = 0x00;   // digital outs on PORTB
   TRISC = 0;       // PORTC is for output
   TRISCbits.TRISC4 = 0;

   setup_timer0();  // timer0 - timekeeping services
   setup_timer1();  // timer1 - pulsein measurements
   setup_timer2();  // timer2 - pwm services

   setup_sensors(); // ultrasonic setup
   setup_motors();  // hbrdige controller setup
   setup_port();
   setup_adc();

}

/////////////////////////////////////
/*
            MAIN LOOP
 */
//////////////////////////////////////
int main(int argc, char** argv) {

    unsigned long t, t_control;
    char dire = LEFT;
    char gogo = 1;
    unsigned short testL, testR;
    setup();

    USARTInit(9600);
    t=millis();
    t_control=millis();

    printf("START ROBOT\n");

    buscaHoyo(RIGHT);
        forward(JUMP_SPEED);//left(140);
        __delay_ms(1800);
        stopMotors();

    // Loop forever
    while(1)
    {

        /*
         Main Program Loop, sequence here
         */

        Control(PING);
        //readLimits();
        //printf("limitl = %d\n", limitL);

        //
        // TEST SAMPLING


    }   // end forever loop

    return (1);


}

