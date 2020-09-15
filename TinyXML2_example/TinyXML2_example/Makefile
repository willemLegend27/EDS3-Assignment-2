
CXXFLAGS=-Wall -Werror -Wextra -pedantic -ggdb -O0 -std=c++11


MAIN=src/main.cpp src/tinyxml2.cpp

CXX=g++

.PHONY: all clean

all: main

main: $(MAIN)
	$(CXX) $(CXXFLAGS) $(MAIN) -o $@

clean:
	@rm -rf main

