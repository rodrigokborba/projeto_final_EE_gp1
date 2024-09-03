/* 
 * File:   main.h
 * Author: Labsisco 06
 *
 * Created on August 27, 2024, 9:39 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> //include processor files - each processor file is guarded.

/*Macros do Sistema de Comunicacao*/
#define BUFFER_MAX 8        ///< Maxima quantidade de bytes a serem recebidos.
#define RX_CMD_MNL 0x0     ///< Comando para funcionamento manual
#define RX_CMD_VENT 0x1    ///< Comando para funcionamento controlando ventoinha
#define RX_CMD_VAL 0x2     ///< Comando para funcionamento controlando valvula
#define RX_CMD_RST 0x3     ///< Comando para reset
#define RX_CMD_SZ 7         ///< Comprimento das mensagens de comando

/*Macros do Sistema de Sensoriamento*/
#define GAIN_TEMP 0.1 ///< Aproxima��o do calculo: (1.024/1023)/0.01
#define POSITION_GAIN 0 ///< Aproxima��o do calculo: ;

/**
 Macros do Motor de Passos
 */
#define HORARIO 1
#define ANTIHORARIO 0
#define PARADO 2

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
uint8_t sentido;

//Comunicacao Serial
union{                  ///< Uniao para transmiss�o
    uint16_t v;         ///< Valor inteiro de 16 bits para transmiss�o
    struct{
        uint8_t vH;     ///< MSB do valor a ser transmitido 
        uint8_t vL;     ///< LSB do valor a ser transmitido
    };
}vTx;                   ///< Variavel com os valores usados na transmiss�o

union{                  ///< Uniao para recep��o de dados
    uint16_t v;         ///< Valor inteiro de 16 bits recebido
    struct{
        uint8_t vH;     ///< MSB do valor recebido 
        uint8_t vL;     ///< LSB do valor recebido
    };
}vRx;                   ///< Variavel com os valores recebidos

bool Rx_ctrl = false;
uint8_t func_mode = 0;      ///< Variavel usada para indicar o modo de funcionamento. 0=manual, 1=ventoinha, 2=valvula, 3=reset.
uint16_t sp_height;         ///< Variavel usada para guardar o setpoint de altura.
uint16_t dc;                ///< Variavel usada para guardar o valor do pwm.
uint16_t sp_position;       ///< Setpoint para posi��o da valvula (motor de passo)

uint8_t bufferRx[BUFFER_MAX];       ///< Buffer de Rx
uint8_t countRx = 0;                ///< Contador de bytes recebidos (ponteiro)
uint8_t count_40ms = 0; 
uint8_t count_Tx = 0;

//Medicao de altura
uint16_t height = 0;            ///< Variavel usada para guardar a medicao de altura
float tempo_voo;                ///< Variavel usada para guardar o tempo de voo na medicao de altura.

//Movimento do motor de passo
uint16_t position;
uint8_t passo;
bool fim_curso;

struct {
    uint8_t sos : 2; //Esta � a vari�vel que se incrementa para determinar a configura��o das bobinas.
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

// -------------------------------------------- Comunicacao serial -------------------------------------------------------- 

/**
 * Analisa a mensagem recebida pelo EUSART e realiza o comando.
 */
void analisa_Rx ();

/**
 * Realiza o envio de altura e temperatura. Acionada na interrupcao do TMR0.
 */
void envia_Tx ();

/**
 * Funcao utilizada para receber mensagens de comunicacao. Acionada na interrupcao do EUSART.
 */
void receive();

/**
 * 
 */
void trigger_Rx ();

/**
 * 
 */
void end_Rx ();

// -------------------------------------------- Motor de passos -------------------------------------------------------- 

/**
 * Funcao de passo do motor
 * @param sentido variavel booleana que indica se o motor esta andando no sentido horario ou anti-horario
 * 
 */
void daUmPasso(uint8_t sentido);


/**
 * 
 * @param passo
 * @param sentido
 */
void definePassoMotor(uint8_t passo, uint8_t sentido);

// ------------------------------------------- Temperatura --------------------------------------------------------------

/**
 Fun��o de c�lculo da temperatura do sensor LM35
 */
void calculaTemp();
#endif	/* MAIN_H */

// --------------------------------------------- Distancia ---------------------------------------------------------------
/**
 * Funcao para medicao de distancia. Chamada na interrupcao do gate do timer1.
 */
void mede_height ();