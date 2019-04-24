main: main.o disk.o argtable3.o libpoetry_map.so
		gcc -L. -g -o phile main.o disk.o argtable3.o -lm -lpoetry_map
#                                              ^^^
#   This is for reasons described here:
#   http://www.linuxforums.org/forum/programming-scripting/125526-c-gcc-math-h-lm.html

main.o: main.c main.h
		gcc -g -c main.c

argtable3.o: argtable3.c argtable3.h
		gcc -g -c argtable3.c

disk.o: disk.c disk.h
		gcc -g -c disk.c

libpoetry_map.so: poetry_map.cpp
	  g++ -g -o libpoetry_map.so -shared -fPIC poetry_map.cpp

clean:
		rm -f phile *.o *.so
