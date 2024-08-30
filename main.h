/* 
 * File:   main.h
 * Author: Labsisco 06
 *
 * Created on August 27, 2024, 9:39 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> //include processor files - each processor file is guarded.

/*Macros*/
#define RX_INI '$'      ///< Inicio do quadro de comunica��o.
#define RX_END 0x0D     ///< Fim do quadro de comunica��o.
#define BUFFER_MAX 8    ///< M�xima quantidade de bytes a serem recebidos.
#define RX_CMD_PWM 'G'   ///< Comando para definir duty cycle do PWM.
#define RX_CMD_PWM_SZ 5  ///< Comprimento da mensagem RX_CMD_PWM.
#define RX_CMD_STEP 'H'   ///< Comando para definir valor para o motor de passo.
#define RX_CMD_STEP_SZ 5  ///< Comprimento da mensagem RX_CMD_STEP.
#define RX_CMD_HEIGHT 'I'///< Comando para alterar altura da bolinha (set point)
#define RX_CMD_HEIGHT_SZ 5  ///< Comprimento da mensagem RX_CMD_HEIGHT.
#define RX_CMD_CTRL 'J'  ///< Comando para selecionar o m�dulo de controle
#define RX_CMD_CTRL_SZ 2  ///< Comprimento da mensagem RX_CMD_CTRL.

/*Vari�veis de programa*/

//Controle PID
int16_t error;
int16_t balldist,ballset,flux;
uint16_t kpf = 055;
uint16_t kif = 105;
uint16_t kpp = 12;
uint16_t kip = 19;
uint16_t kdp = 01;
uint16_t dinput,outputsum,output;
bool controlchoice = true;
uint8_t timecontrol;

//Comunica��o Serial
union{                  ///< Uni�o para temperatura, usada na transmiss�o
    uint16_t t;         ///< Valor inteiro de 16 bits. Cont�m os 4 nibles
    struct{
        uint8_t n0 : 4; ///< Nible n0 
        uint8_t n1 : 4; ///< Nible n1  
        uint8_t n2 : 4; ///< Nible n2 
        uint8_t n3 : 4; ///< Nible n3
    };
}tTx;                   ///< Vari�vel com os valores guardados para temperatura

union{                  ///< Uni�o para altura, usada na transmiss�o
    uint16_t d;         ///< Valor inteiro de 16 bits. Cont�m os 4 nibles
    struct{
        uint8_t n0 : 4; ///< Nible n0 
        uint8_t n1 : 4; ///< Nible n1  
        uint8_t n2 : 4; ///< Nible n2 
        uint8_t n3 : 4; ///< Nible n3
    };
}dTx;                   ///< Vari�vel com os valores guardados para altura

union{                  ///< Uni�o para altura, usada no Recebimento
    uint16_t d;         ///< Valor inteiro de 16 bits. Cont�m os 4 nibles
    struct{
        uint8_t n0 : 4; ///< Nible n0 
        uint8_t n1 : 4; ///< Nible n1  
        uint8_t n2 : 4; ///< Nible n2 
        uint8_t n3 : 4; ///< Nible n3
    };
}dRx;                   ///< Vari�vel com os valores guardados para altura

union{                      ///< Uni�o para duty cycle, usada no Recebimento
    uint16_t dc;            ///< Valor inteiro de 16 bits. Cont�m os 4 nibles
    struct{
        uint8_t n0 : 4;     ///< Nible n0 
        uint8_t n1 : 4;     ///< Nible n1  
        uint8_t n2 : 4;     ///< Nible n2 
        uint8_t n3 : 4;     ///< Nible n3
    };  
}dcRx;                      ///< Vari�vel com os valores guardados de duty cycle

uint8_t bufferRx[BUFFER_MAX];       ///< Buffer de Rx
uint8_t countRx = 0;                ///< Contador de bytes recebidos (ponteiro)


//Movimento do motor de passo
uint8_t position;

struct {
    uint8_t sos : 2; //Esta � a vari�vel que se incrementa para determinar a configura��o das bobinas.
}pas;                //Ela tem 2 bits para contar de 0 ate 3 (sao 4 combinacoes diferentes de bobina)


/*Funcoes*/

//Controle PID
void fluxcontrol();
void pwmcontrol();
void fluxpos();
void controlchoose();

//Comunica��o Serial

/**
 * Analisa a mensagem recebida pelo EUSART e realiza o comando.
 */
void analisa_Rx ();

/**
 * Realiza o envio de altura e temperatura. Acionada na interrup��o do TMR0.
 */
void envia_Tx ();

/**
 * Converte um valor bin�rio de quatro bits para ASCII.
 * @param vBin
 * @return Valor ASCII
 */
uint8_t bin_ascii(uint8_t vBin);

/**
 * Converte um valor ASCII para um valor bin�rio de quatro bits.
 * @param vAscii
 * @return Valor bin�rio de 4 bits
 */
uint8_t ascii_bin(uint8_t vAscii);

/**
 * Fun��o utilizada para receber mensagens de comunica��o. Acionada na interrup��o do EUSART.
 */
void receive();

//Movimento do motor de passo
void meioPasso(bool sentido);
void move(uint8_t n_passos, bool sentido);
void encontraFimCurso();

#endif	/* MAIN_H */

