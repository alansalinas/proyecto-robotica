/* 
 * File:   motors.h
 * Author: alansalinas
 *
 * Created on March 29, 2015, 8:01 PM
 */

#ifndef MOTORS_H
#define	MOTORS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MOTORS_H */


void setup_motors();
void forward(int vel);
void backward(int vel);
void left(int vel);
void right(int vel);
void stopMotors();
void rotateLeft(int vel);
void rotateRight(int vel);
