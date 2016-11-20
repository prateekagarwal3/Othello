#include <GL/glut.h>
#include <bits/stdc++.h>
#define DIMENSION 8
#define BIG_RADIUS 33.0
#define SMALL_RADIUS 15.0
#include "ai.h"

using namespace std;

typedef vector<vector<int> > v2i;

v2i state(DIMENSION, vector<int> (DIMENSION, 0));
v2i nextMove(DIMENSION, vector<int> (DIMENSION, 0));
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
}

void DrawCircle(GLfloat cx, GLfloat cy, GLfloat r, int num_segments, bool color) {
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

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.09411764705, 0.52156862745, 0.09411764705);
	for(int i = 0; i<DIMENSION; i++) {
		for(int j = 0; j<DIMENSION; j++) {
			glBegin(GL_POLYGON);
				glVertex2f(vertices[i][j][0], vertices[i][j][1]);
				glVertex2f(vertices[i][j][0], vertices[i][j][3]);
				glVertex2f(vertices[i][j][2], vertices[i][j][3]);
				glVertex2f(vertices[i][j][2], vertices[i][j][1]);
			glEnd();
			if(state[i][j] == 1) {
				glColor3f(0.5294117647, 0.5294117647, 0.5294117647);
				DrawCircle(centres[i][j][0], centres[i][j][1], BIG_RADIUS, 2000, true);
				glColor3f(0.09411764705, 0.52156862745, 0.09411764705);
			} else if(state[i][j] == 2) {
				glColor3f(0.14117647058, 0.16078431372, 0.09803921568);
				DrawCircle(centres[i][j][0], centres[i][j][1], BIG_RADIUS, 2000, false);
				glColor3f(0.09411764705, 0.52156862745, 0.09411764705);
			} else if(nextMove[i][j] == 1) {
				glColor3f(0.5294117647, 0.5294117647, 0.5294117647);
				DrawCircle(centres[i][j][0], centres[i][j][1], SMALL_RADIUS, 1000, true);
				glColor3f(0.09411764705, 0.52156862745, 0.09411764705);
			} else if(nextMove[i][j] == 2) {
				glColor3f(0.14117647058, 0.16078431372, 0.09803921568);
				DrawCircle(centres[i][j][0], centres[i][j][1], SMALL_RADIUS, 1000, false);
				glColor3f(0.09411764705, 0.52156862745, 0.09411764705);
			}
		}
	}
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
 		gluOrtho2D(0.0, 835.0, 0.0, 835.0*(GLfloat) h/(GLfloat) w);
 	else
 		gluOrtho2D(0.0, 835.0*(GLfloat) w/(GLfloat) h, 0.0, 835.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(550,550);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Othello Game");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	state[0][0] = 1;
	state[0][1] = 2;
	nextMove[0][3] = 1;
	nextMove[0][4] = 2;
	glutMainLoop();
	return 0;
}