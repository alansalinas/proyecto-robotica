/* 
 * File:   ultrasonic.h
 * Author: alansalinas
 *
 * Created on March 29, 2015, 8:13 PM
 */

#ifndef ULTRASONIC_H
#define	ULTRASONIC_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* ULTRASONIC_H */

extern char seesleft,seesright;

void setup_sensors();
void readSensors();
void readHC();
void readLimits();
char seesOne();
char seesBoth();
void sees_debounce();
