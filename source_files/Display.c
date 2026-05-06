/*
 This is my display processssss
 */


// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"


extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern char *display[4];

unsigned int display_state;

//carlson display process
void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }

switch(display_state){      //this is my display state so i can just keep everything here
case 0:
    break;
case 1:     //arrived at 1
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "ARRIVED 01");
    display_changed = TRUE;
    break;
case 2:     //arrived at 2
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "ARRIVED 02");
    display_changed = TRUE;
    break;
case 3:     //arrived at 3
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "ARRIVED 03");
    display_changed = TRUE;
    break;
case 4:     //arrived at 4
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "ARRIVED 04");
    display_changed = TRUE;
    break;
case 5:     //arrived at 5
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "ARRIVED 05");
    display_changed = TRUE;
    break;
case 6:     //arrived at 6
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "ARRIVED 06");
    display_changed = TRUE;
    break;
case 7:     //arrived at 7
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "ARRIVED 07");
    display_changed = TRUE;
    break;
case 8:     //arrived at 8
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "ARRIVED 08");
    display_changed = TRUE;
    break;
case 9:     //bl start
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "BL   START");
    display_changed = TRUE;
    break;
case 10:     //intercept
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "INTERCEPT ");
    display_changed = TRUE;
    break;
case 11:     //bl turn
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "BL    TURN");
    display_changed = TRUE;
    break;
case 12:     //bl travel
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "BL  TRAVEL");
    display_changed = TRUE;
    break;
case 13:     //bl circle
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "BL  CIRCLE");
    display_changed = TRUE;
    break;
case 14:     //bl exit
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "BL    EXIT");
    display_changed = TRUE;
    break;
case 15:     //bl stop
    strcpy(display_line[0], " Nat Hatt ");
    strcpy(display_line[1], "BL    STOP");
    display_changed = TRUE;
    break;
default: break;
}

}

void splash_baud(char speed){       //i was lowkey such an idiot for using the varibale "speed"
    if(speed == 0){                            // If 115,200 baud
        strcpy(display_line[2], " 115,200  ");
    } else {                                   // If 57,600 baud
        strcpy(display_line[2], " 57,600   ");
    }
    display_changed = TRUE;
}


int i;
void center(char *array)        //this function just centers a string i pass through so it looks pretty
{
    int len = strlen(array);
    int total_pad = 10 - len;
    int left_pad =  total_pad / 2;   // floor division
    int right_pad = (total_pad +1) / 2;        // ceiling division
    char result[11];
    for (i = 0; i < left_pad; i++)
    {
        result[i] = ' ';
    }
    for (i = 0; i < len; i++)
    {
        result[left_pad + i] = array[i];
    }
    for (i = 0; i < right_pad; i++)
    {
        result[left_pad + len + i] = ' ';
    }
    result[10] = '\0';
    strcpy(array, result);
}

