CC = gcc
CFLAGS = -Isrc/include -Wall
SDLFLAGS = -Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
objects = time.o player.o ray.o

all: main

%.o: src/%.c
	${CC} ${CFLAGS} -c $^

main: ${objects}
	${CC} $? src/$@.c ${CFLAGS} ${SDLFLAGS} -o main

clean:
	-del main.exe
	-del ${objects}

run: all
	@-del ${objects}
	.\main.exe
	@-del main.exe

debug: CFLAGS += -g
debug: clean all