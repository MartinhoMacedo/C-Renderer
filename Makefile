build:
	gcc -g -Wall -std=c11 -fms-extensions ./src/*.c `sdl2-config --libs --cflags` -F /Library/Frameworks -lm -o renderer

run:
	./renderer

test:
	timeout -k 1s 1s ./renderer

clean:
	rm renderer
