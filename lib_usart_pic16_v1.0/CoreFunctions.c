#include <xc.h>
#include "pin_definitions.h"
#include "usart_pic16.h"
#include <stdio.h>


volatile unsigned long timer0_overflow_count;

volatile unsigned short pwm_LA1, pwm_LA2, pwm_LB1, pwm_LB2;
volatile unsigned short pwm_RA1, pwm_RA2, pwm_RB1, pwm_RB2;
volatile unsigned long ms;

short uLeft, uRight;
unsigned short limitL, limitR;

unsigned long lastreading = 0;  // Discrete time sampling time in microseconds

// Variables de estado
char FORWARD;
volatile unsigned short waitcount;


/*
 Core Functions
 */

void putch(unsigned char byte)
{
    USARTWriteChar(byte);
}

// Timer0 initi - timekeeping services
void setup_timer0()
{
    TMR0 = 0;
    OPTION_REG = 0;
    OPTION_REGbits.PS = 0x03;   // 16 prescaler
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
}

void setup_timer1() // timer1 for compare pulsin functions
{
    T1CON = 0x20;   // 1:4 prescale value, Fosc/4=5Mhz
    TMR1H = 0;
    TMR1L = 0;
}

// Timer2 init - pwm services
void setup_timer2()
{
    T2CON = 0x00;
    T2CONbits.T2CKPS = 0x1;    //prescale 4
    INTCONbits.PEIE = 1;
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    T2CONbits.TMR2ON = 1;
}

void setup_adc()
{
    ANSELbits.ANS5 = 1;
    //ANSELbits.ANS6 = 1;
    ANSELbits.ANS7 = 1;

    leftLimit_mode = INPUT;
    rightLimit_mode = INPUT;

    ADCON0bits.ADCS = 0x2;  // Fosc/32 conversion clock
    
    ADCON1bits.VCFG0 = 0;   // vdd voltage reference +
    ADCON1bits.VCFG1 = 0;   // vss voltage reference -
    
    ADCON1bits.ADFM = 0;    // left justified

    ADCON0bits.ADON = 1;    // adc module enabled

}


void readLimits()
{
   ADCON0bits.CHS = 0x7;   // set channel 6 RE1
   __delay_ms(1);
   ADCON0bits.GO_nDONE = 1;

   while(ADCON0bits.GO_nDONE);

   limitR = ADRESH;

   __delay_ms(8);

   ADCON0bits.CHS = 0x5; // set channel 5 RE0
   __delay_ms(1);
   ADCON0bits.GO_nDONE = 1;

   while(ADCON0bits.GO_nDONE);

   limitL = ADRESH;

}

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

//
//  Front left PING sensor
unsigned long pulseInLeft()
{
    unsigned long t;
    short pulse;
    unsigned short maxcount;

    TMR1L = 0;
    TMR1H = 0;
    pulse = 0;
    maxcount=MAX_WAIT;

    while (!ultraL)
    {

        if(maxcount-- == 0 || maxcount > MAX_WAIT)
            break;
    }              //Waiting for Echo

    T1CONbits.TMR1ON = 1;               //Timer Starts
    while(ultraL){
        if(maxcount-- == 0 || maxcount > MAX_WAIT)
            break;
    }
    T1CONbits.TMR1ON = 0;               //Timer Stops

    pulse = (TMR1L | (TMR1H<<8));

    return pulse;
}

//
//  Front right PING sensor
unsigned long pulseInRight()
{
    unsigned long t;
    short pulse;
    unsigned short maxcount;

    TMR1L = 0;
    TMR1H = 0;
    pulse = 0;
    maxcount=MAX_WAIT;

    while (!ultraR)
    {

        if(maxcount-- == 0 || maxcount > MAX_WAIT)
            break;
    }              //Waiting for Echo

    T1CONbits.TMR1ON = 1;               //Timer Starts
    while(ultraR){
        if(maxcount-- == 0 || maxcount > MAX_WAIT)
            break;
    }
    T1CONbits.TMR1ON = 0;               //Timer Stops

    pulse = (TMR1L | (TMR1H<<8));

    return pulse;
}

//
// Side Left HCR04 sensor
unsigned long pulseInHC_L()
{
    short pulse = 0;
    unsigned short maxcount;

    TMR1L = 0;
    TMR1H = 0;
    maxcount=MAX_WAIT;
    
    while (!echoL)
    {
        
        if(maxcount-- == 0 || maxcount > MAX_WAIT)
            break;
    }              //Waiting for Echo

    T1CONbits.TMR1ON = 1;               //Timer Starts
    while(echoL){
        if(maxcount-- == 0 || maxcount > MAX_WAIT)
            break;
    }
    T1CONbits.TMR1ON = 0;               //Timer Stops

    pulse = (TMR1L | (TMR1H<<8));

    return pulse;
}

//
// Side Right HCR04 sensor
short pulseInHC_R()
{
    unsigned short pulse = 0;
    unsigned int maxcount;

    TMR1L = 0;
    TMR1H = 0;
    maxcount=MAX_WAIT;

   // while(echo && maxcount-- > 0 && maxcount < MAX_WAIT){
    //    asm("NOP");}

    while (!echoR)
    {

        if(maxcount-- == 0 || maxcount > MAX_WAIT)
            break;
    }              //Waiting for Echo

    T1CONbits.TMR1ON = 1;               //Timer Starts
    while(echoR){
        if(maxcount-- == 0 || maxcount > MAX_WAIT)
            break;
        //asm("NOP");//Waiting for Echo goes LOW
    }
    T1CONbits.TMR1ON = 0;               //Timer Stops

    pulse = (TMR1L | (TMR1H<<8));

    return pulse;
}
