/*
   Biblioteca para dispositivos com protocolo PS/2
   Baseada na biblioteca Arduino 'PS2' da autoria de Chris J. Kiick
   Daniel Jose Viana - Junho de 2015 - danjovic@hotmail.com
   Compilador: SDCC versão 3.5
   
*/

#include <stdint.h>
//
void ps2_write(unsigned char data); // Envia um byte para o dispositivo 
//
uint8_t ps2_read(void);// Recebe um byte do dispositivo
//
uint8_t read_SCL(void);  // Libera linha de Clock e retorna estado da linha
//
uint8_t read_SDA(void);  // Libera linha de Dados e retorna estado da linha 
//
void clear_SCL(void); // Força nível baixo na linha de Clock
//
void clear_SDA(void); // Força nível baixo na linha de Dados


#include "myPS2.c"