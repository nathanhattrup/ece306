// Carlson's HEXtoBCD and ADC Line Insert Functions
//------------------------------------------------------------------------------
// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"

char i;
char adc_char[4];
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned int ADC_Thumb;
extern volatile unsigned int ADC_Left_Det;
extern volatile unsigned int ADC_Right_Det;
//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//-----------------------------------------------------------------
void HEXtoBCD (int hex_value) {
    int value = 0;
    for(i=0; i < 4; i++) {
        adc_char[i] = '0';
    }
    while (hex_value  > 999) {
        hex_value = hex_value - 1000;
        value = value + 1;
        adc_char[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99) {
        hex_value = hex_value - 100;
        value = value + 1;
        adc_char[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9) {
        hex_value = hex_value - 10;
        value = value +1;
        adc_char[2] = 0x30 + value;
    }
    adc_char[3] = 0x30 + hex_value;
}

//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 6
//
//-------------------------------------------------------------
void adc_line(char line, char location) {
    for(i=0; i <4; i++) {
        display_line[line] [i+location] = adc_char[i];
        display_changed = TRUE;
    }
}

void thumb_read(unsigned thumb_value) {
    if (thumb_value >= 2048){       //when thumbwheel is to the right
        strcpy(display_line[3], "Right:    ");
        display_changed = TRUE;
        HEXtoBCD(ADC_Right_Det);        //runs hex2bcd
        adc_line(3, 6);                 //displays on line 3
    }
    else if(thumb_value < 2048){    //when thumbhweel is to left
        strcpy(display_line[3], "Left:     ");
        display_changed = TRUE;
        HEXtoBCD(ADC_Left_Det);
        adc_line(3, 6);
    }
}

void detector_display(void){
    strcpy(display_line[2], "Right:    ");
    display_changed = TRUE;
    HEXtoBCD(ADC_Right_Det);
    adc_line(2, 6);

    strcpy(display_line[3], "Left:     ");
    display_changed = TRUE;
    HEXtoBCD(ADC_Left_Det);
    adc_line(3, 6);

}
