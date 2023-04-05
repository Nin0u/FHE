CC=g++ -Wall -std=c++11
FLAGS = -lm -O3 -Wall -Wextra -Wpedantic
INCL=-Iinclude
CCO=$(CC) -c $(INCL) -o $@ $< 

OUT_DIR=@mkdir out -p
OUT=out/Main.o out/BigInt.o out/Polynomial.o

TARGET=main

# ============= GLOBAL RULES ============ #

all: $(TARGET)

$(TARGET): $(OUT)
	$(CC) -o $(TARGET) $(OUT)

clean:
	rm -rf out/ $(TARGET)

# =============== OUTPUTS =============== #
out/Main.o: src/Main.cpp include/BigInt.hpp include/Polynomial.hpp
	$(OUT_DIR)
	$(CCO)

out/BigInt.o: src/bigint.c include/BigInt.hpp include/bigint.h
	$(OUT_DIR)
	gcc -c src/bigint.c $(INCL) -o out/BigInt.o $(FLAGS)

out/Polynomial.o: src/Polynomial.cpp include/Polynomial.hpp include/BigInt.hpp
	$(OUT_DIR)
	$(CCO)