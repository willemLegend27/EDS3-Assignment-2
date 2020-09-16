CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -ggdb -O0 -std=c++17 -Iproduct
LDFLAGS = -lpthread -lrt
SOURCES=main.cpp Server.cpp
HEADERS=Server.h
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = Server

.PHONY: all clean

all: $(TARGET)

$(TARGET) : $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@  $^ 


clean:
	@rm -rf $(TARGET) $(OBJECTS)



