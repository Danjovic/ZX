/* 
   Tek - Emulador de teclado generico
   Versão para ZX-Spectrum/TK90
   Daniel Jose Viana, Junho de 2015 - danjovic@hotmail.com
   
   Tabelas de associacao dos Scancodes normais e extendidos com
   os keycodes (mapa de bits) para o ZXSpectrum
*/

//
// Associacao das teclas com Scancodes normais para teclados ABNT2
// Tabela foi reduzida para caber em 128 bytes. Caso seja necessario
// usar a tecla F7, considerar o scancode 0x7f em vez do scancode 
// nativo (0x83)
const unsigned char PS2Keymap_ABNT[] = { 
_NONE,         // 0x00   
_CS + _9,      // 0x01  F9   (GRAPHICS)
_NONE,         // 0x02  
_NONE,         // 0x03  F5
_CS + _3,      // 0x04  F3   (TRUE VIDEO)
_CS + _1,      // 0x05  F1   (EDIT)
_CS + _2,      // 0x06  F2   (CAPS LOCK)
_NONE,         // 0x07  F12           
_NONE,         // 0x08                
_NONE,         // 0x09  F10           
_NONE,         // 0x0A  F8            
_NONE,         // 0x0B  F6            
_CS + _4,      // 0x0C  F4   (INVERSE VIDEO)            
_CS + _SS,      // 0x0D  TAB  (EXTENDED MODE)          
_SS + _P,      // 0x0E  APOSTROPHE (ASPAS)
_NONE,         // 0x0F                
_NONE,         // 0x10                
_NONE,         // 0x11  L ALT         
_CS,           // 0x12  L SHFT (CAPS SHIFT)       
_SS,           // 0x13  L CTRL (SYMBOL SHIFT)       
_NONE,         // 0x14               
_Q,            // 0x15  Q             
_1,            // 0x16  1              
_NONE,         // 0x17                
_NONE,         // 0x18                
_NONE,         // 0x19                
_Z   ,         // 0x1A  Z             
_S   ,         // 0x1B  S             
_A   ,         // 0x1C  A             
_W   ,         // 0x1D  W             
_2   ,         // 0x1E  2             
_NONE,         // 0x1F                
_NONE,         // 0x20                
_C   ,         // 0x21  C             
_X   ,         // 0x22  X             
_D   ,         // 0x23  D             
_E   ,         // 0x24  E             
_4   ,         // 0x25  4             
_3   ,         // 0x26  3             
_NONE,         // 0x27                
_NONE,         // 0x28                
_SP,           // 0x29  SPACE         
_V ,           // 0x2A  V             
_F ,           // 0x2B  F             
_T ,           // 0x2C  T             
_R ,           // 0x2D  R             
_5 ,           // 0x2E  5             
_NONE,         // 0x2F                
_NONE,         // 0x30                
_N,            // 0x31  N             
_B,            // 0x32  B             
_H,            // 0x33  H             
_G,            // 0x34  G             
_Y,            // 0x35  Y             
_6,            // 0x36  6             
_NONE,         // 0x37                
_NONE,         // 0x38                
_NONE,         // 0x39                
_M,            // 0x3A  M             
_J,            // 0x3B  J             
_U,            // 0x3C  U             
_7,            // 0x3D  7             
_8,            // 0x3E  8             
_NONE,         // 0x3F                
_NONE,         // 0x40                
_SS + _N,      // 0x41  COMMA (SS + N) 
_K,            // 0x42  K             
_I,            // 0x43  I             
_O,            // 0x44  O             
_0,            // 0x45  0             
_9,            // 0x46  9             
_NONE,         // 0x47                
_NONE,         // 0x48                
_SS + _M,      // 0x49  DOT       (SS + M)
_SS + _O,      // 0x4A  SEMICOLON (SS + O)
_L   ,         // 0x4B  L             
_NONE,         // 0x4C  CCCEDIL    Ç
_P,            // 0x4D  P             
_SS + _J,      // 0x4E  MINUS      (SS + J)
_NONE,         // 0x4F  
_NONE,         // 0x50  
_SS + _V,      // 0x51  SLASH      (SS + V)
_NONE,         // 0x52  TILDE         ~
_NONE,         // 0x53  
_NONE,         // 0x54  ACUTE         `
_SS + _L,      // 0x55  EQUAL       (SS + L)
_NONE,         // 0x56  
_NONE,         // 0x57  
_CS + _2,      // 0x58  CAPS    (CAPS SHIFT + 2)
_CS,           // 0x59  R SHFT  (CAPS SHIFT)
_ENT,          // 0x5A  ENTER
_NONE,         // 0x5B  OPENBRACKET   [
_NONE,         // 0x5C  
_NONE,         // 0x5D  CLOSEBRACKET  ]
_NONE,         // 0x5E  
_NONE,         // 0x5F  
_NONE,         // 0x60  
_NONE,         // 0x61  BACKSLASH     
_NONE,         // 0x62  
_NONE,         // 0x63  
_NONE,         // 0x64  
_NONE,         // 0x65  
_CS + _0,      // 0x66  BKSP
_NONE,         // 0x67  
_NONE,         // 0x68  
_1,            // 0x69  KP1
_NONE,         // 0x6A  
_4,            // 0x6B  KP4
_7,            // 0x6C  KP7
_SS + _M,      // 0x6D  KPDOT  (SS + M) 
_NONE,         // 0x6E  
_NONE,         // 0x6F  
_0,            // 0x70  KP0
_SS + _N,      // 0x71  KPCOMMA (SS + N) 
_2,            // 0x72  KP2
_5,            // 0x73  KP5
_6,            // 0x74  KP6
_8,            // 0x75  KP8
_CS + _SP,     // 0x76  ESC  (CS + SPACE)
_NONE,         // 0x77  NUM
_NONE,         // 0x78  F11
_CS + _K,      // 0x79  KPPLUS (SS + K)
_3,            // 0x7A  KP3
_SS + _J,      // 0x7B  KPMINUS  (SS + J)
_SS + _B,      // 0x7C  KPTIMES  (SS + B)
_9,            // 0x7D  KP9
_NONE,         // 0x7E  SCROLL
_NONE,         // 0x7F  F7 - O scancode original da tecla F7 e o 0x83 mas foi
};             //            movido para 0x7f para que a tabela caiba em 128 bytes




//
// Associacao das teclas com Scancodes extendidos para teclados ABNT2
// E possivel economizar algum espaco reduzindo a tabela do codigo 0x10 ao 0x7d
// porem isso nao foi feito em funcao da clareza do codigo.
const unsigned char PS2Keymap_Ext_ABNT[] = { 
_NONE,         // 0x00
_NONE,         // 0x01
_NONE,         // 0x02
_NONE,         // 0x03
_NONE,         // 0x04
_NONE,         // 0x05
_NONE,         // 0x06
_NONE,         // 0x07
_NONE,         // 0x08
_NONE,         // 0x09
_NONE,         // 0x0A
_NONE,         // 0x0B
_NONE,         // 0x0C
_NONE,         // 0x0D
_NONE,         // 0x0E
_NONE,         // 0x0F
_NONE,         // 0x10  WWW_Search (Multimedia)                          
_NONE,         // 0x11  Right ALT                                            
_NONE,         // 0x12                           
_NONE,         // 0x13                       
_SS,           // 0x14  Right Control  (SYMBOL SHIFT)        
_NONE,         // 0x15  Previous_Track  (Multimedia)                     
_NONE,         // 0x16                       
_NONE,         // 0x17                       
_NONE,         // 0x18  WWW_Favorites   (Multimedia)                     
_NONE,         // 0x19                       
_NONE,         // 0x1A                       
_NONE,         // 0x1B                       
_NONE,         // 0x1C                     
_NONE,         // 0x1D                     
_NONE,         // 0x1E                     
_NONE,         // 0x1F  Left Windows                  
_NONE,         // 0x20  WWW_Refresh     (Multimedia)                   
_NONE,         // 0x21  Volume_Down     (Multimedia)                   
_NONE,         // 0x22                     
_NONE,         // 0x23  Mute            (Multimedia)                   
_NONE,         // 0x24                     
_NONE,         // 0x25                     
_NONE,         // 0x26                     
_NONE,         // 0x27  Right Windows                   
_NONE,         // 0x28  WWW_Stop        (Multimedia)                   
_NONE,         // 0x29                     
_NONE,         // 0x2A                     
_NONE,         // 0x2B  Calculator      (Multimedia)
_NONE,         // 0x2C                         
_NONE,         // 0x2D                         
_NONE,         // 0x2E                      
_NONE,         // 0x2F  Context menu                      
_NONE,         // 0x30  WWW_Forward   (Multimedia)                   
_NONE,         // 0x31                     
_NONE,         // 0x32  Volume_Up     (Multimedia)                   
_NONE,         // 0x33                     
_NONE,         // 0x34  Play/Pause    (Multimedia)                   
_NONE,         // 0x35                     
_NONE,         // 0x36                     
_NONE,         // 0x37  Power         (Multimedia)                   
_NONE,         // 0x38  WWW_Back      (Multimedia)                   
_NONE,         // 0x39                     
_NONE,         // 0x3A  WWW_Home      (Multimedia)                   
_NONE,         // 0x3B  Stop          (Multimedia)                   
_NONE,         // 0x3C                     
_NONE,         // 0x3D                     
_NONE,         // 0x3E                                
_NONE,         // 0x3F  Sleep         (Multimedia)                      
_NONE,         // 0x40  My_Computer   (Multimedia)                      
_NONE,         // 0x41                        
_NONE,         // 0x42                        
_NONE,         // 0x43                        
_NONE,         // 0x44                        
_NONE,         // 0x45                        
_NONE,         // 0x46                         
_NONE,         // 0x47                         
_NONE,         // 0x48  E_Mail        (Multimedia)    
_NONE,         // 0x49                     
_SS + _V,      // 0x4A  Keypad SLASH  (SS + V)                   
_NONE,         // 0x4B                     
_NONE,         // 0x4C                     
_NONE,         // 0x4D  Next_Track    (Multimedia)                   
_NONE,         // 0x4E                      
_NONE,         // 0x4F                      
_NONE,         // 0x50  Media_Select  (Multimedia)                    
_NONE,         // 0x51                      
_NONE,         // 0x52                      
_NONE,         // 0x53                      
_NONE,         // 0x54                      
_NONE,         // 0x55                      
_NONE,         // 0x56                      
_NONE,         // 0x57                     
_NONE,         // 0x58                     
_NONE,         // 0x59                     
_ENT,           // 0x5A  Keypad Enter              
_NONE,         // 0x5B                      
_NONE,         // 0x5C                      
_NONE,         // 0x5D                      
_NONE,         // 0x5E  Wake          (Multimedia)                    
_NONE,         // 0x5F                      
_NONE,         // 0x60                      
_NONE,         // 0x61                      
_NONE,         // 0x62                      
_NONE,         // 0x63                      
_NONE,         // 0x64                      
_NONE,         // 0x65                      
_NONE,         // 0x66
_NONE,         // 0x67
_NONE,         // 0x68
_NONE,         // 0x69  END
_NONE,         // 0x6A  
_CS + _5,      // 0x6B  LEFT Arrow (CS + 5)
_NONE,         // 0x6C  HOME 
_NONE,         // 0x6D  
_NONE,         // 0x6E  
_NONE,         // 0x6F           
_NONE,         // 0x70  INSERT                        
_NONE,         // 0x71  DELETE                       
_CS + _6,      // 0x72  DOWN Arrow (CS + 6)                      
_NONE,         // 0x73                       
_CS + _8,   // 0x74  RIGHT Arrow   (CS + 8)                  
_CS + _7,   // 0x75  UP Arrow      (CS + 7)                
_NONE,         // 0x76                        
_NONE,         // 0x77                           
_NONE,         // 0x78                        
_NONE,         // 0x79                       
_NONE,         // 0x7A  PG_DN
_NONE,         // 0x7B  
_NONE,         // 0x7C  
_NONE,         // 0x7D  PG_UP 
_NONE,         // 0x7E
_NONE          // 0x7F
};