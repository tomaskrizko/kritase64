COMPILER=clang++

SRC=src/*.cpp
INCLUDE=-Iinclude

FLAGS=-std=c++17 -Wall
DEBUG_FLAGS=-O0 -g -fsanitize=undefined

OUTPUT=kritase64

all : $(SRC)
	$(COMPILER) $(SRC) $(INCLUDE) $(FLAGS) -o $(OUTPUT)
debug : $(SRC)
	$(COMPILER) $(SRC) $(INCLUDE) $(FLAGS) $(DEBUG_FLAGS) -o $(OUTPUT)