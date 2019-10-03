dragonshell: builtin.o dragonshell.o
	g++ -std=c++11 -Wall -o dragonshell builtin.o dragonshell.o

compile: builtin.o dragonshell.o

builtin.o: builtin.cc builtin.h
	g++  -std=c++11 -Wall -c builtin.cc

dragonshell.o: dragonshell.cc
	g++  -std=c++11 -Wall -c dragonshell.cc

clean:
	rm builtin.o dragonshell.o dragonshell

