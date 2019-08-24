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
#define _1	   (0<<3) + 0 
#define _2	   (1<<3) + 0 
#define _3	   (2<<3) + 0 
#define _4	   (3<<3) + 0 
#define _5	   (4<<3) + 0 
#define _6	   (4<<3) + 3 
#define _7	   (3<<3) + 3 
#define _8	   (2<<3) + 3 
#define _9	   (1<<3) + 3 
#define _0	   (0<<3) + 3 
#define _Q	   (0<<3) + 1 
#define _W	   (1<<3) + 1 
#define _E	   (2<<3) + 1 
#define _R	   (3<<3) + 1 
#define _T	   (4<<3) + 1 
#define _Y	   (4<<3) + 4 
#define _U	   (3<<3) + 4 
#define _I	   (2<<3) + 4 
#define _O	   (1<<3) + 4 
#define _P	   (0<<3) + 4 
#define _A	   (0<<3) + 2 
#define _S	   (1<<3) + 2 
#define _D	   (2<<3) + 2 
#define _F	   (3<<3) + 2 
#define _G	   (4<<3) + 2 
#define _H	   (4<<3) + 6 
#define _J	   (3<<3) + 6 
#define _K	   (2<<3) + 6 
#define _L	   (1<<3) + 6 
#define _ENT   (0<<3) + 6 
#define _CAPS  (0<<3) + 5  // Col 0 linha 5	
#define _Z	   (1<<3) + 5 	
#define _X	   (2<<3) + 5 	
#define _C	   (3<<3) + 5 	
#define _V	   (4<<3) + 5 	
#define _B	   (4<<3) + 7 	
#define _N	   (3<<3) + 7 	
#define _M	   (2<<3) + 7 	
#define _SYMB  (1<<3) + 7  // Col 1 linha 7	
#define _SP	   (0<<3) + 7 	
#define _NONE  (7<<3) + 7    

// ZX Spectrum Map flag bits
#define _CS 0x80
#define _SS 0x40

//
#define _SH_COL  0 
#define _SH_LINE 5

#define _EX_COL  1 
#define _EX_LINE 7