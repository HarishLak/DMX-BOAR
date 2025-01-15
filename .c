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
#define COLOR_CHANGE_TIME 50 // 50ms for each step (can be adjusted)
 // 10 seconds = 200 intervals of 50ms
/*
                         Main application
 */
#define RX_BUFFER_SIZE 24
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
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t rxIndex = 0;
#define START_BYTE 0xFF 
#define END_BYTE 0XF0
uint8_t startflag =0;
uint8_t RED = 255;
uint8_t GREEN =0;
uint8_t BLUE = 0;
uint8_t color[3] = {255,0,0};
uint8_t colorWaveTimer = 0; // Timer to control the wave timing
int waveDurationTimer = 0; // Timer to count 10 seconds
uint8_t waveActive = 0; // Flag to indicate whether the wave is still active
uint8_t wavePosition = 0;
uint8_t WAVE_DURATION =200;
uint16_t count = 0;
uint8_t Flag = 0;
uint8_t Flag2=0;
uint8_t Flagset =0;
void reset(void);
void processReceivedData(void);
void shiftcolor(uint8_t colors[3]);
void wavepattern(void);
void strobepattern(void);
void strobepatternoff(void);\
void Alternateblink(void);
void Blinkodd(void);
uint8_t strobeState = 0; // 0 = Off, 1 = On
uint8_t strobeTimerCount = 0; // Counter for strobe toggling
uint8_t s1 = 255;
uint8_t s2 = 255;
uint8_t s3 = 255;
uint8_t T =0;
uint8_t a1 = 255;
uint8_t a2 = 0;
uint8_t a3 = 0;
uint8_t Flag3 =0; 
void main(void) {
    // Initialize the device
    SYSTEM_Initialize();
    Enable_SetHigh();

    // Enable Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    while (1) {
        // Main loop
        if (timecount == 1) {
            callInterval();
            timecount = 0;
        }

        if (timecount20msf == 1) {
            timecount20msf =0;
        }
        if (timecount50msf == 1) {
           timecount50msf = 0;
          if (PIR8bits.U2RXIF == 1) {
           PIR8bits.U2RXIF = 0;
           uint8_t receivedByte = U2RXB;
          
     if (receivedByte == 1 ) 
      {
         Flag=1;
      }
      if(Flag ==1)
      {
      while(count < 1500){
          count++;
          wavepattern();
          IND_Toggle();
          PIE4bits.U1TXIE = 1;
      }
          count =0;
          Flag = 0;
      } 
           
    // Color wave effect parameters
      
 
      else if(receivedByte == 2)
           {    
               Flag2 =1;
              
            }
      if(Flag2 ==1)
      {
      while(count < 90){
          count++;
          strobepattern();
          IND_Toggle();
          PIE4bits.U1TXIE = 1;
      }
      count =0;
      Flag2 = 0;
      } 
           
  else if(receivedByte == 3)
           {
             Flag3 =1;
           }
      if(Flag3 ==1)
      {
      while(count < 20){
          count++;
          Alternateblink();
          Blinkodd();
          IND_Toggle();
          PIE4bits.U1TXIE = 1;
          __delay_ms(500);
         
      }
      count =0;
      Flag3 = 0;
      } 
            else if(receivedByte == '4')
           {
               for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++){
                if(LightAddress ==4){
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = RED;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = GREEN ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = BLUE; 
                }else{
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = 0x00;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = 0x00 ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = 0x00; 
                } 
            }
           }
           else if(receivedByte == '5')
           {
               for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++){
                if(LightAddress == 5){
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = RED;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = GREEN ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = BLUE; 
                }else{
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = 0x00;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = 0x00 ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = 0x00; 
                } 
            }
           }
           else if(receivedByte == '6')
           {
               for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++){
                if(LightAddress ==6){
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = RED;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = GREEN ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = BLUE; 
                }else{
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = 0x00;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = 0x00 ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = 0x00; 
                } 
            }
           }
           else if(receivedByte == '7')
           {
               for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++){
                if(LightAddress ==7){
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = RED;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = GREEN ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = BLUE; 
                }else{
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = 0x00;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = 0x00 ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = 0x00; 
                } 
            }
           }
           else if(receivedByte == '8')
           {
               for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++){
                if(LightAddress ==8){
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = RED;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = GREEN ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = BLUE; 
                }else{
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = 0x00;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = 0x00;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = 0x00; 
                } 
            }
           }
           else
           {
               for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++){
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = 0x00;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = 0x00 ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = 0x00;     
            }
           }
           
           
        IND_Toggle();
        PIE4bits.U1TXIE = 1;
    }
 }
 }               
 }

 void wavepattern(void)
 {
     
     for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++)
     {
           dmx512SendBuffer[((LightAddress - 1)*3) + 1] = color[0] ;
           dmx512SendBuffer[((LightAddress - 1)*3) + 2] = color[1];
           dmx512SendBuffer[((LightAddress - 1)*3) + 3] = color[2];     
           shiftcolor(color);
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
        if (++strobeTimerCount >= 10) { // Toggle every 1 second (10 * 100ms)
        strobeState = !strobeState; // Toggle strobe state
        strobeTimerCount = 0;      // Reset counter
    }
    }
    
    if (timecount500ms++ == TMR_COUNT_500MSEC) {
        timecount500ms = 0;
        timecount500msf = 1;
    }
}
void shiftcolor(uint8_t colors[3])
{
    uint8_t temp = colors[0]; // Store the Red component
    colors[0] = colors[1];    // Shift Green to Red
    colors[1] = colors[2];    // Shift Blue to Green
    colors[2] = temp;
}
void strobepattern()
{
     __delay_ms(100);
      for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++)
      {
                 dmx512SendBuffer[((LightAddress - 1)*3) + 1] = s1 ;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 2] = s2;
                 dmx512SendBuffer[((LightAddress - 1)*3) + 3] = s3; 
      } 
      strobepatternoff();
    
}
void strobepatternoff(void)
{
    T = ~T;
    if(T == 0xFF){
    s1 =0;
    s2 =0;
    s3 =0;
    }
    else {
    s1 =255;
    s2 =255;
    s3 =255;   
    }
    
}
void Alternateblink(void)
{
     for (int LightAddress = 1; LightAddress <= 170 ; LightAddress++)
     {
           dmx512SendBuffer[((LightAddress - 1)*3) + 1] = color[0] ;
           dmx512SendBuffer[((LightAddress - 1)*3) + 2] = color[1];
           dmx512SendBuffer[((LightAddress - 1)*3) + 3] = color[2];     
           Blinkodd();
     }
}
void Blinkodd(void)
{
    uint8_t temp = color[0];
    color[0] = color[2];
    color[2] = temp;
}
/**
 End of File
 */
