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
        fluxpos();
    }
}

void pwmcontrol(){
    error = ballset-balldist; //Calculo do erro baseado na dist setado com a dist real
    if(error > 150 || error < 150){ // Caso o erro seja maior do que 5%, roda o codigo
        outputsum += ((kip*timecontrol*error)/1000);//Kintegrativa com erro e o tempo do timer 
        if (outputsum > 10230) outputsum = 10230; //caso a soma seja maior que 1023(*10 por causa de 1 casa decimai) fixa em 1023
        output = ((kpp*error + outputsum)/10); //voltando a escala padr�o
        if(output > 1023) output = 1023; //saturando o output
        EPWM1_LoadDutyValue(output); //mandando o valor ap�s o controle
    }
}

void fluxpos(){
    flux = output - position;
    if(flux>0) sentido = true;
    else if( flux <0) sentido = false;
    
}

void controlchoose(){
    if (controlchoice == true){ //escolhendo qual malha de controle com uma variavel bool
        pwmcontrol ();
    }
//        else {
//        fluxcontrol ();
//    }
    TMR4_LoadPeriodRegister(0);
    TMR4_StartTimer();
}

void analisa_Rx (){
    Rx_ctrl = false;
    switch(bufferRx[0]){
        case RX_CMD_MNL:            //caso para comando em funcionamento manual
            if(countRx==RX_CMD_SZ){     //se possui o tamanho correto
                func_mode = bufferRx[0];    //atualiza modo de funcionamento
                vRx.vH = bufferRx[3];   //MSB do setpoint de posicao da valvula salvo na uniao
                vRx.vL = bufferRx[4];   //LSB do setpoint de posicao da valvula salvo na uniao
                sp_position = vRx.v;    //Transfere-se valor recebido para variavel do setpoint da valvula
                vRx.vH = bufferRx[5];   //MSB do setpoint do dutycycle salvo na uniao
                vRx.vL = bufferRx[6];   //LSB do setpoint do dutycycle salvo na uniao
                dc = vRx.v;             //Transfere-se valor recebido para variavel do dutycycle
                EPWM1_LoadDutyValue(dc);    //atualiza-se PWM com novo dutycycle recebido
            }
            countRx = 0;
            break;
        case RX_CMD_VENT:
            if(countRx==RX_CMD_SZ){
                func_mode = bufferRx[0];    //atualiza modo de funcionamento
                controlchoice = true;
                vRx.vH = bufferRx[1];   //MSB do setpoint de altura salvo na uniao
                vRx.vL = bufferRx[2];   //LSB do setpoint de altura salvo na uniao
                sp_height = vRx.v;      //Transfere-se valor recebido para variavel do setpoint de altura
                ballset = vRx.v / 2;    //Define-se ballset para on controle
                vRx.vH = bufferRx[3];   //MSB do setpoint de posicao da valvula salvo na uniao
                vRx.vL = bufferRx[4];   //LSB do setpoint de posicao da valvula salvo na uniao
                sp_position = vRx.v;    //Transfere-se valor recebido para variavel do setpoint da valvula
            }
            countRx = 0;
            break;            
        case RX_CMD_VAL:
            if(countRx==RX_CMD_SZ){
                func_mode = bufferRx[0];    //atualiza modo de funcionamento
                controlchoice = false;
                vRx.vH = bufferRx[1];   //MSB do setpoint de altura salvo na uniao
                vRx.vL = bufferRx[2];   //LSB do setpoint de altura salvo na uniao
                sp_height = vRx.v;      //Transfere-se valor recebido para variavel do setpoint de altura
                ballset = vRx.v / 2;    //Define-se ballset para on controle
                vRx.vH = bufferRx[5];   //MSB do setpoint do dutycycle salvo na uniao
                vRx.vL = bufferRx[6];   //LSB do setpoint do dutycycle salvo na uniao
                dc = vRx.v;             //Transfere-se valor recebido para variavel do dutycycle
                EPWM1_LoadDutyValue(dc);    //atualiza-se PWM com novo dutycycle recebido
            }
            countRx = 0;
            break;
        case RX_CMD_RST:
            if(countRx==RX_CMD_SZ){
                RESET();//instrucao de reset
            }
            countRx = 0;
            break;
        default:
            countRx = 0;
    } 
}

void envia_Tx (){                       //funcao para transmissao de dados
    EUSART_Write(func_mode);            //Envia modo de funcionamento atual
    vTx.v = sp_height;                  //Passa setpoint de altura para uniao de envio
    EUSART_Write(vTx.vH);               //Envia MSB do setpoint de altura
    EUSART_Write(vTx.vL);               //Envia LSB do setpoint de altura
    vTx.v = height;                     //Passa a medicao de altura para uniao de envio
    EUSART_Write(vTx.vH);               //Envia MSB da medicao de altura
    EUSART_Write(vTx.vL);               //Envia LSB da medicao de altura
    vTx.v = position;                   //Passa a medicao de altura para uniao de envio
    EUSART_Write(vTx.vH);               //Envia MSB da posicao da valvula
    EUSART_Write(vTx.vL);               //Envia LSB da posicao da valvula
    vTx.v = dc;                         //Passa o valor do dutycycle do PWM para uniao de envio
    EUSART_Write(vTx.vH);               //Envia MSB do dutycycle do PWM
    EUSART_Write(vTx.vL);               //Envia LSB do dutycycle do PWM
    /*vTx.v = tempo;                  //Passa o valor da contagem de tempo para uniao de envio
    EUSART_Write(vTx.vH);               //Envia MSB do valor da contagem de tempo
    EUSART_Write(vTx.vL); */             //Envia LSB do valor da contagem de tempo
    vTx.v = float_temp*10;              //Passa o valor da temperatura com uma casa decimal para uniao de envio
    EUSART_Write(vTx.vH);               //Envia MSB do valor da temperatura com uma casa decimal
    EUSART_Write(vTx.vL);               //Envia LSB do valor da temperatura com uma casa decimal
}

void end_Rx () {
    if(count_40ms >= 10){
        count_40ms = 0;
        if(Rx_ctrl == true){
            analisa_Rx();
        }
    }
    else{
        count_40ms++;
    }
}

void receive(){
    count_40ms = 0;
    TMR0_Reload();
    Rx_ctrl = true;
    if(countRx<BUFFER_MAX-1){          // se o buffer nao esta cheio
        bufferRx[countRx] = EUSART_Read();   // guarda valor
        countRx++;
    }
}

void trigger_Rx (){
    Trigger_SetHigh();
    __delay_us(20);
    Trigger_SetLow();
    if(count_Tx >= 24){
        envia_Tx ();
        count_Tx = 0;
    }
    else{
        count_Tx++;
    }
}


void definePassoMotor(uint8_t passo, uint8_t sentido) {
    if (sentido == HORARIO) {  // Sentido hor�rio (abrindo a porta)
        switch(passo) {
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
    }
    else if(sentido == ANTIHORARIO){  // Sentido anti-hor�rio (fechando a porta)
        switch(passo) {
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
    }else{
        break;
    }
    
}

void daUmPasso(uint8_t sentido) {
    // Atualiza o passo e garante que ele esteja no intervalo de 0 a 3
    passo++;
    passo = passo & 0b00000011;
    // Verifica o fim de curso e atualiza a posi��o se necess�rio
    if (CMP1_GetOutputStatus()) {
        fim_curso = true;
        position = 0;  // Reset da posi��o no fim de curso
    } else {
        fim_curso = false;
    }
    // Atualiza a posi��o com base no sentido e fim de curso
    if (fim_curso) {
        // Incrementa ou decrementa a posi��o conforme o sentido
        if(sentido == HORARIO){
            position--;
        } 
        else if(sentido == ANTIHORARIO){
            position++;
        }
        definePassoMotor(passo, sentido);
    } else {
        definePassoMotor(passo, HORARIO);
    }
}

void calculaTemp(){
    float_temp = adc_temp * GAIN_TEMP;
}

void mede_height (){
    tempo_voo = TMR1_ReadTimer() * 0.00025;
    height = (tempo_voo * 170); //Isso eh a velocidade do som dividido por 2 em m/s
    balldist = height/2;
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR0_SetInterruptHandler(end_Rx);
    EUSART_SetRxInterruptHandler(receive);
    TMR6_SetInterruptHandler(fluxControlChoice);
    TMR1_SetGateInterruptHandler(mede_height);
    TMR2_SetInterruptHandler(trigger_Rx);
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
        adc_temp = ADC_GetConversion(channel_AN8);
        if((timecontrol = TMR4_ReadTimer()) >= 209){//timer incrementa a cada 96us, 96us*209 = 20.096ms, tempo para escolher a malha de controle
            TMR4_StopTimer();
            controlchoose();
        }
        if(TMR6_ReadTimer() >= 200 & !controlchoice){
            daUmPasso(sentido);
        }
    }
}
/**
 End of File
*/