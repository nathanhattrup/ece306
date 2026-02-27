//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"



  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
 volatile unsigned char display_changed;
 volatile unsigned char update_display;
 volatile unsigned int update_display_count;
volatile unsigned int Time_Sequence;
volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;

unsigned int wheel_move;
char forward;

unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
unsigned int cycle_time; // is a new time base used to control making shapes
unsigned int time_change; // is an identifier that a change has occurred

extern char event;      //event tag

//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD

  motors_off();                        // turns off motors
  Init_Switches();                     // turns off switches
  event = NONE;                         //sets event to nonew

  //P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], " Nathan   ");
  strcpy(display_line[1], " Hattrup  ");
  strcpy(display_line[2], " SW1 SW2  ");
  strcpy(display_line[3], " <-- -->  ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

  wheel_move = 0;
  forward = TRUE;

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
//    Carlson_StateMachine();            // Run a Time Based State Machine
//    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

    if(Last_Time_Sequence != Time_Sequence){
    Last_Time_Sequence = Time_Sequence;
    cycle_time++;
    time_change = 1;
    }

    switch(event){
    case PROJ5: // Runs project 5 movement
        strcpy(display_line[0], " Proj5    ");
        display_changed = TRUE;
        Run_Straight();
        break; //
    default: break;
    }

  }
//------------------------------------------------------------------------------

}


