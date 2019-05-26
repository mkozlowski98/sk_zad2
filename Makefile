TARGETS = netstore-server netstore-client

CC = g++
CFLAGS = -Wall -Wextra -O2
LFLAGS = -Wall

all: $(TARGETS)

netstore-server: netstore-server.o err.o structures.o

netstore-client: netstore-client.o err.o structures.o

.PHONY: clean

clean:
	rm -r $(TARGETS) *.o
