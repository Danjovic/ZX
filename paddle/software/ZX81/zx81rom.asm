;ZX81 ROM routines
PRINT			EQU $10
PRINTAT			EQU $08F5
DISPROUT		EQU $0281
DISPLAY_3		EQU $0292 ;restore registers after a 'slow' display
DISPLAY_5		EQU $02B5
PLOT			EQU $0BB2 	; note ROM plot is 0baf - however first thing it does try to pull BC from the calculator stack
							; so we'll skip past that and set BC ourselves
CLS				EQU $0A2A
KSCAN           EQU $02BB 
FINDCHR         EQU $07BD
COPY            EQU $0869
FAST            EQU $0F23
SLOW            EQU $0F2B
SLOWORFAST		EQU $0207
;sys vars
ERR_NR			EQU $4000