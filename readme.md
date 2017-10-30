# Introduction
This repository contains a C++/OpenGL implementation of Othello(Reversi) game.
It is platform specific (Linux). You can remove nanosleep calls to make it platform independent.
# Installation
Make sure OpenGL is installed. For Linux:
	
	sudo apt-get install build-essential
	sudo apt-get install freeglut3 freeglut3-dev
	sudo apt-get install binutils-gold

Move to the cloned directory and do:

	make

Now, to run the game:
	
	./game
