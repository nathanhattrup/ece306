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
extern unsigned int left_calibration_75;
extern unsigned int right_calibration_75;
extern unsigned int left_calibration_85;
extern unsigned int right_calibration_85;
extern unsigned int left_calibration_90;
extern unsigned int right_calibration_90;
extern unsigned int cmd_state;

extern unsigned int white_diff;
extern unsigned int left_white;
extern unsigned int right_white;

unsigned int SLOW;      //redefined as variables pls dont kill me carlson
unsigned int SLOW_L;

int detect_diff;

char exit_flag;
unsigned int pad8_state;
extern unsigned int display_state;

unsigned int line_int_state;

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
    LEFT_FORWARD_SPEED = SLOW_L;
}

void motors_reverse(void){

    RIGHT_FORWARD_SPEED = WHEEL_OFF;  // right forward off
    LEFT_FORWARD_SPEED = WHEEL_OFF;   // left forward off

    RIGHT_REVERSE_SPEED = SLOW;
    LEFT_REVERSE_SPEED = SLOW_L;
}

void motors_turn_cw(void){      //turns clockwise

    motors_off();

    LEFT_FORWARD_SPEED = SLOW_L;
    RIGHT_REVERSE_SPEED = SLOW;
}

void motors_turn_ccw(void){     //turns counterclockwise

    motors_off();

    RIGHT_FORWARD_SPEED = SLOW;
    LEFT_REVERSE_SPEED = SLOW_L;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Switch Statements
//------------------------------------------------------------------------------


void Run_Straight(void){        //it says straight but this is the movement pattern i use for pad 8 stuff

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
    if (universal_count >= 50){               //waits for half a second
        state = START;        //moves to start
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

//------------------------------------------------------

void run_case_line_detect(void){
//    detector_display();     //displays values

    switch(proj_count){
    case 0: //proj6 state 0

        //checks if either right or left is within 80% of calibration (was 70)
        if ((ADC_Left_Det >= (left_calibration_80)) || (ADC_Right_Det >= (right_calibration_80))) {
                motors_off();
                proj_count++;      //inc proj 6 states
                second_count = 0;       //time reset
                universal_count = 0;
                display_state = 10;     //intercept
        }
        break;
    case 1: //proj 6 state 1
        if(second_count >= 10) {        //waits 10 seconds
            display_state = 11;     //turnings
            motors_turn_ccw();   //turn     was 80
            if ((ADC_Left_Det >= (left_calibration_85)) && (ADC_Right_Det >= (right_calibration_85))) {
                motors_off();
                proj_count++;      //inc proj states
                second_count = 0;       //time reset
                universal_count = 0;

            }
        }
        break;
    case 2:
        if (second_count >= 10){    //waits 10s
            display_state = 12;     //traveling
            proj_count++;      //inc proj states
            second_count = 0;       //time reset
            universal_count = 0;
        }
        break;
    case 3: //proj7 state
        run_case_line_follow();
        if(second_count >= 12){     //waits 12s
            motors_off();
            proj_count++;      //inc proj states
            second_count = 0;       //time reset
            universal_count = 0;
            display_state = 13;     //le circle
        }
        break;
    case 4: //proj7 state
        if(second_count >= 10){     //waits 10s
            proj_count++;      //inc proj states
            second_count = 0;       //time reset
            universal_count = 0;
        }
        break;
    case 5: //proj7 state
        run_case_line_follow();
        if(exit_flag){     //CHANGED TO LINE EXIT
            motors_off();
            proj_count++;      //inc proj states
            second_count = 0;       //time reset
            universal_count = 0;
            P2OUT &=  ~IOT_RUN_CPU;
            cmd_state = 1;
            display_state = 14;     //exiting
            exit_flag = 0;
        }
        break;
    case 6: //proj7 end
        switch(second_count){
        case 10:     //waits 10 sec to go forward
            motors_forward();
            break;
        case 13:
            //waits another 3s
        motors_off();
        state = END;
        display_state = 15;
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
    if(abs(detect_diff) <= 90){     //first statement checks if car is on SAME COLOR
        if(ADC_Left_Det >= (left_calibration_75)){  //if car is on black line         75 left is decent
            motors_forward();
        }
        else{                   //car on white
            motors_reverse();
        }
    }
    else{       //diff > 150 meaning car on  DIFF COLORS
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

//=================================================================
//IOT commands

void stop_cmd(void){        //le stop
    motors_off();
    P2OUT &=  ~IOT_RUN_CPU;
    cmd_state = 1;
}

void fwd_cmd(int time)
{     //time in ticks of 10ms, 200 -> 2s
    motors_forward();

    if (time != 0)
    {  //if time == 0, run forever
        if (((second_count * 100) + universal_count) >= time)   //seconds + ticks
        {
            motors_off();
            P2OUT &= ~IOT_RUN_CPU;
            cmd_state = 1;
        }
    }
    else
    {
        cmd_state = 1;      //even with 0, need to read next command
    }
}

void rev_cmd(int time){     //time in ticks of 10ms, 200 -> 2s
    motors_reverse();

    if (time != 0)
        {  //if time == 0, run forever
            if (((second_count * 100) + universal_count) >= time)
            {
                motors_off();
                P2OUT &= ~IOT_RUN_CPU;
                cmd_state = 1;
            }
        }
        else
        {
            cmd_state = 1;      //even with 0, need to read next command
        }

}

void lturn_cmd(int angle){      //left turn
    motors_turn_ccw();
    int time;
    switch(angle){
    case 0: //slow carrrrr
        time = 20;
        break;
    case 1:// fast carrrrrr
        time = 10;
        break;
    default: break;
    }

    if(((second_count * 100) + universal_count) >= time){
        motors_off();
        P2OUT &=  ~IOT_RUN_CPU;
        cmd_state = 1;
    }
}

void rturn_cmd(int angle){      //right turn
    motors_turn_cw();
    int time;
    switch(angle){
    case 0: //slow carrrrr
        time = 20;
        break;
    case 1: //fast carrrrrr
        time = 10;
        break;
    default: break;
    }
    if(((second_count * 100) + universal_count) >= time){
        motors_off();
        P2OUT &=  ~IOT_RUN_CPU;
        cmd_state = 1;
    }
}

void speed_change(int num){     //use this to change car speed
    switch(num){
    case 0:
        SLOW = 15000;   //im sorryyyyy
        SLOW_L = 16000;
        P2OUT &=  ~IOT_RUN_CPU;
        cmd_state = 1;
        break;
    case 1:
        SLOW = 39000;   //im sorryyyyy
        SLOW_L = 50000;
        P2OUT &=  ~IOT_RUN_CPU;     //carlson said changing pwm values like this is evil and im failing the class :(
        cmd_state = 1;
        break;
    default: break;
    }
}

//UNUSED
void line_int(void){        //intercepts that jawn
    event = PROJ;           //just does basic proj7 rn
    P2OUT &=  ~IOT_RUN_CPU;
    cmd_state = 1;

}

void pad8(void){        //lowkey just hitting this on pad 8

    display_state = 9;
    switch (pad8_state)
    {
    case 0:
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;

        LEFT_FORWARD_SPEED = 34000;         //big ole arch      32000 - 17000 was working PERFECTLYYY until it didnt
        RIGHT_FORWARD_SPEED = 17000;        //actually lost my mind trying to make an arch
        if (second_count >= 2)
        {
            if (ADC_Right_Det <= right_white + 100)
            {      //looks for white after 2s
                pad8_state++;
                universal_count = 0;
            }
        }
        break;
    case 1:
        if (universal_count >= 25)
        {     //waits another quarter second
            if (ADC_Right_Det <= right_white + 100)    //checks for another white
            {      //looks for white after 5s
                motors_turn_cw();
                pad8_state++;       //moves forward state
                universal_count = 0;
            }else{
                pad8_state--;   //moves back state
            }
        }

        break;
    case 2:
        if (universal_count >= 30)
        {      //then a half second turn
            motors_off();
            event = PROJ;       //STARTS THE PROJECT BABYYYY

            P2OUT &= ~IOT_RUN_CPU;
            cmd_state = 1;
            second_count = 0;
            universal_count = 0;
            pad8_state = 0;
        }
        break;
    default: break;
    }
}

