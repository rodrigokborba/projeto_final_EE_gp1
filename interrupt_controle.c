#include "mcc_generated_files/mcc.h"
#include "main.h"

void fluxcontrol(){
    error = (ballset-balldist)*100; //Calculo do erro baseado na dist setado com a dist real, *10 pra duas casas decimais
    if(error > 150 || error < 150){ // Caso o erro seja maior do que 5%
        outputsum += ((kif*timecontrol*error)/1000); //Kintegrativa com erro e o tempo do timer   
        if (outputsum > 45000) outputsum = 45000; //caso a soma seja maior que 450(*100 por causa de 2 casas decimais), fixa em 450
        output = ((kpf*error + outputsum)/100); // definindo output com kpf e voltando a escala
        if (output > 450) output = 450; //saturando o output
        fluxpos();
    }
}

void pwmcontrol(){
    error = (ballset-balldist)*10; //Calculo do erro baseado na dist setado com a dist real
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
    if(flux>0) sentido = ANTIHORARIO;
    else if( flux <0) sentido = HORARIO;
    else sentido = PARADO;
}

void controlchoose(){
    timecontrol = TMR4_ReadTimer();
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
    TMR0_SetInterruptHandler(end_Rx);
    EUSART_SetRxInterruptHandler(receive);
    TMR1_SetGateInterruptHandler(mede_height);
    TMR2_SetInterruptHandler(trigger_Rx);
    TMR4_SetInterruptHandler(controlchoice);
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
        if(TMR6_ReadTimer() >= 0x25 && !controlchoice){
            TMR6_StopTimer();
            daUmPasso(sentido);
            TMR6_LoadPeriodRegister(0);
            TMR6_StartTimer();
        }
        if (EUSART_is_rx_ready){
            count_40ms = 0;
            TMR0_Reload();
            Rx_ctrl = true;
          
            while(countRx<BUFFER_MAX-1){          // se o buffer nao esta cheio
                while(!EUSART_is_rx_ready){
                    __delay_us(1);
                }
                bufferRx[countRx] = EUSART_Read();   // guarda valor
                countRx++;
                
            }
        }
    }
}