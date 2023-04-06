CC=g++ -Wall -std=c++11
INCL=-Iinclude
CCO=$(CC) -c $(INCL) -o $@ $< 

OUT_DIR=@mkdir out -p
OUT=out/Main.o out/Polynomial.o out/SHE.o

TARGET=main

# ============= GLOBAL RULES ============ #

all: $(TARGET)

$(TARGET): $(OUT)
	$(CC) -o $(TARGET) $(OUT) -Iinclude -lgmp -lgmpxx

clean:
	rm -rf out/ $(TARGET)

# =============== OUTPUTS =============== #
out/Main.o: src/Main.cpp include/SHE.hpp
	$(OUT_DIR)
	$(CCO)

out/SHE.o: src/SHE.cpp include/SHE.hpp include/Polynomial.hpp
	$(OUT_DIR)
	$(CCO)

out/Polynomial.o: src/Polynomial.cpp include/Polynomial.hpp
	$(OUT_DIR)
	$(CCO)