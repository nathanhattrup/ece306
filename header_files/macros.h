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
#define PROJ5 ('P')

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
