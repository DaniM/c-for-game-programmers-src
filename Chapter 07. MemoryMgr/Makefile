CXXFLAGS=-g -Wall -Wextra $(OPTFLAGS)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))


all: $(OBJECTS) main

main: CXXFLAGS+=$(OBJECTS)
main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(LDLIBS) $@