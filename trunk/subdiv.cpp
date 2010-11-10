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
#include "mouse.h"

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
GLfloat zoomFactor = 1.0; 

/* local function declarations */
void init(void);
void display(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void endSubdiv(int status);

bool wireMode = false;
bool pointMode = false;
bool shineMode = false;




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
	glutMotionFunc(myMouseMotion);
  glutMainLoop();
  return 0;
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);
}

void resetCamera( void ) {
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	 * glFrustum must receive positive values for the near and far clip planes
	 * ( arguments 5 and 6 ).
	 */
	//glFrustum(fleft, fright, fbottom, ftop, -zNear, -zFar);

	/* Set the drawing point at the center of the frustum */
	//glMatrixMode( GL_MODELVIEW );
	//glLoadIdentity( );
	/* Go to the center of the scene */
	//glTranslatef(
		//(fleft + fright) / 2,
		//(fbottom + ftop) / 2,
		//(zNear + zFar) / 2);

	//zoomFactor = 1.0;
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

    switch(disp){
     //2D or 3D
      case DRAW2D:
        //printf("CALLING draw2D\n");
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(0.0, -H);
        glVertex2f(0.0, H);
        glEnd();
        draw2D();
        break;
      case DRAW3D:
        //printf("CALLING draw3D\n");
        draw3D(wireMode, pointMode, shineMode);

        break;
      default:
        break;
    }
	//drawSurface();
    //draw3D();
	//draw2D();

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
		case 'w' :
		  if(num_i0_pts < 5){
        //print error message
        printf("Need more than 5 points to do this \n");
      }
      else
      {
        disp = DRAW3D;
        printf("Draw mode changed to 3D\n");
      }
      break;
    case 'z':
      if (disp == DRAW3D)
      {
        disp = DRAW2D;
        resetCamera(); 
        cleanPolyList();
        
        subdiv_v = 0;
        subdiv_h = 0;
        
        printf("Draw mode changed to 2D\n");
        
      }
      break;
          
    case 'e':
      if (disp == DRAW3D)
      {
        wireMode = !wireMode;
        if (wireMode)
          printf("Switched to wire frame mode\n");
        else
          printf("Switched to solid mode\n");
      }
      break;
      
    case 'r':
      if (disp == DRAW3D)
      {
        pointMode = !pointMode;
        if (pointMode)
          printf("Switched to point mode\n");
        else
          printf("Switched to faces mode\n");
      }
      break;
      
    case 'a':
      if (disp == DRAW3D)
      {
        if (subdiv_v < 6)
        {
          ++subdiv_v;
          printf("Applying vertical subdivision\n");
        }
        else
          printf("Maximum vertical subdivisions reached\n");
      }
      break;
      
    case 'b':
      if (disp == DRAW3D)
      {

        if (subdiv_h < 6)
        {
          ++subdiv_h;
          printf("Applying horizontal subdivision\n");
        } 
        else
          printf("Maximum horizontal subdivisions reached\n");
      }
      break;
    
    case 's':
      shineMode = !shineMode;
      printf("Shine value toggled\n");
      break;
      
		default:
			/* Unrecognized keypress */
		  break;
	}

	glutPostRedisplay();
  //display();

	return;
}

/* The current mode the mouse is in, based on what button(s) is pressed */
int mouse_mode;

/* The last position of the mouse since the last callback */
int m_last_x, m_last_y;

void myMouseButton(int button, int state, int x, int y) {
  if(disp == DRAW2D)
  {
  
    // reset rotate vals
    m_last_x = 0;
    m_last_y = 0;
    
	  if (state == GLUT_DOWN) {
		  if (button == GLUT_LEFT_BUTTON) {
			  // Add a point, if there is room

		      if (num_i0_pts < MAX_POINT && x >= 200)
              {
                 // std::cout << "real coordinate";
      		//	printf("%i: (%3d, %3d)\n", num_i0_pts, x, y);
      			x = x - W / 2;
      			y = -(y - H / 2);
      			if (x >= -5 && x <= 5)
      			    x = 0;
      			printf("%i: (%3d, %3d)\n", num_i0_pts, x, y);
                  i0_x[num_i0_pts] = x;
                  i0_y[num_i0_pts] = y;
                  ++num_i0_pts;
                  
              }
              else
              {
                  printf("Exceeded the maximum number of control points or the point is out of range \n");
              }
		  }
	      else if (button == GLUT_RIGHT_BUTTON)
	      {
	          if (num_i0_pts > 0)
	          {
	              printf("deleted control point %i\n", num_i0_pts - 1); 
      	        --num_i0_pts;
              }
	      }
	  }
  }
  else 
  {
  	if (state == GLUT_DOWN) {
		  m_last_x = x;
		  m_last_y = y;

		  if (button == GLUT_LEFT_BUTTON) {
			  mouse_mode = MOUSE_ROTATE_YX;
		  } else if (button == GLUT_MIDDLE_BUTTON) {
			  mouse_mode = MOUSE_ROTATE_YZ;
		  } else if (button == GLUT_RIGHT_BUTTON) {
			  mouse_mode = MOUSE_ZOOM;
		  }
	  }
  }
  display();
}

void myMouseMotion(int x, int y) {
  if (disp == DRAW3D)
  {
	  double d_x, d_y;	/* The change in x and y since the last callback */

	  d_x = x - m_last_x;
	  d_y = y - m_last_y;

	  m_last_x = x;
	  m_last_y = y;

	  if (mouse_mode == MOUSE_ROTATE_YX) {
		  /* scaling factors */
		  d_x /= 2.0;
		  d_y /= 2.0;
      //printf("ROTATING YX\n");
		  glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
		  glRotatef(-d_y, 1.0, 0.0, 0.0);	/* x-axis rotation */

	  } else if (mouse_mode == MOUSE_ROTATE_YZ) {
		  /* scaling factors */
		  d_x /= 2.0;
		  d_y /= 2.0;
      //printf("ROTATING YZ\n");
		  glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
		  glRotatef(-d_y, 0.0, 0.0, 1.0);	/* z-axis rotation */

	  } 
  #if 0
    else if (mouse_mode == MOUSE_ZOOM) {
		  d_y /= 100.0;

		  zoomFactor += d_y;

		  if (zoomFactor <= 0.0) {
			  /* The zoom factor should be positive */
			  zoomFactor = 0.001;
		  }

		  glMatrixMode(GL_PROJECTION);
		  glLoadIdentity();

		  /*
		   * glFrustum must receive positive values for the near and far
		   * clip planes ( arguments 5 and 6 ).
		   */
		  glFrustum(fleft*zoomFactor, fright*zoomFactor,
			  fbottom*zoomFactor, ftop*zoomFactor,
			  -zNear, -zFar);
	  }
  #endif

	  /* Redraw the screen */
	  glutPostRedisplay();
	}
}

void endSubdiv(int status) {
  printf("\nQuitting subdivision program.\n\n");
  if (disp == DRAW3D)
    cleanPolyList();
  fflush(stdout);
  exit(status);
}


/* end of subdiv.cpp */
