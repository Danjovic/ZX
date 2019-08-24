/*
   Biblioteca para dispositivos com protocolo PS/2
   Baseada na biblioteca Arduino 'PS2' da autoria de Chris J. Kiick
   Daniel Jose Viana - Junho de 2015 - danjovic@hotmail.com
   Compilador: CCS C versão 4
   
*/

#include "myDelay.h"
//
// Envia um byte para o dispositivo 
void ps2_write(unsigned char data) {
	unsigned char i;
	unsigned char parity = 1;
	
	read_SDA();           		
	read_SCL();           		
	delay_us(255);        		 
	clear_SCL();          		
	delay_us(255);        		
	clear_SDA();          		
	delay_us(10);         		
	read_SCL();  // start bit	
	
	// Aguarda dispositivo tomar controle da linha de clock 
	while (	read_SCL() );	
	
	// pronto para enviar dados
	for (i=0; i < 8; i++)
	{
		if (data & 0x01)
		{
			read_SDA();
		} else {
			clear_SDA();
		}
		// aguarda transição de clock
		while (!read_SCL() );
		while  (read_SCL() );
		parity = parity ^ (data & 0x01);
		data = data >> 1;
	}
	// bit de paridade
	if (parity)
	{
		read_SDA();
	} else {
		clear_SDA();
	}
	// ack
	while (!read_SCL() );
	while  (read_SCL() );

	// bit de parada
	read_SDA();                
	delay_us(50);              
	while  (read_SCL() );

	// troca de modo
	while (  !(read_SCL() ) || !(read_SDA() ) );

	// trava a linha de clock para pausar envio de dados 
	clear_SCL();                 
}

//
// Recebe um byte do dispositivo
uint8_t ps2_read(void) {
	unsigned char data = 0x00;
	unsigned char i;
	unsigned char bit = 0x01;

	// start clock
	read_SCL();           		
	read_SDA();           		
	delay_us(50);        		
	while ( read_SCL() )       
		;
	delay_us(5);  	
	while ( !read_SCL() ); // recebe bit de start
	
	// Recebe bits de dados (Scancode)
	for (i=0; i < 8; i++)
	{
		while ( read_SCL() )         
			;
		if ( read_SDA() )            
		{
			data = data | bit;
		}
		while ( !read_SCL() )        
			;
		bit = bit << 1;
	}
	// Recebe bit de paridade (mas ignora).
	while ( read_SCL() );
	while ( !read_SCL() );
	
	// Recebe bit de stop
	while ( read_SCL() );
	while ( !read_SCL() );
	
	// trava a linha de clock para pausar envio de dados 
	clear_SCL();
	
	return data;
}

//
// Libera linha de Clock e retorna estado da linha
uint8_t read_SCL(void) 
{
  TRIS_SCL = 1; 
  return (PIN_SCL==0); 
}

//
// Libera linha de Dados e retorna estado da linha 
uint8_t read_SDA(void)  
{
  TRIS_SDA=1;      //set as output
  return (PIN_SDA==0); 
}

//
// Força nível baixo na linha de Clock
void clear_SCL(void)
{
	TRIS_SCL=0; //set as output
	PIN_SCL=0;
}

//
// Força nível baixo na linha de Dados
void clear_SDA(void)
{
	TRIS_SDA=0; //set as output
	PIN_SDA=0;
}