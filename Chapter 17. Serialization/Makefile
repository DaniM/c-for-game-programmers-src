CXXFLAGS=-g -Wall -Wextra
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

all: $(OBJECTS) main

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ 

