.PHONY: clean

CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
BIN = hw1

all: $(BIN)

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $<

clean:
	rm -f $(BIN)