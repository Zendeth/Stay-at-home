CC=gcc #-fsanitize=address
CPPFLAGS= -Wall -Wextra -std=c99 -O3 -g -D__NO_INLINE__
LDFLAGS= 
LDLIBS= -lSDL2 -lSDL2_image  -lm `sdl2-config --cflags --libs`
GTK=`pkg-config --cflags --libs gtk+-3.0` -export-dynamic


all: setup build

build: preprocessing mysdl patternfinder validfinder draw alignmentfinder geotrans tomatrix segmentation extractcode main decoder bch blocks data modules pbm rs
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

alignmentfinder: 
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/AlignmentFinder.o -c src/Source/AlignmentFinder.c

geotrans:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/GeoTans.o -c src/Source/GeoTrans.c

tomatrix:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/ToMatrix.o -c src/Source/ToMatrix.c

segmentation:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/Segmentation.o -c src/Source/Segmentation.c

extractcode:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/ExtractCode.o -c src/Source/ExtractCode.c


decoder: 
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/decoder.o -c src/Source/decoder.c

bch:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/bch.o -c src/Source/bch.c

blocks:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/blocks.o -c src/Source/blocks.c

data:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/data.o -c src/Source/data.c

modules:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/modules.o -c src/Source/modules.c

pbm:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/pbm.o -c src/Source/pbm.c

rs:
	$(CC) $(CFLAGS) $(LDLIBS) -o bin/rs.o -c src/Source/rs.c

#demo: setup fromnayuki encoder demoencoder
#	${CC} ${CFLAGS} bin/demoencoder.o bin/encoder.o bin/fromnayuki.o ${LDLIBS} -o demo

#demoencoder:
#	$(CC) $(CFLAGS) -I./src/Header -o bin/demoencoder.o -c src/Source/demoencoder.c

#encoder:
#	$(CC) $(CFLAGS) -I./src/Header -o bin/encoder.o -c src/Source/encoder.c

#fromnayuki:
#	$(CC) $(CFLAGS) -I./src/Header -o bin/fromnayuki.o -c src/Source/fromnayuki.c

setup : fix_libs setup_dirs

setup_dirs:
#	mkdir -p tmp
	mkdir -p bin
	mkdir -p output

fix_libs:
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/run/current-system/sw/lib/

clean:
	rm -f main demo test*.pbm res_decode.txt res_seg.pbm
	rm -rf tmp
	rm -rf bin
	rm -rf output
