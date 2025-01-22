/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F27Q43
        Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"
#include "main.h"
#define BULB 8
/*
                         Main application
 */
#include "mcc_generated_files/mcc.h"
#include "main.h"
#define BULB 8
#define NUM_LEDS 8
#define RGB_DATA_PER_LED 3
/*
                         Main application
 */
#define RX_BUFFER_SIZE 8
uint8_t ledRGB[NUM_LEDS][RGB_DATA_PER_LED]; 
void callInterval(void);
uint8_t timecount = 0;
uint8_t dmx512SendBuffer[513] = {0};
uint8_t sendBreakFlag = 0;
uint16_t dmx512SendCounter = 0;
uint8_t Data = 0;
uint8_t timecount20ms = 0;
uint8_t timecount50ms = 0;
uint8_t timecount100ms = 0;
uint8_t timecount500ms = 0;
uint8_t timecount20msf = 0;
uint8_t timecount50msf = 0;
uint8_t timecount100msf = 0;
uint8_t timecount500msf = 0;
uint8_t i=0;
uint8_t receivedDataFlag = 0;
static uint8_t dataIndex = 0;
uint8_t rxBuffer[RX_BUFFER_SIZE]= {0};
uint8_t rxIndex = 0;
#define START_BYTE 0xFF 
#define END_BYTE 0XF0
uint8_t startflag =0;
uint8_t RED = 255;
uint8_t GREEN =255;
uint8_t BLUE = 255;
uint8_t Flag = 0;
void processReceivedData(void);
void resetRxBuffer(void);
void main(void) {
    // Initialize the device
    SYSTEM_Initialize();
    Enable_SetHigh();

    // Enable Global Interrupts
    INTERRUPT_GlobalInterruptEnable();
    LOAD1_SetHigh();
    LOAD2_SetHigh();
    LOAD3_SetHigh(); 
    while (1) {
        // Main loop
      if (PIR8bits.U2RXIF == 1) {
            PIR8bits.U2RXIF = 0;
            uint8_t receivedByte = U2RXB;
            
            if (rxIndex < RX_BUFFER_SIZE) {
                rxBuffer[rxIndex++] = receivedByte;
                if (receivedByte == '\n') {
                    Flag = 1; // Mark data as ready to process
                }
            } else {
                resetRxBuffer(); // Prevent buffer overflow
            }
         }
        if (timecount == 1) {
            callInterval();
            timecount = 0;
        }

        if (timecount20msf == 1) {
            timecount20msf =0;
        }
        if (timecount50msf == 1) {
           timecount50msf = 0;
           if(Flag ==1)
           {
              for(uint8_t LightAddress = 1; LightAddress <= 170 ; LightAddress++){
                for(uint8_t i=0;i<8;i++){
                    if(rxBuffer[i] == '1'){
                      dmx512SendBuffer[1] = RED;
                      dmx512SendBuffer[2] = GREEN ;
                      dmx512SendBuffer[3] = BLUE;  
                      LOAD1_SetLow(); 
                    }
                    else if(rxBuffer[i] == '2'){
                      dmx512SendBuffer[4] = RED;
                      dmx512SendBuffer[5] = GREEN ;
                      dmx512SendBuffer[6] = BLUE; 
                      LOAD1_SetLow(); 
                    }
                    else if(rxBuffer[i] == '3'){
                      dmx512SendBuffer[7] = RED;
                      dmx512SendBuffer[8] = GREEN ;
                      dmx512SendBuffer[9] = BLUE; 
                      LOAD1_SetLow(); 
                    }
                     else if(rxBuffer[i] == '4'){
                      dmx512SendBuffer[10] = RED;
                      dmx512SendBuffer[11] = GREEN ;
                      dmx512SendBuffer[12] = BLUE;
                      LOAD2_SetLow();
                    }
                     else if(rxBuffer[i] == '5'){
                      dmx512SendBuffer[13] = RED;
                      dmx512SendBuffer[14] = GREEN ;
                      dmx512SendBuffer[15] = BLUE; 
                      LOAD2_SetLow();
                    }
                     else if(rxBuffer[i] == '6'){
                      dmx512SendBuffer[16] = RED;
                      dmx512SendBuffer[17] = GREEN ;
                      dmx512SendBuffer[18] = BLUE; 
                      LOAD2_SetLow();
                    }
                    else if(rxBuffer[i] == '7'){
                      dmx512SendBuffer[19] = RED;
                      dmx512SendBuffer[20] = GREEN ;
                      dmx512SendBuffer[21] = BLUE; 
                      LOAD3_SetLow();
                    }
                    else if(rxBuffer[i] == '8'){
                      dmx512SendBuffer[22] = RED;
                      dmx512SendBuffer[23] = GREEN ;
                      dmx512SendBuffer[24] = BLUE; 
                      LOAD3_SetLow();
                    }
                    
                    else{
                        dmx512SendBuffer[((LightAddress - 1)*3) + 1] = 0x00;
                        dmx512SendBuffer[((LightAddress - 1)*3) + 2] = 0x00;
                        dmx512SendBuffer[((LightAddress - 1)*3) + 3] = 0x00; 
                        LOAD1_SetHigh();
                        LOAD2_SetHigh();
                        LOAD3_SetHigh(); 
                    }
                }   
            }
            Flag =0;
            i=0;
             for (int j = 0; j < RX_BUFFER_SIZE; j++) {
               rxBuffer[j] = 0;
             }
        IND_Toggle();
        PIE4bits.U1TXIE = 1;
    }
 
 }
                
}

}




void callInterval(void) {
    if (timecount20ms++ == TMR_COUNT_20MSEC) {
        timecount20ms = 0;
        timecount20msf = 1;
    }
    
    if (timecount50ms++ == TMR_COUNT_50MSEC) {
        timecount50ms = 0;
        timecount50msf = 1;
    }
    
    if (timecount100ms++ == TMR_COUNT_100MSEC) {
        timecount100ms = 0;
        timecount100msf = 1;
    }
    
    if (timecount500ms++ == TMR_COUNT_500MSEC) {
        timecount500ms = 0;
        timecount500msf = 1;
    }
}
void resetRxBuffer(void) {
    for (uint8_t i = 0; i < RX_BUFFER_SIZE; i++) {
        rxBuffer[i] = 0;
    }
    rxIndex = 0;
}
/**
 End of File
 */
