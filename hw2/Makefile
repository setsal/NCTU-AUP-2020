.PHONY: clean

CXX = g++
CXXFLAGS = -Wall -g
BIN = sandbox
LIB = sandbox.so

all: sandbox sandbox.so sample sample2

sandbox: main.cpp
	$(CXX) -o $@ $(CXXFLAGS) $<

sandbox.so: sandbox.cpp
	$(CXX) -o $@ $(CXXFLAGS) $< --shared -fPIC -ldl

sample: sample.c
	gcc -o $@ $<	

sample2: sample.c
	gcc -o $@ $< -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

clean:
	rm -f $(BIN) $(LIB) sample sample2