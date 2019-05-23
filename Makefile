TARGETS = netstore-server netstore-client

CC = g++
CFLAGS = -Wall -Wextra -O2
LFLAGS = -Wall

all: $(TARGETS)

netstore-server: netstore-server.o err.o

netstore-client: netstore-client.o err.o
.PHONY: clean

clean:
	rm -r $(TARGETS) *.o
