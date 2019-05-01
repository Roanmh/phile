main: main.o disk.o argtable3.o poetry_map.o
		g++ -g -o phile main.o disk.o argtable3.o -lm poetry_map.o
#                                              ^^^
#   This is for reasons described here:
#   http://www.linuxforums.org/forum/programming-scripting/125526-c-gcc-math-h-lm.html

main.o: main.c main.h
		gcc -g -c main.c

argtable3.o: argtable3.c argtable3.h
		gcc -g -c argtable3.c

disk.o: disk.c disk.h
		gcc -g -c disk.c

poetry_map.o: poetry_map.cpp
	  g++ -g -c poetry_map.cpp

clean:
		rm -f phile *.o *.so
