# Makefile for the z80 sample program.
# It uses the pasmo assembler.


PROJ = readPaddleDemo

ASM = pasmo
P_FILE = $(PROJ).p
MAIN_ASM = main.asm
EXT_OBJ = main.obj
ASM_FILES = $(MAIN_ASM) 

all:	default

default:	main

clean:
	-rm -f $(P_FILE)

main:	$(P_FILE)

$(P_FILE):	$(ASM_FILES) Makefile 
	$(ASM) $(MAIN_ASM) $(P_FILE)
