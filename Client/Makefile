CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -ggdb -O0 -std=c++17 -Iproduct
LDFLAGS = -lpthread -lrt
SOURCES=main.cpp Client.cpp
HEADERS=Client.h
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = Client

.PHONY: all clean

all: $(TARGET)

$(TARGET) : $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@  $^ 


clean:
	@rm -rf $(TARGET) $(OBJECTS)



