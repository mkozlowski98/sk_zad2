TARGETS = netstore-server

CC = gcc
CFLAGS = -Wall -Wextra -O2
LFLAGS = -Wall

all: $(TARGETS)

netstore-server: netstore-server.o err.o

.PHONY: clean

clean:
	rm -r $(TARGETS) *.o *~
