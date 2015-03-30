
#include "pin_definitions.h"
#include "CoreFunctions.h"
#include "motors.h"


int integral_rot_L, deriv_rot_L;
int integral1_rot_L = 0;
int deriv1_rot_L = 0;

int integral_rot_R, deriv_rot_R;
int integral1_rot_R = 0;
int deriv1_rot_R = 0;

int integral_pos, deriv_pos;
int integral1_pos = 0;
int deriv1_pos = 0;

//
// PID routines

void reset_rot_L()
{
  integral_rot_L = 0;
  integral1_rot_L = 0;
  deriv_rot_L = 0;
  deriv1_rot_L = 0;
}

void reset_rot_R()
{
  integral_rot_R = 0;
  integral1_rot_R = 0;
  deriv_rot_R = 0;
  deriv1_rot_R = 0;
}

 int pid_rot_L(int error)
{
  //float kp = 0.00012;
  //float ti = 0.022;
  //float td = 4.1;//9.03;

  reset_rot_R();

  if (error < 0)
    error -= 2*error;

  if (integral_rot_L < 0)
    integral_rot_L = 0;

  integral1_rot_L = integral_rot_L;
  if (integral1_rot_L < 0)
    integral1_rot_L = 0;

  integral_rot_L=(int)(kp*(TS/ti)*error + integral1_rot_L);

  if (deriv_rot_L < 0)
    deriv_rot_L = 0;

  deriv1_rot_L = deriv_rot_L;

  if (deriv1_rot_L < 0)
    deriv1_rot_L = 0;
  deriv_rot_L =  (int)(kp*(td/TS)*error - deriv1_rot_L);

  //Serial.print("rot correction: ");Serial.println((kp*error + integral_rot_L + deriv_rot_L));

  return (int)(kp*error + integral_rot_L + deriv_rot_L);
}

 int pid_rot_R(int error)
{
  //float kp = 0.00012;
  //float ti = 0.022;
  //float td = 4.1;//9.03;

  reset_rot_L();

  if (error < 0)
    error -= 2*error;

  if (integral_rot_R < 0)
    integral_rot_R = 0;

  integral1_rot_R = integral_rot_R;
  if (integral1_rot_R < 0)
    integral1_rot_R = 0;

  integral_rot_R=(int)(kp*(TS/ti)*error + integral1_rot_R);

  if (deriv_rot_R < 0)
    deriv_rot_R = 0;

  deriv1_rot_R = deriv_rot_R;

  if (deriv1_rot_R < 0)
    deriv1_rot_R = 0;
  deriv_rot_R = (int)( kp*(td/TS)*error - deriv1_rot_R);

  //Serial.print("rot correction: ");Serial.println((kp*error + integral_rot_R + deriv_rot_R));

  return (int)(kp*error + integral_rot_R + deriv_rot_R);
}

int pid_pos(int error)
{
  float kp_pos = 5.20515;
  float ti_pos = 0.0000001;
  float td_pos = 0;//30.305;

  if (error < 0)
    error -= 2*error;

  integral1_pos = integral_pos;
  integral_pos=(int)(kp_pos*(TS/ti)*error + integral1_pos);

  deriv1_pos = deriv_pos;
  deriv_pos =  (int)(kp_pos*(td/TS)*error - deriv1_pos);

  return (int)(kp_pos*error + integral_pos + deriv_pos);
}


void rotationControl()
{
  int error, dist;

  // Comprobar si hay error, detener movimientos previos
   readSensors();
   error = uRight - uLeft;

   if (error > ERR_ROT || error < -ERR_ROT && error < 300 && error > -300)
   {
     stopMotors();
     delay(120);
   }

  do{
   if (millis() - lastreading >= TS)
    {
    readSensors();
    //if(uRight > 0 && uLeft > 0)
    error = uRight - uLeft;
    dist = SETPOS - (uLeft+uRight) - absval(uLeft - uRight);

    //Serial.print(uLeft);Serial.print("-");Serial.print(uRight);Serial.print("/");Serial.print((uLeft+uRight) - absval(error));Serial.print("=");Serial.println(error);
    if (error < 300 && error > -300){
    if (error > ERR_ROT)
      rotateLeft(pid_rot_L(error));
     else if (error < -ERR_ROT)
     rotateRight(pid_rot_R(error-(2*error)));
    }

     lastreading = millis();
  }

  }while(error > ERR_ROT || error < -ERR_ROT && error < 300 && error > -300);


}

void positionControl(float setpoint)
{
   int error;

   // Comprobar si hay error, detener movimientos previos
   readSensors();
   error = setpoint - (uLeft+uRight) - absval(uLeft - uRight);

   if (error > ERR_POS || error < -ERR_POS && error < SETPOS+100 && error > -SETPOS-100)
   {
     stopMotors();
     delay(190);
   }

  do{
     if (millis() - lastreading >= TS)
    {
    readSensors();
    //if(uRight > 0 && uLeft > 0)
    error = setpoint - (uLeft+uRight) - absval(uLeft - uRight);

    //Serial.print(uLeft);Serial.print("-");Serial.print(uRight);Serial.print("=pos=");Serial.println(error);
    if (error < SETPOS+100 && error > -SETPOS-100){
    if (error > ERR_POS)
      backward(pid_pos(error));
     else if (error < -ERR_POS)
     forward(pid_pos(error-(2*error)));
    }

    }


  }while(error > ERR_POS || error < -ERR_POS && error < SETPOS+100 && error > -SETPOS-100);


}


