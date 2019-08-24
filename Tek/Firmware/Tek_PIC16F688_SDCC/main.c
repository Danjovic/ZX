/* 
   Tek - Emulador de teclado generico
   Versão para ZX-81/TK85
   Daniel Jose Viana, Junho de 2015 - danjovic@hotmail.com
  
   V0.7 - 02 de Junho de 2015 - Versão inicial 
   V0.71  06 de Junho de 2015 - Complemento da tabela de scancodes
   V0.72  07 de Junho de 2015 - Tratamento dos scancodes atraves de consulta de tabela
                                para permitir personalizacao das tabelas sem alteracao
                                no codigo do rotina principal
   V0.73  14 de Junho de 2015 - definicoes de linha e coluna para teclas Shift normal 
                                e estendida migraram para arquivos de configuracao
   V0.74  31 de Julho de 2015 - Modificado para compilacao no SDCC 3.5
   								
*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                         ///
///                                    CONFIGURAÇÃO DO CHIP                                                 ///
///                                                                                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <pic16f688.h>
#include <stdint.h>
#include <stdbool.h>

//#use delay(clock=8000000)
//#fuses INTRC_IO, NOPROTECT, NOBROWNOUT, NOWDT, PUT 
uint16_t __at _CONFIG configWord = _INTRC_OSC_NOCLKOUT & _CPD_OFF &  _CP_OFF & _WDT_OFF & _PWRTE_ON; // watchdog off

/*
   PIC16F688
                +--___--+ 
          +5V --|1    14|-- GND                          
      RCK RA5 --|2    13|-- RA0 PGD         
      SCK RA4 --|3    12|-- RA1 PGC           
    /MCLR RA3 --|4    11|-- RA2              
       RX RC5 --|5    10|-- RC0 PS2_Dat         
       TX RC4 --|6     9|-- RC1 PS2_Clk              
          RC3 --|7     8|-- RC2 DOUT       
                +-------+  
*/

// Shift Register Pins
#define RCK       RA5  
#define SCK       RA4
#define DOUT      RC2

#define TRIS_RCK  TRISA5  
#define TRIS_SCK  TRISA4
#define TRIS_DOUT TRISA2



// PS/2 Keyboard Pins
#define PIN_SDA  RC0
#define PIN_SCL  RC1

#define TRIS_SDA TRISC0
#define TRIS_SCL TRISC1




/*
   Mapa dos bits da matriz de teclado 
   bit  7   6   5   4   3   2   1   0
        SH  EX  c2  c1  c0  l2  l1  l0  

   c0..c2:  coluna [0..4]  mapa das linhas de dados    D0  D1  D2 D3  D4 
   l0..l2:  linha  [0..7]  mapa das linhas de endereco A11 A10 A9 A12 A13 A8 A14 A15

   SH:      flag indicador de SHIFT para teclas compostas (como setas direcionais)
   EX:      flag indicador de SHIFT extendido para teclas compostas
 
*/

#define _SH 0x80  // _SH_COL e _SH_LINE devem ser definidos no arquivo de configuração da matriz do teclado 
#define _EX 0x40  // _EX_COL e _EX_LINE devem ser definidos no arquivo de configuração da matriz do teclado  

//#define _SH_COL
//#define _SH_LINE

//#define _EX_COL
//#define _EX_LINE




///////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                         ///
///                                    MODULOS E DEFINICOES EXTERNOS                                        ///
///                                                                                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ZX81       81
#define ZXSPECTRUM 82

#define ZX_Keymap_Size 8  // 8 linhas para ZX81/ZX Spectrum

#ifndef ZXMODEL
#define ZXMODEL ZXSPECTRUM
#endif

#if ZXMODEL == ZX81
#include "ZX81_Keycodes.h"
#include "Scancodes_to_keycodes_ZX81.h"

#elif ZXMODEL == ZXSPECTRUM
#include "ZXSpectrum_Keycodes.h"
#include "Scancodes_to_keycodes_ZXSpectrum.h"

#endif


#include "myPS2.h"
#include "myDelay.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                         ///
///                                    DEFINIÇÃO DAS VARIAVEIS                                              ///
///                                                                                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////



uint8_t Keymap[ZX_Keymap_Size] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t EXT=false;
uint8_t BRK=false;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                         ///
///                                              FUNCOES                                                    ///
///                                                                                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


//
// Envia matriz keymap[] serialmente para registradores de deslocamento
void Envia_Matriz(void) {
unsigned char linha,coluna,temp;

	for (linha=0;linha<8;linha++) { 
		temp=Keymap[linha];

		// Envia Byte para registrador de deslocamento, lsb primeiro
		for (coluna=0;coluna<8;coluna++) {
			if ( temp & 0x01)
				DOUT=1;
			else 
				DOUT=0;
			temp>>=1;
			SCK=1;     // Pulsa SCK
			SCK=0;          
		}           
	}
	// Transfere dos registradores de deslocamento para os latches de saída
	RCK=1;  // Pulsa SCL
	RCK=0;
}


//
// Limpa  a matriz e os registradores de deslocamento (desativa todas as teclas)
void Limpa_Matriz() {
unsigned char i;

	// Limpa matriz, colocando todos os bits em nível alto
	for (i=0;i<ZX_Keymap_Size;i++)     
	 Keymap[i]=0xff;
	// Envia matriz para os registradores de deslocamento
	Envia_Matriz();
}


//
// Atualiza matriz do teclado baseado num mapa de linha e coluna
void Atualiza_Matriz( unsigned char m){
unsigned char linha,coluna;

    linha = m & 0x07;
	coluna =  (m>>3) & 0x07;
	
	if (BRK==true) { 
		BRK=false;
		Keymap[linha] |= (1<<coluna);         // liga bit para desativar tecla 
		if (m & _SH) Keymap[_SH_LINE] |=(1<<_SH_COL);  // se bit 7 estiver ligado desativa SHIFT na linha 5, coluna 0
		if (m & _EX) Keymap[_EX_LINE] |=(1<<_EX_COL);  // se bit 6 estiver ligado desativa EXTENDED SHIFT na linha 7, coluna 1
					
	} else {                             
		Keymap[linha] &= ~(1<<coluna);        // zera bit para ativar a tecla
		if (m & _SH) Keymap[_SH_LINE] &= ~(1<<_SH_COL);   // se bit 7 estiver ligado ativa SHIFT na linha 5, coluna 0	
		if (m & _EX) Keymap[_EX_LINE] &= ~(1<<_EX_COL);   // se bit 6 estiver ligado ativa EXTENDED SHIFT na linha 7, coluna 1
	}
	
    Envia_Matriz();                           // Atualiza registradores de deslocamento 
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                         ///
///                                         PROGRAMA PRINCIPAL                                              ///
///                                                                                                         ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main (void)
{
  uint8_t ack,code,m;

  // inicializa hardware
  OSCCON = (_IRCF2 | _IRCF1 | _IRCF0 | _SCS) ;   // 8MHZ, oscilador interno
  
  CMCON0 = ( _CM2 | _CM1 | _CM0); // comparadores analogicos desligados, pinos como I/O

   
  
  TRIS_RCK  = 0;  // Pinos dos registradores de deslocamento como saídas
  TRIS_SCK  = 0;
  TRIS_DOUT = 0;

  TRIS_SDA  = 1;  // Pinos do teclado PS/2 como entradas
  TRIS_SCL  = 1;
  
  // Inicializa variaveis
  Limpa_Matriz();
  
  // Inicializa teclado    
  ps2_write(0xff);      // Envia comando de RESET T
  ack=ps2_read();       // recebe ACK
  ack=ps2_read();       // recebe POST (TODO: Verificar codigo de erro)

  // Loop principal de leitura do teclado PS/2 e atualização da matriz 
  for (;;) {
	  code=ps2_read(); 
	 
	  if (code==0xE0) { 
			EXT=true;
		} else if (code==0xF0) {
			BRK=true;
		} else {
			if (EXT==true) { // Caracteres expandidos <E0> + <Scancode>
				EXT=false;
				if (code<128) {					  
					m=PS2Keymap_Ext_KEY_ABNT[code]; // Associa o Scancode extendido ao keymap
				} else m=_KEY_NONE;
			} else { // Caracteres normais <Scancode>
				if (code==0x83) code=0x7f; // substitui scancode da tecla F7 para encurtar tabela
				if (code<128) {
					m=PS2Keymap_KEY_ABNT[code]; // Associa o Scancode ao keymap
				} else m=_KEY_NONE;
			}		
			// TODO: checar se mapa da tecla é válido.	
			Atualiza_Matriz(m); // usa o código recebido (e o estado de BRK) para atualizar a matriz 
		}
  } // loop principal 
} // main

