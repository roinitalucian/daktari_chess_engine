build: main.o engine.o utils.o minimax.o debugging.o
	g++ -o engine main.o engine.o utils.o minimax.o debugging.o
main.o: main.cpp
	g++ -c main.cpp
engine.o: engine.cpp engine.h
	g++ -c engine.cpp
utils.o: utils.cpp utils.h
	g++ -c utils.cpp
minimax.o: minimax.cpp minimax.h
	g++ -c minimax.cpp
debugging.o: debugging.cpp debugging.h
	g++ -c debugging.cpp

clean:
	rm -f *.o
	rm -f engine
	rm -f *.txt
	rm -f *.debug

run:
	./engine

check:
	../xboard -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 1 -sgf partide.txt -reuseFirst false -pieceImageDirectory ../svg/ -debug

check2:
	../xboard -scp "make run" -fcp "fairymax" -firstInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 1 -sgf partide.txt -reuseFirst false -pieceImageDirectory ../svg/ -debug

check10:
	../xboard -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 10 -sgf partide.txt -reuseFirst false -pieceImageDirectory ../svg/ -debug

test: build
	../xboard -fcp ./engine -debug -pieceImageDirectory ../svg/