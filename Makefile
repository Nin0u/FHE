CC=g++ -Wall -std=c++11
INCL=-Iinclude
CCO=$(CC) -c $(INCL) -o $@ $< 

OUT_DIR=@mkdir out -p
OUT=out/Main.o out/Polynomial.o out/SHE.o out/Algo.o out/Cipher.o out/Gen.o

TARGET=main

# ============= GLOBAL RULES ============ #

all: $(TARGET)

$(TARGET): $(OUT)
	$(CC) -o $(TARGET) $(OUT) -Iinclude -lgmp -lgmpxx

clean:
	rm -rf out/ $(TARGET)

# =============== OUTPUTS =============== #
out/Main.o: src/Main.cpp include/SHE.hpp include/Algo.hpp include/Cipher.hpp
	$(OUT_DIR)
	$(CCO)

out/SHE.o: src/SHE.cpp include/SHE.hpp include/Polynomial.hpp include/Algo.hpp include/Cipher.hpp
	$(OUT_DIR)
	$(CCO)

out/Polynomial.o: src/Polynomial.cpp include/Polynomial.hpp
	$(OUT_DIR)
	$(CCO)

out/Algo.o: src/Algo.cpp include/Algo.hpp include/Cipher.hpp
	$(OUT_DIR)
	$(CCO)

out/Cipher.o: src/Cipher.cpp include/Cipher.hpp include/SHE.hpp
	$(OUT_DIR)
	$(CCO)

out/Gen.o: src/Gen.cpp include/Gen.hpp include/Polynomial.hpp
	$(OUT_DIR)
	$(CCO)