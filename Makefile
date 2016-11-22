CC = g++
LDFLAGS = -Wall
GLUFLAGS = -lGL -lGLU -lglut

game : AI.o othello.o
	$(CC) $(LDFLAGS) AI.o othello.o -o game $(GLUFLAGS)
AI.o : AI.cpp
	$(CC) $(LDFLAGS) -c AI.cpp
othello.o : othello.cpp ai.h
	$(CC) $(LDFLAGS) -c othello.cpp $(GLUFLAGS)

clean : 
	rm othello.o AI.o game
