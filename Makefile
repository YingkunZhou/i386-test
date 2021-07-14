all: i386-test

i386-test: i386-test.c insn.c insn.h
	gcc -c -fno-builtin i386-test.c -O2 -fno-pic
	gcc -c -fno-builtin insn.c -O3 -msse2 -fno-pic
	ld -static -T i386-test.lds -o i386-test i386-test.o insn.o

clean:
	rm -f  *.o i386-test
