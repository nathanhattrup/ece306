//Movement state machines
//============================================
// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  <stdio.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"

extern char event;      //event tag
char state = WAIT;     //state tag

extern volatile unsigned int universal_count;
extern volatile unsigned int second_count;

extern char display_line[4][11];
extern volatile unsigned char display_changed;

unsigned char proj6_count;
extern volatile unsigned int ADC_Left_Det;
extern volatile unsigned int ADC_Right_Det;
extern volatile unsigned int left_calibration;
extern volatile unsigned int right_calibration;

//------------------------------------------------------------------------------
//MOTORS
//------------------------------------------------------------------------------
void motors_off(void){          //turns all motors off

    RIGHT_FORWARD_SPEED = WHEEL_OFF;  // right forward off
    LEFT_FORWARD_SPEED = WHEEL_OFF;   // left forward off
    RIGHT_REVERSE_SPEED = WHEEL_OFF;  // right reverse off
    LEFT_REVERSE_SPEED = WHEEL_OFF;   // left reverse off
}

void motors_forward(void){

    RIGHT_REVERSE_SPEED = WHEEL_OFF;  // right reverse off
    LEFT_REVERSE_SPEED = WHEEL_OFF;   // left reverse off

    RIGHT_FORWARD_SPEED = SLOW;
    LEFT_FORWARD_SPEED = SLOW;
}

void motors_reverse(void){

    RIGHT_FORWARD_SPEED = WHEEL_OFF;  // right forward off
    LEFT_FORWARD_SPEED = WHEEL_OFF;   // left forward off

    RIGHT_REVERSE_SPEED = SLOW;
    LEFT_REVERSE_SPEED = SLOW;
}

void motors_turn_cw(void){

/*    strcpy(display_line[1], " Spin CW  ");
    strcpy(display_line[2], "          ");
    display_changed = TRUE; */

    motors_off();

    LEFT_FORWARD_SPEED = SLOW;
    RIGHT_REVERSE_SPEED = SLOW;
}

void motors_turn_ccw(void){

/*    strcpy(display_line[1], " Spin CCW ");
    strcpy(display_line[2], "          ");
    display_changed = TRUE; */

    motors_off();

    RIGHT_FORWARD_SPEED = SLOW;
    LEFT_REVERSE_SPEED = SLOW;
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
    proj6_count = 0;

}

void end_case(void){

    motors_off();       //TURNS OFF MOTOR
    detector_display();     //displays values
    universal_count = 0;
    second_count = 0;
    state = WAIT;       //moves to wait
    event = NONE;       //sets event to none
}

void run_case_straight(void){
    detector_display();     //displays values

        strcpy(display_line[1], "Black Line");
        strcpy(display_line[2], " Detected ");
        display_changed = TRUE;

    switch(proj6_count){
    case 0: //proj6 state 0
        //checks if either right or left is within 90% of calibration
        if ((ADC_Left_Det >= (left_calibration * 0.9)) || (ADC_Right_Det >= (right_calibration * 0.9))) {
                motors_off();
                proj6_count++;      //inc proj 6 states
                second_count = 0;       //time reset
                universal_count = 0;
        }
        break;
    case 1: //proj 6 state 1
        if(second_count >= 4) {  //after 4 seconds
            motors_turn_cw();   //turn
            if ((ADC_Left_Det >= (left_calibration * 0.9)) && (ADC_Right_Det >= (right_calibration * 0.9))) {
            state = END;
            }
        }
        break;
    default: break;
    }

}

