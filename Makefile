all: mrproper main

main: main.o
	g++ -g -o main main.o -lm

main.o: main.cpp
	g++ -g -c -o main.o main.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf main