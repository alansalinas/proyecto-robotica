#include <stdint.h>
#include <xc.h>

#include "usart_pic16.h"
#include "pin_definitions.h"
#include "CoreFunctions.h"


volatile unsigned char pwmtime;


void interrupt   t0_int  (void)        // interrupt function

{
    // Timer0 Interrupt handling timekeeping
        if(INTCONbits.T0IF && INTCONbits.T0IE)
    {                                     // if timer flag is set & interrupt enabled
               TMR0 = 0;               // reload the timer
               INTCONbits.T0IF = 0;   // clear interrupt flag
                timer0_overflow_count++;
        }


        //
        // USART interrupt handling
        if (RCIE && RCIF) {
        USARTHandleRxInt();
        //return;
        }


        /*

         PWM interrupt at  TMR2
         */

        if(PIE1bits.TMR2IE && PIR1bits.TMR2IF)
        {

            // Reset counts
            PR2 = PR2_INIT;

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
                

            // Actualizar paso pwm
            pwmtime++;

            // Limpiar interrupcion al final, evitar deadlocks
            PIR1bits.TMR2IF = 0;

        }

        return;


}   // end interrupt service routine

//void interrupt ISR(void)
void ISR(void)
{
    if (RCIE && RCIF) {
        USARTHandleRxInt();
        return;
    }
}