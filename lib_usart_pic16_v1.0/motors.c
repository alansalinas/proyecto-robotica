
#include <xc.h>
#include "pin_definitions.h"
#include "CoreFunctions.h"

// Individual motor motions definitions
#define FRONT_LEFT_FORWARD(x) pwm_LA1=x;pwm_LA2=0;
#define FRONT_LEFT_BACKWARD(x) pwm_LA1=0;pwm_LA2=x;

#define FRONT_RIGHT_FORWARD(x) pwm_RA1=x;pwm_RA2=0;
#define FRONT_RIGHT_BACKWARD(x) pwm_RA1=0;pwm_RA2=x;

#define BACK_LEFT_FORWARD(x) pwm_LB1=x;pwm_LB2=0;
#define BACK_LEFT_BACKWARD(x) pwm_LB1=0;pwm_LB2=x;

#define BACK_RIGHT_FORWARD(x) pwm_RB1=x;pwm_RB2=0;
#define BACK_RIGHT_BACKWARD(x) pwm_RB1=0;pwm_RB2=x;

//
// motor pin initi
void setup_motors(){
  // A control outputs for the motor controller
  // A0-B0 used as PWM outputs, A1-B1 digital
  LA1_mode = OUTPUT;
  LA2_mode = OUTPUT;
  LB1_mode = OUTPUT;
  LB2_mode = OUTPUT;

  RA1_mode = OUTPUT;
  RA2_mode = OUTPUT;
  RB1_mode = OUTPUT;
  RB2_mode = OUTPUT;

}

/*
  DC motor control routines
*/

void forward(int vel)
{

  vel = map(vel, 0, 490, 0, MAX_VEL);

  if(vel > MAX_VEL)
  vel = MAX_VEL;

  FORWARD = TRUE;

  //Serial.print("forward: ");Serial.println(vel);

 FRONT_LEFT_FORWARD(vel)
 FRONT_RIGHT_FORWARD(vel)
 BACK_LEFT_FORWARD(vel)
 BACK_RIGHT_FORWARD(vel)
}

void backward(int vel)
{

  vel = map(vel, 0, 490, 0, MAX_VEL);

  if(vel > MAX_VEL)
  vel = MAX_VEL;

  FORWARD = FALSE;

  //Serial.print("backward: ");Serial.println(vel);

 FRONT_LEFT_BACKWARD(vel)
 FRONT_RIGHT_BACKWARD(vel)
 BACK_LEFT_BACKWARD(vel)
 BACK_RIGHT_BACKWARD(vel)
}


void left(int vel)
{
 FRONT_LEFT_BACKWARD(vel)
 BACK_LEFT_FORWARD(vel)

 FRONT_RIGHT_FORWARD(vel)
 BACK_RIGHT_BACKWARD(vel)
}

void right(int vel)
{
 FRONT_LEFT_FORWARD(vel)
 BACK_LEFT_BACKWARD(vel)

 FRONT_RIGHT_BACKWARD(vel)
 BACK_RIGHT_FORWARD(vel)
}

void stopMotors()
{
 FRONT_LEFT_FORWARD(0)
 FRONT_RIGHT_FORWARD(0)
 BACK_LEFT_FORWARD(0)
 BACK_RIGHT_FORWARD(0)
}

void rotateLeft(int vel)
{

  vel = map(vel, 0, 300, 0, MAX_VEL);

  if(vel > MAX_VEL)
  vel = MAX_VEL;

  //Serial.print("left: ");Serial.println(vel);

  FRONT_LEFT_BACKWARD(vel)
 BACK_LEFT_BACKWARD(vel)
 FRONT_RIGHT_FORWARD(vel)
 BACK_RIGHT_FORWARD(vel)

}

void rotateRight(int vel)
{

  vel = map(vel, 0, 300, 0, MAX_VEL);

  if(vel > MAX_VEL)
  vel = MAX_VEL;

  //Serial.print("right: ");Serial.println(vel);

  FRONT_RIGHT_BACKWARD(vel)
 BACK_RIGHT_BACKWARD(vel)
 FRONT_LEFT_FORWARD(vel)
 BACK_LEFT_FORWARD(vel)

}
