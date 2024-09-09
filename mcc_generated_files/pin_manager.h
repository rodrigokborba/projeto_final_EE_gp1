/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1827
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA0 procedures
#define RA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define RA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define RA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define RA0_GetValue()              PORTAbits.RA0
#define RA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define RA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define RA0_SetAnalogMode()         do { ANSELAbits.ANSA0 = 1; } while(0)
#define RA0_SetDigitalMode()        do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set SM1 aliases
#define SM1_TRIS                 TRISAbits.TRISA1
#define SM1_LAT                  LATAbits.LATA1
#define SM1_PORT                 PORTAbits.RA1
#define SM1_ANS                  ANSELAbits.ANSA1
#define SM1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define SM1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define SM1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define SM1_GetValue()           PORTAbits.RA1
#define SM1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define SM1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define SM1_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define SM1_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set SM2 aliases
#define SM2_TRIS                 TRISAbits.TRISA2
#define SM2_LAT                  LATAbits.LATA2
#define SM2_PORT                 PORTAbits.RA2
#define SM2_ANS                  ANSELAbits.ANSA2
#define SM2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define SM2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define SM2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define SM2_GetValue()           PORTAbits.RA2
#define SM2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define SM2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define SM2_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define SM2_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set SM3 aliases
#define SM3_TRIS                 TRISAbits.TRISA3
#define SM3_LAT                  LATAbits.LATA3
#define SM3_PORT                 PORTAbits.RA3
#define SM3_ANS                  ANSELAbits.ANSA3
#define SM3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define SM3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define SM3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define SM3_GetValue()           PORTAbits.RA3
#define SM3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define SM3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define SM3_SetAnalogMode()      do { ANSELAbits.ANSA3 = 1; } while(0)
#define SM3_SetDigitalMode()     do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set SM4 aliases
#define SM4_TRIS                 TRISAbits.TRISA4
#define SM4_LAT                  LATAbits.LATA4
#define SM4_PORT                 PORTAbits.RA4
#define SM4_ANS                  ANSELAbits.ANSA4
#define SM4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define SM4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define SM4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define SM4_GetValue()           PORTAbits.RA4
#define SM4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define SM4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define SM4_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define SM4_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set Trigger aliases
#define Trigger_TRIS                 TRISAbits.TRISA6
#define Trigger_LAT                  LATAbits.LATA6
#define Trigger_PORT                 PORTAbits.RA6
#define Trigger_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define Trigger_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define Trigger_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define Trigger_GetValue()           PORTAbits.RA6
#define Trigger_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define Trigger_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)

// get/set LED aliases
#define LED_TRIS                 TRISAbits.TRISA7
#define LED_LAT                  LATAbits.LATA7
#define LED_PORT                 PORTAbits.RA7
#define LED_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define LED_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define LED_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define LED_GetValue()           PORTAbits.RA7
#define LED_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()              PORTBbits.RB0
#define RB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()             do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()           do { WPUBbits.WPUB0 = 0; } while(0)

// get/set RB1 procedures
#define RB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()              PORTBbits.RB1
#define RB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()             do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()           do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode()         do { ANSELBbits.ANSB1 = 1; } while(0)
#define RB1_SetDigitalMode()        do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set RB2 procedures
#define RB2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define RB2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define RB2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define RB2_GetValue()              PORTBbits.RB2
#define RB2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define RB2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define RB2_SetPullup()             do { WPUBbits.WPUB2 = 1; } while(0)
#define RB2_ResetPullup()           do { WPUBbits.WPUB2 = 0; } while(0)
#define RB2_SetAnalogMode()         do { ANSELBbits.ANSB2 = 1; } while(0)
#define RB2_SetDigitalMode()        do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set RB3 procedures
#define RB3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define RB3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define RB3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define RB3_GetValue()              PORTBbits.RB3
#define RB3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define RB3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define RB3_SetPullup()             do { WPUBbits.WPUB3 = 1; } while(0)
#define RB3_ResetPullup()           do { WPUBbits.WPUB3 = 0; } while(0)
#define RB3_SetAnalogMode()         do { ANSELBbits.ANSB3 = 1; } while(0)
#define RB3_SetDigitalMode()        do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set channel_AN8 aliases
#define channel_AN8_TRIS                 TRISBbits.TRISB4
#define channel_AN8_LAT                  LATBbits.LATB4
#define channel_AN8_PORT                 PORTBbits.RB4
#define channel_AN8_WPU                  WPUBbits.WPUB4
#define channel_AN8_ANS                  ANSELBbits.ANSB4
#define channel_AN8_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define channel_AN8_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define channel_AN8_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define channel_AN8_GetValue()           PORTBbits.RB4
#define channel_AN8_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define channel_AN8_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define channel_AN8_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define channel_AN8_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define channel_AN8_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define channel_AN8_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/