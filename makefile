main: main.o argtable3.o
		gcc -o main main.o argtable3.o -lm
#                              ^^^
#   This is for reasons described here:
#   http://www.linuxforums.org/forum/programming-scripting/125526-c-gcc-math-h-lm.html

main.o: main.c main.h
		gcc -c main.c

argtable3.o: argtable3.c argtable3.h
		gcc -lm -c argtable3.c

clean:
		rm main *.o
