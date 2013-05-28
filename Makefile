CC=gcc
LDFLAGS= -I/usr/local/lib:/usr/lib
CFLAGS= -lmonome -lpthread -lyaml
TEST_CFLAGS= $(CFLAGS) -lcheck 
TARGET_OBJS=bin/build/mumble/*.o
TEST_TARGET_OBJS=bin/*.o

all:
	mkdir -p bin/build/mumble
	$(CC) -c src/mumble/list.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/list.o
	$(CC) -c src/mumble/config_node.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/config_node.o
	$(CC) -c src/mumble/config.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/config.o
	$(CC) -c src/mumble/midi.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/midi.o
	$(CC) -c src/mumble/dispatcher_functions.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/dispatcher_functions.o
	$(CC) -c src/mumble/muxer.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/muxer.o
	$(CC) -c src/mumble/loop.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/loop.o
	$(CC) -c src/mumble/session.c $(LDFLAGS) $(CFLAGS) -o bin/build/mumble/session.o
	$(CC) src/mumble.c $(LDFLAGS) $(CFLAGS) $(TARGET_OBJS) -o bin/mumble

test:
	mkdir -p bin/test
	$(CC)  tests/mumble/list_test.c $(LDFLAGS) $(TEST_CFLAGS) bin/build/mumble/list.o -o bin/list_test
	$(CC) tests/mumble/config_node_test.c $(LDFLAGS) $(TEST_CFLAGS) bin/build/mumble/config_node.o -o bin/config_node_test
	# Session tests cannot be run until crappy functions are refactored out of main :\
	# $(CC) tests/mumble/session_test.c $(LDFLAGS) $(TEST_CFLAGS) bin/build/mumble/list.o bin/build/mumble/loop.o bin/build/mumble/session.o -o bin/session_test

	./bin/list_test
	./bin/config_node_test
	# ./bin/session_test
clean:
	rm -rf bin/*

install:
	sudo cp bin/mumble /usr/bin/mumble
