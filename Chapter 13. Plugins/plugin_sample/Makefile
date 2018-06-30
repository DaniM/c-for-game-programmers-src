#CXXFLAGS +=-std=c++11 -Wall -Iinc
CXXFLAGS +=-Wall -Iinc

.PHONY: clean

default: tests/test_plugins fooplugin

fooplugin: CXXFLAGS +=-DDLL -fpic
fooplugin: src/plugins/fooplugin.o
	mkdir -p src/plugins/build
	$(CXX) -shared -o src/plugins/build/$@.so $+
	cp src/plugins/build/$@.so build

tests/test_plugins: CXXFLAGS+=-g
tests/test_plugins: src/pluginmanager.o
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/test_plugins $@.cpp $+ -ldl


clean:
	rm -f src/plugins/build/*
	rm -f src/plugins/*.o
	rm -f src/*.o
	rm -f build/*

