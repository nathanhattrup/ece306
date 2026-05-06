/*
Serial communication interrupts and setup and ahhhhhhh
 */

//------------------------------------------------------------------------------
// INCLUDES UPDATED 02.16.26
#include  "msp430.h"
#include  <string.h>
#include  "header_files\functions.h"
#include  "header_files\LCD.h"
#include  "header_files\ports.h"
#include  "header_files\macros.h"

//===================================================================

// USB (PC)
volatile unsigned int usb_rx_wr;             // Write index for USB RX ring buffer
volatile unsigned int usb_rx_rd;             // Read index for USB RX ring buffer

volatile char USB_Char_Rx[SMALL_RING_SIZE];    // Ring buffer: bytes received from PC (UCA1 RX)

// IOT (J9)
volatile unsigned int iot_rx_wr;             // Write index for IOT RX ring buffer
volatile unsigned int iot_rx_rd;             // Read index for IOT RX ring buffer
volatile char IOT_Char_Rx[SMALL_RING_SIZE];    // Ring buffer: bytes received from IOT/J9 (UCA0 RX)

// Transmit and receive process buffers
volatile char transmit_buffer[64];
char receive_buffer[64];
volatile char tx_index;
volatile char rx_index;
char ssid_buffer[20];
char ip_buffer[20];
char ip_buffer1[20];
char ip_buffer2[20];

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;

char rx_flag;
extern volatile unsigned int second_count;
unsigned int iot_state = 0;

unsigned int cmd_state;
char cmd_buffer[20];
char cmd_buffer1[20];
char cmd_id;
int cmd_num;

extern volatile unsigned int second_count;
extern volatile unsigned int universal_count;

extern char exit_flag;

extern unsigned int display_state;

extern volatile char google_flag;

//Inits
//Init UCA0
void Init_Serial_UCA0(void)
{
    int i;                                     // Loop index for buffer clearing

    for(i = 0; i < SMALL_RING_SIZE; i++){      // Loop through entire RX ring buffer
        IOT_Char_Rx[i] = 0x00;                // Zero out each element
    }
    iot_rx_wr = BEGINNING;                   // Reset RX write index to start
    iot_rx_rd = BEGINNING;                   // Reset RX read index to start


//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
// 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
// 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
// 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
    UCA0CTLW0 = 0;
    UCA0CTLW0 |= UCSWRST; // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
    UCA0CTLW0 &= ~UCMSB; // MSB, LSB select
    UCA0CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA0CTLW0 &= ~UCPEN; // No Parity
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |= UCMODE_0;
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// UCA?MCTLW = UCSx + UCFx + UCOS16
    UCA0BRW = 4;                       // 115,200 baud
    UCA0MCTLW = 0x5551;                 // UCSx=0x55, UCFx=5, UCOS16=1

    UCA0CTLW0 &= ~UCSWRST; // release from reset
    UCA0TXBUF = 0x00; // Prime the Pump
    UCA0IE |= UCRXIE; // Enable RX interrupt
//------------------------------------------------------------------------------
}

//Init UCA1
void Init_Serial_UCA1(void)
{
    int i;                                     // Loop index for buffer clearing

    for(i = 0; i < SMALL_RING_SIZE; i++){      // Loop through entire RX ring buffer
        USB_Char_Rx[i] = 0x00;                // Zero out each element
    }
    usb_rx_wr = BEGINNING;                   // Reset RX write index to start
    usb_rx_rd = BEGINNING;                   // Reset RX read index to start

//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
// 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
// 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
// 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A1 for UART mode
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST; // Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
    UCA1CTLW0 &= ~UCMSB; // MSB, LSB select
    UCA1CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA1CTLW0 &= ~UCPEN; // No Parity
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// UCA?MCTLW = UCSx + UCFx + UCOS16

    UCA1BRW = 4;                       // 115,200 baud
    UCA1MCTLW = 0x5551;                 // UCSx=0x55, UCFx=5, UCOS16=1

    UCA1CTLW0 &= ~UCSWRST; // release from reset
    UCA1TXBUF = 0x00; // Prime the Pump
    UCA1IE |= UCRXIE; // Enable RX interrupt
//------------------------------------------------------------------------------
}


//========================================================================
//Interrupts

// eUSCI_A0 ISR � from Lecture 13 Big Picture slides 7-9, 17
// UCA0 is connected to IOT / J9 loopback
#pragma vector=EUSCI_A0_VECTOR                // Interrupt vector for eUSCI_A0
__interrupt void eUSCI_A0_ISR(void)
{           // ISR for UCA0 serial port
    unsigned int temp;                         // Temp variable for buffer index
    switch (__even_in_range(UCA0IV, 0x08))
    {      // Read interrupt vector register
    case 0:                                // Vector 0 - no interrupt pending
        break;
    case 2:                                // Vector 2 - RXIFG (byte received from IOT/J9)
        temp = UCA0RXBUF;                  // Read received byte from UCA0 hardware buffer

        IOT_Char_Rx[iot_rx_wr++] = temp; // Store byte into IOT RX ring buffer, advance write index

        if(iot_rx_wr >= (sizeof(IOT_Char_Rx))){ // If write index past end of array
            iot_rx_wr = BEGINNING;       // Wrap around to beginning (circular buffer)
        }

        UCA1TXBUF = temp;     // Forward byte out UCA1 to PC

        break;
    case 4:                                // Vector 4 - TXIFG
        UCA0TXBUF = transmit_buffer[tx_index]; // Transmit current byte out UCA0 to IOT
            transmit_buffer[tx_index++] = 0x00;    // Null transmitted location
            if(transmit_buffer[tx_index] == 0x00){ // Next location NULL - end of string
                UCA0IE &= ~UCTXIE;                // Disable TX interrupt
            }
        break;                             // Not used yet
    default:                               // All other vectors
        break;
    }
}

// eUSCI_A1 ISR � from Lecture 13 Big Picture slide 18
// UCA1 is connected to PC via USB backdoor (Termite)
#pragma vector=EUSCI_A1_VECTOR                // Interrupt vector for eUSCI_A1
__interrupt void eUSCI_A1_ISR(void){           // ISR for UCA1 serial port
    unsigned int temp;                         // Temp variable for buffer index
    switch(__even_in_range(UCA1IV, 0x08)){
        case 0:                                // Vector 0 - no interrupt
            break;
        case 2:                                // Vector 2 - RXIFG (byte received from PC)
            temp = UCA1RXBUF;                  // Read received byte from UCA1 hardware buffer

            USB_Char_Rx[usb_rx_wr++] = temp; // Store byte into USB RX ring buffer, advance write index

            if(usb_rx_wr >= (sizeof(USB_Char_Rx))){ // If write index past end of array
                usb_rx_wr = BEGINNING;       // Wrap around to beginning (circular buffer)
            }

            UCA0TXBUF = temp;                  // Forward byte out UCA0 to IOT/J9 loopback


            break;
        case 4:                                // Vector 4 - TXIFG (ready to transmit)
            UCA1TXBUF = transmit_buffer[tx_index]; // Transmit current byte out UCA1 to PC
                transmit_buffer[tx_index++] = 0x00;    // Null transmitted location
                if(transmit_buffer[tx_index] == 0x00){ // Next location NULL - end of string
                    UCA1IE &= ~UCTXIE;                // Disable TX interrupt
                }
            break;
        default:
            break;
    }
}

//==============================================================

void baud_speed(char speed){
    splash_baud(speed);

    UCA0CTLW0 |= UCSWRST;                 // Put UCA0 into reset before changing baud
    UCA1CTLW0 |= UCSWRST;                 // Put UCA1 into reset before changing baud
    switch(speed){
    case 0:
        // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
        UCA0BRW = 4;                       // 115,200 baud
        UCA0MCTLW = 0x5551;                 // UCSx=0x55, UCFx=5, UCOS16=1
        UCA1BRW = 4;                       // 115,200 baud
        UCA1MCTLW = 0x5551;                 // UCSx=0x55, UCFx=5, UCOS16=1
        break;
    case 1:
        // 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
        UCA0BRW = 8;                       //57600 baud
        UCA0MCTLW = 0xF7A1;                // UCSx=0xF7, UCFx=10, UCOS16=1
        UCA1BRW = 8;                       //57600 baud
        UCA1MCTLW = 0xF7A1;                // UCSx=0xF7, UCFx=10, UCOS16=1
        break;
    default: break;
    }
    UCA0CTLW0 &= ~UCSWRST;  // Release UCA0 from reset
    UCA1CTLW0 &= ~UCSWRST;  // Release UCA1 from reset
    UCA0IE |= UCRXIE;        // Re-enable UCA0 RX interrupt
    UCA1IE |= UCRXIE;        // Re-enable UCA1 RX interrupt
}

void USCI_A0_Transmit(void){                   // Transmit Function for USCI_A0
    // Contents must be in process_buffer
    // End of Transmission is identified by NULL character in process_buffer
    // process_buffer includes Carriage Return and Line Feed
    tx_index = 0;                              // Set Array index to first location
    UCA0IE |= UCTXIE;                         // Enable TX interrupt � this kicks off case 4
}

void USCI_A1_Transmit(void){                   // Transmit Function for USCI_A0
    // Contents must be in process_buffer
    // End of Transmission is identified by NULL character in process_buffer
    // process_buffer includes Carriage Return and Line Feed
    tx_index = 0;                              // Set Array index to first location
    UCA1IE |= UCTXIE;                         // Enable TX interrupt � this kicks off case 4
}


//==========================================================================================
//==========================================================================================
//==========================================================================================
//THIS IS MY TRANSMIT/RECEIVE CODE FROM MAIN FOR PROJ8
//==========================================================================================
//==========================================================================================
//==========================================================================================

void clear_rx_buffer(void){
    memset(receive_buffer, 0, 64);      //this just clears my rx buffer
    rx_index = BEGINNING;

}

void comms_process(void){

    //---------------------------------------------------------------------------------
    //this reads from iot side into receive buffer and sets rx_flag high
    unsigned int iot_rx_wr_temp;    //temp
    iot_rx_wr_temp = iot_rx_wr;
    while(iot_rx_rd != iot_rx_wr_temp){       // While there are unread bytes in ring buffer

        receive_buffer[rx_index] = IOT_Char_Rx[iot_rx_rd++]; // Copy one byte from ring buffer, advance read index

        if(receive_buffer[rx_index] == 0x0A){   //if line feed
            receive_buffer[rx_index] = 0x00;    //make that jawn null
            rx_flag = 1;        //raise rx flag
            rx_index = BEGINNING;
        }
        else{
        rx_index++;                                  // Advance receive_buffer write position
        rx_flag = 0;
        }

        if(iot_rx_rd >= SMALL_RING_SIZE){            // If read index past end of ring buffer
            iot_rx_rd = BEGINNING;                   // Wrap around to beginning
        }

    }


    //-------------------------------------------------------------------------------------
    //iot set up state machine :)
    switch(iot_state){
    case 0: //check for wifi got ip
        if(!strncmp(receive_buffer, "WIFI GOT IP", 11)){
            iot_state++;
            clear_rx_buffer();
        }
        break;
    case 1: //first command
        strcpy(transmit_buffer, "AT+SYSSTORE=0\r\n");
        USCI_A0_Transmit();     //sends to iot
        iot_state++;
        break;
    case 2: //check for OK
        if(!strncmp(receive_buffer, "OK", 2)){
            iot_state++;
            clear_rx_buffer();
        }
        break;
    case 3: //second command
        strcpy(transmit_buffer, "AT+CIPMUX=1\r\n");
        USCI_A0_Transmit();
        iot_state++;
        break;
    case 4: //check for OK
        if(!strncmp(receive_buffer, "OK", 2)){
            iot_state++;
            clear_rx_buffer();
        }
        break;
    case 5: //third command
        strcpy(transmit_buffer, "AT+CIPSERVER=1,5256\r\n");
        USCI_A0_Transmit();
        iot_state++;
        break;
    case 6: //check for OK
        if(!strncmp(receive_buffer, "OK", 2)){
            iot_state++;
            clear_rx_buffer();
        }
        break;
    case 7: //SSID command
        strcpy(transmit_buffer, "AT+CWJAP?\r\n");
        USCI_A0_Transmit();
        iot_state++;
        break;
    case 8: //check for SSID and OK
        if(!strncmp(receive_buffer, "+CWJAP", 6)){
            strncpy(ssid_buffer, receive_buffer + 8, 4);    //hard codes stealing the SSID
        }
        if(!strncmp(receive_buffer, "OK", 2)){
            iot_state++;
            clear_rx_buffer();
        }
        break;
    case 9: //IP command
        strcpy(transmit_buffer, "AT+CIFSR\r\n");
        USCI_A0_Transmit();
        iot_state++;
        break;
    case 10: //check for IP and OK
        if(!strncmp(receive_buffer, "+CIFSR:STAIP", 12)){
                    strncpy(ip_buffer, receive_buffer + 14, 13);        //hard codes stealing the IP
                }
        if(!strncmp(receive_buffer, "OK", 2)){
            iot_state++;
            clear_rx_buffer();
        }
        break;
    case 11:
        //steal away bits
        strncpy(ip_buffer1, ip_buffer, 6);
        strncpy(ip_buffer2, ip_buffer + 7, 6);
        //centers
        center(ssid_buffer);
        center(ip_buffer1);
        center(ip_buffer2);
        //displays
        strcpy(display_line[1], ssid_buffer);
        strcpy(display_line[0], ip_buffer1);
        strcpy(display_line[2], ip_buffer2);
        display_changed = TRUE;
        iot_state++;


        P3OUT &=  ~IOT_LINK_CPU;
        break;
    case 12:        //checks for when Im connected
        if(!strncmp(receive_buffer +1, ",CONNECT", 8)){
            cmd_state = 1;                                  //ALLOWS COMMANDS TO BE RECIEVED
            P3OUT |=  IOT_LINK_CPU; //light on
            iot_state++;
            clear_rx_buffer();
        }
        break;
    case 13:        //for pinging google but didnt use
        break;
    default: break;
    }


    //------------------------------------------------
    //command state machine >:)
    switch(cmd_state){
    case 0:     //default
        break;
    case 1:        //receives command
        if(!strncmp(receive_buffer + 9, "$$", 2) && rx_flag){       //parsing the code
            P2OUT |=  IOT_RUN_CPU;
            strncpy(cmd_buffer, receive_buffer + 11, 5);        //HAS to be in format $$X0123
            clear_rx_buffer();
            cmd_state = 2;

        }
        break;
    case 2:     //displays command
        //strcpy(transmit_buffer, cmd_buffer);
        //USCI_A1_Transmit();               //echos to pc

        strcpy(cmd_buffer1, cmd_buffer);
        center(cmd_buffer1);

        display_state = 0;
        strcpy(display_line[0], " DEMO DAY ");
        strcpy(display_line[1], " Nat Hatt ");
        strcpy(display_line[2], cmd_buffer1);
        display_changed = TRUE;

        cmd_id = cmd_buffer[0];     //the leading letter "ID"
        cmd_num = my_atoi(&cmd_buffer[1]);      //reads ascii "0123" to int 123

        cmd_state = 3;
        universal_count = 0;
        second_count = 0;
        break;
    case 3:     //execution switch case

        //time is in terms of 10ms, 200 => 2s
        //0 runs it forever
        switch(cmd_id){
        case 'S':       //stop
            stop_cmd();
            break;
        case 'F':       //forward
            fwd_cmd(cmd_num);
            break;
        case 'B':       //backward
            rev_cmd(cmd_num);
            break;
        case 'L':       //left
            lturn_cmd(cmd_num);
            break;
        case 'R':       //right
            rturn_cmd(cmd_num);
            break;
        case 'V':       //speeeeeeed change 0 is slow 1 is fast
            speed_change(cmd_num);
            break;
        case 'I':       //line intercept
            break;
        case 'E':       //exit
            exit_flag = 1;
            break;
        case 'A':       //arrived at
            switch(cmd_num){
            case 1:
                display_state = 1;
                break;
            case 2:
                display_state = 2;
                break;
            case 3:
                display_state = 3;
                break;
            case 4:
                display_state = 4;
                break;
            case 5:
                display_state = 5;
                break;
            case 6:
                display_state = 6;
                break;
            case 7:
                display_state = 7;
                break;
            case 8:
                display_state = 8;
                break;
            default: break;
            }
            P2OUT &=  ~IOT_RUN_CPU;
            cmd_state = 1;
            break;
        case 'P':       //pad 8 arc
            pad8();
            break;
        default: break;
        }

        break;
    default: break;
    }



}
