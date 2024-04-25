#Bu dosyada gcc ile build işlemi yapılıyor.
OBJECTS= ./build/compiler.o ./build/cprocess.o ./build/helpers/buffer.o ./build/helpers/vector.o
# "-I./" Dahil etme dizisini göreceli hale getirir.
INCLUDES= -I./

# "-g" debugging sembollerini, "-o" output dosyasını temsil ediyor.
all: ${OBJECTS}
	gcc main.c ${INCLUDES} ${OBJECTS} -g -o ./main

./build/compiler.o: ./compiler.c
	gcc ./compiler.c ${INCLUDES} -o ./build/compiler.o -g -c

./build/cprocess.o: ./cprocess.c
	gcc ./cprocess.c ${INCLUDES} -o ./build/cprocess.o -g -c

./build/helpers/buffer.o: ./helpers/buffer.c
	gcc ./helpers/buffer.c ${INCLUDES} -o ./build/helpers/buffer.o -g -c

./build/helpers/vector.o: ./helpers/vector.c
	gcc ./helpers/vector.c ${INCLUDES} -o ./build/helpers/vector.o -g -c

# clean komutu main dosyasını ve bütün objeleri silecek.
clean:
	rm ./main
	rm -rf ${OBJECTS}