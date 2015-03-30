#include <xc.h>
#include "pin_definitions.h"


volatile unsigned long timer0_overflow_count;

volatile unsigned char pwmtime;
volatile unsigned short pwm_LA1, pwm_LA2, pwm_LB1, pwm_LB2;
volatile unsigned short pwm_RA1, pwm_RA2, pwm_RB1, pwm_RB2;
volatile unsigned long ms;

unsigned short uLeft, uRight;

unsigned long lastreading = 0;  // Discrete time sampling time in microseconds

// Variables de estado
char FORWARD;


void interrupt   t0_int  (void)        // interrupt function

{
    // Timer0 Interrupt handling timekeeping
        if(INTCONbits.T0IF && INTCONbits.T0IE)
    {                                     // if timer flag is set & interrupt enabled
               TMR0 = 0;               // reload the timer
               INTCONbits.T0IF = 0;   // clear interrupt flag
                //PORTC = 0x10;             // toggle a bit to say we're alive
                timer0_overflow_count++;
        }


        /*

         PWM interrupt at  TMR2
         */

        if(PIE1bits.TMR2IE && PIR1bits.TMR2IF)
        {
            PR2 = PR2_INIT;
            PIR1bits.TMR2IF = 0;

            //if(pwmtime >= 255) pwmtime = 0;

               //
               // 8 PWM OUTPUTS FOR MOTORCONTROLLER
            //  LEFT SIDE
            if(pwmtime < pwm_LA1)
                L_A1 = 1;
            else
                L_A1 = 0;

            if(pwmtime < pwm_LA2)
                L_A2 = 1;
            else
                L_A2 = 0;

            if(pwmtime < pwm_LB1)
                L_B1 = 1;
            else
                L_B1 = 0;

            if(pwmtime < pwm_LB2)
                L_B2 = 1;
            else
                L_B2 = 0;

            // RIGHT SIDE
            /*
            if(pwmtime < pwm_RA1)
                R_A1 = 1;
            else
                R_A1 = 0;

            if(pwmtime < pwm_RA2)
                R_A2 = 1;
            else
                R_A2 = 0;

            if(pwmtime < pwm_RB1)
                R_B1 = 1;
            else
                R_B1 = 0;

            if(pwmtime < pwm_RB2)
                R_B2 = 1;
            else
                R_B2 = 0;
                */

            // Actualizar paso pwm
            pwmtime++;

        }


}   // end interrupt service routine



long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int absval(int val)
 {
     return (val<0 ? (-val) : val);
 }



unsigned long micros()
{
    return((timer0_overflow_count << 8) + TMR0)*(8);
}


unsigned long millis()
{
    return(((timer0_overflow_count << 8) + TMR0)*(8))/1000;
}

void delay(unsigned long milliseconds)
{
    unsigned long t = millis();
    while (millis() - t < milliseconds)
    {
        asm("NOP");
    }
}

void delayMicroseconds(unsigned long microseconds)
{
    unsigned long t = micros();
    while(micros() - t < microseconds)
    {
        asm("NOP");
    }

}

unsigned long pulseInLeft()
{
    unsigned long t = micros();
    unsigned long x = 0;

    while (ultraL && micros() - t < 180000)
    {
        asm("NOP");
    }

    x = micros() -t;
    t = 0;
    return  x;
}

unsigned long pulseInRight()
{
    unsigned long t = micros();

    while (ultraR && micros() - t < 180000)
    {
        asm("NOP");
    }

    return micros() - t;
}

unsigned long pulseInHC()
{
    unsigned long t;
    unsigned long pulse = 0;
    unsigned long x;

    TMR1L = 0;
    TMR1H = 0;
    t = micros();

    while(echo && micros() - t < 60000){asm("NOP");}
    
    while (!echo && micros() - t < 60000)
    {
        asm("NOP");
    }              //Waiting for Echo

    T1CONbits.TMR1ON = 1;               //Timer Starts
    while(echo && (TMR1L | (TMR1H<<8)) < 60000){
        asm("NOP");//Waiting for Echo goes LOW
    }
    T1CONbits.TMR1ON = 0;               //Timer Stops

    pulse = (TMR1L | (TMR1H<<8));

    ms = pulse;

    x = pulse;

    if (pulse > 60000)
        pulse = 60000;

    return pulse;
}