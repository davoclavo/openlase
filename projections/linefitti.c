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

#define MAXPOINTS 300

#define INACTIVE GLUT_UP 
#define ACTIVE GLUT_DOWN

#define RESOLUTION 3 //In pixels, it describes the allowance to draw a point next to other

float PointArray[MAXPOINTS][3]; // 0: x, 1: y, 2: state
int thispoint = 0; // Last point stack pointer

// Window size in pixels
int WindowHeight;
int WindowWidth;

float normresolution; //Normalized resolution [0-1]

// Key, mouse X, mouse Y
void keyboardManager(unsigned char key, int x, int y){
	switch (key) {
	case ' ':
		removeAllPoints();
		glutPostRedisplay();
		break;
	case 27:			// Escape key
		exit(0);
		break;
	}
}

// Left button presses place a control point.
void mouseManager( int button, int state, int x, int y ) {
	printf("button: %d, state: %d\n",button, state);

	float xPos = -((float)x-(float)(WindowWidth)/2)/((float)(WindowWidth)/2);
	float yPos = ((float)y)/((float)(WindowHeight)/2);

	yPos = 1.0f-yPos;			// Flip value since y position is from top row.

	if(state==GLUT_DOWN){
		//NOTHING LOL
	}

	addNewPoint( xPos, yPos, INACTIVE );
	
}

// Mouse moved while pressing a button.
void mouseMovedPressed( int x, int y ) {
	float xPos = -((float)x-(float)(WindowWidth)/2)/((float)(WindowWidth)/2);
	float yPos = ((float)y)/((float)(WindowHeight)/2);

	yPos = 1.0f-yPos;			// Flip value since y position is from top row.
	
	addNewPoint( xPos, yPos, ACTIVE );
}

// Mouse moved without pressing a button.
void mouseMoved( int x, int y ) {
	/*
	float xPos = ((float)x-(float)(WindowWidth)/2)/((float)(WindowWidth)/2);
	float yPos = ((float)y)/((float)(WindowHeight)/2);
	yPos = 1.0f-yPos;			// Flip value since y position is from top row.	
	printf("x: %d, y: %d -- xPos: %f, yPos: %f\n",x,y,xPos,yPos);
	*/
}

// Move the pointer to the starting position
void removeAllPoints() {
	int i;
	for(i=0;i<MAXPOINTS;i++){
		PointArray[i][0] = 0;
		PointArray[i][1] = 0;
		PointArray[i][2] = INACTIVE;

	}
	thispoint = 0;
}

// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void addNewPoint( float x, float y, int state ) {
	//Avoid overflow with %MAXPOINTS
	int lastpoint = (MAXPOINTS+thispoint-1)%MAXPOINTS,
		nextpoint = (thispoint+1)%MAXPOINTS;



	
	float dist = sqrt(pow((x-PointArray[lastpoint][0]),2)+pow(y-PointArray[lastpoint][1],2));
	//printf("Distance between %d and %d is %f\n", thispoint, lastpoint,dist);
	

	if(state == ACTIVE && dist < normresolution){
		printf(".");
		return;
	}
	printf("%d) x: %f, y: %f, st: %d\n",thispoint,x,y,state);


	PointArray[thispoint][0] = x;
	PointArray[thispoint][1] = y;
	PointArray[thispoint][2] = state;
	thispoint = nextpoint; 
	PointArray[(nextpoint+1)%MAXPOINTS][2] = INACTIVE;
	



	glutPostRedisplay();
}


	
void displayLines(void){
	//printf("DISPLAY LINES!\n");
	int i, j;

	glClear(GL_COLOR_BUFFER_BIT);
	olLoadIdentity();
	
	
	glBegin( GL_LINE_STRIP );
	olBegin(OL_LINESTRIP);
	for ( i=0; i<MAXPOINTS; i++ ) {
		j = (thispoint + i)%MAXPOINTS;
		// Skip if this point is inactive or if it is the current point (to not close the cycle)
		if(PointArray[j][2] == INACTIVE || 
			j == thispoint || j == ((MAXPOINTS+thispoint-1)%MAXPOINTS)){
			glColor4f(1.0f, 0.0f, 0.0f, 0.0f); // Black line
			olVertex( PointArray[j][0], PointArray[j][1], C_BLACK );
		} else {
			glColor3f(0.0f, 1.0f, 0.0f); // Green line
			olVertex( PointArray[j][0], PointArray[j][1], C_GREEN );
		}

		glVertex2f( PointArray[j][0], PointArray[j][1] );
	}
	olEnd();
	glEnd();
	
	olRenderFrame(60);

	glFlush();
}

void initRendering() {
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); // Black background
	glLineWidth(1); // Line width
	removeAllPoints();
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines

	//Allow transparency, it only affects the usage of the gl ui, so lines are not overlapped with black lines
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void resizeWindow(int w, int h){
	WindowHeight = (h>1) ? h : 2;
	WindowWidth = (w>1) ? w : 2;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(1,-1,-1,1);  // Always view [0,1]x[0,1].
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	normresolution = (float)RESOLUTION/((WindowHeight)/2);
	printf("Normalized resolution: %f\n",normresolution);

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
	glutKeyboardFunc(keyboardManager);

	glutMouseFunc(mouseManager);
	glutMotionFunc(mouseMovedPressed);
	//glutPassiveMotionFunc(mouseMoved);

	glutMainLoop();

	return 0;					// This line is never reached
}
