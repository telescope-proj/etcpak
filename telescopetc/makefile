all:
	gcc main.c -o main.o -c -g

decode:
	gcc main.c -o main.o -c -g
	g++ -c wrapper.cpp ../mmap.cpp
	g++ main.o wrapper.o -o telescopetc -lpthread -lpng -llz4 -lm -lstdc++
	rm -f *.o

clean:
	rm -f *.o telescopetc