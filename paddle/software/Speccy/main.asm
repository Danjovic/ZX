;******************************************************************************
;          ___         _    _ _         _      _           _           
;         | _ \__ _ __| |__| | |___    /_\  __| |__ _ _ __| |_ ___ _ _ 
;         |  _/ _` / _` / _` | / -_)  / _ \/ _` / _` | '_ \  _/ -_) '_|
;         |_| \__,_\__,_\__,_|_\___| /_/ \_\__,_\__,_| .__/\__\___|_|  
;                                                    |_| 
;
;******************************************************************************
; Using Sega Master System Paddles with ZX Spectrum
; Paddle shall be connected on Kempston interface
; Daniel José Viana, 2019
; danjovic@gmail.com
;
; This code is GPL
;
; basic release 19 jul 2019
;
;******************************************************************************
;                            ___ _               _   
;                           | _ (_)_ _  ___ _  _| |_ 
;                           |  _/ | ' \/ _ \ || |  _|
;                           |_| |_|_||_\___/\_,_|\__|
;
;******************************************************************************
;  PINOUT:
;  DE-9 Female                 DE-9 Male
;  to Kempston  Function       to SMS Paddle   Function
;   1             up               1            bit 0/4
;   2             down             2            bit 1/5
;   3             left             3            bit 2/6
;   4             right            4            bit 3/7
;     5           btn 3              9            TR                    
;   6             btn 1            6              TL   
;     7           +5V                5           +5V                     
;   8             GND              8             GND
;     9           btn2               7            TH


;******************************************************************************
;                    ___       __ _      _ _   _             
;                   |   \ ___ / _(_)_ _ (_) |_(_)___ _ _  ___
;                   | |) / -_)  _| | ' \| |  _| / _ \ ' \(_-<
;                   |___/\___|_| |_|_||_|_|\__|_\___/_||_/__/
;
;******************************************************************************


CHOPEN:      EQU  1601h
CLS:         EQU 0d6bh
IO_KEMPSTON: EQU 31
LAST_K:      EQU 23560         ; LAST K system variable

org 08000h

;******************************************************************************
;                         ___ _            _             
;                        / __| |_ __ _ _ _| |_ _  _ _ __ 
;                        \__ \  _/ _` | '_|  _| || | '_ \
;                        |___/\__\__,_|_|  \__|\_,_| .__/
;                                                  |_|  
;
;******************************************************************************

Initialize:
      ld A,2      ; a=screen
      call CHOPEN ; call channel open
      call CLS    ; call cls

      ; Title
      ld hl,msgbanner1
      call PRINTAT
      ld hl,msgbanner2
      call PRINTAT

      ; clear LAST K system variable.
      ld hl,LAST_K
      ld (hl),0


;******************************************************************************
;                    __  __      _        _                  
;                   |  \/  |__ _(_)_ _   | |   ___  ___ _ __ 
;                   | |\/| / _` | | ' \  | |__/ _ \/ _ \ '_ \
;                   |_|  |_\__,_|_|_||_| |____\___/\___/ .__/
;                                                      |_|  
;******************************************************************************


Mainloop:
      call DetectJapanesePaddle
	  or a
	  jr nz,thenRead
	  ld hl,msgPaddleOff
	  call PRINTAT
      jr repeatLoop
	  call delay100ms

thenRead:
      ld hl,msgBlank
	  call PRINTAT

      ld bc,0301h ; b= line 03 / c = column01
      call LOCATE
      call ReadPaddle  ; a = value
      ld h,0
      ld l,a
      call DispHL  ; print number


repeatLoop:
      ld a,(LAST_K)       ; new value of LAST K.
      cp 0                ; is it still zero?
	  jr nz,gracefulEnd   ; NZ-> a key was pressed
	  call delay100ms
	  jr Mainloop

gracefulEnd:
      ret



;******************************************************************************
;                         ___          _   _             
;                        | _ \___ _  _| |_(_)_ _  ___ ___
;                        |   / _ \ || |  _| | ' \/ -_|_-<
;                        |_|_\___/\_,_|\__|_|_||_\___/__/
;                                                    
;******************************************************************************
;
; Wait for 100ms
; 3.575Mhz-> 3575000 cycles per second
;             357500 cycles for 1/10 second
delay100ms:
      ld hl,13750;   357500/26
dl0:  dec hl     ; 6
      ld a,h     ; 4    10
      or l       ; 4    14
      jr nz,dl0  ; 12/7 26
      ret
	  
;
; Read SMS Paddle in Kempston port
; http://www.smspower.org/Development/Paddle   
;
ReadPaddle:
rpd0:
    in     a,(IO_KEMPSTON)         ; Wait for TR input to be 0
    bit    5,a
    jr     nz,rpd0
    and    $0f             ; Save RLDU bits in e
    ld     e,a
rpd1:    
    in     a,(IO_KEMPSTON)         ; Wait for TR input to be 1
    bit    5,a
    jr     z,rpd1
    rrca                   ; Get low 4 bits into 4 high bits
    rrca
    rrca
    rrca
    and    $f0
    or     e               ; Merge with e
    ret                    ; result in a (position 0..255)
    
;
; Detect Paddle presence in Kempston port
; http://www.smspower.org/Development/Paddle
;
DetectJapanesePaddle:
    ld     bc,$0000        ; b = 0 for 256 loops; c = 0 for counting
djp0:
    in     a,(IO_KEMPSTON)
    bit    5,a             ; Get TR input
    jr     nz,djp1            ; If 1, don't increment c
    inc    c
djp1:
    djnz   djp0               ; repeat 256 times
    ld     a,c             ; inspect counter
    ; If the counter value is between $60 and $a0 (50% +/- 12.5% of 256)
    ; then set b=$ff else set b=0
    ld     b,$00
    sub    $60
    jr     c,_DetectionEnd
    cp     $40
    jr     nc,_DetectionEnd
    dec    b
_DetectionEnd:
    ld     a,b
    ret                    ; result in a ($ff if detected, 0 if not)


;----------------------------------------------------
; PRINT routine
;
; from ADVANCED SPECTRUM MACHINE LANGUAGE
; by David Webb
; Entry: A=Char Code
; Preserved: C
; Exit: B=0, DE=Attribute Address


; Construct Character Data Address
PRINT1:  ld L,A
         ld H,0
         add HL,HL
         add HL,HL
         add HL,HL
         ld DE,(BASE)
         add HL,DE
; Take Display File Address
         ld DE,(DFCC)
         ld B,8
; Print Character Row By Row
NXTROW:  ld A,(HL)
         ld (DE),A
         inc HL
         inc D
         djnz NXTROW
; Construct Attribute Address
         ld A,D
         rrca
         rrca
         rrca
         dec A
         and 3
         or 58h
         ld D,A
         ld HL,(ATT) ; H=Mask, L=Attribute
         ld A,(DE)
; Construct New One
         xor L
         and H
         xor L
; Replace Attribute
         ld (DE),A
; Finally Set DFCC to Next Print Position
         ld HL,DFCC
         inc (HL)
         ret nz
         inc HL
         ld A,(HL)
         add A,8
         ld (HL),A
         ret

;----------------------------------------------------  
;
; LOCATE routine
;
; from ADVANCED SPECTRUM MACHINE LANGUAGE
; by David Webb
; Entry:B=Line, C=Column
; Preserved: BC
; Exit: HL=D.F. Address, DE=ATTR.
; DF-CC is Altered
;
;DFCC: equ 5c84h
;
LOCATE:  ld A,B
         and 18h
         ld H,A
         set 6,H
         rrca
         rrca
         rrca
         or 58h
         ld D,A
         ld A,B
         and 7
         rrca
         rrca
         rrca
         add A,C
         ld L,A
         ld E,A
         ld A,(DE)
         ld (DFCC),HL
         ret

;----------------------------------------------------  
;
; Print a message at given coordinates
;
; Entry: HL=Message, preceeded by Line,Column and terminated by 0
; Preserved:
; Exit:
;
PRINTAT:
      ld B,(HL)
      inc HL
      ld C,(HL)
      inc HL
      push HL
      call LOCATE
      pop HL
      call PRINTS
      ret

;----------------------------------------------------  
;
; PRINTS Print a message terminated by 0
;
; Entry: HL=Message
; Preserved:
; Exit:
;
PRINTS:  push HL
         ld A,(HL)
         or A
         jr z,PRTS0 ; termina
         call PRINT1
         pop HL
         inc HL
         jr PRINTS
PRTS0:   ;termina
         pop HL
         ret

;----------------------------------------------------  
;
; Print Decimal number in register HL
;
;Number in hl to decimal ASCII
;Thanks to z80 Bits
;inputs:	hl = number to ASCII
;example: hl=300 outputs '00300'
;destroys: af, bc, hl, de used
DispHL:
	ld	bc,-10000
	call	Num1
	ld	bc,-1000
	call	Num1
	ld	bc,-100
	call	Num1
	ld	bc,-10
	call	Num1
	ld	bc,-1
Num1:	ld	a,"0"-1
Num2:	inc	a
	add	hl,bc
	jr	c,Num2
	sbc	hl,bc
	push hl
	call PRINT1
	pop hl
	ret

;******************************************************************************
;                       ___             _            _      
;                      / __|___ _ _  __| |_ __ _ _ _| |_ ___
;                     | (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
;                      \___\___/_||_/__/\__\__,_|_||_\__/__/
;
;******************************************************************************
CONST:

		                      ;12345678901234567890123456789012
        msgbanner1:   db  1,1, "Sega Master System Paddle Demo",0
        msgbanner2:   db  2,1, "Danjovic 2019",0
		msgPaddleOff: db  3,1, "no paddle",0
        msgBlank:     db  3,1, "         ",0
        
;******************************************************************************
;                      __   __        _      _    _        
;                      \ \ / /_ _ _ _(_)__ _| |__| |___ ___
;                       \ V / _` | '_| / _` | '_ \ / -_|_-<
;                        \_/\__,_|_| |_\__,_|_.__/_\___/__/
;                                                          
;******************************************************************************
VARS:

;
       BASE: dw 3C00h
       DFCC: dw 4000h
       ATT:  db 38h
;      MASK: db 0



end 08000h
