/*
  Define pin assignments
*/
#define pinA1 5
#define pinA2 7
#define pinA3 8
#define pinA4 9

#define led 13

// Individual motor motions definitions
#define FRONT_LEFT_FORWARD(x) analogWrite(pinA1,map(x,0,100,0,255));digitalWrite(pinA2,LOW);
#define FRONT_LEFT_BACKWARD(x) analogWrite(pinA1,map(x,0,100,255,0));digitalWrite(pinA2,HIGH);
#define FRONT_RIGHT_FORWARD(x) analogWrite(pinA3,map(x,0,100,0,255));digitalWrite(pinA4,LOW);
#define FRONT_RIGHT_BACKWARD(x) analogWrite(pinA3,map(x,0,100,255,0));digitalWrite(pinA4,HIGH);


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
  // A1-A3 used as PWM outputs, A2-A4 digital
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);
  pinMode(pinA3, OUTPUT);
  pinMode(pinA4, OUTPUT);
  pinMode(led, OUTPUT);  //status LED
  
  
  
}



void loop(){
  
forward(100);
delay(3000);
backward(100);
delay(3000);
 
  


}
