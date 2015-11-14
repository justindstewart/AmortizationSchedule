amort.out: amortmain.o amortdriver.o paymentcalc.o
	gcc -m64 -o amort.out amortmain.o amortdriver.o paymentcalc.o -lm

amortmain.o: amortmain.asm
	nasm -f elf64 -l amortmain.lis -o amortmain.o amortmain.asm

amortdriver.o: amortdriver.c
	gcc -c -m64 -Wall -std=c99 -l amortdriver.lis -o amortdriver.o amortdriver.c

paymentcalc.o: paymentcalc.c
	gcc -c -m64 -Wall -std=c99 -l paymentcalc.lis -o paymentcalc.o paymentcalc.c

clean:
	rm -f amortmain.lis amortdriver.lis paymentcalc.lis amortmain.o amortdriver.o paymentcalc.o amort.out
