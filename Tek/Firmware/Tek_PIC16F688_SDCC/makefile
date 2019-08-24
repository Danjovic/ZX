 
all: main

 
main: main.c
	sdcc  --use-non-free -mpic14 -p16f688 -DZXMODEL=ZX81 main.c -o "zx81"
	sdcc  --use-non-free -mpic14 -p16f688 -DZXMODEL=ZXSPECTRUM main.c -o "speccy"

program:
	pk2cmd -B/usr/share/pk2/ -PPIC16F688 -Fmain.hex -M -R