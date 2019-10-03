dragonshell: builtin.o dragonshell.o
	g++ -o dragonshell builtin.o dragonshell.o

compile: builtin.o dragonshell.o

builtin.o: builtin.cc builtin.h
	g++ -c builtin.cc

dragonshell.o: dragonshell.cc
	g++ -c dragonshell.cc

clean:
	rm builtin.o dragonshell.o dragonshell

