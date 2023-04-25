/* 
   Tek - Emulador de teclado generico
   Versão para ZX-Spectrum/TK90
   Daniel Jose Viana, Junho de 2015 - danjovic@hotmail.com

*/


/*
   Mapa dos bits da matriz de teclado do ZXSpectrum
   bit  7   6   5   4   3   2   1   0
        CS  SS  c2  c1  c0  l2  l1  l0  

   c0..c2:  coluna [0..4]  mapa das linhas de dados    D0  D1  D2 D3  D4 
   l0..l2:  linha  [0..7]  mapa das linhas de endereco A11 A10 A9 A12 A13 A8 A14 A15

   CS:      flag indicador de CAPS SHIFT para teclas compostas (como setas direcionais)
   SS:      flag indicador de SYMBOL SHIFT
*/

// ZX Matrix map codes
//    Tecla   coluna  linha  
#define _KEY_1	   (uint8_t) ( (0<<3) + 0 )
#define _KEY_2	   (uint8_t) ( (1<<3) + 0 )
#define _KEY_3	   (uint8_t) ( (2<<3) + 0 )
#define _KEY_4	   (uint8_t) ( (3<<3) + 0 )
#define _KEY_5	   (uint8_t) ( (4<<3) + 0 )
#define _KEY_6	   (uint8_t) ( (4<<3) + 3 )
#define _KEY_7	   (uint8_t) ( (3<<3) + 3 )
#define _KEY_8	   (uint8_t) ( (2<<3) + 3 )
#define _KEY_9	   (uint8_t) ( (1<<3) + 3 )
#define _KEY_0	   (uint8_t) ( (0<<3) + 3 )
#define _KEY_Q	   (uint8_t) ( (0<<3) + 1 )
#define _KEY_W	   (uint8_t) ( (1<<3) + 1 )
#define _KEY_E	   (uint8_t) ( (2<<3) + 1 )
#define _KEY_R	   (uint8_t) ( (3<<3) + 1 )
#define _KEY_T	   (uint8_t) ( (4<<3) + 1 )
#define _KEY_Y	   (uint8_t) ( (4<<3) + 4 )
#define _KEY_U	   (uint8_t) ( (3<<3) + 4 )
#define _KEY_I	   (uint8_t) ( (2<<3) + 4 )
#define _KEY_O	   (uint8_t) ( (1<<3) + 4 )
#define _KEY_P	   (uint8_t) ( (0<<3) + 4 )
#define _KEY_A	   (uint8_t) ( (0<<3) + 2 )
#define _KEY_S	   (uint8_t) ( (1<<3) + 2 )
#define _KEY_D	   (uint8_t) ( (2<<3) + 2 )
#define _KEY_F	   (uint8_t) ( (3<<3) + 2 )
#define _KEY_G	   (uint8_t) ( (4<<3) + 2 )
#define _KEY_H	   (uint8_t) ( (4<<3) + 6 )
#define _KEY_J	   (uint8_t) ( (3<<3) + 6 )
#define _KEY_K	   (uint8_t) ( (2<<3) + 6 )
#define _KEY_L	   (uint8_t) ( (1<<3) + 6 )
#define _KEY_ENT   (uint8_t) ( (0<<3) + 6 )
#define _KEY_CAPS  (uint8_t) ( (0<<3) + 5 ) // Col 0 linha 5	
#define _KEY_Z	   (uint8_t) ( (1<<3) + 5 )	
#define _KEY_X	   (uint8_t) ( (2<<3) + 5 )	
#define _KEY_C	   (uint8_t) ( (3<<3) + 5 )	
#define _KEY_V	   (uint8_t) ( (4<<3) + 5 )	
#define _KEY_B	   (uint8_t) ( (4<<3) + 7 )	
#define _KEY_N	   (uint8_t) ( (3<<3) + 7 )	
#define _KEY_M	   (uint8_t) ( (2<<3) + 7 )	
#define _KEY_SYMB  (uint8_t) ( (1<<3) + 7 ) // Col 1 linha 7	
#define _KEY_SP	   (uint8_t) ( (0<<3) + 7 )	
#define _KEY_NONE  (uint8_t) ( (7<<3) + 7 )   

// ZX Spectrum Map flag bits
#define _CS (uint8_t)0x80
#define _SS (uint8_t)0x40

//
#define _SH_COL  (uint8_t)0 
#define _SH_LINE (uint8_t)5

#define _EX_COL  (uint8_t)1 
#define _EX_LINE (uint8_t)7