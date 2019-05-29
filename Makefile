TARGETS = netstore-server netstore-client

CC = g++
CFLAGS = -Wall -Wextra -O2
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -std=c++17 -Wall -Wextra -O2
LFLAGS = -lstdc++fs

all: $(TARGETS)

netstore-server: netstore-server.o err.o structures.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LFLAGS)

netstore-client: netstore-client.o err.o structures.o

.PHONY: clean

clean:
	rm -r $(TARGETS) *.o *~
