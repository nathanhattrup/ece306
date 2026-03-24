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

unsigned char proj_count;
extern volatile unsigned int ADC_Left_Det;
extern volatile unsigned int ADC_Right_Det;
extern unsigned int left_calibration;
extern unsigned int right_calibration;
extern unsigned int left_calibration_60;
extern unsigned int right_calibration_60;
extern unsigned int left_calibration_70;
extern unsigned int right_calibration_70;
extern unsigned int left_calibration_80;
extern unsigned int right_calibration_80;

int detect_diff;

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

    RIGHT_FORWARD_SPEED = SLOW_L;
    LEFT_FORWARD_SPEED = SLOW_L;
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
        strcpy(display_line[0], "Project 7 ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "          ");
        display_changed = TRUE;

    switch(state){
    case WAIT: // Begin wait
      wait_case();
      break; //
    case START: // Begin start
      start_case();
      break; //
    case RUN: // Run
      run_case_line_detect();
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
    proj_count = 0;

}

void end_case(void){
    strcpy(display_line[0], "Stopped   ");
    display_changed = TRUE;

    motors_off();       //TURNS OFF MOTOR
    universal_count = 0;
    second_count = 0;
    state = WAIT;       //moves to wait
    event = NONE;       //sets event to none
}

void run_case_line_detect(void){
//    detector_display();     //displays values

    switch(proj_count){
    case 0: //proj6 state 0
        strcpy(display_line[0], "Intercept.");
        display_changed = TRUE;

        //checks if either right or left is within 70% of calibration
        if ((ADC_Left_Det >= (left_calibration_70)) || (ADC_Right_Det >= (right_calibration_70))) {
                motors_off();
                proj_count++;      //inc proj 6 states
                second_count = 0;       //time reset
                universal_count = 0;
        }
        break;
    case 1: //proj 6 state 1
        strcpy(display_line[0], "Waiting   ");
        display_changed = TRUE;

        if(second_count >= 1) {
            strcpy(display_line[0], "Turning   ");
            display_changed = TRUE;

            motors_turn_cw();   //turn
            if ((ADC_Left_Det >= (left_calibration_80)) && (ADC_Right_Det >= (right_calibration_80))) {
                motors_off();
                proj_count++;      //inc proj states
                second_count = 0;       //time reset
                universal_count = 0;
            }
        }
        break;
    case 2: //proj7 state
        strcpy(display_line[0], "Circling  ");
        display_changed = TRUE;

        run_case_line_follow();
        if(second_count >= 45){     //gives 40s to do 2x full circles
            motors_off();
            proj_count++;      //inc proj states
            second_count = 0;       //time reset
            universal_count = 0;
        }
        break;
    case 3: //proj7 end
        strcpy(display_line[0], "Ending    ");
        display_changed = TRUE;

        switch(second_count){
        case 0:
           motors_turn_cw();
           if(universal_count >= 50){
               motors_off();
           }
           break;
        case 1:
            motors_forward();
            break;
        case 2:
            if(universal_count >= 50){
               motors_off();
               state = END;
            }
            break;
        default: break;
        }
        break;
    default: break;
    }

}

void run_case_line_follow(void){
    detect_diff = ADC_Left_Det - (ADC_Right_Det + 50 );     //Positive: Left on black     Negative: Right on black

    //150 is magic number for now. change later to BLACK-WHITE / 4
    if(abs(detect_diff) <= 150){     //first statement checks if car is on SAME COLOR
        if(ADC_Right_Det >= (right_calibration_60)){  //if car is on black line
            motors_forward();
        }
        else{                   //car on white
            motors_reverse();
        }
    }
    else{       //diff > 150 meaning car on DIFF COLORS
        if(detect_diff > 0){        //positive so left on black right off
            RIGHT_FORWARD_SPEED = WHEEL_OFF;  // right forward off
            LEFT_FORWARD_SPEED = WHEEL_OFF;   // left forward off
            RIGHT_REVERSE_SPEED = WHEEL_OFF;     //right rev off
            LEFT_REVERSE_SPEED = SLOW;      //turn left
        }
        else{       //negative so right on black left off
            RIGHT_FORWARD_SPEED = WHEEL_OFF;  // right forward off
            LEFT_FORWARD_SPEED = WHEEL_OFF;   // left forward off
            LEFT_REVERSE_SPEED = WHEEL_OFF;     //left rev off
            RIGHT_REVERSE_SPEED = SLOW;     //turn to right
        }
    }
}
