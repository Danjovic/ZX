; TASM cross-assembler definitions
;
;#define	db		.byte
;#define	dw		.word
;#define	ds		.block
;#define	org	.org
;#define	end	.end

; A useful reference
;
;  ____0___1___2___3___4___5___6___7___8___9___A___B___C___D___E___F____
;  00 SPC GRA GRA GRA GRA GRA GRA GRA GRA GRA GRA  "  GBP  $   :   ?  0F
;  10  (   )   >   <   =   +   -   *   /   ;   ,   .   0   1   2   3  1F
;  20  4   5   6   7   8   9   A   B   C   D   E   F   G   H   I   J  2F
;  30  K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z  3F


	; -------------=-=-=-=-=-=-=-=-=-=-=-------------
	;               ASSEMBLER  TEMPLATE
	; -------------=-=-=-=-=-=-=-=-=-=-=-------------
include "zx81rom.asm"
include "charcodes.asm"


			org	$4009

;ERR_NR	equ	$4000
FLAGS	equ	$4001
ERR_SP	equ	$4002
RAMTOP	equ	$4004
MODE	equ	$4006
PPC		equ	$4007

; System variables as saved to tape

VERSN:	db		0
E_PPC:	dw		0
D_FILE:	dw		dfile
DF_CC:	dw		dfile+1
VARS:		dw		var
DEST:		dw		0
E_LINE:	dw		var+1
CH_ADD:	dw		last-1
X_PTR:	dw		0
STKBOT:	dw		last
STKEND:	dw		last
BERG:		db		0
MEM:		dw		MEMBOT
			db		0
DF_SZ:	db		2
S_TOP:	dw		1
LAST_K:	db		$FF,$FF,$FF
MARGIN:	db		55
NXTLIN:	dw		line1
OLDPPC:	dw		0
FLAGX:	db		0
STRLEN:	dw		0
T_ADDR:	dw		$0C8D
SEED:		dw		0
FRAMES:	dw		$FFFF
COORDS:	db		0,0
PR_CC:	db		$BC
S_POSN:	db		33,24
CDFLAG:	db		01000000B
PRTBUF:	ds		33			; print buffer - iow scratch ram for us :)
MEMBOT:	ds		30			; calculator's scratch
			ds		2

line0:
	db		$00,$00		; line number
	dw		line1-$-2	; line length

	db		$ea			; REM

	db		$7e			; m/c for ld a,(hl),  BASIC token for 'next 5 bytes are fp number'
	jp		go				; 3 bytes
	db		0,0			; +2 bytes ... this effectively hides the m/c in the REM


;******************************************************************************
;                    ___         _       ___           _      
;                   / __|___  __| |___  | _ ) ___ __ _(_)_ _  
;                  | (__/ _ \/ _` / -_) | _ \/ -_) _` | | ' \ 
;                   \___\___/\__,_\___| |___/\___\__, |_|_||_|
;                                                |___/ 		
;
;******************************************************************************
go:

IO_KEMPSTON: EQU 31

Startup:
		LD		HL,msgbanner		;load HL with address of line
        call    Prints

      ld bc,0200h ; b= line 03 / c = column01
      call PRINTAT


Mainloop:
      call DetectJapanesePaddle
	  or a
      jr thenRead
;	  jr nz,thenRead
      ld bc,0200h ; b= line 03 / c = column01
      call PRINTAT
	  ld hl,msgPaddleOff
	  call Prints
      jr repeatLoop
;	  call delay100ms

thenRead:
      ld bc,0200h ; b= line 03 / c = column01
      call PRINTAT
      ld hl,msgBlank
	  call Prints

      ld bc,0200h ; b= line 03 / c = column01
      call PRINTAT

      call ReadPaddle
      ld h,0
      ld l,a
      call DispHL  ; print number


repeatLoop:
      call KSCAN   ; scan keyboard
      ld a,l       ; l=0ffh if no key pressed
      inc a        ; 
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


; print a string
Prints:  
		LD		A,(HL)		;load A with a character at HL
		CP		$FF			;is this $FF
		RET     Z   		;if so, then jump to end
		CALL	PRINT		;print character
		INC		HL			;increment HL to get to next character
		JR      Prints      ;jump to beginning of loop

; print an integer
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
Num1:	ld	a,_0-1
Num2:	inc	a
	add	hl,bc
	jr	c,Num2
	sbc	hl,bc
	push hl
	call PRINT
	pop hl
	ret



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
ReadPaddle: ret
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







msgbanner:  db _S,_E,_G,_A,$00,_M,_A,_S,_T,_E,_R,$00,_S,_Y,_S,_T,_E,_M,$00
            db _P,_A,_D,_D,_L,_E,$00,_D,_E,_M,_O,$76
            db  _D,_A,_N,_J,_O,_V,_I,_C,$00,_2,_0,_1,_9,$76,$ff


msgPaddleOff: db _N,_O,$00,_P,_A,_D,_D,_L,_E,$76,$ff
msgBlank:     db $00,$00,$00,$00,$00,$00,$00,$00,$00,$76,$ff
;msgBlank:     db $0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$76,$ff


	ret
	db		$76							; N/L - end of line 0

;******************************************************************************
;                        ___         _       ___         _ 
;                       / __|___  __| |___  | __|_ _  __| |
;                      | (__/ _ \/ _` / -_) | _|| ' \/ _` |
;                       \___\___/\__,_\___| |___|_||_\__,_|
;                                                          
;******************************************************************************




line1:
	db		0,1   						; line number
	dw		dfile-$-2					; line length

	db		$f9,$d4,$c5					; RAND USR VAL
	db		$b,$1d,$22,$21,$1d,$20,$b	; "16514"

	db		$76    						; N/L - end of line 1


;- Display file --------------------------------------------

dfile:
	db		$76
	db		$76,$76,$76,$76,$76,$76,$76,$76
	db		$76,$76,$76,$76,$76,$76,$76,$76
	db		$76,$76,$76,$76,$76,$76,$76,$76


;- BASIC-Variables ----------------------------------------

var:
	db		$80


;- End of program area ----------------------------

last:
	end
