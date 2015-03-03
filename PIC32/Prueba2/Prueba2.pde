
#include "plib.h"

#define SAMPLE_RATE     5000        // 200us interval: Enough time for 1440 instructions at 72Mhz.
#define FPB                    36000000
#define MAX_PWM           FPB/SAMPLE_RATE // eg 7200 at 5000hz.
 
unsigned int channel4;// conversion result as read from result buffer
unsigned int channel5;// conversion result as read from result buffer
unsigned int offset; // points to the base of the idle buffer
int PWMvalue1, PWMvalue2;
int boton1=0;
int boton2=0;

extern "C"{
void __ISR( _TIMER_2_VECTOR, ipl2) T2Interrupt(void)
 {
 SetDCOC1PWM(PWMvalue1);     // these functions send a new value (0 to 7200) to PWM modulation with some PWM value.
 SetDCOC2PWM(PWMvalue2);    // interrupt will fire again 7200 counts later than now  = 200us. (at 5 khz)
  mT2ClearIntFlag();                  // clear this interrupt .
 } 
}

void setupADC()
{
  // configure and enable the ADC
CloseADC10();// ensure the ADC is off before setting the configuration
// define setup parameters for OpenADC10
#define PARAM1 ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON
#define PARAM2 ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_ON
#define PARAM3 ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_12
#define PARAM4 SKIP_SCAN_ALL
#define PARAM5 ENABLE_AN4_ANA | ENABLE_AN5_ANA
// configure to sample AN4 & AN5
SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN4 | ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN5);
// configure ADC and enable it
OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 );
// Now enable the ADC logic
EnableADC10();
}


void setupPWM(){
int samplerate;
 SYSTEMConfigPerformance(72000000L);  // This function sets the PB-Div to 1. Also optimises cache for 72Mhz etc..
 mOSCSetPBDIV(OSC_PB_DIV_2);           // Therefore, configure the PB bus to run at 1/2 CPU Frequency
                                                              // you may run at PBclk of 72Mhz if you like too (omit this step)
                                                              // This will double the PWM frequency.
 INTEnableSystemMultiVectoredInt();         // make separate interrupts possible
 
 samplerate = SAMPLE_RATE;
 
 OpenOC1(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE,0,0); // init OC1 module, T2 =source 
 OpenOC2(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE,0,0); // init OC2 module, T2 =source(you could do more OCx)
 OpenTimer2(T2_ON | T2_PS_1_1 | T2_SOURCE_INT,FPB/samplerate);         // init Timer2 mode and period reg (PR2)
 
 PR2 = FPB/samplerate-1;
 
 mT2SetIntPriority(2);  // you don't have to use ipl7, but make sure INT definition is the same as your choice here
 mT2ClearIntFlag();     // make sure no int will be pending until 7200 counts from this point.  
 mT2IntEnable(1);       // allow T2 int
 

}

void readADC()
{
  // determine which buffer is idle and create an offset
offset = 8 * ((~ReadActiveBufferADC10() & 0x01));
// read the result of channel 4 conversion in the idle buffer
channel4 = ReadADC10(offset);
// read the result of channel 5 conversion in the idle buffer
channel5 = ReadADC10(offset + 1);
}

void setup(){
  mPORTBSetPinsAnalogIn(BIT_4 | BIT_5);  // Analog inputPins A1 and A7
  
  //PWM output pins
  PORTSetPinsDigitalOut(IOPORT_D, BIT_1); // Pin 5: A1 signal
  PORTSetPinsDigitalOut(IOPORT_D, BIT_0);  // Pin 3: A3 signal
  
  //Digital Button inputs
  PORTSetPinsDigitalIn(IOPORT_D, BIT_10);   // Pin 8
  PORTSetPinsDigitalIn(IOPORT_D, BIT_2);   // Pin 6
  
  //Digital A's control output pins
  PORTSetPinsDigitalOut(IOPORT_D, BIT_8);  // A4 signal: Pin 2
  PORTSetPinsDigitalOut(IOPORT_D, BIT_9);  // A2 signal: Pin 7
  
  setupADC();
  setupPWM();
  
}


void loop(){
  
  readADC();
  
  boton1 = PORTReadBits(IOPORT_D, BIT_10);
  boton2 = PORTReadBits(IOPORT_D, BIT_2);
  
  if (boton1){
    PWMvalue1 = map(channel4, 0, 1023, 7200, 0);
    PORTSetBits(IOPORT_D, BIT_8);
  }
  else
  {
    PWMvalue1 = map(channel4, 0, 1023, 0, 7200);
    PORTClearBits(IOPORT_D, BIT_8);
  }
  
  if (boton2){
    PWMvalue2 = map(channel5, 0, 1023, 7200, 0);
    PORTSetBits(IOPORT_D, BIT_9);
  }
  else
  {
    PWMvalue2 = map(channel5, 0, 1023, 0, 7200);
    PORTClearBits(IOPORT_D, BIT_9);
  }
}
  
