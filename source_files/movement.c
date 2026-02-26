/*
 * Movement functions and motor controls

 */

// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"

extern unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
extern unsigned int cycle_time; // is a new time base used to control making shapes
extern unsigned int time_change; // is an identifier that a change has occurred

extern char event;      //event tag
char state = WAIT;     //state tag
unsigned int delay_start;
unsigned int right_motor_count;
unsigned int left_motor_count;
unsigned int segment_count;
unsigned int circle_count;
unsigned int turn_count;

extern char display_line[4][11];
extern volatile unsigned char display_changed;

//turning these defines into variables
int CRCL_DIST = 29;
int CRCL_DIST_HLF = 14;
int TURN_DIST = 3;
int STR_DIST = 6;

//------------------------------------------------------------------------------
// OLD MOTORS
//------------------------------------------------------------------------------
void motors_off(void){          //turns all motors off

    P6OUT  &= ~R_FORWARD;       //turns off r forward
    P6OUT  &= ~L_FORWARD;       //turns off l forward
    P6OUT  &= ~R_REVERSE;       //turns off r reverse
    P6OUT  &= ~L_REVERSE;       //turns off l reverse
}

void motors_forward(void){

    P6OUT  &= ~R_REVERSE;       //turns off r reverse
    P6OUT  &= ~L_REVERSE;       //turns off l reverse

    P6OUT  |=  R_FORWARD;       //turns on r forward
    P6OUT  |=  L_FORWARD;       //turns on l forward
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
      run_straight();
      break; //
    case END: // End
      end_case();
      break; //
    default: break;
}
}


void Run_Circle(void){      //circle
  switch(state){
    case WAIT: // Begin wait
      wait_case();
      break; //
    case START: // Begin start
        CRCL_DIST = 29;
        CRCL_DIST_HLF = 14;
        circle_count = 0;
        start_case();
      break; //
    case RUN: // Run
      run_case_circle_ccw();        //helper function outerloop
      break; //
    case END: // End
        end_case_circle();
      break; //
    default: break;
}
}

void Run_Fig8(void){        //figure 8
  switch(state){
    case WAIT: // Begin wait
      wait_case();
      break; //
    case START: // Begin start
        CRCL_DIST = 29;     //reset distances
        CRCL_DIST_HLF = 14;
        STR_DIST = 6;
        circle_count = 0;
        start_case();
      break; //
    case RUN: // Run
      run_case_fig8();        //helper function outerloop
      break; //
    case END: // End
        end_case();
      break; //
    default: break;
}
}

void Run_Triangle(void){        //triangle
  switch(state){
    case WAIT: // Begin wait
      wait_case();
      break; //
    case START: // Begin start
        TURN_DIST = 3;
        turn_count = 0;
        start_case();
      break; //
    case RUN: // Run
      run_case_triangle();        //helper function outerloop
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
  if(time_change){
    time_change = 0;        //reset time change
    if(delay_start++ >= WAITING2START){     //sees when delay is met
      delay_start = 0;      //reset delay counter
      state = START;        //moves to start
    }
  }
}

void start_case(void){
  cycle_time = 0;       //reset cycle time
  right_motor_count = 0;        //reset right count
  left_motor_count = 0;         //reset left count
  motors_forward();         //TURN ON MOTORS
  segment_count = 0;        //reset segment count
  state = RUN;          //moves to run
}

void end_case(void){
strcpy(display_line[0], " Nathan   ");
strcpy(display_line[1], " Hattrup  ");
strcpy(display_line[2], " SW1 SW2  ");
strcpy(display_line[3], " <-- -->  ");
display_changed = TRUE;

motors_off();       //TURNS OFF MOTOR
state = WAIT;       //moves to wait
event = NONE;       //sets event to none
}

void end_case_circle(void){
strcpy(display_line[0], " Nathan   ");
strcpy(display_line[1], " Hattrup  ");
strcpy(display_line[2], " SW1 SW2  ");
strcpy(display_line[3], " <-- -->  ");
display_changed = TRUE;

motors_off();       //TURNS OFF MOTOR
state = WAIT;       //moves to wait
event = FIG8;       //sets event to none
}

//------------------------------------------------------------------------------
//Individual cases
//------------------------------------------------------------------------------

//STRAIGHT----------------------------------------------------------------------
void run_straight(void){
    P1OUT ^= RED_LED;
    if (time_change) {
        time_change = 0;
        if (segment_count <= STR_DIST) {
            if (right_motor_count++ >= R_CNT_STR) {
                P6OUT &= ~R_FORWARD;   // Turn off right wheel
            }
            if (left_motor_count++ >= L_CNT_STR) {
                P6OUT &= ~L_FORWARD;   // Turn off left wheel
            }
            if (cycle_time >= W_CNT) {
                cycle_time = 0;            // Reset the period
                right_motor_count = 0;     // Reset wheel counters
                left_motor_count = 0;
                segment_count++;           // One segment complete
                motors_forward();            // Turn both wheels back ON
            }
        } else {
            turn_count++;
            segment_count = 0;
            right_motor_count = 0;     // Reset wheel counters
            left_motor_count = 0;
            motors_forward();            // Turn both wheels back ON
        }
    }
}


//Full CCW Circle---------------------------------------------------------------
void run_circle_ccw(void){
    if (time_change) {
        time_change = 0;
        if (segment_count <= CRCL_DIST) {
            if (right_motor_count++ >= R_CNT_CRCL_CCW) {
                P6OUT &= ~R_FORWARD;   // Turn off right wheel
            }
            if (left_motor_count++ >= L_CNT_CRCL_CCW) {
                P6OUT &= ~L_FORWARD;   // Turn off left wheel
            }
            if (cycle_time >= W_CNT) {
                cycle_time = 0;            // Reset the period
                right_motor_count = 0;     // Reset wheel counters
                left_motor_count = 0;
                segment_count++;           // One segment complete
                motors_forward();            // Turn both wheels back ON
            }
        } else {
            circle_count++;
            segment_count = 0;
            right_motor_count = 0;     // Reset wheel counters
            left_motor_count = 0;
            motors_forward();            // Turn both wheels back ON
        }
    }
}

//Full CW Circle---------------------------------------------------------------
void run_circle_cw(void){
    if (time_change) {
        time_change = 0;
        if (segment_count <= CRCL_DIST) {
            if (right_motor_count++ >= R_CNT_CRCL_CW) {
                P6OUT &= ~R_FORWARD;   // Turn off right wheel
            }
            if (left_motor_count++ >= L_CNT_CRCL_CW) {
                P6OUT &= ~L_FORWARD;   // Turn off left wheel
            }
            if (cycle_time >= W_CNT) {
                cycle_time = 0;            // Reset the period
                right_motor_count = 0;     // Reset wheel counters
                left_motor_count = 0;
                segment_count++;           // One segment complete
                motors_forward();            // Turn both wheels back ON
            }
        } else {
            circle_count++;
            segment_count = 0;
            right_motor_count = 0;     // Reset wheel counters
            left_motor_count = 0;
            motors_forward();            // Turn both wheels back ON
        }
    }
}

//Half CCW Circle---------------------------------------------------------------
void run_circle_ccw_half(void){
    if (time_change) {
        time_change = 0;
        if (segment_count <= CRCL_DIST_HLF) {
            if (right_motor_count++ >= R_CNT_CRCL_CCW) {
                P6OUT &= ~R_FORWARD;   // Turn off right wheel
            }
            if (left_motor_count++ >= L_CNT_CRCL_CCW) {
                P6OUT &= ~L_FORWARD;   // Turn off left wheel
            }
            if (cycle_time >= W_CNT) {
                cycle_time = 0;            // Reset the period
                right_motor_count = 0;     // Reset wheel counters
                left_motor_count = 0;
                segment_count++;           // One segment complete
                motors_forward();            // Turn both wheels back ON
            }
        } else {
            circle_count++;
            segment_count = 0;
            right_motor_count = 0;     // Reset wheel counters
            left_motor_count = 0;
            motors_forward();            // Turn both wheels back ON
        }
    }
}

//Half CW Circle---------------------------------------------------------------
void run_circle_cw_half(void){
    if (time_change) {
        time_change = 0;
        if (segment_count <= CRCL_DIST_HLF) {
            if (right_motor_count++ >= R_CNT_CRCL_CW) {
                P6OUT &= ~R_FORWARD;   // Turn off right wheel
            }
            if (left_motor_count++ >= L_CNT_CRCL_CW) {
                P6OUT &= ~L_FORWARD;   // Turn off left wheel
            }
            if (cycle_time >= W_CNT) {
                cycle_time = 0;            // Reset the period
                right_motor_count = 0;     // Reset wheel counters
                left_motor_count = 0;
                segment_count++;           // One segment complete
                motors_forward();            // Turn both wheels back ON
            }
        } else {
            circle_count++;
            segment_count = 0;
            right_motor_count = 0;     // Reset wheel counters
            left_motor_count = 0;
            motors_forward();            // Turn both wheels back ON
        }
    }
}

//Turn---------------------------------------------------------------------------
void run_turn(void){
    if (time_change) {
        time_change = 0;
        if (segment_count <= TURN_DIST) {
            if (right_motor_count++ >= R_CNT_TRN) {
                P6OUT &= ~R_FORWARD;   // Turn off right wheel
            }
            if (left_motor_count++ >= L_CNT_TRN) {
                P6OUT &= ~L_FORWARD;   // Turn off left wheel
            }
            if (cycle_time >= W_CNT_TRN) {
                cycle_time = 0;            // Reset the period
                right_motor_count = 0;     // Reset wheel counters
                left_motor_count = 0;
                segment_count++;           // One segment complete
                motors_forward();            // Turn both wheels back ON
            }
        } else {
            turn_count++;
            segment_count = 0;
            right_motor_count = 0;     // Reset wheel counters
            left_motor_count = 0;
            motors_forward();            // Turn both wheels back ON
        }
    }
}


//------------------------------------------------------------------------------
//My silly helper functions
//------------------------------------------------------------------------------
//Circle------------------------------------------------------------------------
void run_case_circle_ccw(void){      //runs a circle twice
//Display
    strcpy(display_line[0], " Running  ");
    strcpy(display_line[1], " Circle   ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;

   switch(circle_count){
   case 0:
       run_circle_ccw();
       break;
   case 1:
       run_circle_ccw();
       break;
   case 2:      //after 2x loops it moves on
       state = END;
       circle_count = 0;
       break;
   default:
       break;
   }
}

//FIG8--------------------------------------------------------------------------
void run_case_fig8(void){
    //Display
        strcpy(display_line[0], " Running  ");
        strcpy(display_line[1], " Figure 8 ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;

   switch(circle_count){
   case 0:
       run_circle_ccw_half();
       break;
   case 1:
       CRCL_DIST = 35;
       run_circle_cw();
       break;
   case 2:
       CRCL_DIST = 32;
       run_circle_ccw();
       break;
   case 3:
       CRCL_DIST = 35;
       run_circle_cw();
       break;
   case 4:
       run_circle_ccw_half();
       break;
   case 5:
       state = END;
       circle_count = 0;
       break;
   default:
       break;
   }
}

//Triangle-----------------------------------------------------------------------
void run_case_triangle(void){
    //Display
        strcpy(display_line[0], " Running  ");
        strcpy(display_line[1], " Triangle ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;

   switch(turn_count){
   case 0:
       run_straight();
       break;
   case 1:
       run_turn();
       break;
   case 2:
       run_straight();
       break;
   case 3:
       TURN_DIST = 2;
       run_turn();
       break;
   case 4:
       run_straight();
       break;
   case 5:
       TURN_DIST = 3;
       run_turn();
       break;
   case 6:
       run_straight();
       break;
   case 7:
       TURN_DIST = 2;
       run_turn();
       break;
   case 8:
       run_straight();
       break;
   case 9:
       TURN_DIST = 3;
       run_turn();
       break;
   case 10:
       run_straight();
       break;
   case 11:        //change back to 11 after testing
       state = END;
       turn_count = 0;
       break;
   default:
       break;
   }
}
