/*
IOT yayyy
 */
//------------------------------------------------------------------------------
// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"

extern volatile unsigned int universal_count;
extern volatile char transmit_buffer[32];
char iot_init_flag;
extern volatile char dac_flag;

//idk why i made this its own .c ngl
void Init_IOT(void){

    if(dac_flag && iot_init_flag){     //waits until 100ms have elapsed
        P3OUT |= IOT_EN_CPU;        //sets high FOREVER
        P3OUT |=  IOT_LINK_CPU;
        iot_init_flag = 0;
    }


}
