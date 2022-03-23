CC=g++
CFLAGS=--std=c++14

all: decoder.cpp characternode.h
	$(CC) $(CFLAGS) decoder.cpp -o decoder

clean:
	rm -rf decoder
