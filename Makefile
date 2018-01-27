CXX = g++ -std=c++11
SOURCES = src/tas.cpp src/noeud.cpp src/donnees.cpp
CFLAGS = -Wall -g -Ofast
LDFLAGS =
.PHONY = clean

all: main

main: $(SOURCES:.cpp=.o) $(LDFLAGS)

clean:
	rm main
	rm src/*.o