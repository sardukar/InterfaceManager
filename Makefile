# Makefile for Hello World project

main: main.o InterfaceManager.o
	g++ -o main main.o InterfaceManager.o

main.o: main.cpp
	g++ -c main.cpp

InterfaceManager.o: InterfaceManager.cpp
	g++ -c InterfaceManager.cpp

clean:
	rm -f *.o main
