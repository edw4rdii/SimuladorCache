CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
ALVO = simulador

all: $(ALVO)

$(ALVO): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build $(ALVO)

.PHONY: all clean
