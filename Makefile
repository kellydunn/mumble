CC=gcc
LDFLAGS= -I/usr/local/lib:/usr/lib
CFLAGS= -lmonome -lpthread
TARGET_OBJS=bin/build/mumble/*.o

all:
	mkdir -p bin/build/mumble
	$(CC) -c src/mumble/list.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/list.o
	$(CC) -c src/mumble/midi.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/midi.o
	$(CC) -c src/mumble/dispatcher_functions.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/dispatcher_functions.o
	$(CC) -c src/mumble/muxer.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/muxer.o
	$(CC) -c src/mumble/loop.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/loop.o
	$(CC) -c src/mumble/session.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/session.o
	$(CC) src/mumble.c $(LDFLAGS) $(CFLAGS) $(TARGET_OBJS) -o bin/mumble

clean:
	rm -rf bin/*

install:
	sudo cp bin/mumble /usr/bin/mumble
