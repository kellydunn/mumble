CC=gcc
LDFLAGS=$(shell pkg-config --libs libmonome )
CFLAGS=$(shell pkg-config --cflags libmonome )

all:
	$(CC) src/mumble.c $(LDFLAGS) $(CFLAGS) $(TARGET) -o bin/mumble

clean:
	rm bin/*

install:
	sudo cp bin/mumble /usr/bin/mumble
