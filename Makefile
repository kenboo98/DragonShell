dragonshell: builtin.o dragonshell.o
	g++ -Wall -o dragonshell builtin.o dragonshell.o

compile: builtin.o dragonshell.o

builtin.o: builtin.cc builtin.h
	g++ -Wall -c builtin.cc

dragonshell.o: dragonshell.cc
	g++ -Wall -c dragonshell.cc

clean:
	rm builtin.o dragonshell.o dragonshell

