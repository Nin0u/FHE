CC=g++ -Wall
INCL=-Iinclude
CCO=$(CC) $(INCL) -o $@ $< 

OUT_DIR=@mkdir out -p
OUT=out/Polynomial.o

TARGET=main

# ============= GLOBAL RULES ============ #

all: $(TARGET)

$(TARGET): $(OUT)
	$(CC) -o $(TARGET) $(OUT)

clean:
	rm -rf out/

# =============== OUTPUTS =============== #
out/Polynomial.o: src/Polynomial.cpp include/Polynomial.hpp
	$(OUT_DIR)
	$(CCO)