/* 
 * File:   main.h
 * Author: Labsisco 06
 *
 * Created on August 27, 2024, 9:39 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> //include processor files - each processor file is guarded.



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
int24_t error,errorp;
int24_t balldist,ballset,flux;
int24_t kpf = 200;
int24_t kif = 70;
int24_t kpp = 310;
int24_t kip = 150;
int24_t kdp = 5;
int24_t dinput,outputsum;
uint16_t output;
uint8_t controlchoice = 0;
int24_t timecontrol = 4;
uint8_t sentido;
int24_t outpre;


//Medicao de altura
__eeprom const float lookupTable[51] = {        ///< Tabela lookup com os valores de (velocidade_som*0,00025)/2 para cada um dos 51 valores de temperaturas
    0.041431,
    0.041507,
    0.041583,
    0.041658,
    0.041734,
    0.041809,
    0.041884,
    0.041959,
    0.042034,
    0.042108,
    0.042183,
    0.042257,
    0.042332,
    0.042406,
    0.042480,
    0.042554,
    0.042627,
    0.042701,
    0.042775,
    0.042848,
    0.042921,
    0.042994,
    0.043067,
    0.043140,
    0.043213,
    0.043286,
    0.043358,
    0.043431,
    0.043503,
    0.043575,
    0.043647,
    0.043719,
    0.043791,
    0.043863,
    0.043934,
    0.044006,
    0.044077,
    0.044148,
    0.044219,
    0.044290,
    0.044361,
    0.044432,
    0.044503,
    0.044573,
    0.044644,
    0.044714,
    0.044784,
    0.044854,
    0.044924,
    0.044994,
    0.045064
};

uint16_t height = 0;            ///< Variavel usada para guardar a medicao de altura
uint16_t avg_height = 0;        ///< Variavel usada para guardar a media movel de quatro medicoes de altura
uint16_t tempo_voo = 0;         ///< Variavel usada para guardar o tempo de voo medido
float avg_tempo_voo = 0;     ///< Variavel usada para guardar a media movel de quatro medicoes do tempo de voo 


//Movimento do motor de passo
uint16_t position = 0;
uint16_t sp_position = 0;       ///< Setpoint para posicao da valvula (motor de passo)
uint8_t passo;
bool fim_curso;
bool passo_ctrl = false;

struct {
    uint8_t sos : 2; //Esta � a vari�vel que se incrementa para determinar a configura��o das bobinas.
}pas;                //Ela tem 2 bits para contar de 0 ate 3 (sao 4 combinacoes diferentes de bobina)

//Sensor de Temperatura
uint16_t adc_temp;

/*Funcoes*/

//Controle PID
//void fluxcontrol();
//void pwmcontrol();
//void fluxpos();
//void controlchoose();

// ---------------------------------------------------------- Comunicacao Serial -------------------------------------------------------------------------------------- 
//MACROS---------------------------------
#define BUFFER_MAX 8        ///< Maxima quantidade de bytes a serem recebidos.
#define RX_CMD_MNL 0x00     ///< Comando para funcionamento manual
#define RX_CMD_VENT 0x01    ///< Comando para funcionamento controlando ventoinha
#define RX_CMD_VAL 0x02     ///< Comando para funcionamento controlando valvula
#define RX_CMD_RST 0x03     ///< Comando para reset
#define RX_CMD_SZ 7         ///< Comprimento das mensagens de comando

//Variaveis------------------------------
union{                  ///< Uniao para transmissao
    uint16_t v;         ///< Valor inteiro de 16 bits para transmissao
    struct{
        uint8_t vL;     ///< LSB do valor a ser transmitido
        uint8_t vH;     ///< MSB do valor a ser transmitido 
    };
}vTx;                   ///< Variavel com os valores usados na transmissao

union{                  ///< Uniao para recepcao de dados
    uint16_t v;         ///< Valor inteiro de 16 bits recebido
    struct{
        uint8_t vL;     ///< LSB do valor recebido
        uint8_t vH;     ///< MSB do valor recebido 
    };
}vRx;                   ///< Variavel com os valores recebidos

uint16_t sp_height = 0;             ///< Variavel usada para guardar o setpoint de altura.
uint8_t func_mode = 0;              ///< Variavel usada para indicar o modo de funcionamento. 0=manual, 1=ventoinha, 2=valvula.
uint16_t dc = 0;                    ///< Variavel usada para guardar o valor do pwm.
uint8_t bufferRx[BUFFER_MAX];       ///< Buffer de Rx.
uint8_t countRx = 0;                ///< Contador de bytes recebidos (ponteiro).
uint8_t count_Tx = 0;               ///< Contador usado para alcancar 100ms atraves dos 16ms do timer1.
bool nao_salva = false;             ///< Variavel booleana usada para indcar os 40ms do fim da recepcao e impedir que dados a mais sejam salvos.
bool first_read = true;

//Funcoes---------------------------------
/**
 * Analisa a mensagem recebida pelo EUSART e realiza o comando.
 */
void analisa_Rx ();

/**
 * Realiza o envio dos dados adquirdos pelo sistema. Acionada a cada 100ms.
 */
void envia_Tx ();




// -------------------------------------------- Motor de passos --------------------------------------------------------------------------- 

/**
 * Funcao de passo do motor
 * @param sentido variavel booleana que indica se o motor esta andando no sentido horario ou anti-horario
 */
 void daUmPasso(uint8_t sentido);
//
//
///**
// * 
// * @param passo
// * @param sentido
// */
void definePassoMotor(uint8_t passom, uint8_t sentido);
//
//// ------------------------------------------- Temperatura --------------------------------------------------------------
//
///**
// Fun��o de c�lculo da temperatura do sensor LM35
// */
//void calculaTemp();

// --------------------------------------------- Distancia ---------------------------------------------------------------
/**
 * Funcao para medicao de altura, assim como para definicao da media movel de altura e tempo de voo. Chamada na interrupcao do gate do timer1.
 */
void mede_height ();

#endif	/* MAIN_H */