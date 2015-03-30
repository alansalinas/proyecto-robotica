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

#define _XTAL_FREQ 8000000  // main Fosc operating frecuency

#define OUTPUT 0
#define INPUT 1
#define HIGH 1
#define LOW 0

#define TRUE 1
#define FALSE 0

#define PR2_INIT 0x75

/*
  Define pin assignments
*/

//
// Motor controller pinouts
#define LA1_mode TRISCbits.TRISC0
#define LA2_mode TRISCbits.TRISC1
#define LB1_mode TRISCbits.TRISC2
#define LB2_mode TRISCbits.TRISC3

#define L_A1 PORTCbits.RC0
#define L_A2 PORTCbits.RC1
#define L_B1 PORTCbits.RC2
#define L_B2 PORTCbits.RC3

#define RA1_mode TRISDbits.TRISD0
#define RA2_mode TRISDbits.TRISD1
#define RB1_mode TRISDbits.TRISD2
#define RB2_mode TRISDbits.TRISD3

#define R_A1 PORTDbits.RD0
#define R_A2 PORTDbits.RD1
#define R_B1 PORTDbits.RD2
#define R_B2 PORTDbits.RD3

//
// Ultrasonic sensor pinouts
#define ultraL_mode TRISBbits.TRISB0
#define ultraR_mode TRISBbits.TRISB1

#define ultraL PORTBbits.RB0
#define ultraR PORTBbits.RB1

#define trigger_mode TRISBbits.TRISB2
#define echo_mode TRISBbits.TRISB3

#define trigger PORTBbits.RB2
#define echo PORTBbits.RB3

#define led 13

//
// Control paramters definitions
#define ERR_ROT 5
#define ERR_POS 35
#define SETPOS 530

#define ORDER 11
#define MIN_MAN 170
#define MAX_VEL 200
#define TS 10

#define kp 5.90520
#define ti 0.00000001
#define td 0.000

#define MAX_DELAY 180000
