all:
	gcc main.c -o main.o -c -O3 -DNO_GZIP -DPNG_INTEL_SSE

c:
	g++ -c wrapper.cpp BlockDataPublic.cpp ../Timing.cpp ../TaskDispatch.cpp ../System.cpp ../ProcessDxtc.cpp ../Dither.cpp ../BlockData.cpp ../ProcessRGB.cpp ../Tables.cpp ../Bitmap.cpp ../BitmapDownsampled.cpp ../Debug.cpp

link:
	g++ main.o wrapper.o Timing.o TaskDispatch.o System.o ProcessDxtc.o Dither.o BlockDataPublic.o ProcessRGB.o Tables.o Bitmap.o DataProvider.o BitmapDownsampled.o Debug.o -o telescopetc -lpthread -lpng -llz4 -lm -lstdc++

decode:
	gcc main.c -o main.o -c -g
	g++ -c wrapper.cpp ../mmap.cpp
	g++ main.o wrapper.o -o telescopetc -lpthread -lpng -llz4 -lm -lstdc++
	rm -f *.o

clean:
	rm -f *.o telescopetc

debugg:
	g++ -c wrapper.cpp -c -O3 -DNO_GZIP -DPNG_INTEL_SSE
	g++ main.o wrapper.o Timing.o TaskDispatch.o System.o ProcessDxtc.o Dither.o BlockDataPublic.o ProcessRGB.o Tables.o Bitmap.o BitmapDownsampled.o Debug.o -o telescopetc -lpthread -lpng -llz4 -lm -lstdc++