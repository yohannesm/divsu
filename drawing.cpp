/*
 * drawing.cpp
 * -----------
 * Contains the drawing routines and related helper functions for the
 * subdivision surface
 *
 * Group Members: Yohannes Himawan, David Lee 
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "common.h"
#include "drawing.h"
#include "data.h"


/* Globals */
int subdiv_v; // The user-specified subdivision level, vertical
int subdiv_h; // The user-specified subdivision level, horizontal

/* The parameter list may need to be changed for the functions in this file */
DISP_MODE disp = DRAW2D;
void drawSurface(void) {

}

void draw3D(){
   if(num_draw_pts <5){
     //print error message
     printf("Need more than 5 points to do this \n");
   }
   
   else{
   poly_list = new GLfloat**[num_draw_pts];

   //draw the 3-D extrusion
   for(int i=0; i<num_draw_pts; ++i){
        GLfloat** poly = new GLfloat*[3];

        GLfloat* p1 = new GLfloat[3];
        GLfloat* p2 = new GLfloat[3];
        GLfloat* p3 = new GLfloat[3];
        p1[0] = i0_x[i]; p1[1] = i0_y[i]; p1[2] = 0; 
        GLfloat d = cosLaw(p1[0], p1[0], 120.0);
        p2[0] = d*cos(30*PI/180); p2[1] = i0_y[i]; p2[0] = - (d*sin(30*PI/180));
        p3[0] = d*cos(30*PI/180); p3[1] = i0_y[i]; p3[0] =  (d*sin(30*PI/180));

        poly[0] = p1;
        poly[1] = p2;
        poly[2] = p3;

        poly_list[i] = poly;
   }
   }
   
/*
   glMatrixMode(GL_MODELVIEW);
   glBegin(GL_LINES);
   for(int i=0; i<num_draw_pts; ++i){
      glVertex3f(i0_x[i], i0_y[i], 0);
   }
   //glEnd();

   glPushMatrix();
      glRotatef(120.0, 0.0, 1.0, 0.0);
      glTranslatef(0, 200, 0);
   //glBegin(GL_LINES);
   for(int i=0; i<num_draw_pts; ++i){
      glVertex3f(i0_x[i], i0_y[i], 0);
   }
      glPopMatrix();
   //glEnd();
   
   //glBegin(GL_LINES);
   glPushMatrix();
      glRotatef(-120.0, 0.0, 1.0, 0.0);
      glTranslatef(0, 200, 0);
   for(int i=0; i<num_draw_pts; ++i){
      glVertex3f(i0_x[i], i0_y[i], 0);
   }
      glPopMatrix();
   glEnd();
   
}

}
*/
}

void draw2D (){
    glPointSize(2);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    for (int q = 0; q < num_draw_pts; ++q)
    {
        //printf("Drawing point (%f, %f)\n", i0_x[q], i0_y[q]);
        glVertex2f(i0_x[q], i0_y[q]);
    }

    glEnd();
    
    if (num_draw_pts > 1)
    {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        for (int q = 0; q < num_draw_pts - 1; ++q)
        {
            //printf("Drawing line (%f, %f) to (%f, %f)\n", i0_x[q], i0_y[q], i0_x[q + 1], i0_y[q + 1]);
            glVertex2f(i0_x[q], i0_y[q]);
            glVertex2f(i0_x[q + 1], i0_y[q + 1]);
        }
        glEnd();
    }
}

GLfloat cosLaw(GLfloat a, GLfloat b, GLfloat theta){
   theta = (theta*PI/180);
   return a*a + b*b - 2*a*b* cos(theta);
}


/* end of drawing.cpp */
