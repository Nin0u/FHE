CC=g++ -Wall -std=c++11
INCL=-Iinclude
CCO=$(CC) -c $(INCL) -o $@ $< 

OUT_DIR=@mkdir out -p
OUT=out/Polynomial.o out/Main.o out/UnboundedInt.o

TARGET=main

# ============= GLOBAL RULES ============ #

all: $(TARGET)

$(TARGET): $(OUT)
	$(CC) -o $(TARGET) $(OUT)

clean:
	rm -rf out/

# =============== OUTPUTS =============== #
out/Main.o: src/Main.cpp include/Polynomial.hpp
	$(OUT_DIR)
	$(CCO)

out/Polynomial.o: src/Polynomial.cpp include/Polynomial.hpp
	$(OUT_DIR)
	$(CCO)

out/UnboundedInt.o: src/UnboundedInt.cpp include/UnboundedInt.hpp
	$(OUT_DIR)
	$(CCO)