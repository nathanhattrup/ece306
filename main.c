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

extern char event;      //event tag

extern volatile unsigned int second_count;
extern volatile unsigned int ADC_Thumb;

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
  Init_ADC();                          //Initialize ACD
  Init_DAC();

  motors_off();                        // turns off motors
  event = NONE;                         //sets event to none

  Init_Serial_UCA0();      //sets to 115k default
  Init_Serial_UCA1();



  //P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  lcd_BIG_mid();
  strcpy(display_line[0], "<SW1  SW2>");
  strcpy(display_line[1], "  Nathan  ");
  strcpy(display_line[2], "  Hattrup ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

//  splash_baud(speed);       //displays baud on start (115k)

  wheel_move = 0;
  forward = TRUE;

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run

    Init_IOT();
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE

    //movement state machine ruins
    switch(event){
    case PROJ: // Runs project 10 line detect and following
        Run_Straight();
        break; //
    default: break;
    }

    comms_process();    //this controls ALL of my serial stuff and commands

//    detector_display();
//    thumb_read(ADC_Thumb);
  }
//------------------------------------------------------------------------------

}


