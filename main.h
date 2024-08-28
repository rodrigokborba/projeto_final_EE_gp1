/* 
 * File:   main.h
 * Author: Labsisco 06
 *
 * Created on August 27, 2024, 9:39 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> // include processor files - each processor file is guarded.

#define RX_INI '$'      ///< Inicio do quadro de comunicação.
#define RX_END 0x0D     ///< Fim do quadro de comunicação.
#define BUFFER_MAX 8    ///< Máxima quantidade de bytes a serem recebidos.
#define RX_CMD_PWM 'G'   ///< Comando para definir duty cycle do PWM.
#define RX_CMD_PWM_SZ 5  ///< Comprimento da mensagem RX_CMD_PWM.
#define RX_CMD_STEP 'H'   ///< Comando para definir valor para o motor de passo.
#define RX_CMD_STEP_SZ 5  ///< Comprimento da mensagem RX_CMD_STEP.
#define RX_CMD_HEIGHT 'I'///< Comando para alterar altura da bolinha (set point)
#define RX_CMD_HEIGHT_SZ 5  ///< Comprimento da mensagem RX_CMD_HEIGHT.
#define RX_CMD_CTRL 'J'  ///< Comando para selecionar o módulo de controle
#define RX_CMD_CTRL_SZ 2  ///< Comprimento da mensagem RX_CMD_CTRL.

    int16_t error;
    int16_t balldist,ballset;
    uint16_t kpf = 055;
    uint16_t kif = 105;
    uint16_t kpp = 12;
    uint16_t kip = 19;
    uint16_t kdp = 01;
    uint16_t dinput,outputsum,output;
    bool controlchoice = true;
    uint8_t timecontrol;
union{                  //estrutura de união para temperatura
    uint16_t t;
    struct{
        uint8_t tL;
        uint8_t tH;
    };
}tTx;

union{                  //estrutura de união para distância, usada no envio
    uint16_t d;
    struct{
        uint8_t dL;
        uint8_t dH;
    };
}dTx;

union{                 //estrutura de união para distância, usada no recebimento
    uint16_t d;
    struct{
        uint8_t n0 : 4; ///< Nible n0 
        uint8_t n1 : 4; ///< Nible n1  
        uint8_t n2 : 4; ///< Nible n2 
        uint8_t n3 : 4; ///< Nible n3
    };
}dRx;

union{                //estrutura de união para duty cycle, usada no Recebimento
    uint16_t dc;
    struct{
        uint8_t n0 : 4; ///< Nible n0 
        uint8_t n1 : 4; ///< Nible n1  
        uint8_t n2 : 4; ///< Nible n2 
        uint8_t n3 : 4; ///< Nible n3
    };
}dcRx;

uint8_t bufferRx[BUFFER_MAX];       ///< Buffer de Rx
uint8_t countRx = 0;                ///< Contador de bytes recebidos (ponteiro)

#endif	/* MAIN_H */

