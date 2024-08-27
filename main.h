/* 
 * File:   main.h
 * Author: Labsisco 06
 *
 * Created on August 27, 2024, 9:39 AM
 */

#ifndef MAIN_H
#define	MAIN_H
/*Macros*/



/*Variáveis de programa*/

//Controle PID
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

//Movimento do motor de passo
uint8_t position;

struct {
    uint8_t sos : 2; //Esta é a variável que se incrementa para determinar a configuração das bobinas.
}pas;                //Ela tem 2 bits para contar de 0 ate 3 (sao 4 combinacoes diferentes de bobina)


/*Funcoes*/

//Controle PID
void fluxcontrol();
void pwmcontrol();
void fluxpos();
void controlchoose();

//Movimento do motor de passo
void meioPasso(bool sentido);
void move(uint8_t n_passos, bool sentido);


#endif	/* MAIN_H */

