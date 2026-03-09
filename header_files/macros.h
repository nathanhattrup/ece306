/*
 * macros.h
 *
 *  Created on: Feb 5, 2026
 *      Author: nahat
 */


#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //

// STATES ======================================================================
#define NONE ('N')
#define STRAIGHT ('L')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define PROJ6 ('P')

// SWITCHES ======================================================================
#define PRESSED (0x00)
#define RELEASED (0x01)
#define OKAY (0x01)
#define NOT_OKAY (0x00)
#define DEBOUNCE_RESTART (0x00)
#define DEBOUNCE_TIME (300)
#define P4PUD (P4OUT)
#define P2PUD (P2OUT)

//TIMERS ========================================================================
#define TB0CCR0_INTERVAL (1250)
#define TB0CCR1_INTERVAL (62500)
#define TB0CCR2_INTERVAL (62500)

//PWM ===========================================================================
#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLIGHT_DIMING (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_FORWARD_SPEED (TB3CCR3)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LEFT_REVERSE_SPEED (TB3CCR5)

#define WHEEL_PERIOD (50005)
#define WHEEL_OFF (0)
#define SLOW (10000)
#define FAST (50000)
#define BRIGHT (50000)
#define DIM (45000)
