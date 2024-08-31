/* 
 * File:   main.h
 * Author: Labsisco 06
 *
 * Created on August 27, 2024, 9:39 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> //include processor files - each processor file is guarded.

/*Macros do Sistema de Comunicaï¿½ï¿½o*/
#define RX_INI '$'      ///< Inicio do quadro de comunicaï¿½ï¿½o.
#define RX_END 0x0D     ///< Fim do quadro de comunicaï¿½ï¿½o.
#define BUFFER_MAX 8    ///< Mï¿½xima quantidade de bytes a serem recebidos.
#define RX_CMD_PWM 'G'   ///< Comando para definir duty cycle do PWM.
#define RX_CMD_PWM_SZ 5  ///< Comprimento da mensagem RX_CMD_PWM.
#define RX_CMD_STEP 'H'   ///< Comando para definir valor para o motor de passo.
#define RX_CMD_STEP_SZ 5  ///< Comprimento da mensagem RX_CMD_STEP.
#define RX_CMD_HEIGHT 'I'///< Comando para alterar altura da bolinha (set point)
#define RX_CMD_HEIGHT_SZ 5  ///< Comprimento da mensagem RX_CMD_HEIGHT.
#define RX_CMD_CTRL 'J'  ///< Comando para selecionar o mï¿½dulo de controle
#define RX_CMD_CTRL_SZ 2  ///< Comprimento da mensagem RX_CMD_CTRL.

/*Macros do Sistema de Sensoriamento*/
#define GAIN_TEMP 0.1 ///< Aproximaï¿½ï¿½o do calculo: (1.024/1023)/0.01
#define POSITION_GAIN 0 ///< Aproximaï¿½ï¿½o do calculo: ;

/**
 Macros do Motor de Passos
 */
#define HORARIO 1
#define ANTIHORARIO 0


/*Variï¿½veis de programa*/
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


//Comunicaï¿½ï¿½o Serial
union{                  ///< Uniï¿½o para temperatura, usada na transmissï¿½o
    uint16_t t;         ///< Valor inteiro de 16 bits. Contï¿½m os 4 nibles
    struct{
        uint8_t n0 : 4; ///< Nible n0 
        uint8_t n1 : 4; ///< Nible n1  
        uint8_t n2 : 4; ///< Nible n2 
        uint8_t n3 : 4; ///< Nible n3
    };
}tTx;                   ///< Variï¿½vel com os valores guardados para temperatura

union{                  ///< Uniï¿½o para altura, usada na transmissï¿½o
    uint16_t d;         ///< Valor inteiro de 16 bits. Contï¿½m os 4 nibles
    struct{
        uint8_t n0 : 4; ///< Nible n0 
        uint8_t n1 : 4; ///< Nible n1  
        uint8_t n2 : 4; ///< Nible n2 
        uint8_t n3 : 4; ///< Nible n3
    };
}dTx;                   ///< Variï¿½vel com os valores guardados para altura

union{                  ///< Uniï¿½o para altura, usada no Recebimento
    uint16_t d;         ///< Valor inteiro de 16 bits. Contï¿½m os 4 nibles
    struct{
        uint8_t n0 : 4; ///< Nible n0 
        uint8_t n1 : 4; ///< Nible n1  
        uint8_t n2 : 4; ///< Nible n2 
        uint8_t n3 : 4; ///< Nible n3
    };
}dRx;                   ///< Variï¿½vel com os valores guardados para altura

union{                      ///< Uniï¿½o para duty cycle, usada no Recebimento
    uint16_t dc;            ///< Valor inteiro de 16 bits. Contï¿½m os 4 nibles
    struct{
        uint8_t n0 : 4;     ///< Nible n0 
        uint8_t n1 : 4;     ///< Nible n1  
        uint8_t n2 : 4;     ///< Nible n2 
        uint8_t n3 : 4;     ///< Nible n3
    };  
}dcRx;                      ///< Variï¿½vel com os valores guardados de duty cycle

uint8_t bufferRx[BUFFER_MAX];       ///< Buffer de Rx
uint8_t countRx = 0;                ///< Contador de bytes recebidos (ponteiro)


//Movimento do motor de passo
uint8_t position;
uint8_t incPos = 1;
uint8_t passo;
bool fim_curso;

struct {
    uint8_t sos : 2; //Esta ï¿½ a variï¿½vel que se incrementa para determinar a configuraï¿½ï¿½o das bobinas.
}pas;                //Ela tem 2 bits para contar de 0 ate 3 (sao 4 combinacoes diferentes de bobina)

//Sensor de Temperatura
uint16_t adc_temp;
float float_temp;

/*Funcoes*/

//Controle PID
void fluxcontrol();
void pwmcontrol();
void fluxpos();
void controlchoose();

//Comunicaï¿½ï¿½o Serial

/**
 * Analisa a mensagem recebida pelo EUSART e realiza o comando.
 */
void analisa_Rx ();

/**
 * Realiza o envio de altura e temperatura. Acionada na interrupï¿½ï¿½o do TMR0.
 */
void envia_Tx ();

/**
 * Converte um valor binï¿½rio de quatro bits para ASCII.
 * @param vBin
 * @return Valor ASCII
 */
uint8_t bin_ascii(uint8_t vBin);

/**
 * Converte um valor ASCII para um valor binï¿½rio de quatro bits.
 * @param vAscii
 * @return Valor binï¿½rio de 4 bits
 */
uint8_t ascii_bin(uint8_t vAscii);

/**
 * Funï¿½ï¿½o utilizada para receber mensagens de comunicaï¿½ï¿½o. Acionada na interrupï¿½ï¿½o do EUSART.
 */
void receive();

// -------------------------------------------- Motor de passos -------------------------------------------------------- 

/**
 * Funcao de passo do motor
 * @param sentido variavel booleana que indica se o motor esta andando no sentido horario ou anti-horario
 * 
 */
void daUmPasso(bool sentido);
/**
 * Função que decide se a função fluxcontrol vai ser chamada a partir da variável controlchoice
 */
void fluxControlChoice(); 

/**
 * 
 * @param passo
 * @param sentido
 */
void definePassoMotor(uint8_t passo, bool sentido);

// ------------------------------------------- Temperatura --------------------------------------------------------------

/**
 Função de cálculo da temperatura do sensor LM35
 */
void calculaTemp();
#endif	/* MAIN_H */

