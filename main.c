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

void fluxcontrol(){
    error = (ballset-balldist)*10; //Calculo do erro baseado na dist setado com a dist real, *10 pra duas casas decimais
    if(error > 150 || error < 150){ // Caso o erro seja maior do que 5%
        outputsum += ((kif*timecontrol*error)/1000); //Kintegrativa com erro e o tempo do timer   
        if (outputsum > 45000) outputsum = 45000; //caso a soma seja maior que 450(*100 por causa de 2 casas decimais), fixa em 450
        output = ((kpf*error + outputsum)/100); // definindo output com kpf e voltando a escala
        if (output > 450) output = 450; //saturando o output
    }
}

void pwmcontrol(){
    error = ballset-balldist; //Calculo do erro baseado na dist setado com a dist real
    if(error > 150 || error < 150){ // Caso o erro seja maior do que 5%, roda o codigo
        outputsum += ((kip*timecontrol*error)/1000);//Kintegrativa com erro e o tempo do timer 
        if (outputsum > 10230) outputsum = 10230; //caso a soma seja maior que 1023(*10 por causa de 1 casa decimai) fixa em 1023
        output = ((kpp*error + outputsum)/10); //voltando a escala padrão
        if(output > 1023) output = 1023; //saturando o output
        EPWM1_LoadDutyValue(output); //mandando o valor após o controle
    }
}

void fluxpos(){
    
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

void analisa_Rx (){
    switch(bufferRx[0]){
        case RX_CMD_PWM:
            if(countRx==RX_CMD_PWM_SZ){
                dcRx.n3 = ascii_bin(bufferRx[1]);
                dcRx.n2 = ascii_bin(bufferRx[2]);
                dcRx.n1 = ascii_bin(bufferRx[3]);
                dcRx.n0 = ascii_bin(bufferRx[4]);
                EPWM1_LoadDutyValue(dcRx.dc);
            }
            else{
                //se houver mensagem de erro no roteiro
            }
            break;
        case RX_CMD_STEP:
            if(countRx==RX_CMD_STEP_SZ){
                
            }
            else{
                //se houver mensagem de erro no roteiro
            }
            break;            
        case RX_CMD_HEIGHT:
            if(countRx==RX_CMD_HEIGHT_SZ){
                dRx.n3 = ascii_bin(bufferRx[1]);
                dRx.n2 = ascii_bin(bufferRx[2]);
                dRx.n1 = ascii_bin(bufferRx[3]);
                dRx.n0 = ascii_bin(bufferRx[4]);
                ballset = dRx.d / 20;
            }
            else{
                //se houver mensagem de erro no roteiro
            }
            break;
        case RX_CMD_CTRL:
            if(countRx==RX_CMD_CTRL_SZ){
                if(ascii_bin(bufferRx[1])== 0){
                    controlchoice = false;
                }
                else{
                    controlchoice = true;
                }
            }
            else{
                //se houver mensagem de erro no roteiro
            }
            break;
        default:
            ; //se houver mensagem de erro no roteiro
    } 
}

void envia_Tx (){         //função para enviar dados de altura e temperatura
    EUSART_Write(bin_ascii(dTx.n3));     //Envia n3 da altura
    EUSART_Write(bin_ascii(dTx.n2));     //Envia n2 da altura
    EUSART_Write(bin_ascii(dTx.n1));     //Envia n1 da altura
    EUSART_Write(bin_ascii(dTx.n0));     //Envia n0 da altura
    EUSART_Write(bin_ascii(tTx.n3));     //Envia n3 da temperatura
    EUSART_Write(bin_ascii(tTx.n2));     //Envia n2 da temperatura
    EUSART_Write(bin_ascii(tTx.n1));     //Envia n1 da temperatura
    EUSART_Write(bin_ascii(tTx.n0));     //Envia n0 da temperatura
    EUSART_Write(RX_END);     //Envia caracter de final de mensagem
}

uint8_t bin_ascii(uint8_t vBin){
    vBin = vBin & 0x0F;                        // Apaga nible alto
    if(vBin<10){                               // Se valor < 0xA
        vBin = vBin + 0x30; // 0..9            // Acresecenta 0x30 (ascii '0')
    }
    else{                                      // se não
        vBin = vBin + 0x37; // A..F            // accrescenta 0x37 (ascii '0'+7)
    }
    return vBin;
}

uint8_t ascii_bin(uint8_t vAscii){
    if(vAscii<0x3A){                               // Se valor <= 0x39 ('9') 
        vAscii = vAscii - 0x30; // '0'..'9'        // subtrai 0x30 (ascii '0')
    }
    else{                                          // se não
        vAscii = vAscii - 0x37; // 'A'..'F'        // subtrai 0x37 (ascii '0'+7)
    }
    return vAscii;
}

void receive(){
    uint8_t rx_byte = EUSART_Read();       // Lê-se EUSART e guarda-se em rxChar
    if(rx_byte==RX_INI){                   // Se for o inicio do quadro
        countRx = 0;                       // zera contador
    }
    else if(rx_byte==RX_END){              // Se for o final
        analisa_Rx();                      // analiza dados recebidos
    }
    else{                                  // se não é nem inicio nem fim
        if(countRx<BUFFER_MAX-1){          // e o buffer não etsá cheio
            bufferRx[countRx] = rx_byte;   // guarda valor
            countRx++;
        }
    }
}


void meioPasso(bool sentido){
    if(sentido){
        switch(pas.sos){
            case 0:
                SM1_SetHigh();
                SM2_SetHigh();
                SM3_SetLow();
                SM4_SetLow();
                break;
            case 1:
                SM1_SetLow();
                SM2_SetHigh();
                SM3_SetHigh();
                SM4_SetLow();
                break;
            case 2:
                SM1_SetLow();
                SM2_SetLow();
                SM3_SetHigh();
                SM4_SetHigh();
                break;
            case 3:
                SM1_SetHigh();
                SM2_SetLow();
                SM3_SetLow();
                SM4_SetHigh();
                break;
        }
    }else{
        switch(pas.sos){
            case 0:
                SM4_SetHigh();
                SM3_SetHigh();
                SM2_SetLow();
                SM1_SetLow();
                break;
            case 1:
                SM4_SetLow();
                SM3_SetHigh();
                SM2_SetHigh();
                SM1_SetLow();
                break;
            case 2:
                SM4_SetLow();
                SM3_SetLow();
                SM2_SetHigh();
                SM1_SetHigh();
                break;
            case 3:
                SM4_SetHigh();
                SM3_SetLow();
                SM2_SetLow();
                SM1_SetHigh();
                break;
        }
    }
}

void move(uint8_t n_passos, bool sentido){
    uint8_t incPos = 0;
    if(sentido){
        incPos = 1;
    }else{
        incPos = -1;
    }
    for(uint8_t i=0; i<=n_passos; i = i++){
        pas.sos++;
        position = position + incPos;
        meioPasso(sentido);
        __delay_ms(3);
    }
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR0_SetInterruptHandler(envia_Tx);
    EUSART_SetRxInterruptHandler(receive);

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

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