/* 
 * File:   CoreFunctions.h
 * Author: alansalinas
 *
 * Created on March 29, 2015, 5:52 PM
 */

#ifndef COREFUNCTIONS_H
#define	COREFUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* COREFUNCTIONS_H */




extern volatile unsigned long timer0_overflow_count;

extern volatile unsigned short pwm_LA1, pwm_LA2, pwm_LB1, pwm_LB2;
extern volatile unsigned short pwm_RA1, pwm_RA2, pwm_RB1, pwm_RB2;

extern unsigned short uLeft, uRight;

extern unsigned long lastreading;

extern char FORWARD;

long map(long x, long in_min, long in_max, long out_min, long out_max);

int absval(int val);


unsigned long micros();
unsigned long millis();

void delay(unsigned long milliseconds);
void delayMicroseconds(unsigned long microseconds);

unsigned long pulseInLeft();
unsigned long pulseInRight();
