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
	rm *.debug

run:
	./engine

check:
	../xboard -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 10 -sgf partide.txt -reuseFirst false -pieceImageDirectory ../svg/ -debug

test: build
	../xboard -fcp ./engine -debug -pieceImageDirectory ../svg/