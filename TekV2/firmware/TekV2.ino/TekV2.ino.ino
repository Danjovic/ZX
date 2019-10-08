/*
   _____    _         _  __          _                         _
  |_   _|__| | __    | |/ /___ _   _| |__   ___   __ _ _ __ __| |
    | |/ _ \ |/ /____| ' // _ \ | | | '_ \ / _ \ / _` | '__/ _` |
    | |  __/   <_____| . \  __/ |_| | |_) | (_) | (_| | | | (_| |
    |_|\___|_|\_\    |_|\_\___|\__, |_.__/ \___/ \__,_|_|  \__,_|
                               |___/
   _____                 _       _              __     ______
  | ____|_ __ ___  _   _| | __ _| |_ ___  _ __  \ \   / /___ \
  |  _| | '_ ` _ \| | | | |/ _` | __/ _ \| '__|  \ \ / /  __) |
  | |___| | | | | | |_| | | (_| | || (_) | |      \ V /  / __/
  |_____|_| |_| |_|\__,_|_|\__,_|\__\___/|_|       \_/  |_____|


              Daniel José Viana, August 5 2019


   Tek is a ZX Spectrum keyboard emulator based on an AVR microcontroller

   It works by converting the PS/2 keystrokes into a bitmap representation
   of the keyboard matrix and updating its outputs with the corresponding
   keys pressed depending on the state of lines A8..A15 each time the
   the Z80 reads the port 0xFE

   The circuit is heavilly based on Arduino Nano V3 but any Arduino or
   AVR chip with enough pins and flash should do it. 
   Note: It is necessary to use a flip-flop (74xx74) to hold the wait state.

Versioning:
-October 2, 2019 
 Alfa release (functional)
-October 7, 2019 
 Added option to generate code to ZX81 and Speccy 




*/

/*
   ___ _               _
  | _ (_)_ _  ___ _  _| |_
  |  _/ | ' \/ _ \ || |  _|
  |_| |_|_||_\___/\_,_|\__|


                        +--___--+
                RESET --|1    28|-- PC5 RELEASE Z80
               RX PD0 --|2    27|-- PC4 KB4
               TX PD1 --|3    26|-- PC3 KB3
      INT0 /ULARD PD2 --|4    25|-- PC2 KB2
          Z80 A15 PD3 --|5    24|-- PC1 KB1
          Z80 A14 PD4 --|6    23|-- PC0 KB0
                  VCC --|7    22|-- GND
                  GND --|8    21|-- AREF
            XTAL1 PB6 --|9    20|-- AVCC
            XTAL2 PB7 --|10   19|-- PB5 ENABLE 
          Z80 A8  PD5 --|11   18|-- PB4 PS2 CLK
          Z80 A13 PD6 --|12   17|-- PB3 PS2 DAT
          Z80 A9  PD7 --|13   16|-- PB2 Z80 A11
          Z80 A12 PB0 --|14   15|-- PB1 Z80 A10
                        +-------+

*/

#include <ps2.h> //  PS2 library by Chris J. Kiick

/*
   ___       __ _      _ _   _
  |   \ ___ / _(_)_ _ (_) |_(_)___ _ _  ___
  | |) / -_)  _| | ' \| |  _| / _ \ ' \(_-<
  |___/\___|_| |_|_||_|_|\__|_\___/_||_/__/

*/


#define DEBUG

#define Pin_PS2_DAT 11  //  same as MOSI
#define Pin_PS2_CLK 12  //  same as MISO

#define Keymap_Size 8  // 8 characters for ZX Spectrum



#define ZX81       81
#define ZXSPECTRUM 82

#define ZXMODEL ZX81
//#define ZXMODEL ZXSPECTRUM

#define _SH 0x80  // _SH_COL e _SH_LINE devem ser definidos no arquivo de configuração da matriz do teclado 
#define _EX 0x40  // _EX_COL e _EX_LINE devem ser definidos no arquivo de configuração da matriz do teclado  


/*
    ___             _            _
   / __|___ _ _  __| |_ __ _ _ _| |_ ___
  | (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
   \___\___/_||_/__/\__\__,_|_||_\__/__/

*/


#ifndef ZXMODEL
#define ZXMODEL ZXSPECTRUM
#endif

#if ZXMODEL == ZX81
#include "ZX81keyboardMatrixMap.h"
#include "ZX81scancodesToKeycodes.h"

#elif ZXMODEL == ZXSPECTRUM
#include "ZXspectrumKeyboadMatrixMap.h"
#include "ZXspectrumScancodesToKeycodes.h"

#endif


/*
  __   __        _      _    _
  \ \ / /_ _ _ _(_)__ _| |__| |___ ___
   \ V / _` | '_| / _` | '_ \ / -_|_-<
    \_/\__,_|_| |_\__,_|_.__/_\___/__/

*/


PS2 kbd(Pin_PS2_CLK, Pin_PS2_DAT);
volatile char Keymap[Keymap_Size] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
bool EXT = false;
bool BRK = false;

/*
   ___             _   _
  | __|  _ _ _  __| |_(_)___ _ _  ___
  | _| || | ' \/ _|  _| / _ \ ' \(_-<
  |_| \_,_|_||_\__|\__|_\___/_||_/__/

*/

//
// Initialize Keyboard
//
bool kbd_init()
{
  char ack;
#ifdef DEBUG
  Serial.print("\nInit KBD:");
#endif
  kbd.write(0xff);  // send reset code
  ack = kbd.read();  // byte, kbd does self test
#ifdef DEBUG
  Serial.print("[");
  Serial.print(ack,HEX);
  Serial.print("]");    
#endif
  ack = kbd.read();  // another ack when self test is done
#ifdef DEBUG
  Serial.print("[");
  Serial.print(ack,HEX);
  Serial.print("]");    
#endif
  return true;
}





//
// Clear the Keyboard Matrix
//
void Clear_Matrix()
{
  uint8_t i;
  for (i = 0; i < Keymap_Size; i++) // clear keymap
    Keymap[i] = 0xff;
}


//
// Update the Keyboard Matrix based on a map code for a row and a line
//
/*
   ZX spectrum keyboard matrix map codes
   bit  7   6   5   4   3   2   1   0
        CS  SS  r2  r1  r0  l2  l1  l0

   r0..r2 is row [0..4]   mapping bits D0 to D4
   l0..l2 is line [0..7]  mapping address lines A8 to A15

   CS Caps Shift for composed keys (like directional keys)
   SS Symbol Shift for composed keys

*/


//
// Atualiza matriz do teclado baseado num mapa de linha e coluna
/*
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
	

}
*/
void Update_matrix( char m) {
  uint8_t line = m & 0x07;
  uint8_t row =  (m >> 3) & 0x07;

  if (BRK == true) {
    BRK = false;
    Keymap[line] |= (1 << row);      //  set bits to break
    if (m & 0x80) Keymap[0] |= (1 << 0); // if bit 7 is set then set Caps Shift bit at row=0, line=0
    if (m & 0x40) Keymap[7] |= (1 << 1); // if bit 6 is set then set Symbol Shift bit at row=1, line=7
  } else {
    Keymap[line] &= ~(1 << row);      // reset bits to make
    if (m & 0x80) Keymap[0] &= ~(1 << 0); // if bit 7 is set then reset Caps Shift bit at row=0, line=0
    if (m & 0x40) Keymap[7] &= ~(1 << 1); // if bit 6 is set then reset Symbol Shift bit at row=1, line=7
  }
#ifdef DEBUG
  // debug
  Serial.println ("\n76543210");
  for (line = 0; line < 8; line++) {
    Serial.print((uint8_t)Keymap[line], BIN);
    Serial.print(" ");
    Serial.println(line);
  }
#endif
}

/*
   Check the state of each line A8..A15 then add its active buttons
   The ZX bios always activates one line at a time but some games can
   activate more than one to quick ckeck for any key pressed
*/

ISR (INT0_vect, ISR_NAKED) {

  asm volatile (

    "in __tmp_reg__,__SREG__ \n\t"   // Salva registrador de Status

    "push r17\n\t"                   // keep PINB state
    "push r18\n\t"                   // keep PIND state
    "push r19\n\t"                   // temp Data to write on Output
    "push r20\n\t"                   // register to read keymap
    "push r26\n\t"                   // Pointer to Keymap
    "push r27\n\t"                   //

    "ldi r26,lo8(Keymap) \n\t"       // X register [r27:26] point to Keymap
    "ldi r27,hi8(Keymap)\n\t"        //

    "in r17,%0 \n\t"                 // Sample Port B  (x.x.x.x.x.A11.A10.A9)
    "in r18,%1 \n\t"                 // Sample Port D  (A12.A13.A8.A14.A15.x.x.x)
    "ldi r19,0x1f \n\t"              // Initial mask => bits 0..5 unactive (high)


    "ld r20,X+ \n\t"                 // R20 = Keymap[0]
    "sbrs R18,5 \n\t"                // Bit D.5 = 0 ? ->A8
    "and r19,r20 \n\t"               // yes, copy active bits from Keymap[0] to R19

    "ld r20,X+ \n\t"                 // R20 = Keymap[1]
    "sbrs R17,0 \n\t"                // Bit B.0 = 0 ? ->A9
    "and r19,r20 \n\t"               // yes, copy active bits from Keymap[1] to R19

    "ld r20,X+ \n\t"                 // R20 = Keymap[2]
    "sbrs R17,1 \n\t"                // Bit B.1 = 0 ? ->A10
    "and r19,r20 \n\t"               // yes, copy active bits from Keymap[2] to R19

    "ld r20,X+ \n\t"                 // R20 = Keymap[3]
    "sbrs R17,2 \n\t"                // Bit B.2 = 0 ? ->A11
    "and r19,r20 \n\t"               // yes, copy active bits from Keymap[3] to R19

    "ld r20,X+ \n\t"                 // R20 = Keymap[4]
    "sbrs R18,7 \n\t"                // Bit D.7 = 0 ? ->A12
    "and r19,r20 \n\t"               // yes, copy active bits from Keymap[4] to R19

    "ld r20,X+ \n\t"                 // R20 = Keymap[5]
    "sbrs R18,6 \n\t"                // Bit D.6 = 0 ? ->A13
    "and r19,r20 \n\t"               // yes, copy active bits from Keymap[5] to R19

    "ld r20,X+ \n\t"                 // R20 = Keymap[6]
    "sbrs R18,4 \n\t"                // Bit D.4 = 0 ? ->A14
    "and r19,r20 \n\t"               // yes, copy active bits from Keymap[6] to R19

    "ld r20,X+ \n\t"                 // R20 = Keymap[7]
    "sbrs R18,3 \n\t"                // Bit D.3 = 0 ? ->A15
    "and r19,r20 \n\t"               // yes, copy active bits from Keymap[7] to R19


    "out %2,r19 \n\t"                // Write bits D0-D4 to the outputs

    "sbi %3,5 \n\t"                  // release !Wait line
    "cbi %2,5 \n\t"                  // release !Wait line
    "nop \n\t"
    "nop \n\t"
    "sbi %2,5 \n\t"                  // pulse !Wait line high briefly
    "cbi %3,5 \n\t"                  // to release Z80 from Wait State
 
    "pop r27\n\t"
    "pop r26\n\t"
    "pop r20\n\t"
    "pop r19\n\t"
    "pop r18\n\t"
    "pop r17\n\t"

    "out __SREG__,__tmp_reg__ \n\t"  // restore Status register
    "reti \n\t"
    :: "I" (_SFR_IO_ADDR(PINB)), "I" (_SFR_IO_ADDR(PIND)) , "I" (_SFR_IO_ADDR(PORTC)), "I" (_SFR_IO_ADDR(DDRC)) );


}


// ***************************************************************************
/*
     _          _      _             ___     _
    /_\  _ _ __| |_  _(_)_ _  ___   / __|___| |_ _  _ _ __
   / _ \| '_/ _` | || | | ' \/ _ \  \__ Y -_)  _| || | '_ \
  /_/ \_\_| \__,_|\_,_|_|_||_\___/  |___|___|\__|\_,_| .__/
                                                   |_|
*/
void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
  Serial.print("\n\rInit\n\r");
#endif

  DDRD = 0x00;   // all pins from port D as inputs
  PORTD = 0xfb;  // port D pullups active, except that one from ULA RD

  DDRB = 0x00 ;  // all pins from port B as inputs
  PORTB = 0xff ; // port B pullups  active

  DDRC = 0x3F ;  // all pins from port C as outputs
  PORTC = 0x3F;  // initialize all pins from PORT C in HIGH level //

  
  pinMode(13, INPUT); // 
  Clear_Matrix();

  // Initialize PS/2 keyboard
  kbd_init();

#ifdef DEBUG
  Serial.print("\n\rInit Keyboard \n\r");
#endif
  // Initialize ineterrupts
   EIMSK |= (1 << INT0); //Enable INT0
   EICRA |= (1 << ISC01); //Trigger on falling edge of INT0
   sei();

}


/*
   __  __      _        _
  |  \/  |__ _(_)_ _   | |   ___ ___ _ __
  | |\/| / _` | | ' \  | |__/ _ Y _ \ '_ \
  |_|  |_\__,_|_|_||_| |____\___|___/ .__/
                                  |_|
*/
void loop()
{
  unsigned char code;
  unsigned char m;

  // pin 13 shall be at HIGH to enable Wait States to Z80
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
 
   // Loop principal de leitura do teclado PS/2 e atualização da matriz 
  for (;;) {
	  code=kbd.read(); 
	 
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
//			Atualiza_Matriz(m); // usa o código recebido (e o estado de BRK) para atualizar a matriz 
            Update_matrix(m); // usa o keymap (e o estado de BRK) para atualizar a matriz			
		}
  } // loop principal 
 
 
  
/* 
  for (;;) { // ever
    //  Get a keycode from the keyboard and convert and update Keyboard Map  
    code = kbd.read();


    if (code == 0xE0) {
      EXT = true;
    } else if (code == 0xF0) {
      BRK = true;
    } else {
      if (EXT == true) { // extended set
        EXT = false;
        switch (code) {
          case _PS2_UP:   	m = 0x80 | _7; break; // Caps Shift bit + map code for ZX key 7
          case _PS2_DOWN: 	m = 0x80 | _6; break;
          case _PS2_LEFT: 	m = 0x80 | _5; break;
          case _PS2_RIGHT:	m = 0x80 | _8; break;
          case _PS2_KPENT:	m = _ENT     ; break; // Enter key
          case _PS2_LCONTROL:	m = _SYMB    ; break; // Enter key
          default:                m = _NONE;
        }

      } else { // normal set
        if (code < 128) {
          m = pgm_read_byte(PS2Keymap + code); // Associa o Scancode ao keymaps
        } else m = _NONE;
      }

      // todo: test if valid code
      Update_matrix(m); // usa o keymap (e o estado de BRK) para atualizar a matriz
    }
  }
*/ 
}
