/*
 * subdiv.cpp
 * ----------
 * Contains function callbacks and bookkeeping for window management.
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "drawing.h"
#include "data.h"
#include <iostream>

/* GLOBAL VARIABLES */
/* (storage is actually allocated here) */
int W=400;		/* window width */
int H=400;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

/* The dimensions of the viewing frustum */
GLfloat fleft   = -200.0;
GLfloat fright  =  200.0;
GLfloat fbottom = -200.0;
GLfloat ftop    =  200.0;
GLfloat zNear   =  200.0;
GLfloat zFar    = -200.0;

/* local function declarations */
void init(void);
void display(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void endSubdiv(int status);

int main (int argc, char** argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("subdiv");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMouseFunc(myMouseButton);
  glutMainLoop();
  return 0;
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/*
	 * See drawing.c for the definition of these routines.
	 *
	 * Note: Only one should be called at a time (based on the
	 * display mode).
	 */
	 
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(0.0, -H);
    glVertex2f(0.0, H);
    glEnd();

	//drawSurface();
    //draw3D();
	draw2D();

    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
		case 'q':
			endSubdiv(0);
			break;
        case 'w':
            draw3D();
            break;
		default:
			/* Unrecognized keypress */
		return;
	}

	glutPostRedisplay();
    //display();

	return;
}

void myMouseButton(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			// Add a point, if there is room

		    if (num_draw_pts < MAX_POINT)
            {
                std::cout << "real coordinate";
    			printf("%i: (%3d, %3d)\n", num_draw_pts, x, y);
    			x = x - W / 2;
    			y = -(y - H / 2);
    			if (x >= -5 && x <= 5)
    			    x = 0;
    			printf("%i: (%3d, %3d)\n", num_draw_pts, x, y);
                i0_x[num_draw_pts] = x;
                i0_y[num_draw_pts] = y;
                ++num_draw_pts;
            }
            else
            {
                printf("Exceeded the maximum number of control points\n");
            }
		}
	    else if (button == GLUT_RIGHT_BUTTON)
	    {
	        if (num_draw_pts > 0)
	        {
	            printf("deleted control point %i\n", num_draw_pts - 1); 
    	        --num_draw_pts;
            }
	    }
	    display();
	}
}

void endSubdiv(int status) {
  printf("\nQuitting subdivision program.\n\n");
  fflush(stdout);

  exit(status);
}


/* end of subdiv.cpp */
