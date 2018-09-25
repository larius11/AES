CC = g++
CFLAGS = -std=c++11
CLANG-FORMAT := clang-format-3.8

all: aes

aes: aes.c++
	$(CC) $(CFLAGS) aes.c++ -o aes
	
format:
	$(CLANG-FORMAT) -i aes.c++

clean: 
	$(RM) aes *.o