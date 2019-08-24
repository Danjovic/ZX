/*
   Biblioteca de delay para PIC com SDCC
   Baseada no artigo Programming a PIC on Linux Tutorial de Joshua Henderson
   http://www.digitalpeer.com/blog/programming-a-pic-on-linux-tutorial
   Daniel Jose Viana - Março de 2016 - danjovic@hotmail.com
   Compilador: SDCC
   
*/
/* If FOSC not defined consider internal Clock of 8 Mhz */
#ifndef FOSC
#define FOSC 8000000L
#endif

#define delay_us(x) {  uint8_t us;	\
      us = (x)*FOSC/36000000L - 4/9;		\
      while(--us != 0) continue; }

