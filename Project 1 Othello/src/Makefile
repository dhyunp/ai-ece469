.PHONY: clean debug run

othello.exe: main.o game.o board.o
	g++ -o othello.exe main.o game.o board.o

main.o: main.cpp game.h board.h
	g++ -c main.cpp

game.o: game.cpp game.h board.h properties.h
	g++ -c game.cpp

board.o: board.cpp board.h properties.h
	g++ -c board.cpp

debug:
	g++ -g -o othelloDebug.exe main.cpp game.cpp board.cpp

clean:
	rm -f *.exe *.o *.stackdump *~
