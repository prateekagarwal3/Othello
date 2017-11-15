/*
 * board dimensions: 835*835
*/

#include <GL/glut.h>
#include <bits/stdc++.h>
#include <thread>
#include <time.h>
#define DIMENSION 8
//radii of circles
#define BIG_RADIUS 40.0
#define SMALL_RADIUS 27.0
//no. of line segments to use to draw the circle
#define BIG_SEGMENTS 5000
#define SMALL_SEGMENTS 3000
//depths for each difficulty level
#define EASY 2
#define MEDIUM 3
#define HARD 4
#define EXPERT 5
//sleep time between moves in nanoseconds
#define SLEEP_TIME 500000000L
#include "ai.h"

using namespace std;

int DEPTH = 5;

typedef vector<vector<int> > v2i;

bool gameStart;   //whether the game has started or not
bool gameFinish;  //whether the game has finished or not
bool thinking;	  //whether the computer is thinking or not
v2i defaultState(DIMENSION, vector<int> (DIMENSION, 0));
v2i state(DIMENSION, vector<int> (DIMENSION, 0));
v2i nextMove(DIMENSION, vector<int> (DIMENSION, 0));
bool displayNextMove = false;   //whether the mouse is at a location where move is possible
int nextMoveX, nextMoveY;      //coordinates of the highlighted move
bool nextMoveColor;

int currPlayer;  //the current player

const GLfloat vertices[DIMENSION][DIMENSION][4] = {  {{0.0,735.0,100.0,835.0},{105.0,735.0,205.0,835.0},{210.0,735.0,310.0,835.0},{315.0,735.0,415.0,835.0},{420.0,735.0,520.0,835.0},{525.0,735.0,625.0,835.0},{630.0,735.0,730.0,835.0},{735.0,735.0,835.0,835.0}},
													 {{0.0,630.0,100.0,730.0},{105.0,630.0,205.0,730.0},{210.0,630.0,310.0,730.0},{315.0,630.0,415.0,730.0},{420.0,630.0,520.0,730.0},{525.0,630.0,625.0,730.0},{630.0,630.0,730.0,730.0},{735.0,630.0,835.0,730.0}},
													 {{0.0,525.0,100.0,625.0},{105.0,525.0,205.0,625.0},{210.0,525.0,310.0,625.0},{315.0,525.0,415.0,625.0},{420.0,525.0,520.0,625.0},{525.0,525.0,625.0,625.0},{630.0,525.0,730.0,625.0},{735.0,525.0,835.0,625.0}},
													 {{0.0,420.0,100.0,520.0},{105.0,420.0,205.0,520.0},{210.0,420.0,310.0,520.0},{315.0,420.0,415.0,520.0},{420.0,420.0,520.0,520.0},{525.0,420.0,625.0,520.0},{630.0,420.0,730.0,520.0},{735.0,420.0,835.0,520.0}},
													 {{0.0,315.0,100.0,415.0},{105.0,315.0,205.0,415.0},{210.0,315.0,310.0,415.0},{315.0,315.0,415.0,415.0},{420.0,315.0,520.0,415.0},{525.0,315.0,625.0,415.0},{630.0,315.0,730.0,415.0},{735.0,315.0,835.0,415.0}},
													 {{0.0,210.0,100.0,310.0},{105.0,210.0,205.0,310.0},{210.0,210.0,310.0,310.0},{315.0,210.0,415.0,310.0},{420.0,210.0,520.0,310.0},{525.0,210.0,625.0,310.0},{630.0,210.0,730.0,310.0},{735.0,210.0,835.0,310.0}},	
													 {{0.0,105.0,100.0,205.0},{105.0,105.0,205.0,205.0},{210.0,105.0,310.0,205.0},{315.0,105.0,415.0,205.0},{420.0,105.0,520.0,205.0},{525.0,105.0,625.0,205.0},{630.0,105.0,730.0,205.0},{735.0,105.0,835.0,205.0}},
													 {{0.0,0.0,100.0,100.0},{105.0,0.0,205.0,100.0},{210.0,0.0,310.0,100.0},{315.0,0.0,415.0,100.0},{420.0,0.0,520.0,100.0},{525.0,0.0,625.0,100.0},{630.0,0.0,730.0,100.0},{735.0,0.0,835.0,100.0}}
												 };

const GLfloat centres[DIMENSION][DIMENSION][2] ={ {{50.0,785.0},{155.0,785.0},{260.0,785.0},{365.0,785.0},{470.0,785.0},{575.0,785.0},{680.0,785.0},{785.0,785.0}},
												  {{50.0,680.0},{155.0,680.0},{260.0,680.0},{365.0,680.0},{470.0,680.0},{575.0,680.0},{680.0,680.0},{785.0,680.0}},
												  {{50.0,575.0},{155.0,575.0},{260.0,575.0},{365.0,575.0},{470.0,575.0},{575.0,575.0},{680.0,575.0},{785.0,575.0}},
												  {{50.0,470.0},{155.0,470.0},{260.0,470.0},{365.0,470.0},{470.0,470.0},{575.0,470.0},{680.0,470.0},{785.0,470.0}},
												  {{50.0,365.0},{155.0,365.0},{260.0,365.0},{365.0,365.0},{470.0,365.0},{575.0,365.0},{680.0,365.0},{785.0,365.0}},
												  {{50.0,260.0},{155.0,260.0},{260.0,260.0},{365.0,260.0},{470.0,260.0},{575.0,260.0},{680.0,260.0},{785.0,260.0}},
												  {{50.0,155.0},{155.0,155.0},{260.0,155.0},{365.0,155.0},{470.0,155.0},{575.0,155.0},{680.0,155.0},{785.0,155.0}},
												  {{50.0,50.0},{155.0,50.0},{260.0,50.0},{365.0,50.0},{470.0,50.0},{575.0,50.0},{680.0,50.0},{785.0,50.0}}
												};
void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void printString(GLfloat x, GLfloat y, void *font, const char *str) { //renders a string in the window
	glRasterPos2f(x, y);
	int i = 0;
	while(str[i]) {
		glutBitmapCharacter(font, str[i]);
		i++;
	}
}

void display(void); //function declaration

void actualLocation(int a, int b, GLdouble &x, GLdouble &y, GLdouble &z) {  //takes in the pixels and returns actual unprojected position
	GLint viewport[4];
	GLdouble modelview[16], projection[16];
	GLdouble wx = a, wy, wz;
	glGetIntegerv(GL_VIEWPORT, viewport);
	b = viewport[3]-b;
	wy = b;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glReadPixels(a, b, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
	gluUnProject(wx, wy, wz, modelview, projection, viewport, &x, &y, &z);
}

void mouseHover(int x, int y) {       //callback function for passivemotion
	if(currPlayer != 2)
		return;
	GLdouble mouseX, mouseY, mouseZ;
	actualLocation(x, y, mouseX, mouseY, mouseZ);
	if(mouseX > 835.0 || mouseY > 835.0)
		return;
	int i = 7, j = 0;
	while(j<DIMENSION && mouseX > vertices[0][j][2])      // x is greater than the right x coordinate of the element
		j++;
	while(i>=0 && mouseY > vertices[i][0][3])
		i--;
	if(i<0 || i>7 || j<0 || j>7)
		return;
	if( nextMove[i][j] != 0) {
		if(displayNextMove && nextMoveX == i && nextMoveY == j  //check if the particular box is already being highlighted
			&& nextMoveColor == (nextMove[i][j]==1)?true:false)
			return;
		displayNextMove = true;
		nextMoveX = i;
		nextMoveY = j;
		nextMoveColor = (nextMove[i][j] == 1)? true:false;
		display();
	} else if(displayNextMove) {   //a box was being highlighted, stop now
		displayNextMove = false;
		display();
	}
}

void intToString(int i, string &str) {   //converts int to string
	if(i==0) {
		str = "0";
		return;
	}
	while(i>0) {
		str.push_back('0' + i%10); 
		i/=10;
	}
	reverse(str.begin(), str.end());
}

void restart(void) {        //restarts the game
	state = defaultState;
	glutMouseFunc(NULL);
	glutPassiveMotionFunc(NULL);
	currPlayer = 2;
	gameStart = gameFinish = thinking = false;
	display();
}

void resultMouseClick(int button, int mouse_state, int x, int y) {  //callback for mouse clicks on results screen
	if(button != GLUT_LEFT_BUTTON || mouse_state != GLUT_UP)
		return;
	GLdouble mouseX, mouseY, mouseZ;
	actualLocation(x, y, mouseX, mouseY, mouseZ);
	if(mouseX > 835.0 || mouseY > 835.0)
		return;
	if(mouseY > 300 || mouseY < 250)
		return;
	if( (mouseX>250 && mouseX<350) || (mouseX>450 && mouseX<550) ) {

		if(mouseX>250 && mouseX<350) {
			restart();
			return;
		}
		else if(mouseX>450 && mouseX<550)
			exit(0);
	}

}

void showResults(void) {        //utility function for display(), never call display() from it
	glColor4f(0.0,0.0,0.0,0.8);
	
	glBegin(GL_POLYGON);
		glVertex2f(0.0, 210.0);
		glVertex2f(0.0, 625.0);
		glVertex2f(835.0, 625.0);
		glVertex2f(835.0, 210.0);
	glEnd();

	glColor3f(0.98823529411, 0.82352941176, 0.32549019607);
	glBegin(GL_LINE_LOOP);  //Restart
		glVertex2f(250.0, 250.0);
		glVertex2f(250.0, 300.0);
		glVertex2f(350.0, 300.0);
		glVertex2f(350.0, 250.0);
	glEnd();
	glBegin(GL_LINE_LOOP);   //Quit
		glVertex2f(450.0, 250.0);
		glVertex2f(450.0, 300.0);
		glVertex2f(550.0, 300.0);
		glVertex2f(550.0, 250.0);
	glEnd();
	
	int compDisks = disks(state, DIMENSION, 1);
	int humanDisks = disks(state, DIMENSION, 2);
	if(compDisks > humanDisks)
		printString(310.0, 500.0, GLUT_BITMAP_TIMES_ROMAN_24, "YOU LOSE");
	else if(compDisks < humanDisks)
		printString(310.0, 500.0, GLUT_BITMAP_TIMES_ROMAN_24, "YOU WIN");
	else
		printString(310.0, 500.0, GLUT_BITMAP_TIMES_ROMAN_24, "DRAW");
	string comp, human;
	intToString(compDisks, comp);
	intToString(humanDisks, human);
	string print = human + " - " + comp;
	printString(325.0, 400.0, GLUT_BITMAP_TIMES_ROMAN_24, print.c_str());
	printString(260.0, 260.0, GLUT_BITMAP_HELVETICA_12, "RESTART");
	printString(460.0, 260.0, GLUT_BITMAP_HELVETICA_12, "QUIT");
}

// void computerMoves(void) {  //this function makes computer move while human has no moves remaining
// 						//call this function just after human plays and change in state has happened
// 	v2i copy(DIMENSION, vector<int> (DIMENSION));  //unused
// 	int humanMoves, compMoves;
// 	loadNextMoves(state, DIMENSION, 2, copy, humanMoves);  //precomputing human moves for this state

// 	//number of availabe human moves is always 0 in this do-while loop (except in the first case)
// 	do {
		
// 		loadNextMoves(state, DIMENSION, 1, copy, compMoves);
// 		if(humanMoves == 0 && compMoves == 0) { //end the program if no moves are left for either player
// 			gameFinish = true;
// 			display();
// 			break;
// 		}
		
// 	    thinking = true;
// 	    display();
// 	    if(nanosleep((const struct timespec[]){{0, SLEEP_TIME}}, NULL) < 0)   
// 	    {
// 	  		printf("Nano sleep system call failed \n");
// 	    	exit(0);
// 	    }
// 		minimaxDecision(state, DIMENSION, 1, DEPTH);  //changes state
// 		thinking = false;
// 		loadNextMoves(state, DIMENSION, 2, nextMove, humanMoves); // loading the next available human moves
		
// 		display();
// 		currPlayer = 2;      //allows next moves to be shown
	
// 	} while(humanMoves == 0);
// }

void computerMoves(void) {  //this function makes computer move while human has no moves remaining
						//call this function just after human plays and change in state has happened
	v2i copy(DIMENSION, vector<int> (DIMENSION));  //unused
	int humanMoves, compMoves;
	loadNextMoves(state, DIMENSION, 2, copy, humanMoves);  //precomputing human moves for this state

	//number of availabe human moves is always 0 in this do-while loop (except in the first case)
	do {
		
		loadNextMoves(state, DIMENSION, 1, copy, compMoves);
		if(humanMoves == 0 && compMoves == 0) { //end the program if no moves are left for either player
			gameFinish = true;
			display();
			break;
		}
		
	    thinking = true;
	    display();
	    if(nanosleep((const struct timespec[]){{0, SLEEP_TIME}}, NULL) < 0)   
	    {
	  		printf("Nano sleep system call failed \n");
	    	exit(0);
	    }
		minimaxDecision(state, DIMENSION, 1, DEPTH);  //changes state
		thinking = false;
		loadNextMoves(state, DIMENSION, 2, nextMove, humanMoves); // loading the next available human moves
		
		display();
		currPlayer = 2;      //allows next moves to be shown
	
	} while(humanMoves == 0);
}

void mouseClick(int button, int mouse_state, int x, int y) { //callback for mouse clicks
	if(button != GLUT_LEFT_BUTTON || mouse_state != GLUT_UP || currPlayer != 2)
		return;
	GLdouble mouseX, mouseY, mouseZ;
	actualLocation(x, y, mouseX, mouseY, mouseZ);
	if(mouseX > 835.0 || mouseY > 835.0)
		return;
	int i = 7, j = 0;
	while(j<DIMENSION && mouseX > vertices[0][j][2])   // x is greater than the right x coordinate of the element
		j++;
	while(i>=0 && mouseY > vertices[i][0][3])
		i--;
	if(i<0 || i>7 || j<0 || j>7 || nextMove[i][j] != 2)
		return;

	currPlayer = 1;  //disable showing of next moves
	displayNextMove = false;
	playMove(state, DIMENSION, 2, i, j);
	display();
	if(nanosleep((const struct timespec[]){{0, SLEEP_TIME}}, NULL) < 0)   
    {
  		printf("Nano sleep system call failed \n");
    	exit(0);
    }
	computerMoves();
}

void simulate() {
	v2i copy(DIMENSION, vector<int>(DIMENSION));
	int playerMoves, iterationsWithoutMove = 0;
	while(true) {
		loadNextMoves(state, DIMENSION, currPlayer, copy, playerMoves);
		if(playerMoves == 0) {
			iterationsWithoutMove++;
			if(iterationsWithoutMove >= 2) { //both players can't move, stop the game
				gameFinish = true;
				//display();
				return;
			}
			continue;
		}
		thinking = true;
	    copy = state;
	    thread ai_thread(minimaxDecision, ref(copy), DIMENSION, currPlayer, DEPTH);
	    //display();
	    if(nanosleep((const struct timespec[]){{0, SLEEP_TIME}}, NULL) < 0)   
	    {
	  		printf("Nano sleep system call failed \n");
	    	exit(0);
	    }
	    ai_thread.join();
	    thinking = false;    //since thread is joined, computation must be complete at this point
	    state = copy;
	    //minimaxDecision(state, DIMENSION, currPlayer, DEPTH);  //changes state

		//display();
		if(nanosleep((const struct timespec[]){{0, SLEEP_TIME}}, NULL) < 0)   
	    {
	  		printf("Nano sleep system call failed \n");
	    	exit(0);
	    }
		currPlayer = (currPlayer==1) ? 2 : 1;
	}
}

void selectOption(int optionSelected) {   //right click option menu
	if(optionSelected == 2)   //quit
		exit(0);
	if(optionSelected == 1) { //autoplay
		if(currPlayer != 2)
			return;
		currPlayer = 1;
		displayNextMove = false;

		thinking = true;
		display();
		if(nanosleep((const struct timespec[]){{0, SLEEP_TIME}}, NULL) < 0)   
	    {
	  		printf("Nano sleep system call failed \n");
	    	exit(0);
	    }
		minimaxDecision(state, DIMENSION, 2, DEPTH);
		thinking = false;
		display();
		if(nanosleep((const struct timespec[]){{0, SLEEP_TIME}}, NULL) < 0)   
	    {
	  		printf("Nano sleep system call failed \n");
	    	exit(0);
	    }
		computerMoves();
		return;
	}
	if(optionSelected == 4) {  //simulation
		if(currPlayer != 2)
			return;
		glutMouseFunc(NULL);
		glutPassiveMotionFunc(NULL);
		thread simulation(simulate);
		simulation.detach();
		//simulate();
		//simulation.join();
		return;
	}
	if(optionSelected == 3) {  //restart
		restart();
		return;
	}
}

void DrawCircle(GLfloat cx, GLfloat cy, GLfloat r, int num_segments) {  //renders a circle onto the screen
	float theta = 2 * 3.1415926 / (GLfloat)num_segments; 
	float tangetial_factor = tanf(theta);//calculate the tangential factor 
	float radial_factor = cosf(theta);//calculate the radial factor 	
	GLfloat x = r;//we start at angle = 0 
	GLfloat y = 0; 
    
	glBegin(GL_POLYGON);
	bool shade = false;
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 
		float tx = -y; 
		float ty = x; 
		//add the tangential vector 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		//correct using the radial factor 
		x *= radial_factor; 
		y *= radial_factor; 
		if(!shade && ii > 2*num_segments/3) {
			shade = true;
			glColor3f(0.86274509803, 0.87058823529, 0.85098039215);
		}
	} 
	glEnd(); 
}

void welcomeMouseClick(int button, int mouse_state, int x, int y) {  //mouse click callback on welcome screen
	if(button != GLUT_LEFT_BUTTON || mouse_state != GLUT_UP || gameStart == true)
		return;
	GLdouble mouseX, mouseY, mouseZ;
	actualLocation(x, y, mouseX, mouseY, mouseZ);
	if(mouseX > 835.0 || mouseY > 835.0)
		return;
	if(mouseY > 300 || mouseY < 250)
		return;
	if( (mouseX>50 && mouseX<150) || (mouseX>250 && mouseX<350)
		|| (mouseX>450 && mouseX<550) || (mouseX>650 && mouseX<750) ) {

		if(mouseX>50 && mouseX<150)
			DEPTH = EASY;
		else if(mouseX>250 && mouseX<350)
			DEPTH = MEDIUM;
		else if(mouseX>450 && mouseX<550)
			DEPTH = HARD;
		else
			DEPTH = EXPERT;

		glutMouseFunc(mouseClick);
		glutPassiveMotionFunc(mouseHover);
		
		int unused;  //for next function call
		loadNextMoves(state, DIMENSION, currPlayer, nextMove, unused);
		//initialization ends
		
		gameStart = true;
		thinking = false;
		gameFinish = false;

		display();
	}

}

void showWelcomeScreen(void) {      //utility function for display(), never call display() from it
	glColor4f(0.0,0.0,0.0,0.8);
	
	glBegin(GL_POLYGON);
		glVertex2f(0.0, 210.0);
		glVertex2f(0.0, 625.0);
		glVertex2f(835.0, 625.0);
		glVertex2f(835.0, 210.0);
	glEnd();
	
	glColor3f(0.98823529411, 0.82352941176, 0.32549019607);
	glBegin(GL_LINE_LOOP); //easy
		glVertex2f(50.0, 250.0);
		glVertex2f(50.0, 300.0);
		glVertex2f(150.0, 300.0);
		glVertex2f(150.0, 250.0);
	glEnd();
	glBegin(GL_LINE_LOOP);  //medium
		glVertex2f(250.0, 250.0);
		glVertex2f(250.0, 300.0);
		glVertex2f(350.0, 300.0);
		glVertex2f(350.0, 250.0);
	glEnd();
	glBegin(GL_LINE_LOOP);   //hard
		glVertex2f(450.0, 250.0);
		glVertex2f(450.0, 300.0);
		glVertex2f(550.0, 300.0);
		glVertex2f(550.0, 250.0);
	glEnd();
	glBegin(GL_LINE_LOOP);   //expert
		glVertex2f(650.0, 250.0);
		glVertex2f(650.0, 300.0);
		glVertex2f(750.0, 300.0);
		glVertex2f(750.0, 250.0);
	glEnd();
	printString(50.0, 360.0, GLUT_BITMAP_HELVETICA_18, "Choose the difficulty level:");
	printString(310.0, 500.0, GLUT_BITMAP_TIMES_ROMAN_24, "OTHELLO");
	printString(60.0, 260.0, GLUT_BITMAP_HELVETICA_12, "EASY");
	printString(260.0, 260.0, GLUT_BITMAP_HELVETICA_12, "MEDIUM");
	printString(460.0, 260.0, GLUT_BITMAP_HELVETICA_12, "HARD");
	printString(660.0, 260.0, GLUT_BITMAP_HELVETICA_12, "EXPERT");
}

void display(void) {       //callback for display
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.09411764705, 0.52156862745, 0.09411764705);
	for(int i = 0; i<DIMENSION; i++) {
		bool dark;
		if(i%2==0) {	
			dark = true;
			glColor3f(0.14509803921, 0.41176470588, 0.15294117647);
		} else {
			dark = false;
			glColor3f(0.11764705882, 0.58823529411, 0.13333333333);
		}
		for(int j = 0; j<DIMENSION; j++) {
			glBegin(GL_POLYGON);
				glVertex2f(vertices[i][j][0], vertices[i][j][1]);
				glVertex2f(vertices[i][j][0], vertices[i][j][3]);
				glVertex2f(vertices[i][j][2], vertices[i][j][3]);
				glVertex2f(vertices[i][j][2], vertices[i][j][1]);
			glEnd();
			if(state[i][j] == 1) {
				glColor3f(0.5294117647, 0.5294117647, 0.5294117647);
				DrawCircle(centres[i][j][0], centres[i][j][1], BIG_RADIUS, BIG_SEGMENTS);
				glColor3f(0.09411764705, 0.52156862745, 0.09411764705);
			} else if(state[i][j] == 2) {
				glColor3f(0.14117647058, 0.16078431372, 0.09803921568);
				DrawCircle(centres[i][j][0], centres[i][j][1], BIG_RADIUS, BIG_SEGMENTS);
				glColor3f(0.09411764705, 0.52156862745, 0.09411764705);
			} 
			if(dark) {
				dark = false;
				glColor3f(0.11764705882, 0.58823529411, 0.13333333333);
			} else {
				dark = true;
				glColor3f(0.14509803921, 0.41176470588, 0.15294117647);
			}
		}
	}
	if(displayNextMove) {
		glColor4f(0.13725490196, 0.29019607843, 0.16862745098, 0.95);
		DrawCircle(centres[nextMoveX][nextMoveY][0], centres[nextMoveX][nextMoveY][1], BIG_RADIUS, BIG_SEGMENTS);
	}
	if(!gameStart) {
		showWelcomeScreen();
		glutMouseFunc(welcomeMouseClick);
	}
	if(gameFinish) {
		showResults();
		glutMouseFunc(resultMouseClick);
	}
	if(thinking) {
		glColor4f(0.87843137254, 0.85882352941, 0.21960784313, 0.4);
		glBegin(GL_POLYGON);
			glVertex2f(1.0, 600.0);
			glVertex2f(1.0, 834.0);
			glVertex2f(300.0, 834.0);
			glVertex2f(300.0, 600.0);
		glEnd();
		glColor3f(0.19215686274, 0.14117647058, 0.45098039215);
		printString(110.0, 750.0, GLUT_BITMAP_TIMES_ROMAN_24, "?");
		printString(40.0, 680.0, GLUT_BITMAP_TIMES_ROMAN_24, "THINKING");
	}
	glutSwapBuffers();
}

void reshape(int w, int h) { //callback function for reshape
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//projecting according to the window size while preserving the aspect ratio
	if (w <= h)
 		gluOrtho2D(0.0, 835.0, 0.0, 835.0*(GLfloat) h/(GLfloat) w);
 	else
 		gluOrtho2D(0.0, 835.0*(GLfloat) w/(GLfloat) h, 0.0, 835.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idleFunc(void) {
	display();
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(550,550);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Othello Game");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idleFunc);
	
	//initialization of the game
	defaultState[3][3] = 1;
	defaultState[3][4] = 2;
	defaultState[4][3] = 2;
	defaultState[4][4] = 1;
	state = defaultState;
	currPlayer = 2;
	gameStart = gameFinish = thinking = false;

	//the right click menu follows
	glutCreateMenu(selectOption);
	glutAddMenuEntry("Simulate", 4);
	glutAddMenuEntry("Autoplay", 1);
	glutAddMenuEntry("Restart", 3);
	glutAddMenuEntry("Quit",2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}