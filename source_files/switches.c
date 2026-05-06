/*
 * This is where I have my functions for reading switches 1 and 2
 *
 *  Created on: Feb 18, 2026
 *      Author: nahat
 */

// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"


//defines global variables
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char update_display;
extern volatile unsigned char display_changed;
extern volatile unsigned int update_display_count;
char event;      //event tag

extern volatile unsigned int universal_count;
extern volatile unsigned int second_count;
extern volatile unsigned int stupid_count;

extern volatile unsigned int ADC_Left_Det;
extern volatile unsigned int ADC_Right_Det;
extern volatile unsigned int left_calibration;
extern volatile unsigned int right_calibration;

char speed;
char transmit_flag;
char baud_flag;
extern unsigned int display_rx_index;
//------------------------------------------------------------------------------
// Switch Interrupts yayy
//------------------------------------------------------------------------------
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
// Switch 1
    if (P4IFG & SW1) {      //redundant
        P4IE  &= ~SW1; // SW1 interrupt Disable
        P4IFG &= ~SW1; // IFG SW1 cleared
        TB0CCTL1 &= ~CCIFG; // Clear CCR1 interrupt flag
        TB0CCR1 = TB0R + TB0CCR1_INTERVAL;
        TB0CCTL1 |= CCIE;   //enable ccr1 interrupt

        P1OUT |= RED_LED; //turn on led

        universal_count = 0;
        second_count = 0;
        stupid_count = 0;
        event = PROJ;


    }
}
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
// Switch 2
    if (P2IFG & SW2) {
        P2IE &= ~SW2; //sw2 interrupt disable
        P2IFG &= ~SW2; // IFG SW2 cleared
        TB0CCTL2 &= ~CCIFG; //clear ccr2 interrupt flag
        TB0CCR2 = TB0R + TB0CCR2_INTERVAL;
        TB0CCTL2 |= CCIE;   //enable ccr2 interrupt

        P6OUT |= GRN_LED;       //turn on led

        universal_count = 0;
        second_count = 0;

        adc_calibrate();

//        speed = !speed;             //flips between 115k baud and 57k baud
//        baud_flag = 1;

    }
}
