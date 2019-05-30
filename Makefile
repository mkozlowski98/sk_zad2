TARGETS = netstore-server netstore-client

CC = g++
CFLAGS = -Wall -Wextra -O2
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -std=c++17 -Wall -Wextra -O2
LFLAGS = -lstdc++fs -pthread

all: $(TARGETS)

netstore-server: netstore-server.o err.o sock.o messages.o globals.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LFLAGS)

netstore-client: netstore-client.o err.o sock.o messages.o globals.o

.PHONY: clean

clean:
	rm -r $(TARGETS) *.o *~
