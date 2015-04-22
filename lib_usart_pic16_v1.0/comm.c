
#include <xc.h>
#include "pin_definitions.h"


void setup_port(){
    data0_mode = INPUT;
    data1_mode = INPUT;
    data2_mode = INPUT;
    data3_mode = INPUT;
    ack_mode = OUTPUT;
}

unsigned char readPort()
{
    unsigned char data = 0x00;

    data |= data0;
    data |= data1 << 1;
    data |= data2 << 2;
    data |= data3 << 3;

    return data;
    
}



