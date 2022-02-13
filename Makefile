CC=gcc #-fsanitize=address
CPPFLAGS= -Wall -Wextra -std=c99 -O3 -g -D__NO_INLINE__
LDFLAGS= 
LDLIBS= -lSDL2 -lSDL2_image -lSDL2_ttf -lm `sdl2-config --cflags --libs`
GTK=`pkg-config --cflags --libs gtk+-3.0` -export-dynamic


all : setup build

build: preprocessing mysdl patternfinder validfinder draw main
	${CC} ${CFLAGS} bin/*.o ${LDLIBS} -o main

main:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/main.o -c src/Source/main.c

preprocessing:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/PreProcessing.o -c src/Source/PreProcessing.c

mysdl:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/MySDL.o -c src/Source/MySDL.c

patternfinder:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/PatternFinder.o -c src/Source/PatternFinder.c

validfinder:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/ValidFinder.o -c src/Source/ValidFinder.c

draw:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/Draw.o -c src/Source/Draw.c

setup : fix_libs setup_dirs

setup_dirs:
#	mkdir -p tmp
	mkdir -p bin
	mkdir -p output

fix_libs:
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/run/current-system/sw/lib/

clean:
	rm -f main
	rm -rf tmp
	rm -rf bin
	rm -rf output