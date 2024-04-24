#Bu dosyada gcc ile build işlemi yapılıyor.
OBJECTS= ./build/compiler.o ./builds/cprocess.o
# "-I./" Dahil etme dizisini göreceli hale getirir.
INCLUDES= -I./

# "-g" debugging sembollerini, "-o" output dosyasını temsil ediyor.
all: ${OBJECTS}
	gcc main.c ${INCLUDES} ${OBJECTS} -g -o ./main

./build/compiler.o: ./compiler.c
	gcc ./compiler.c ${INCLUDES} -o ./build/compiler.o -g -c

./build/compiler.o: ./cprocess.c
	gcc ./compiler.c ${INCLUDES} -o ./build/compiler.o -g -c

# clean komutu main dosyasını ve bütün objeleri silecek.
clean:
	rm ./main
	rm -rf ${OBJECTS}