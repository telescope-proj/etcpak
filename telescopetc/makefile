all:
	gcc main.c -o main.o -c -O3 -DNO_GZIP -DPNG_INTEL_SSE -march=native
	g++ main.o -o telescopetc build/telescopetc/libttc.a -lpthread -lpng -llz4 -lm -lstdc++
	rm -f *.o
