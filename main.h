/* 
 * File:   main.h
 * Author: Equipe 1
 *
 * Created on August 27, 2024, 9:39 AM
 */

#ifndef MAIN_H
#define	MAIN_H
#include <xc.h> //include processor files - each processor file is guarded.


// ---------------------------------------- Controle PID ------------------------------------------------------------

// Variaveis ----------------------------

int24_t error,errorp;               ///<error atual e error passado
int24_t balldist,ballset,flux;      ///<distancia da bola pelo sensor, distancia da bola setada, fluxo recebido
int24_t kpf = 21;                   ///<ganho proporcional do fluxo
int24_t kdf = 109;                  ///<ganho derivativo do fluxo
int24_t kpp = 32;                    ///<ganho proporcional do pwm do motor
int24_t kip = 17;                   ///<ganho integrativo do pwm do motor
int24_t kdp = 15;                  ///<ganho derivativo do pwm do motor
int24_t outputsum;
uint16_t output;                    ///<saida do controle
uint8_t controlchoice = 0;          ///<escolha do tipo de controle
int24_t timecontrol = 4;            ///<recepcao do tempo do timer
uint8_t sentido;                    ///<definindo sentido do motor de passo
int24_t outpre;                     ///<output pre calculado no controle, o qual nao esta em ponto fixo

// Funcoes -----------------------------

/**
 * Algoritmo para controle por valvula. Saida utilizada para definir a posicao desejada do motor de passos.
 */
void fluxcontrol();

/**
 * Algoritmo para controle por ventoinha. Saida utilizada para definir o duty cycle do PWM.
 */
void pwmcontrol();

/**
 * Funcao utilizada para dar passos no motor da valvula com o objetivo de igualar a posicao atual com a posicao desejada.
 */
void fluxpos();

/**
 * Funcao utilizada para chamar um dos algoritmos de controle, dependendo do funcionamento atual. No funcionamento manual (00), a funcao n�o chama nenhum dos algoritmos e tudo eh definido manualmente.
 */
void controlchoose();

// ----------------------------------------------- Comunicacao Serial -------------------------------------------------------------------------------------- 
//MACROS---------------------------------
#define BUFFER_MAX 8        ///< Maxima quantidade de bytes a serem recebidos.
#define RX_CMD_MNL 0x00     ///< Comando para funcionamento manual
#define RX_CMD_VENT 0x01    ///< Comando para funcionamento controlando ventoinha
#define RX_CMD_VAL 0x02     ///< Comando para funcionamento controlando valvula
#define RX_CMD_RST 0x03     ///< Comando para reset
#define RX_CMD_SZ 7         ///< Comprimento das mensagens de comando

//Variaveis------------------------------
union{                      ///< Uniao para transmissao
    uint16_t v;             ///< Valor inteiro de 16 bits para transmissao
    struct{
        uint8_t vL;         ///< LSB do valor a ser transmitido
        uint8_t vH;         ///< MSB do valor a ser transmitido 
    };
}vTx;                       ///< Variavel com os valores usados na transmissao

union{                      ///< Uniao para recepcao de dados
    uint16_t v;             ///< Valor inteiro de 16 bits recebido
    struct{
        uint8_t vL;         ///< LSB do valor recebido
        uint8_t vH;         ///< MSB do valor recebido 
    };
}vRx;                       ///< Variavel com os valores recebidos

uint16_t sp_height = 0;             ///< Variavel usada para guardar o setpoint de altura em milmetros. 
uint8_t func_mode = 0;              ///< Variavel usada para indicar o modo de funcionamento. 0=manual, 1=ventoinha, 2=valvula.
uint16_t dc = 0;                    ///< Variavel usada para guardar o valor do pwm.
uint8_t bufferRx[BUFFER_MAX];       ///< Buffer de Rx.
uint8_t countRx = 0;                ///< Contador de bytes recebidos (ponteiro).
uint8_t count_Tx = 0;               ///< Contador usado para alcancar 100ms atraves dos 16ms do timer1 (16*6=98).
bool nao_salva = false;             ///< Variavel booleana usada para indcar os 40ms do fim da recepcao, impedir que dados a mais sejam salvos e chamar a funcao que analisa os dados recebidos.

//Funcoes---------------------------------
/**
 * Analisa a mensagem recebida pelo EUSART e realiza o comando. Aconada 40ms depois do ultimo byte recebido.
 */
void analisa_Rx ();

/**
 * Realiza o envio dos dados adquirdos pelo sistema. Acionada a cada 100ms.
 */
void envia_Tx ();


// -------------------------------------------- Motor de passos --------------------------------------------------------------------------- 

// MACROS -------------------------

#define ANTIHORARIO 0   ///< MACRO usada para definir o sentido de fechar a porta
#define HORARIO 1       ///< MACRO usada para definir o sentifo de abrir a porta

// Variaveis -----------------------------------------  

uint16_t position = 0;          ///< Variavel que guarda a posicao da valvula
uint16_t sp_position = 0;       ///< Setpoint para posicao da valvula (motor de passo)
uint8_t passo;                  ///< Variavel utilizada para o incremento do passo do motor
bool fim_curso;                 ///< Variavel booleana que indica se a porta chegou ao fim de curso (completamente aberta), ou nao
bool passo_ctrl = false;        ///< Variavel utilizada no while para fazer com que a funcao fluxpos (a qual chama a funcao daUmPasso) seja chamada a cada 8ms 

// Funcoes-------------------------------------------

/**
 * Funcao na qual ocorre os calculos necessarios para incremento de passo do motor e da posicao da porta
 * @param sentido: variavel booleana que indica se o motor esta andando no sentido horario ou anti-horario
 */
 void daUmPasso(uint8_t sentido);

/**
 * @param passom: variavel utilizada como parametro associada ao passo do motor
 * @param sentido: variavel utilizada como parametro para definir o sentido da porta (horario ou anti-horario)
 */
void definePassoMotor(uint8_t passom, uint8_t sentido);

/**
 * Funcao chamada no setup do codigo para encontrar o fim de curso e zerar a posicao. Para movimentar o motor de passo, eh utilizado o delay em software.
 */
void setaPorta();
// ------------------------------------------- Temperatura --------------------------------------------------------------

// Variaveis -----------------------

uint16_t adc_temp; ///< Variavel que recebe a conversao do ADC. Ela nao faz o uso de ganhos, pois utilizamos a tensao de referencia como sendo 1.024V, dessa forma, obtemos a temperatura com o ponto fixo de uma casa decimal

// --------------------------------------------- Distancia ---------------------------------------------------------------

// Variaveis ----------------------

uint16_t height = 0;            ///< Variavel usada para guardar a medicao de altura em milimetros
uint16_t tempo_voo = 0;         ///< Variavel usada para guardar o tempo de voo medido em unidades do timer1
float avg_tempo_voo = 0;        ///< Variavel usada para guardar a media movel de duas medicoes do tempo de voo em unidades do timer1
bool first_read = true;         ///< Variavel booleana que indica se a primeira medcao de altura para a media movel 

// Funcoes ------------------------

/**
 * Funcao para medicao de altura, assim como para definicao da media movel do tempo de voo. Chamada na interrupcao do gate do timer1.
 */
void mede_height ();

// Tabela de medicao de altura ----

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

#endif	/* MAIN_H */
