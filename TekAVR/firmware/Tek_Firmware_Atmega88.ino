/*
    _____    _         _  __          _                         _
   |_   _|__| | __    | |/ /___ _   _| |__   ___   __ _ _ __ __| |
     | |/ _ \ |/ /____| ' // _ \ | | | '_ \ / _ \ / _` | '__/ _` |
     | |  __/   <_____| . \  __/ |_| | |_) | (_) | (_| | | | (_| |
     |_|\___|_|\_\    |_|\_\___|\__, |_.__/ \___/ \__,_|_|  \__,_|
       _____                 _  |___/_
      | ____|_ __ ___  _   _| | __ _| |_ ___  _ __
      |  _| | '_ ` _ \| | | | |/ _` | __/ _ \| '__|
      | |___| | | | | | |_| | | (_| | || (_) | |
      |_____|_| |_| |_|\__,_|_|\__,_|\__\___/|_|

                ZX Spectrum version
  
              Daniel José Viana, 2014
 
 * Tek is a ZX Spectrum keyboard emulator based on an AVR microcontroller
 * and some glue logic chips.
 * 
 * It works by converting the PS/2 keystrokes into a bitmap representation
 * of the keyboard matrix and updating its outputs with the corresponding
 * keys pressed depending on the state of lines A8..A15 each time the 
 * the Z80 reads the port 0xFE
 *
 * The circuit is heavilly based on ATMega88 but any Arduino with enough
 * pins should do it. 
 
 */
 
 /*
    ___ _               _
   | _ (_)_ _  ___ _  _| |_
   |  _/ | ' \/ _ \ || |  _|
   |_| |_|_||_\___/\_,_|\__|

 * 
 *                       +--___--+ 
 *               RESET --|1    28|-- PC5 RELEASE to WAIT generator
 *              RX PD0 --|2    27|-- PC4 DD0 to LS367
 *              TX PD1 --|3    26|-- PC3 DD1 to LS367  
 *     INT0 /ULARD PD2 --|4    25|-- PC2 DD2 to LS367 
 *     INT1 PS2CLK PD3 --|5    24|-- PC1 DD3 to LS367    
 *          PS2DAT PD4 --|6    23|-- PC0 DD4 to LS367         
 *     Shift Clock VCC --|7    22|-- GND            
 *                 GND --|8    21|-- AREF         
 *           XTAL1 PB6 --|9    20|-- AVCC       
 *           XTAL2 PB7 --|10   19|-- PB5 Z80 A13                    
 *        not used PD5 --|11   18|-- PB4 Z80 A12       
 *         Z80 A14 PD6 --|12   17|-- PB3 Z80 A11      
 *         Z80 A15 PD7 --|13   16|-- PB2 Z80 A10        
 *         Z80 A8  PB0 --|14   15|-- PB1 Z80 A9                 
 *                       +-------+  
 
 */


#include <ps2.h>

/*
 ___       __ _      _ _   _
|   \ ___ / _(_)_ _ (_) |_(_)___ _ _  ___
| |) / -_)  _| | ' \| |  _| / _ \ ' \(_-<
|___/\___|_| |_|_||_|_|\__|_\___/_||_/__/

*/
#define Pin_PS2_DAT 4  // PD4 is Arduino Digital 4
#define Pin_PS2_CLK 3  // PD3 is Arduino Digital 3
//#define Pin_RELEASE 19 // PC5 is Arduino Digital 19 (Analog 5) 

const int Pin_RELEASE=19;

#define Pin_Z80_A8  8  // PB0 is Arduino Digital 8
#define Pin_Z80_A9  9  // PB1 is Arduino Digital 9
#define Pin_Z80_A10 10 // PB2 is Arduino Digital 10
#define Pin_Z80_A11 11 // PB3 is Arduino Digital 11
#define Pin_Z80_A12 12 // PB4 is Arduino Digital 12
#define Pin_Z80_A13 13 // PB5 is Arduino Digital 13
#define Pin_Z80_A14 6  // PD6 is Arduino Digital 6
#define Pin_Z80_A15 7  // PD7 is Arduino Digital 7



#define Keymap_Size 8  // 8 characters for ZX Spectrum

// PC Extended Scan Codes
#define _PS2_UP 0x75
#define _PS2_DOWN 0x72
#define _PS2_LEFT 0x6B
#define _PS2_RIGHT 0x74
#define _PS2_KPENT 0x5A
#define _PS2_LCONTROL 0x14 

// ZX Matrix map codes
#define _1	 0x03
#define _2	 0x0B
#define _3	 0x13
#define _4	 0x1B
#define _5	 0x23
#define _6	 0x24
#define _7	 0x1C
#define _8	 0x14
#define _9	 0x0C
#define _0	 0x04
#define _Q	 0x02
#define _W	 0x0A
#define _E	 0x12
#define _R	 0x1A
#define _T	 0x22
#define _Y	 0x25
#define _U	 0x1D
#define _I	 0x15
#define _O	 0x0D
#define _P	 0x05
#define _A	 0x01
#define _S	 0x09
#define _D	 0x11
#define _F	 0x19
#define _G	 0x21
#define _H	 0x26
#define _J	 0x1E
#define _K	 0x16
#define _L	 0x0E
#define _ENT	0x06
#define _CAPS	 0x00	
#define _Z	 0x08	
#define _X	 0x10	
#define _C	 0x18	
#define _V	 0x20	
#define _B	 0x27	
#define _N	 0x1F	
#define _M	 0x17	
#define _SYMB	 0x0F	
#define _SP	 0x07	
#define _NONE 0x30 // No key  

// ZX Map flag bits
#define _CS 0x80
#define _SS 0x40


/*
  ___             _            _
 / __|___ _ _  __| |_ __ _ _ _| |_ ___
| (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
 \___\___/_||_/__/\__\__,_|_||_\__/__/

*/
const uint8_t PS2Keymap[] PROGMEM = { 
_NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _CS+_SS,_NONE,  _NONE, 
_NONE,  _NONE,  _CAPS,  _NONE,  _SYMB,  _Q,     _1,     _NONE,  _NONE,  _NONE,  _Z,     _S,     _A,     _W,     _2,     _NONE,
_NONE,  _C,     _X,     _D,     _E,     _4,     _3,     _NONE,  _NONE,  _SP,    _V,     _F,     _T,     _R,     _5,     _NONE,
_NONE,  _N,     _B,     _H,     _G,     _Y,     _6,     _NONE,  _NONE,  _NONE,  _M,     _J,     _U,     _7,     _8,     _NONE, 
_NONE,  _NONE,  _K,     _I,     _O,     _0,     _9,     _NONE,  _NONE,  _NONE,  _NONE,  _L,     _NONE,  _P,     _NONE,  _NONE,  
_NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _CS+_2, _NONE,  _ENT,   _NONE,  _NONE,  _NONE,  _NONE,  _NONE, 
_NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _NONE,  _CS+_0, _NONE,  _NONE,  _1,     _NONE,  _4,     _7,     _NONE,  _NONE,  _NONE, 
_0,     _SS+_M, _2,     _5,     _6,     _8,     _CS+_SP,_NONE,  _NONE,  _SS+_K, _3,     _SS+_J, _SS+_B, _9,     _NONE,  _NONE  };



 
/*
__   __        _      _    _
\ \ / /_ _ _ _(_)__ _| |__| |___ ___
 \ V / _` | '_| / _` | '_ \ / -_|_-<
  \_/\__,_|_| |_\__,_|_.__/_\___/__/

*/
PS2 kbd(Pin_PS2_CLK, Pin_PS2_DAT);
char Keymap[Keymap_Size] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
bool EXT=false;
bool BRK=false;



/*
 ___             _   _
| __|  _ _ _  __| |_(_)___ _ _  ___
| _| || | ' \/ _|  _| / _ \ ' \(_-<
|_| \_,_|_||_\__|\__|_\___/_||_/__/

*/

//
// Initialize Keyboard
//
void kbd_init()
{
  char ack;

  kbd.write(0xff);  // send reset code
  ack = kbd.read();  // byte, kbd does self test
  ack = kbd.read();  // another ack when self test is done
}



//
// Generate a RELEASE pulse for unlock Z80 from Wait State
//

void Pulse_RELEASE()
{
  digitalWrite(Pin_RELEASE, LOW);
  asm ("nop");  
  asm ("nop");  
  asm ("nop");  
  asm ("nop");  
  digitalWrite(Pin_RELEASE, HIGH); 
}




//
// Clear the Keyboard Matrix 
//
void Clear_Matrix() 
{
  uint8_t i;
  for (i=0;i<Keymap_Size;i++)    // clear keymap  
     Keymap[i]=0xff;
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
void Update_matrix( char m){
	uint8_t line= m & 0x07;
	uint8_t row=  (m>>3) & 0x07;
	
	if (BRK==true) { 
		BRK=false;
		Keymap[line]|=(1<<row);          //  set bits to break
		if (m & 0x80) Keymap[0]|=(1<<0); // if bit 7 is set then set Caps Shift bit at row=0, line=0
		if (m & 0x40) Keymap[7]|=(1<<1); // if bit 6 is set then set Symbol Shift bit at row=1, line=7			
	} else {                              
		Keymap[line]&=~(1<<row);          // reset bits to make
		if (m & 0x80) Keymap[0]&=~(1<<0); // if bit 7 is set then reset Caps Shift bit at row=0, line=0
		if (m & 0x40) Keymap[7]&=~(1<<1); // if bit 6 is set then reset Symbol Shift bit at row=1, line=7	
	} 

        // debug
        Serial.println ("\n76543210");
        for (line=0;line<8;line++) { 
           Serial.print((uint8_t)Keymap[line],BIN);
           Serial.print(" ");
           Serial.println(line);
        }
}




//
// Update the state of the outputs according to the state of the Keyboard Matrix
//

/*
   Check the state of each line A8..A15 then add its active buttons
   The ZX bios always activates one line at a time but some games can
   activate more than one to quick ckeck for any key pressed
*/

void Update_Outputs(void) {

  

  
  asm volatile (     
   "ldi r18,0x1f \n\t"              // Mascara inicial => bits 0..5 em 1
   "out %2,r18 \n\t"                // Escreve resultado nos bits D0-D4 do LS365 e libera Wait State
  
   
   "in r19,%0 \n\t"                 // Amostra bits PB 0..5 (A8..A13)
   "in r19,%1 \n\t"                 // Amostra bits PD 6..7 (A14..A15)
 
   "ldi r18,0x1e \n\t"              // Mascara inicial => bits 0..5 em 1
 
   "out %2,r18 \n\t"                // Escreve resultado nos bits D0-D4 do LS365 e libera Wait State
    
   :: "I" (_SFR_IO_ADDR(PINB)), "I" (_SFR_IO_ADDR(PIND)) , "I" (_SFR_IO_ADDR(PORTC)) ); 
  
  digitalWrite(Pin_RELEASE, LOW);
  asm volatile ("nop");  
  digitalWrite(Pin_RELEASE, HIGH); 
  
}
  
  
  /*
ISR (INT0_vect, ISR_NAKED) {  
   
asm volatile (   
  "in __tmp_reg__,__SREG__ \n\t"   // Salva registrador de Status  
   
   "ldi r26,lo8(Keymap) \n\t"         // Ponteiro X = endereço de Keymap
   "ldi r27,hi8(Keymap)\n\t"          //
   
   "ldi r18,0x1f \n\t"              // Mascara inicial => bits 0..5 em 1
   
   "in r19,%0 \n\t"                 // Amostra bits PB 0..5 (A8..A13)
   
   "ld r20,X+ \n\t"                 // R20 = Keymap[0] 
   "sbrs R19,0 \n\t"                // Bit [0] = 0 ? 
   "and r18,r20 \n\t"               // sim, copia os bits '0' de Keymap[0] para R18
   
   "ld r20,X+ \n\t"                 // R20 = Keymap[1] 
   "sbrs R19,1 \n\t"                // Bit [1] = 0 ? 
   "and r18,r20 \n\t"               // sim, copia os bits '0' de Keymap[1] para R18   
   
   "ld r20,X+ \n\t"                 // R20 = Keymap[2] 
   "sbrs R19,2 \n\t"                // Bit [2] = 0 ? 
   "and r18,r20 \n\t"               // sim, copia os bits '0' de Keymap[2] para R18
   
   "ld r20,X+ \n\t"                 // R20 = Keymap[3] 
   "sbrs R19,3 \n\t"                // Bit [3] = 0 ? 
   "and r18,r20 \n\t"               // sim, copia os bits '0' de Keymap[3] para R18   

   "ld r20,X+ \n\t"                 // R20 = Keymap[4] 
   "sbrs R19,4 \n\t"                // Bit [4] = 0 ? 
   "and r18,r20 \n\t"               // sim, copia os bits '0' de Keymap[4] para R18   

   "ld r20,X+ \n\t"                 // R20 = Keymap[5] 
   "sbrs R19,5 \n\t"                // Bit [5] = 0 ? 
   "and r18,r20 \n\t"               // sim, copia os bits '0' de Keymap[5] para R18

   "in r19,%1 \n\t"                 // Amostra bits PD 6..7 (A14..A15)

   "ld r20,X+ \n\t"                 // R20 = Keymap[6] 
   "sbrs R19,4 \n\t"                // Bit [6] = 0 ? 
   "and r18,r20 \n\t"               // sim, copia os bits '0' de Keymap[6] para R18   

   "ld r20,X+ \n\t"                 // R20 = Keymap[7] 
   "sbrs R19,5 \n\t"                // Bit [7] = 0 ? 
   "and r18,r20 \n\t"               // sim, copia os bits '0' de Keymap[7] para R18
   
   "out %2,r18 \n\t"                // Escreve resultado nos bits D0-D4 do LS365 e libera Wait State
   "nop \n\t"
   "sbi %2,5 \n\t"                  // libera flip flop
   
   "out __SREG__,__tmp_reg__ \n\t"  // restaura registrador de Status
   "reti \n\t" 
   :: "I" (_SFR_IO_ADDR(PINB)), "I" (_SFR_IO_ADDR(PIND)) , "I" (_SFR_IO_ADDR(PORTC)) );

/*
  uint8_t i=0x1f;         // in binary 0001.1111  
  uint8_t j=PINC & 0xE0;  // in binary 1110.0000  keep the bits 6, 7, 8 
   
  if (digitalRead(Pin_Z80_A8 )==LOW) i &= Keymap[0]; 
  if (digitalRead(Pin_Z80_A9 )==LOW) i &= Keymap[1]; 
  if (digitalRead(Pin_Z80_A10)==LOW) i &= Keymap[2]; 
  if (digitalRead(Pin_Z80_A11)==LOW) i &= Keymap[3]; 
  if (digitalRead(Pin_Z80_A12)==LOW) i &= Keymap[4]; 
  if (digitalRead(Pin_Z80_A13)==LOW) i &= Keymap[5]; 
  if (digitalRead(Pin_Z80_A14)==LOW) i &= Keymap[6]; 
  if (digitalRead(Pin_Z80_A15)==LOW) i &= Keymap[7]; 
  
  j |= i;  
  PORTC=j & 0xfe;
//  PORTC=j; // Update outputs
  
*/  


  
 
 /* 


*/


  //Pulse_RELEASE(); // release Z80 from Wait State
  //Serial.print('!');


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
  
  Serial.begin(9600);
  Serial.print("\n\rInit\n\r");
 
 
  Pulse_RELEASE();
  
  
/* 
  pinMode(Pin_Z80_A8 , INPUT);
  pinMode(Pin_Z80_A9 , INPUT); 
  pinMode(Pin_Z80_A10, INPUT); 
  pinMode(Pin_Z80_A11, INPUT); 
  pinMode(Pin_Z80_A12, INPUT); 
  pinMode(Pin_Z80_A13, INPUT);
 
  pinMode(Pin_Z80_A14, INPUT); 
  pinMode(Pin_Z80_A15, INPUT); 

 */
 // pinMode(Pin_Z80_A14, INPUT); 



  DDRD = 0x00;
  PORTD = 0x00;  
 
  DDRB = 0x00 ; // all pins from port B as inputs
  PORTB = 0x00 ; // port B pullups not active
  
  DDRC = 0x3F ; // all pins from port C as outputs 
  PORTC = 0x3F; // initialize all pins from PORT C in HIGH level
 
  pinMode(Pin_RELEASE, OUTPUT);
  //digitalWrite(Pin_RELEASE,HIGH);
  
  Pulse_RELEASE();
  Clear_Matrix(); 
  
  attachInterrupt(0, Update_Outputs, FALLING);
 // EIMSK |= _BV(INT0);  //Enable INT0
 // EICRA |= _BV(ISC01); //Trigger on falling edge of INT0
 // sei();
 
 
 kbd_init();
 
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
  
   
  
  
  for (;;) { /* ever */  
    /* Get a keycode from the keyboard and convert and update Keyboard Map */
    code = kbd.read();
    
    if (code==0xE0) { 
		EXT=true;
	} else if (code==0xF0) {
		BRK=true;
	} else {
		if (EXT==true) { // extended set
			EXT=false;
			switch (code) {
				case _PS2_UP:   	m=0x80 | _7; break; // Caps Shift bit + map code for ZX key 7
				case _PS2_DOWN: 	m=0x80 | _6; break;
				case _PS2_LEFT: 	m=0x80 | _5; break;
				case _PS2_RIGHT:	m=0x80 | _8; break;
				case _PS2_KPENT:	m=_ENT     ; break; // Enter key
				case _PS2_LCONTROL:	m=_SYMB    ; break; // Enter key
				default:                m=_NONE;
			}
		
		} else { // normal set
			if (code<128) {
				m=pgm_read_byte(PS2Keymap+code); // Associa o Scancode ao keymaps
			} else m=_NONE;
		}
		
		// todo: test if valid code	
		Update_matrix(m); // usa o keymap (e o estado de BRK) para atualizar a matriz 
	} 
  }
}



