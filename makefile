 
CC=g++
CFLAGS=-c -Wall -std=c++11 -fexceptions

main: main.o utf8-util.o
		$(CC) -o main main.o utf8-util.o

main.o: main.cpp utf8-util/utf8-util.h
		$(CC) $(CFLAGS) main.cpp

utf8-util.o: utf8-util/utf8-util.cpp utf8-util/utf8-util.h
		$(CC) $(CFLAGS) utf8-util/utf8-util.cpp

run: main
		./main

clean: 
		rm *.o
		rm main
    
    