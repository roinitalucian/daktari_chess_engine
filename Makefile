build: main.o engine.o utils.o
	g++ -o engine main.o engine.o utils.o
main.o: main.cpp
	g++ -c main.cpp
engine.o: engine.cpp engine.h
	g++ -c engine.cpp
utils.o: utils.cpp utils.h
	g++ -c utils.cpp

clean:
	rm *.o
	rm engine
	rm received_commands.txt

run: build
	../xboard -fcp ./engine -debug -pieceImageDirectory ../svg/
