CC=gcc
CPPFLAGS= -Wall -Wextra -std=c99 -O3 -g -D__NO_INLINE__
LDFLAGS= -fsanitize=address
LDLIBS= -lSDL2 -lSDL2_image -lSDL2_ttf -lm `sdl2-config --cflags --libs`
GTK=`pkg-config --cflags --libs gtk+-3.0` -export-dynamic


all: setup build demo

build: preprocessing mysdl main
	${CC} ${CFLAGS} bin/main.o bin/PreProcessing.o bin/MySDL.o ${LDLIBS} -o main

main:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/main.o -c src/Source/main.c

preprocessing:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/PreProcessing.o -c src/Source/PreProcessing.c

mysdl:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/MySDL.o -c src/Source/MySDL.c

demo: fromnayuki encoder demoencoder
	${CC} ${CFLAGS} bin/demoencoder.o bin/encoder.o bin/fromnayuki.o ${LDLIBS} -o demo

demoencoder:
	$(CC) $(CFLAGS) -I./src/Header -o bin/demoencoder.o -c src/Source/demoencoder.c

encoder:
	$(CC) $(CFLAGS) -I./src/Header -o bin/encoder.o -c src/Source/encoder.c

fromnayuki:
	$(CC) $(CFLAGS) -I./src/Header -o bin/fromnayuki.o -c src/Source/fromnayuki.c

setup : fix_libs setup_dirs

setup_dirs:
#	mkdir -p tmp
	mkdir -p bin
	mkdir -p output

fix_libs:
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/run/current-system/sw/lib/

clean:
	rm -f main demo test*.pbm
	rm -rf tmp
	rm -rf bin
	rm -rf output
