CXXFLAGS = -g -Wall
exclude := 
source1 = $(wildcard **/*.cpp)
source = $(wildcard *.cpp) $(source1)
tests_src := $(wildcard main.cpp)
cpp := $(filter-out $(exclude),$(source))
cpp := $(filter-out $(tests_src),$(cpp))
obj := $(patsubst %.cpp,%.o,$(cpp))
tests := $(patsubst %.cpp,%,$(tests_src))

#LDLIBS :=
#LDFLAGS :=

.PHONY: clean

default: $(obj)

$(tests): %: $(obj) %.o
	$(CXX) $(CXXFLAGS) -v -o $@ $^
	
tests: $(tests)

clean:
	rm -f *.o
