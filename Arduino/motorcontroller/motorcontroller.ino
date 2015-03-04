/*
  Define pin assignments
*/
#define L_A0 5
#define L_A1 7
#define L_B0 8
#define L_B1 9
#define R_A0 5
#define R_A1 7
#define R_B0 8
#define R_B1 9

#define led 13

// Individual motor motions definitions
#define FRONT_LEFT_FORWARD(x) analogWrite(L_A0,map(x,0,100,0,255));digitalWrite(L_A1,LOW);
#define FRONT_LEFT_BACKWARD(x) analogWrite(L_A0,map(x,0,100,255,0));digitalWrite(L_A1,HIGH);

#define FRONT_RIGHT_FORWARD(x) analogWrite(R_A0,map(x,0,100,0,255));digitalWrite(R_A1,LOW);
#define FRONT_RIGHT_BACKWARD(x) analogWrite(R_A0,map(x,0,100,255,0));digitalWrite(R_A1,HIGH);

#define BACK_LEFT_FORWARD(x) analogWrite(L_B0,map(x,0,100,0,255));digitalWrite(L_B1,LOW);
#define BACK_LEFT_BACKWARD(x) analogWrite(L_B0,map(x,0,100,255,0));digitalWrite(L_B1,HIGH);

#define BACK_RIGHT_FORWARD(x) analogWrite(R_B0,map(x,0,100,0,255));digitalWrite(R_B1,LOW);
#define BACK_RIGHT_BACKWARD(x) analogWrite(R_B0,map(x,0,100,255,0));digitalWrite(R_B1,HIGH);



/*
  DC motor control routines
*/

void forward(int vel)
{
 FRONT_LEFT_FORWARD(vel)
 FRONT_RIGHT_FORWARD(vel) 
}

void backward(int vel)
{
 FRONT_LEFT_BACKWARD(vel)
 FRONT_RIGHT_BACKWARD(vel) 
}

void rotateLeft(int vel)
{
 FRONT_LEFT_BACKWARD(vel)
 FRONT_RIGHT_FORWARD(vel) 
}

void rotateRight(int vel)
{
 FRONT_LEFT_FORWARD(vel)
 FRONT_RIGHT_BACKWARD(vel) 
}



void setup(){
  // A control outputs for the motor controller
  // A0-B0 used as PWM outputs, A1-B1 digital
  pinMode(L_A0, OUTPUT);
  pinMode(L_A1, OUTPUT);
  pinMode(L_B0, OUTPUT);
  pinMode(L_B1, OUTPUT);
  
  pinMode(R_A0, OUTPUT);
  pinMode(R_A1, OUTPUT);
  pinMode(R_B0, OUTPUT);
  pinMode(R_B1, OUTPUT);
  
  pinMode(led, OUTPUT);  //status LED
  
  
  
}



void loop(){
  
forward(100);
delay(3000);
backward(100);
delay(3000);


}
