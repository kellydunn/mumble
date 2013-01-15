CC=gcc
LDFLAGS=$(shell pkg-config --libs libmonome liblo libusb-1.0)
CFLAGS=$(shell pkg-config --cflags libmonome liblo libusb-1.0 )

all:
	$(CC) src/mumble.c $(LDFLAGS) $(CFLAGS) $(TARGET) -o bin/mumble

clean:
	rm bin/*

install:
	sudo cp bin/mumble /usr/bin/mumble