CC=gcc
# TODO Find out why pkg-config isn't working on arch
#LDFLAGS=$(shell pkg-config --libs libmonome)
#CFLAGS=$(shell pkg-config --cflags libmonome)

LDFLAGS= -I/usr/local/lib:/usr/lib
CFLAGS= -lmonome
TARGET_OBJS=bin/build/mumble/*.o

# TODO choose a better build system
all:
	mkdir -p bin/build/mumble
	$(CC) -c src/mumble/midi.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/midi.o
	$(CC) -c src/mumble/dispatcher_functions.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/dispatcher_functions.o
	$(CC) -c src/mumble/muxer.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/muxer.o
	$(CC) -c src/mumble/sample.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/sample.o
	$(CC) -c src/mumble/session.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/session.o
	$(CC) src/mumble.c $(LDFLAGS) $(CFLAGS) $(TARGET_OBJS) -o bin/mumble

clean:
	rm -rf bin/*

install:
	sudo cp bin/mumble /usr/bin/mumble
