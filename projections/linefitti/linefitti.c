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
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

#define MAXPTS 3000

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

	float xPos = ((float)x)/((float)(WindowWidth-1));
	float yPos = ((float)y)/((float)(WindowHeight-1));

	yPos = 1.0f-yPos;			// Flip value since y position is from top row.

	if(state==GLUT_DOWN){
		addNewPoint( xPos, yPos, INACTIVE );
		/*
		addNewPoint( xPos, yPos, INACTIVE );
		addNewPoint( xPos, yPos, INACTIVE );
		addNewPoint( xPos, yPos, ACTIVE );
		*/
	} else {
		addNewPoint( xPos, yPos, INACTIVE );
	}
	glutPostRedisplay();

	/*
	if ( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN ) {
		float xPos = ((float)x)/((float)(WindowWidth-1));
		float yPos = ((float)y)/((float)(WindowHeight-1));

		yPos = 1.0f-yPos;			// Flip value since y position is from top row.

		addNewPoint( xPos, yPos, ACTIVE );
		glutPostRedisplay();
	}
	*/
}

// Left button presses place a control point.
void mouseMovedPressed( int x, int y ) {
	float xPos = ((float)x)/((float)(WindowWidth-1));
	float yPos = ((float)y)/((float)(WindowHeight-1));

	yPos = 1.0f-yPos;			// Flip value since y position is from top row.
	
	addNewPoint( xPos, yPos, ACTIVE );
	
	glutPostRedisplay();
}

// Left button presses place a control point.
void mouseMoved( int x, int y ) {
	float xPos = ((float)x)/((float)(WindowWidth-1));
	float yPos = ((float)y)/((float)(WindowHeight-1));

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

	printf("%d) x: %f, y: %f, st: %d\n",NumPts,x,y,state);

	NumPts++;
	
}

void displayLines(void){
	int i;

	glClear(GL_COLOR_BUFFER_BIT);

	
	if ( NumPts>1 ) {
		glBegin( GL_LINE_STRIP );
		for ( i=0; i<MAXPTS; i++ ) {
			if(PointArray[i][2] == INACTIVE || i == NumPts || i == NumPts-1)
				glColor3f(0.0f, 0.0f, 0.0f); // Black line
			else
				glColor3f(1.0f, 1.0f, 1.0f); // White line

				

			glVertex2f( PointArray[i][0], PointArray[i][1] );
		}
		glEnd();
	}



	glFlush();
}

void initRendering() {
	// Black background
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	// Line width
	glLineWidth(1);

}

void resizeWindow(int w, int h){
	WindowHeight = (h>1) ? h : 2;
	WindowWidth = (w>1) ? w : 2;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);  // Always view [0,1]x[0,1].
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB ); 

	glutInitWindowSize(500, 500);
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

	return 0;					// This line is never reached
}
