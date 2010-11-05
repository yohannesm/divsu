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

void drawSurface(void) {

}

#if 1
void draw3D(){
   if(num_draw_pts <5){
     //print error message
   }
   else{
   //draw the 3-D extrusion
   glMatrixMode(GL_MODELVIEW);
   glBegin(GL_LINES);
   for(int i=0; i<num_draw_pts; ++i){
      glVertex3f(i0_x[i], i0_y[i], 0);
   }
   glEnd();

   glBegin(GL_LINES);
   for(int i=0; i<num_draw_pts; ++i){
      glVertex3f(i0_x[i], i0_y[i], 0);
   }
   glPushMatrix();
      glTranslatef(0, 200, 0);
      glRotatef(120.0, 0.0, 1.0, 0.0);
      glPopMatrix();
   glEnd();
   
   glBegin(GL_LINES);
   for(int i=0; i<num_draw_pts; ++i){
      glVertex3f(i0_x[i], i0_y[i], 0);
   }
   glPushMatrix();
      glTranslatef(0, 200, 0);
      glRotatef(-120.0, 0.0, 1.0, 0.0);
      glPopMatrix();
   glEnd();
   
}

}
#endif 

void draw2D ()
{
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

/* end of drawing.cpp */
