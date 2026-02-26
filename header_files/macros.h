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
#define CIRCLE ('C')
#define FIG8  ('F')
#define TRIANGLE ('T')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define WAITING2START (200)     //delay before start
#define W_CNT (20)      //wheel count
#define R_CNT_STR (20)       //right count straight
#define L_CNT_STR (20)    //left count straight
//#define STR_DIST (6)       //straight distance (10 = ~2ft)

#define R_CNT_CRCL_CCW (20)       //right count circle ccw
#define L_CNT_CRCL_CCW (1)     //left count circle ccw
#define R_CNT_CRCL_CW (1)       //right count circle cw
#define L_CNT_CRCL_CW (20)     //left count circle cw
// #define CRCL_DIST (29)       //circle distance (~full circle)
// #define CRCL_DIST_HLF (15)       //circle distance (~half circle)

#define W_CNT_TRN (50)      //count for turn
#define R_CNT_TRN (50)        //right count for turn (ccw)
#define L_CNT_TRN (1)           //left count for turn (ccw)
//#define TURN_DIST (3)      //turn distance

// SWITCHES ======================================================================
#define PRESSED (0x00)
#define RELEASED (0x01)
#define OKAY (0x01)
#define NOT_OKAY (0x00)
#define DEBOUNCE_RESTART (0x00)
#define DEBOUNCE_TIME (300)
