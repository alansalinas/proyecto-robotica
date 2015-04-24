/* 
 * File:   pin_definitions.h
 * Author: alansalinas
 *
 * Created on March 29, 2015, 6:29 PM
 */

#ifndef PIN_DEFINITIONS_H
#define	PIN_DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* PIN_DEFINITIONS_H */

/*
            BEGIN PARAMETER DEFINTIONS
 */

#define _XTAL_FREQ 20000000  // main Fosc operating frecuency

#define OUTPUT 0
#define INPUT 1
#define HIGH 1
#define LOW 0

#define TRUE 1
#define FALSE 0

#define PING 0
#define HC 1

#define LEFT 0
#define RIGHT 1
#define FORW 2
#define BACKW 3

//
//  GENERAL SETTINGS
//

#define PR2_INIT 170    // pwm step resolution in timer2 interruption
#define MAX_WAIT 0xFC8  // maximum ultrasonic pulsin wait

#define LIMIT_LEFT 200      // left limit sensor threshold
#define LIMIT_RIGHT 200     // right limit sensor threshold

//
//  DEFINE PIN ASSIGNMENTS
//

/*
         Motor Controller pinouts
 */
#define LA1_mode TRISDbits.TRISD7
#define LA2_mode TRISDbits.TRISD6
#define LB1_mode TRISDbits.TRISD5
#define LB2_mode TRISDbits.TRISD4

#define L_A1 PORTDbits.RD7
#define L_A2 PORTDbits.RD6
#define L_B1 PORTDbits.RD5
#define L_B2 PORTDbits.RD4

#define RA1_mode TRISCbits.TRISC5
#define RA2_mode TRISCbits.TRISC4
#define RB1_mode TRISDbits.TRISD3
#define RB2_mode TRISDbits.TRISD2

#define R_A1 PORTCbits.RC5
#define R_A2 PORTCbits.RC4
#define R_B1 PORTDbits.RD3
#define R_B2 PORTDbits.RD2

/*
        Ultrasonic sensor pinouts
 */

// PING frontal sensors
#define ultraL_mode TRISBbits.TRISB0
#define ultraR_mode TRISBbits.TRISB1
#define ultraL PORTBbits.RB0
#define ultraR PORTBbits.RB1

//
// Left HC sensors
#define triggerL_mode TRISBbits.TRISB2
#define echoL_mode TRISBbits.TRISB3
#define triggerL PORTBbits.RB2
#define echoL PORTBbits.RB3

#define triggerR_mode TRISBbits.TRISB4
#define echoR_mode TRISBbits.TRISB5
#define triggerR PORTBbits.RB4
#define echoR PORTBbits.RB5

//
// Limit sensors

// IR sensor
#define leftLimit_mode TRISEbits.TRISE0
#define rightLimit_mode TRISEbits.TRISE2

#define leftLimit PORTEbits.RE0
#define rightLimit PORTEbits.RE2



//
//  PARALLEL PORT INTERFACE
//
#define data0 TRISDbits.TRISD0
#define data1 TRISCbits.TRISC3
#define data2 TRISCbits.TRISC2
#define data3 TRISCbits.TRISC1
#define ack TRISDbits.TRISD1

#define data0_mode PORTDbits.RD0
#define data1_mode PORTCbits.RC3
#define data2_mode PORTCbits.RC2
#define data3_mode PORTCbits.RC1
#define ack_mode    PORTDbits.RD1



/*
        Control Parameter Definitions
 */
#define ERR_ROT 15  // rotational error tolerance
#define ERR_POS 40  // positional error tolerance
#define MAXPOS 500 // maximum position accetable for correction
#define MAXROT 350  // maximum error tolarated for rotation correction
#define POS_SETPOINT_PING 400    // setpoint for position contol during navigation
#define POS_SETPOINT_HC 110

#define ORDER 5    // Digital Filter order
#define MAX_VEL 50 // maximum motor pwm duty cycle
#define MAX_ROT_VEL 57  // maximum rotational velocity for control
#define TS 50       // digital control sampling time
#define CONTROL_TIME 1700   // period of Control() recurrent calling during sweep

#define ROTATION_DELAY 1800     // delay for rotation in switchHC or ping function
#define MAX_DISTANCE 500        // max distance tolerated for seesBoth function

#define SWEEP_SPEED 120
#define STEP_SIZE 200   // robot step time length in milliseconds
#define STEP_VEL 90       // robot step manipulation for motors
#define JUMP_SPEED 220  // speed for jumping into next lane
#define JUMP_DELAY 1100 // delay for bump jump in milliseconds

#define LEFT_LIMIT 48
#define RIGHT_LIMIT 55


