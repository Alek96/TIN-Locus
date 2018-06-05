INC=-Iinclude
LIB=lib/libcryptopp.a

all: Test genServerKeys

Test: testProgram.o Crypto.o
	g++ testProgram.o Crypto.o $(LIB) -o test

Crypto.o: Crypto.cpp Crypto.h
	g++ $(INC) -c Crypto.cpp -std=c++11

testProgram.o: testProgram.cpp Crypto.h
	g++ $(INC) -c testProgram.cpp -std=c++11

genServerKeys: genServerKeys.o
	g++ genServerKeys.o $(LIB) -o genServerKeys

genServerKeys.o: genServerKeys.cpp
	g++ $(INC) -c genServerKeys.cpp

clean:
	rm -f test Crypto.o testProgram.o genServerKeys.o