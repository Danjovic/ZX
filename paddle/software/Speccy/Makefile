# Makefile for the z80 sample program.
# It uses the pasmo assembler.


PROJ = readPaddleDemo

ASM = pasmo
TAP_FILE = $(PROJ).tap
MAIN_ASM = main.asm

ASM_FILES = $(MAIN_ASM) 

all:	default

default:	main

clean:
	-rm -f $(TAP_FILE)

main:	$(TAP_FILE)

$(TAP_FILE):	$(ASM_FILES) Makefile 
	$(ASM) --tapbas $(MAIN_ASM) $(TAP_FILE)
