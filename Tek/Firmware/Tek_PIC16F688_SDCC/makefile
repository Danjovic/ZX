# Makefile by Diego Herranz - https://github.com/diegoherranz/sdcc-examples
SRC=main.c

CC=sdcc
FAMILY=pic14
PROC=16f688
#ZXMODEL=ZXSPECTRUM
ZXMODEL=ZX81

all: $(SRC:.c=.hex)

$(SRC:.c=.hex): $(SRC)
	$(CC) --use-non-free -m$(FAMILY) -p$(PROC) -D$(ZXMODEL) $^

clean:
	rm -f $(SRC:.c=.asm) $(SRC:.c=.cod) $(SRC:.c=.hex) $(SRC:.c=.lst) $(SRC:.c=.o)

.PHONY: all clean
