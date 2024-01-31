COMPILER=clang++

SRC=src/*.cpp
INCLUDE=-Iinclude

FLAGS=-std=c++17 -Wall

OUTPUT=kritase64

all : $(SRC)
	$(COMPILER) $(SRC) $(INCLUDE) $(FLAGS) -o $(OUTPUT)