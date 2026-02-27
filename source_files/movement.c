/*
 * Movement functions and motor controls

 */

// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  <stdio.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"

extern unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
extern unsigned int cycle_time; // is a new time base used to control making shapes
extern unsigned int time_change; // is an identifier that a change has occurred

extern char event;      //event tag
char state = WAIT;     //state tag

extern volatile unsigned int universal_count;
extern volatile unsigned int second_count;

extern char display_line[4][11];
extern volatile unsigned char display_changed;

//turning these defines into variables
int CRCL_DIST = 29;
int CRCL_DIST_HLF = 14;
int TURN_DIST = 3;
int STR_DIST = 6;

//------------------------------------------------------------------------------
//MOTORS
//------------------------------------------------------------------------------
void motors_off(void){          //turns all motors off

    P6OUT  &= ~R_FORWARD;       //turns off r forward
    P6OUT  &= ~L_FORWARD;       //turns off l forward
    P6OUT  &= ~R_REVERSE;       //turns off r reverse
    P6OUT  &= ~L_REVERSE;       //turns off l reverse
}

void motors_forward(void){

    strcpy(display_line[1], " Forward  ");
    strcpy(display_line[2], "          ");
    display_changed = TRUE;

    P6OUT  &= ~R_REVERSE;       //turns off r reverse
    P6OUT  &= ~L_REVERSE;       //turns off l reverse

    P6OUT  |=  R_FORWARD;       //turns on r forward
    P6OUT  |=  L_FORWARD;       //turns on l forward
}

void motors_reverse(void){
    strcpy(display_line[1], " Reverse  ");
    strcpy(display_line[2], "          ");
    display_changed = TRUE;

    P6OUT  &= ~R_FORWARD;       //turns off r forward
    P6OUT  &= ~L_FORWARD;       //turns off l forward

    P6OUT  |=  R_REVERSE;       //turns on r reverse
    P6OUT  |=  L_REVERSE;       //turns on l reverse

}

void motors_turn_cw(void){

    strcpy(display_line[1], " Spin CW  ");
    strcpy(display_line[2], "          ");
    display_changed = TRUE;

    P6OUT  &= ~R_FORWARD;       //turns off r forward
    P6OUT  &= ~L_FORWARD;       //turns off l forward
    P6OUT  &= ~R_REVERSE;       //turns off r reverse
    P6OUT  &= ~L_REVERSE;       //turns off l reverse

    P6OUT  |=  L_FORWARD;       //turns on l forward
    P6OUT  |=  R_REVERSE;       //turns on r reverse
}

void motors_turn_ccw(void){

    strcpy(display_line[1], " Spin CCW ");
    strcpy(display_line[2], "          ");
    display_changed = TRUE;

    P6OUT  &= ~R_FORWARD;       //turns off r forward
    P6OUT  &= ~L_FORWARD;       //turns off l forward
    P6OUT  &= ~R_REVERSE;       //turns off r reverse
    P6OUT  &= ~L_REVERSE;       //turns off l reverse

    P6OUT  |=  R_FORWARD;       //turns on r forward
    P6OUT  |=  L_REVERSE;       //turns on l reverse
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Switch Statements
//------------------------------------------------------------------------------


void Run_Straight(void){        //straight but dont need it
  switch(state){
    case WAIT: // Begin wait
        wait_case();
      break; //
    case START: // Begin start
      start_case();
      break; //
    case RUN: // Run
      run_case_straight();
      break; //
    case END: // End
      end_case();
      break; //
    default: break;
}
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//General cases
//------------------------------------------------------------------------------
void wait_case(void){
    switch(second_count){
    case 2:                 //waits for second count to be 2 (will be a lil off)
        state = START;        //moves to start
        break;
    default: break;
    }
}

void start_case(void){
    universal_count = 0;
    second_count = 0;
    motors_forward();         //TURN ON MOTORS
    state = RUN;          //moves to run

    P1OUT |= RED_LED;
}

void end_case(void){
    strcpy(display_line[0], " Nathan   ");
    strcpy(display_line[1], " Hattrup  ");
    strcpy(display_line[2], " SW1 SW2  ");
    strcpy(display_line[3], " <-- -->  ");
    display_changed = TRUE;

    motors_off();       //TURNS OFF MOTOR
    universal_count = 0;
    second_count = 0;
    state = WAIT;       //moves to wait
    event = NONE;       //sets event to none
}

void run_case_straight(void){
    sprintf(display_line[3], " %d sec. ", second_count);
    display_changed = TRUE;


    switch(second_count){
    case 1:
        motors_off();
        strcpy(display_line[1], " Idling.. ");
        strcpy(display_line[2], "          ");
        display_changed = TRUE;

        break;
    case 2:
        motors_reverse();

        break;
    case 4:
        motors_off();
        strcpy(display_line[1], " Idling.. ");
        strcpy(display_line[2], "          ");
        display_changed = TRUE;

        break;
    case 5:
        motors_forward();

        break;
    case 6:
        motors_off();
        strcpy(display_line[1], " Idling.. ");
        strcpy(display_line[2], "          ");
        display_changed = TRUE;

        break;
    case 7:
        motors_turn_cw();

        break;
    case 10:
        motors_off();
        strcpy(display_line[1], " Idling.. ");
        strcpy(display_line[2], "          ");
        display_changed = TRUE;

        break;
    case 12:
        motors_turn_ccw();

        break;
    case 15:
        motors_off();
        strcpy(display_line[1], " Idling.. ");
        strcpy(display_line[2], "          ");
        display_changed = TRUE;

        break;
    case 17:
        state = END;
        break;
    default: break;
    }

}

