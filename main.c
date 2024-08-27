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
        Device            :  PIC16F1827
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

/*
                         Main application
 */

void fluxcontrol (){
    error = (ballset-balldist)*10; //Calculo do erro baseado na dist setado com a dist real, *10 pra duas casas decimais
    if(error > 150 || error < 150){ // Caso o erro seja maior do que 5%
        outputsum += ((kif*timecontrol*error)/1000); //Kintegrativa com erro e o tempo do timer   
        if (outputsum > 45000) outputsum = 45000; //caso a soma seja maior que 450(*100 por causa de 2 casas decimais), fixa em 450
        output = ((kpf*error + outputsum)/100); // definindo output com kpf e voltando a escala
        if (output > 450) output = 450; //saturando o output
    }
}

void pwmcontrol (){
    error = ballset-balldist; //Calculo do erro baseado na dist setado com a dist real
    if(error > 150 || error < 150){ // Caso o erro seja maior do que 5%, roda o codigo
        outputsum += ((kip*timecontrol*error)/1000);//Kintegrativa com erro e o tempo do timer 
        if (outputsum > 10230) outputsum = 10230; //caso a soma seja maior que 1023(*10 por causa de 1 casa decimai) fixa em 1023
        output = ((kpp*error + outputsum)/10); //voltando a escala padrão
        if(output > 1023) output = 1023; //saturando o output
        EPWM1_LoadDutyValue(output); //mandando o valor após o controle
    }
}

void fluxpos (){
    
}

void controlchoose(){
    if (controlchoice == true){ //escolhendo qual malha de controle com uma variavel bool
        pwmcontrol ();
    } else {
        fluxcontrol ();
    }
    TMR4_LoadPeriodRegister(0);
    TMR4_StartTimer();
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
            
    while (1)
    {
        // Add your application code
        if((timecontrol = TMR4_ReadTimer()) >= 209){//timer incrementa a cada 96us, 96us*209 = 20.096ms, tempo para escolher a malha de controle
            TMR4_StopTimer();
            controlchoose();
        }
    }
}
/**
 End of File
*/