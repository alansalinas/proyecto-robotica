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

#define DEBOUNCE_COUNT 7    // count for debouncing stable state

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
#define data0_mode TRISDbits.TRISD0
#define data1_mode TRISCbits.TRISC3
#define data2_mode TRISCbits.TRISC2
#define data3_mode TRISCbits.TRISC1
#define ack_mode TRISDbits.TRISD1

#define data0 PORTDbits.RD0
#define data1 PORTCbits.RC3
#define data2 PORTCbits.RC2
#define data3 PORTCbits.RC1
#define ack PORTDbits.RD1

//
//  Comm protocol commands
#define POSITION1_CMD	1
#define POSITION2_CMD 	2
#define GOTO_LOADZONE_CMD   3
#define TOOL1_CMD   4
#define TOOL2_CMD   5
#define TOOL3_CMD   6
#define GOTO_OILRIG_R_CMD   7
#define GOTO_OILRIG_T_CMD   8
#define GOTO_OILRIG_C_CMD   9
#define PUSH_TOOL_1	10
#define PUSH_TOOL_2 11
#define PUSH_TOOL_3 12
#define PUSH_TOOL_4 13
#define RETURN_CMD  14



/*
        Control Parameter Definitions
 */
#define ERR_ROT 14  // rotational error tolerance
#define ERR_POS 9  // positional error tolerance
#define MAXPOS 500 // maximum position accetable for correction
#define MAXROT 350  // maximum error tolarated for rotation correction
#define POS_SETPOINT_PING 300    // setpoint for position contol during navigation
#define POS_SETPOINT_HC 110

#define ORDER 5    // Digital Filter order
#define MAX_VEL 50 // maximum motor pwm duty cycle
#define MAX_ROT_VEL 57  // maximum rotational velocity for control
#define TS 50       // digital control sampling time
#define CONTROL_TIME 1700   // period of Control() recurrent calling during sweep

#define ROTATION_DELAY 1400     // delay for rotation in switchHC or ping function
#define MAX_DISTANCE 450        // max distance tolerated for seesBoth function

#define SWEEP_SPEED 145
#define STEP_SIZE 200   // robot step time length in milliseconds
#define STEP_VEL 90       // robot step manipulation for motors
#define JUMP_SPEED 220  // speed for jumping into next lane
#define JUMP_DELAY 1350 // delay for bump jump in milliseconds

#define SWITCH_SPEED 140   // speed for 90dg sensor switching

#define LEFT_LIMIT 50
#define RIGHT_LIMIT 50

#define PIEZA1_THRESHOLD 150    // limite para ver en ping derecho pieza1
#define PIEZA1_DELAY 220    // setpoint para centrarse horizonatlmente
#define PIEZA1_SETPOINT 606 // setpoint para centrar verticalmente
#define PIEZA1_LOADZONE_SETPOINT 112    // setpoint de aproximacion en paso3

// setpoints para piezas con sensor HC derecho
#define POS_2 201
#define POS_3 392

#define POS2_DELAY 3150 // delay desplazamiento cmd1 a cmd2
#define DELAY_CMD_3 500 // delay para alinearse de lado en cmd3

#define METE_SETPOINT_1 280 // setpoint de aproximacion a primera posicion de meter pieza
#define CENTRA_RIG 135      // delay en ms para corregir centro en rig
#define DELAY_PUSH_1 1000
#define DELAY_PUSH_2 350
#define DELAY_PUSH_4 1000


// rigs
#define RIG_CIRCULO 1
#define RIG_TRIANGULO 2
#define RIG_RECTANGULO 3


