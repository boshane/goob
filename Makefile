CC = clang
CFLAGS=-Wall -g -O0 -Wextra -Werror
OBJS = main.o util.o ./soap/buffer.o ./soap/command.o
VPATH = src:include:soap

.PHONY: all clean

all : $(OBJS)
	$(CC) $(CFLAGS) -o goob $(OBJS)
	make clean

main.o : main.c
	$(CC) $(CFLAGS) -c $< -I./soap

util.o : util.c util.h
	$(CC) $(CFLAGS) -c $< -I./soap

term.o : term.c term.h
	$(CC) $(CFLAGS) -c $<

subsystem:
	cd soap && $(MAKE)

clean:
	rm -f $(OBJS) 
