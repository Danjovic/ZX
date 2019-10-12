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

              Daniel José Viana, danjovic@hotmail.com

*/


/*
   Mapa dos bits da matriz de teclado do ZX81
   bit  7   6   5   4   3   2   1   0
        SH  -   c2  c1  c0  l2  l1  l0  

   c0..c2:  coluna [0..4]  mapa das linhas de dados    D0  D1  D2 D3  D4 
   l0..l2:  linha  [0..7]  mapa das linhas de endereco A11 A10 A9 A12 A13 A8 A14 A15

   SH:      flag indicador de SHIFT para teclas compostas (como setas direcionais)
 
*/

/*  ZX 81 keyboard matrix  
   
     D4   D3   D2   D1   D0
 A8  V    C    X    Z    SHFT
 A9  G    F    D    S    A
A10  T    R    E    W    Q
A11  5    4    3    2    1
A12  6    7    8    9    0
A13  Y    U    I    O    P
A14  H    J    K    L    ENT  
A15  B    N    M    .    SPACE

*/

//       KEY       COL     LINE 
#define _KEY_SH   (0<<3) + 0 // Shift
#define _KEY_Z	  (1<<3) + 0 	
#define _KEY_X	  (2<<3) + 0 	
#define _KEY_C	  (3<<3) + 0 	
#define _KEY_V	  (4<<3) + 0 	

#define _KEY_A	  (0<<3) + 1 
#define _KEY_S	  (1<<3) + 1 
#define _KEY_D	  (2<<3) + 1 
#define _KEY_F	  (3<<3) + 1 
#define _KEY_G	  (4<<3) + 1 

#define _KEY_Q	  (0<<3) + 2 
#define _KEY_W	  (1<<3) + 2 
#define _KEY_E	  (2<<3) + 2 
#define _KEY_R	  (3<<3) + 2 
#define _KEY_T	  (4<<3) + 2 
     
#define _KEY_1	  (0<<3) + 3 
#define _KEY_2	  (1<<3) + 3 
#define _KEY_3	  (2<<3) + 3 
#define _KEY_4	  (3<<3) + 3 
#define _KEY_5	  (4<<3) + 3 

#define _KEY_0	  (0<<3) + 4 
#define _KEY_9	  (1<<3) + 4 
#define _KEY_8	  (2<<3) + 4 
#define _KEY_7	  (3<<3) + 4 
#define _KEY_6	  (4<<3) + 4 

#define _KEY_P	  (0<<3) + 5 
#define _KEY_O	  (1<<3) + 5 
#define _KEY_I	  (2<<3) + 5 
#define _KEY_U	  (3<<3) + 5 
#define _KEY_Y	  (4<<3) + 5 

#define _KEY_NL   (0<<3) + 6 // New Line
#define _KEY_L	  (1<<3) + 6 
#define _KEY_K	  (2<<3) + 6 
#define _KEY_J	  (3<<3) + 6 
#define _KEY_H	  (4<<3) + 6 

#define _KEY_SP	  (0<<3) + 7 
#define _KEY_DOT  (1<<3) + 7 
#define _KEY_M	  (2<<3) + 7 
#define _KEY_N	  (3<<3) + 7 
#define _KEY_B	  (4<<3) + 7 

#define _KEY_NONE (7<<3) + 7   

// Flag da tecla de SHIFT
#define _SHIFT 0x80

#define _SH_COL  0 
#define _SH_LINE 0

#define _EX_COL  1 
#define _EX_LINE 7
