/*
 All my timers in one handy place
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
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
volatile unsigned int universal_count;
volatile unsigned int second_count;

void Init_Timers(void){     //from main
    Init_Timer_B0();
}

void Init_Timer_B0(void){
//------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0 and B0_1 to B0_2 and OverFlow
// 8,000,000 / 8 / 8 / [1/time]
// 1,000,000 / 8 / [1/time]
// 125,000 / [1/time]
// 1/time => 1 / 10msec => 1 / 0.010 = 100
// 125,000 / 100 = 1250 – This is Capture Compare Register Interval
// 8usec per clock tick
//------------------------------------------------------------------------------
    TB0CTL = TBSSEL__SMCLK; // SMCLK
    TB0CTL |= MC__CONTINUOUS; // continuous mode
    TB0CTL |= ID__8; // Divide clock by 8
    TB0CTL |= TBCLR; // Clear Count
    TB0EX0 = TBIDEX__8; // Second Divider - Divide clock by 8

    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
    TB0CCTL0 &= ~CCIFG; // Clear CCR0 interrupt flag
    TB0CCTL0 |=  CCIE; // CCR0 enable interrupt

    TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
    TB0CCTL1 &= ~CCIFG; // Clear CCR1 interrupt flag
    TB0CCTL1 &= ~CCIE; // CCR1 disable interrupt

    TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
    TB0CCTL2 &= ~CCIFG; // Clear CCR2 interrupt flag
    TB0CCTL2 &= ~CCIE; // CCR2 disable interrupt

    TB0CTL &= ~TBIE; // Timer A0 overflow interrupt disable
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}


#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler for 10msec timer
//----------------------------------------------------------------------------
    Time_Sequence++;
    update_display_count++;
    universal_count++;

    switch(update_display_count){
    case 20:    //200ms
        update_display = TRUE;
        update_display_count = 0;
        break;
    default: break;
    }

    switch(universal_count){
    case 100:   //1s
        second_count++;
        universal_count = 0;
        break;
    default: break;
    }

    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
//----------------------------------------------------------------------------
}
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
        case 0: break; // No interrupt
        case 2: // CCR1 Used for SW1 Debounce
            TB0CCTL1 &= ~CCIE; // CCR1 disable interrupt
            P4IFG &= ~SW1;  //IFG SW1 cleared
            P4IE |= SW1;    //SW1 interrupt enabled
            P1OUT &= ~RED_LED; //turn off led
// Add Offset to TBCCR1
        break;
        case 4: // CCR2 Used for SW2 Debounce
            TB0CCTL2 &= ~CCIE;  //ccr2 disable interrupt
            P2IFG &= ~SW2;   //IFG sw2 cleared
            P2IE |= SW2;    //SW2 interrupt disabled
            P6OUT &= ~GRN_LED;  //turn off led
        break;
        case 14: // overflow available for greater than 1 second timer
        break;
        default: break;
    }
}

