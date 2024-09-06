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
    error = (ballset-balldist)*100;                     //Calculo do erro baseado na dist setado com a dist real, *10 pra duas casas decimais
    if(error > 150 || error < 150){                     // Caso o erro seja maior do que 5%
        outputsum += ((kif*timecontrol*error)/100);     //Kintegrativa com erro e o tempo do timer   
        if (outputsum > 4500) outputsum = 4500;         //caso a soma seja maior que 450(*100 por causa de 2 casas decimais), fixa em 450
        else if(outputsum <-1000) outputsum = -1000;
        outpre = ((kpf*error + outputsum)/100)+outpre;  // definindo output com kpf e voltando a escala
        if (outpre > 380) output = 380;                 //saturando o output
        else if(outpre <0 ) output = 0;
        else output = (uint16_t)outpre;
        errorp = error;
        
    }
}

void pwmcontrol(){
    error = (ballset-balldist)*10;                      //Calculo do erro baseado na dist setado com a dist real
    if(error > 150 || error < 150){                     // Caso o erro seja maior do que 5%, roda o codigo
        outputsum += ((kip*timecontrol*error));         //Kintegrativa com erro e o tempo do timer 
        if (outputsum > 1000) outputsum = 1000;         //caso a soma seja maior que 1023(*10 por causa de 1 casa decimai) fixa em 1023
        else if (outputsum< -1000) outputsum = -1000;
        outpre = (kpp*error + outputsum + (kdp*(error-errorp)+outpre*10)/10); //voltando a escala padrao
        if(outpre > 1023) output = 1023;                //saturando o output
        else if(outpre <0) output= 0;
        else output = (uint16_t)outpre;
        EPWM1_LoadDutyValue(output);                    //mandando o valor ap�s o controle
        errorp = error;
        
    }
}

void fluxpos(){
    if(controlchoice==2){ 
        flux = output - position;
        if(flux>position) daUmPasso(ANTIHORARIO);
        else if(flux<position) daUmPasso(HORARIO);
    }
    else{
        if(sp_position>position) daUmPasso(ANTIHORARIO);
        else if(sp_position<position) daUmPasso(HORARIO);
    }
}

void setaPorta(){
    while(!CMP1_GetOutputStatus()){
        daUmPasso(HORARIO);
        __delay_ms(6);
    }
    fim_curso = true;
    position = 0;
}

void controlchoose(){
    if (controlchoice == 1){                        //escolhendo qual malha de controle com uma variavel bool
        pwmcontrol ();
    } else if(controlchoice == 2){
        fluxcontrol ();
    }
    TMR4_StartTimer();
}

void analisa_Rx (){
    switch(bufferRx[0]){
        case RX_CMD_MNL:                            //caso para comando em funcionamento manual
            if(countRx==RX_CMD_SZ){                 //se possui o tamanho correto
                controlchoice = bufferRx[0];        //atualiza modo de funcionamento
                vRx.vH = bufferRx[3];               //MSB do setpoint de posicao da valvula salvo na uniao
                vRx.vL = bufferRx[4];               //LSB do setpoint de posicao da valvula salvo na uniao
                sp_position = vRx.v;                //Transfere-se valor recebido para variavel do setpoint da valvula
                if(sp_position > 380 ) sp_position = 380;
                if(sp_position < 0) sp_position = 0;
                vRx.vH = bufferRx[5];               //MSB do setpoint do dutycycle salvo na uniao
                vRx.vL = bufferRx[6];               //LSB do setpoint do dutycycle salvo na uniao
                dc = vRx.v;                         //Transfere-se valor recebido para variavel do dutycycle
                EPWM1_LoadDutyValue(dc);            //atualiza-se PWM com novo dutycycle recebido
            }
            countRx = 0;                            //Zera contador para o proximo recebimento
            break;                                  //Sai do switch
        case RX_CMD_VENT:                           //caso para comando em funcionamento ventoinha
            if(countRx==RX_CMD_SZ){                 //se possuir tamanho esperado
                controlchoice = bufferRx[0];        //atualiza modo de funcionamento
                vRx.vH = bufferRx[1];               //MSB do setpoint de altura salvo na uniao
                vRx.vL = bufferRx[2];               //LSB do setpoint de altura salvo na uniao
                sp_height = vRx.v;                  //Transfere-se valor recebido para variavel do setpoint de altura
                ballset = vRx.v / 2;                //Define-se ballset para on controle
                vRx.vH = bufferRx[3];               //MSB do setpoint de posicao da valvula salvo na uniao
                vRx.vL = bufferRx[4];               //LSB do setpoint de posicao da valvula salvo na uniao
                sp_position = vRx.v;                //Transfere-se valor recebido para variavel do setpoint da valvula
                outputsum = 0;
                errorp=0;
                error =0;
            }
            countRx = 0;                            //Zera contador para o proximo recebimento
            break;                                  //Sai do switch            
        case RX_CMD_VAL:                            //caso para comando em funcionamento valvula
            if(countRx==RX_CMD_SZ){                 //se possuir tamanh esperado
                controlchoice = bufferRx[0];        //atualiza modo de funcionamento
                vRx.vH = bufferRx[1];               //MSB do setpoint de altura salvo na uniao
                vRx.vL = bufferRx[2];               //LSB do setpoint de altura salvo na uniao
                sp_height = vRx.v;                  //Transfere-se valor recebido para variavel do setpoint de altura
                ballset = vRx.v / 2;                //Define-se ballset para on controle
                vRx.vH = bufferRx[5];               //MSB do setpoint do dutycycle salvo na uniao
                vRx.vL = bufferRx[6];               //LSB do setpoint do dutycycle salvo na uniao
                dc = vRx.v;                         //Transfere-se valor recebido para variavel do dutycycle
                EPWM1_LoadDutyValue(dc);            //atualiza-se PWM com novo dutycycle recebido
                outputsum = 0;
                errorp=0;
                error =0;
            }
            countRx = 0;                            //Zera contador para o proximo recebimento
            break;                                  //Sai do switch
        case RX_CMD_RST:                            //caso para comando de reset
            if(countRx==RX_CMD_SZ){                 //se possuir tamanho esperado
                RESET();                            //instrucao de reset
            }
            countRx = 0;                            //Zera contador para o proximo recebimento
            break;                                  //Sai do switch
        default:
            countRx = 0;
    } 
}

void envia_Tx (){                            //funcao para transmissao de dados
    EUSART_Write(controlchoice);            //Envia modo de funcionamento atual
    vTx.v = sp_height;                      //Passa setpoint de altura para uniao de envio
    EUSART_Write(vTx.vH);                   //Envia MSB do setpoint de altura
    EUSART_Write(vTx.vL);                   //Envia LSB do setpoint de altura
    vTx.v = height;                         //Passa a medicao de altura para uniao de envio
    EUSART_Write(vTx.vH);                   //Envia MSB da medicao de altura
    EUSART_Write(vTx.vL);                   //Envia LSB da medicao de altura
    vTx.v = avg_tempo_voo;                  //Passa o valor da media do tempo de voo para uniao de envio
    EUSART_Write(vTx.vH);                   //Envia MSB do valor da media do tempo de voo
    EUSART_Write(vTx.vL);                   //Envia LSB do valor da media do tempo de voo
    vTx.v = adc_temp;                       //Passa o valor da temperatura com uma casa decimal para uniao de envio
    EUSART_Write(vTx.vH);                   //Envia MSB do valor da temperatura com uma casa decimal
    EUSART_Write(vTx.vL);                   //Envia LSB do valor da temperatura com uma casa decimal
    vTx.v = sp_position;                    //Passa o valor do setpoint da posicao do motor de passo para uniao de envio
    EUSART_Write(vTx.vH);                   //Envia MSB do setpoint da posicao da valvula
    EUSART_Write(vTx.vL);                   //Envia LSB do setpoint da posicao da valvula
    vTx.v = position;                       //Passa a posicao da valvula para uniao de envio
    EUSART_Write(vTx.vH);                   //Envia MSB da posicao da valvula
    EUSART_Write(vTx.vL);                   //Envia LSB da posicao da valvula
    vTx.v = dc;                             //Passa o valor do dutycycle do PWM para uniao de envio
    EUSART_Write(vTx.vH);                   //Envia MSB do dutycycle do PWM
    EUSART_Write(vTx.vL);                   //Envia LSB do dutycycle do PWM
}



void definePassoMotor(uint8_t passom, uint8_t sentido) {
    if (sentido == HORARIO) {       // Funcionamento das bobinas para o sentido horario (abertura)
        switch(passom) {
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
    else if(sentido == ANTIHORARIO){  // Funcionamento das bobinas para o sentido anti-horario (fechamento)
        switch(passom) {
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
    // Atualiza o passo e garante que ele esteja no intervalo de 0 a 3
    passo++;
    passo = passo & 0x03;
}

void daUmPasso(uint8_t sentido) {
    if (fim_curso) {                        // Testa se o fim de curso da porta foi atingido durante a execucao
        if(sentido == HORARIO){             // Para o sentido horario (abertura da porta), decrementa a posicao da porta
            position--;
        } 
        else if(sentido == ANTIHORARIO){    // Para o sentido anti-horario (fechamento da porta), incrementa a posicao da porta
            position++;
        }
        definePassoMotor(passo, sentido);   // Chama a funcao que define o passo do motor, de acordo com o sentido definido, ap�s o fim de curso ser confirmado
    } else {
        definePassoMotor(passo, HORARIO);   // Se o fim de curso nunca foi antingido, a abertura da porta continua at� a posicao da porta ser zerada
    }
}

void mede_height (){                    // Mede altura e define media movel de altura e de tempo de voo
    tempo_voo = TMR1_ReadTimer();       // Le-se valor capturado no timer1
    TMR1_Reload();                      // Recarrega o TMR1  
    if (first_read == true){             
        avg_tempo_voo = tempo_voo;
        first_read = false;
    }
    else{
        avg_tempo_voo = ((uint16_t)avg_tempo_voo + (tempo_voo))>>1;          //Media movel dos quatro ultimos valores de tempo de voo (media anterior + nova media/4)
    }
    height = (uint16_t)(avg_tempo_voo*lookupTable[(int)(adc_temp/10)]);     //Altura = tempo de voo*velocidade do som para temperatura*0,00025/2
    balldist = height;
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR1_SetGateInterruptHandler(mede_height);
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
    setaPorta();

    while (1)
    {
        if(PIR3bits.TMR4IF==1){                     //timer incrementa a cada 96us, 96us*209 = 20.096ms, tempo para escolher a malha de controle
            PIR3bits.TMR4IF=0;  
            TMR4_StopTimer();
            controlchoose();
        }
        
        if (EUSART_is_rx_ready()){                  //Se houver um byte para ser lido
            TMR6_LoadPeriodRegister(0xF9);          //define o periodo do timer6 como 40ms
            while(countRx<BUFFER_MAX-1){            //Enquanto o buffer nao estiver cheio
                nao_salva = false;                  //Tira a flag que indica o timeout
                TMR6_WriteTimer(0);                 //Zera o timer6
                PIR3bits.TMR6IF = 0;                //Zera flag de comparacao (40ms)
                while(!EUSART_is_rx_ready()){       //Enquanto nao houver valor pronto no RCREG
                    if(PIR3bits.TMR6IF){            //Checa a flag de comparacao do timer6. Essa flag indica que ocorreu um timeout
                        nao_salva = true;           //Se ocorreu um timeout, ativa a flag de controle para nao salvar valores a mais
                        break;                      //Sai do while de espera do EUSART
                    }
                }
                if(nao_salva == true){              //Se nao_salva estiver ativa
                    break;                          //sai do while para salvar recebimentos
                }
                bufferRx[countRx] = EUSART_Read();  // guarda valor caso nao tenha ocorrido timeout, ou caso nao tenha enchido o buffer
                countRx++;                          //incrementa ponteiro do buffer
            }
            analisa_Rx();                           //Quando termina de guardar os valores recebidos, analisa-se o recebimento
            TMR6_LoadPeriodRegister(0x4A);          //Volta o periodo do timer6 para ser utilizado pelo trigger (12ms)
            TMR6_WriteTimer(0);                     //Zera o timer6
            PIR3bits.TMR6IF = 0;                    //Zera a flag do timer6
        }
        if(INTCONbits.TMR0IF == 1){                 //Se ocorreu overflow no timer0
            INTCONbits.TMR0IF = 0;                  //Zera flag do timer0
            fluxpos();
            passo_ctrl = false;
            count_Tx ++;                            //Incrementa contador
            if (count_Tx == 6){                     //Se contador for igual a 6 (16,384ms*6=98,304ms)
                envia_Tx ();                        //Chama funcao de envio dos dados
                count_Tx = 0;                       //Zera contador
            }
        }
        if(PIR3bits.TMR6IF == 1){                   //Se a comparacao do timer6 aconteceu (12ms)
            PIR3bits.TMR6IF = 0;                    //Limpa flag do timer6
            TMR6_WriteTimer(0);                     //Zera timer6
            adc_temp = ADC_GetConversion(channel_AN8); 
            Trigger_SetHigh();                      //Ativa trigger
            __delay_us(15);                         //Espera 15us
                Trigger_SetLow();                   //Desativa trigger
        }
        if(TMR0_ReadTimer() >= 0x7F && passo_ctrl == false){
            passo_ctrl = true;
            fluxpos();
        }
        
    }
}
/**
 End of File
*/