enigma: main.o enigma.o
	g++ main.o enigma.o -o enigma

enigma.o: enigma.cpp enigma.h
	g++ -Wall -g -c enigma.cpp

main.o: main.cpp enigma.h
	g++ -Wall -g -c main.cpp