/*
   Biblioteca para dispositivos com protocolo PS/2
   Baseada na biblioteca Arduino 'PS2' da autoria de Chris J. Kiick
   Daniel Jose Viana - Junho de 2015 - danjovic@hotmail.com
   Compilador: CCS C versão 4
   
*/

//
void ps2_write(unsigned char data); // Envia um byte para o dispositivo 
//
int8 ps2_read(void);// Recebe um byte do dispositivo
//
int1 read_SCL(void);  // Libera linha de Clock e retorna estado da linha
//
int1 read_SDA(void);  // Libera linha de Dados e retorna estado da linha 
//
void clear_SCL(void); // Força nível baixo na linha de Clock
//
void clear_SDA(void); // Força nível baixo na linha de Dados

#define SCL_PIN  PS2_Clk 
#define SDA_PIN  PS2_Dat 

#include "myPS2.c"