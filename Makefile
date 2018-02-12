CXX = g++ -std=c++11
SOURCES = src/tas.cpp src/donnees.cpp src/heuristiques.cpp src/a_star.cpp
CFLAGS = -Wall -g -Ofast
LDFLAGS = -lm -lglpk
.PHONY = clean

all: main

main: $(SOURCES:.cpp=.o) $(LDFLAGS)

clean:
	rm main
	rm src/*.o