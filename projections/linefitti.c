/*
 * ConnectDots.c
 *
 *     This program draws straight lines connecting dots placed with mouse clicks.
 *
 * Author: David Gomez
 * Based in the work of: Samuel R. Buss
 *	http://www.math.ucsd.edu/~sbuss/MathCG/OpenGLsoft/ConnectDots/ConnectDots.html
 *
 *   Press "d" to remove the first control point
 *	 Press "f" to remove the last control point.
 *	 Press " [space]" to remove all points.
 *	 Press escape to exit.
 */

#include "linefitti.h"

#include <jack/jack.h>
#include "libol.h"

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h> //M_PI

#define MAXPTS 500

#define INACTIVE GLUT_UP 
#define ACTIVE GLUT_DOWN

float PointArray[MAXPTS][3]; // 0: x, 1: y, 2: state
int NumPts = 0; // Point stack pointer

// Window size in pixels
int WindowHeight;
int WindowWidth;

// Key, mouse X, mouse Y
void myKeyboardFunc (unsigned char key, int x, int y){
	switch (key) {
	case ' ':
		removeAllPoints();
		glutPostRedisplay();
		break;
	case 'd':
		removeFirstPoint();
		glutPostRedisplay();
		break;
	case 'f':
		removeLastPoint();
		glutPostRedisplay();
		break;
	case 27:			// Escape key
		exit(0);
		break;
	}
}

void removeFirstPoint() {
	int i;
	if ( NumPts>0 ) {
		// Remove the first point, slide the rest down
		NumPts--;
		for ( i=0; i<NumPts; i++ ) {
			PointArray[i][0] = PointArray[i+1][0];
			PointArray[i][1] = PointArray[i+1][1];
		}
	}
}

// Left button presses place a control point.
void mouseManager( int button, int state, int x, int y ) {
	printf("button: %d, state: %d\n",button, state);

	float xPos = ((float)x-(float)(WindowWidth)/2)/((float)(WindowWidth)/2);
	float yPos = ((float)y)/((float)(WindowHeight)/2);

	yPos = 1.0f-yPos;			// Flip value since y position is from top row.

	if(state==GLUT_DOWN){
		addNewPoint( xPos, yPos, INACTIVE );

	} else {
		addNewPoint( xPos, yPos, INACTIVE );
	}
	glutPostRedisplay();
}

// Left button presses place a control point.
void mouseMovedPressed( int x, int y ) {
	float xPos = ((float)x-(float)(WindowWidth)/2)/((float)(WindowWidth)/2);
	float yPos = ((float)y)/((float)(WindowHeight)/2);

	yPos = 1.0f-yPos;			// Flip value since y position is from top row.
	
	addNewPoint( xPos, yPos, ACTIVE );
	
	glutPostRedisplay();
}

// Left button presses place a control point.
void mouseMoved( int x, int y ) {
	
	float xPos = ((float)x-(float)(WindowWidth)/2)/((float)(WindowWidth)/2);
	float yPos = ((float)y)/((float)(WindowHeight)/2);
	//printf("x: %d, y: %d -- xPos: %f, yPos: %f\n",x,y,xPos,yPos);
	yPos = 1.0f-yPos;			// Flip value since y position is from top row.
	
	glutPostRedisplay();
}

// Decrease the pointer
void removeLastPoint() {
	if ( NumPts>0 ) {
		PointArray[NumPts][2] = INACTIVE;
		NumPts--;
	}
}

// Move the pointer to the starting position
void removeAllPoints() {
	int i;
	for(i=0;i<MAXPTS;i++){
		PointArray[i][0] = 0;
		PointArray[i][1] = 0;
		PointArray[i][2] = INACTIVE;

	}
	NumPts = 0;
}

// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void addNewPoint( float x, float y, int state ) {
	if ( NumPts>=MAXPTS ) {
		NumPts = 0;
		//removeFirstPoint();
	}

	PointArray[NumPts][0] = x;
	PointArray[NumPts][1] = y;
	PointArray[NumPts][2] = state;

	//PointArray[NumPts+1][2] = INACTIVE;

	printf("%d) x: %f, y: %f, st: %d\n",NumPts,x,y,state);

	NumPts++;
}


	
void displayLines(void){
	int i;

	glClear(GL_COLOR_BUFFER_BIT);
	olLoadIdentity();
	
	if ( NumPts>1 ) {
		glBegin( GL_LINE_STRIP );
		olBegin(OL_LINESTRIP);
		for ( i=0; i<MAXPTS; i++ ) {
			if(PointArray[i][2] == INACTIVE || i == NumPts || i == NumPts-1){
				glColor3f(0.0f, 0.0f, 0.0f); // Black line
				olVertex( PointArray[i][0], PointArray[i][1], C_BLACK );

			} else {
				glColor3f(1.0f, 1.0f, 1.0f); // White line
				olVertex( PointArray[i][0], PointArray[i][1], C_WHITE );
			}

			glVertex2f( PointArray[i][0], PointArray[i][1] );
		}
		olEnd();
		glEnd();
	}
	olRenderFrame(60);

	glFlush();
}

void initRendering() {
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); // Black background
	glLineWidth(1); // Line width

	//glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
}

void resizeWindow(int w, int h){
	WindowHeight = (h>1) ? h : 2;
	WindowWidth = (w>1) ? w : 2;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1,1,-1,1);  // Always view [0,1]x[0,1].
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv){
	OLRenderParams params;

	memset(&params, 0, sizeof params);
	params.rate = 48000;
	params.on_speed = 2.0/100.0;
	params.off_speed = 2.0/20.0;
	params.start_wait = 8;
	params.start_dwell = 3;
	params.curve_dwell = 0;
	params.corner_dwell = 8;
	params.curve_angle = cosf(30.0*(M_PI/180.0)); // 30 deg
	params.end_dwell = 3;
	params.end_wait = 7;
	params.snap = 1/100000.0;
	params.render_flags = RENDER_GRAYSCALE;

	if(olInit(3, 30000) < 0)
		return 1;

	olSetRenderParams(&params);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB ); 
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(1800, 200);

	glutCreateWindow("Le Lineffiti"); // Create window
	initRendering();

	glutDisplayFunc(displayLines);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(myKeyboardFunc);

	glutMouseFunc(mouseManager);
	glutMotionFunc(mouseMovedPressed);
	glutPassiveMotionFunc(mouseMoved);

	glutMainLoop();
	//glutMainLoopEvent();

	return 0;					// This line is never reached
}
