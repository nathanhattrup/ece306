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
unsigned int sw1_position;
unsigned int sw2_position;
unsigned int okay_to_look_at_switch1;
unsigned int okay_to_look_at_switch2;
unsigned int count_debounce_SW1;
unsigned int count_debounce_SW2;
char event;      //event tag

void Init_Switches(void){
// sets initial values of switches to released
    sw1_position = RELEASED;
    sw2_position = RELEASED;
    okay_to_look_at_switch1 = OKAY;
    okay_to_look_at_switch2 = OKAY;
    count_debounce_SW1 = DEBOUNCE_RESTART;
    count_debounce_SW2 = DEBOUNCE_RESTART;
}

//------------------------------------------------------------------------------
void Switches_Process(void){
//------------------------------------------------------------------------------
// This function calls the individual Switch Functions
//------------------------------------------------------------------------------
Switch1_Process();
Switch2_Process();
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Switch1_Process(void){
//------------------------------------------------------------------------------
// Switch 1 Configurations
// Port P4 Pin 1
//------------------------------------------------------------------------------
if (okay_to_look_at_switch1 && sw1_position){       //checks if its okay and switch is released
if (!(P4IN & SW1)){         //checks if physical switch is pressed
sw1_position = PRESSED;     //sets low
okay_to_look_at_switch1 = NOT_OKAY;     //sets low
count_debounce_SW1 = DEBOUNCE_RESTART;      //resets timer
// do what you want with button press
event = CIRCLE;       //calls straight function

P1OUT |= RED_LED; //turn on led
}
}
if (count_debounce_SW1 <= DEBOUNCE_TIME){       //if time is below time limmit
count_debounce_SW1++;       //incs time
}else{
okay_to_look_at_switch1 = OKAY;     //otherwise sets switch as okay to look at
if (P4IN & SW1){        //checks if physical switch is released
sw1_position = RELEASED;        //sets high
P1OUT &= ~RED_LED; //turn off led
}
}
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Switch2_Process(void){
//------------------------------------------------------------------------------
// Switch 2 Configurations
// Port P2 Pin 3
//------------------------------------------------------------------------------
if (okay_to_look_at_switch2 && sw2_position){
if (!(P2IN & SW2)){
sw2_position = PRESSED;
okay_to_look_at_switch2 = NOT_OKAY;
count_debounce_SW2 = DEBOUNCE_RESTART;
// do what you want with button press
event = TRIANGLE;
P6OUT |= GRN_LED;       //turn on led
}
}
if (count_debounce_SW2 <= DEBOUNCE_TIME){
count_debounce_SW2++;
}else{
okay_to_look_at_switch2 = OKAY;
if (P2IN & SW2){
sw2_position = RELEASED;
P6OUT &= ~GRN_LED;      //turn off led
}
}
}
//------------------------------------------------------------------------------

