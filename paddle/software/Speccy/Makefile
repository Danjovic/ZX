# Makefile for the z80 sample program.
# It uses the sjasmplus assembler.


PROJ = readPaddleDemo

ASM = pasmo
TAP_FILE = $(PROJ).tap
MAIN_ASM = main.asm
EXT_OBJ = main.obj
ASM_FILES = $(MAIN_ASM) 

all:	default

default:	main

clean:
	-rm -f $(EXT_OBJ)

main:	$(TAP_FILE)

$(TAP_FILE):	$(ASM_FILES) Makefile 
	$(ASM) --tapbas $(MAIN_ASM) $(TAP_FILE)
