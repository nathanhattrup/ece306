//ADC init and interrupts
//==================================
// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;

volatile unsigned int ADC_Thumb;
volatile unsigned int ADC_Left_Det;
volatile unsigned int ADC_Right_Det;
unsigned char ADC_Channel =0;

unsigned int left_calibration;
unsigned int right_calibration;
unsigned int left_calibration_60;
unsigned int right_calibration_60;
unsigned int left_calibration_70;
unsigned int right_calibration_70;
unsigned int left_calibration_80;
unsigned int right_calibration_80;

unsigned int left_calibration_75;
unsigned int right_calibration_75;
unsigned int left_calibration_85;
unsigned int right_calibration_85;
unsigned int left_calibration_90;
unsigned int right_calibration_90;
unsigned int left_white;
unsigned int right_white;
unsigned int white_diff;

int cal_flag;

//ADC Configuration
//-----------------------------------------------------------------------------
void Init_ADC(void)  {
//------------------------------------------------------------------------------
// V_DETECT_L (0x04) // Pin 1.2 A2
// V_DETECT_R (0x08) // Pin 1.3 A3
// V_THUMB (0x20) // Pin 1.5 A5
//------------------------------------------------------------------------------
// ADCCTL0 Register
    ADCCTL0 = 0;              //Reset
    ADCCTL0 |= ADCSHT_2;        //16 ADC clocks
    ADCCTL0 |= ADCMSC;          //MSC
    ADCCTL0 |= ADCON;           //ADC ON

// ADCCTL1 Register
    ADCCTL1 = 0; // Reset
    ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
    ADCCTL1 |= ADCSHP; // ADC sample-and-hold SAMPCON signal from sampling timer.
    ADCCTL1 &= ~ADCISSH; // ADC invert signal sample-and-hold.
    ADCCTL1 |= ADCDIV_0; // ADC clock divider - 000b = Divide by 1
    ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
    ADCCTL1 |= ADCCONSEQ_0; // ADC conversion sequence 00b = Single-channel single-conversion
    // ADCCTL1 & ADCBUSY identifies a conversion is in process

    // ADCCTL2 Register
    ADCCTL2 = 0; // Reset
    ADCCTL2 |= ADCPDIV0; // ADC pre-divider 00b = Pre-divide by 1
    ADCCTL2 |= ADCRES_2; // ADC resolution ??b = 12 bit (14 clock cycle conversion time)
    ADCCTL2 &= ~ADCDF; // ADC data read-back format 0b = Binary unsigned.
    ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
    // ADCMCTL0 Register

    ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR� = AVSS }
    ADCMCTL0 |= ADCINCH_5; // V_THUMB (0x20) Pin 5 A5
    //^^Think this is like initial?

    ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
    ADCCTL0 |= ADCENC; // ADC enable conversion.
    ADCCTL0 |= ADCSC; // ADC start conversion.
}

//-----------------------------------------------------------------------------
//ADC Interrupt
//-----------------------------------------------------------------------------
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
    case ADCIV_NONE:
        break;
    case ADCIV_ADCOVIFG:        //if a conversion is written to ADCMEM0 before previous is read
        break;
    case ADCIV_ADCTOVIFG:       //ADC conversion time overflow
        break;
    case ADCIV_ADCHIIFG:        //window comparator interrupt flags
        break;
    case ADCIV_ADCLOIFG:        // Window comparator interrupt flag
        break;
    case ADCIV_ADCINIFG:        // Window comparator interrupt flag
        break;
    case ADCIV_ADCIFG:          //ADCMEM0 with conversion result
        ADCCTL0 &= ~ADCENC;     // ADC disable conversion.
        switch (ADC_Channel++) {
            case 0:
                ADCMCTL0 &= ~ADCINCH_5;     //Last channel was A5
                ADCMCTL0 |= ADCINCH_3;      //next channel A3
                ADC_Thumb = ADCMEM0;        //read a5
                ADCCTL0 |= ADCSC;           //start next sample
                break;
            case 1:
                ADCMCTL0 &= ~ADCINCH_3;     //Last channel was A3
                ADCMCTL0 |= ADCINCH_2;      //next channel A2
                ADC_Left_Det = ADCMEM0;        //read a3
                ADC_Left_Det = ADC_Left_Det >> 1;  //divide by 2
                ADCCTL0 |= ADCSC;           //start next sample
                break;
            case 2:
                ADCMCTL0 &= ~ADCINCH_2;     //Last channel was A2
                ADCMCTL0 |= ADCINCH_5;      //next channel A5
                ADC_Right_Det = ADCMEM0;        //read a2
                ADC_Right_Det = ADC_Right_Det >> 1;   //divide by 2
                ADC_Channel = 0;        //reset
                break;
            default: break;
        }
        ADCCTL0 |= ADCENC;      //reenable conversions
        break;
        default: break;
    }
}

void adc_calibrate(void){

    switch(cal_flag){
    case 0:
    left_calibration = ADC_Left_Det;
    right_calibration = ADC_Right_Det;

    left_calibration = (left_calibration * 35) >> 5;     //ummm gonna try raising my black calibration to 110%
    right_calibration = (right_calibration * 35) >> 5;

    left_calibration_60 = (left_calibration * 5) >> 3;      //62.5%
    right_calibration_60 = (right_calibration * 5) >> 3;

    left_calibration_70 = (left_calibration * 11) >> 4;     //68.75%
    right_calibration_70 = (right_calibration * 11) >> 4;

    left_calibration_75 = (left_calibration * 3) >> 2;     //75%
    right_calibration_75 = (right_calibration * 3) >> 2;

    left_calibration_80 = (left_calibration * 13) >> 4;     //81.25%
    right_calibration_80 = (right_calibration * 13) >> 4;

    left_calibration_85 = (left_calibration * 27) >> 5;     //85%
    right_calibration_85 = (right_calibration * 27) >> 5;

    left_calibration_90 = (left_calibration * 29) >> 5;     //90%
    right_calibration_90 = (right_calibration * 29) >> 5;

    cal_flag++;
    break;
    case 1:     //this flag lets the first run-through calibrate black and the second do white
        left_white = ADC_Left_Det;      //I DONT WANT TO CALIBRATE WHITE AHGHHHHHGHHGHHGSFGSFKJGF
        right_white = ADC_Right_Det;

        cal_flag = 0;

        strcpy(display_line[0], " Waiting  ");
        strcpy(display_line[1], "for  INPUT");      //carlson wants it to say this that is all
        display_changed = TRUE;
        break;
    }
    //displays the right value for a sanity check
    strcpy(display_line[2], "R Cal:    ");
    display_changed = TRUE;
    HEXtoBCD(ADC_Right_Det);
    adc_line(2, 6);

}
