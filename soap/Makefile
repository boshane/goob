CC=gcc
CFLAGS=-Wall -g -Wpedantic
VPATH = soap:test

UNITY_ROOT=~/src/Unity

test: $(UNITY_ROOT)/src/unity.c test.c buffer.o command.o
	$(CC) $(CFLAGS) -I$(UNITY_ROOT)/src $(UNITY_ROOT)/src/unity.c \
	./command.o ./buffer.o ./test/test.c -o soap.out
	- ./soap.out

buffer.o: buffer.c buffer.h
	$(CC) $(CFLAGS) -c $<

command.o : command.c command.h
	$(CC) $(CFLAGS) -c $<

