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
int subdiv_v = 0; // The user-specified subdivision level, vertical
int subdiv_h = 0; // The user-specified subdivision level, horizontal

int curSubDivV = 0;
int curSubDivH = 0;



/* The parameter list may need to be changed for the functions in this file */
DISP_MODE disp = DRAW2D;
void drawSurface(void) {

}

void genFirstPoints()
{
  poly_list = new GLfloat**[num_i0_pts];

  //draw the 3-D extrusion
  for(int i=0; i<num_i0_pts; ++i){
    GLfloat** poly = new GLfloat*[3];

    GLfloat* p1 = new GLfloat[3];
    GLfloat* p2 = new GLfloat[3];
    GLfloat* p3 = new GLfloat[3];
    p1[0] = i0_x[i]; p1[1] = i0_y[i]; p1[2] = 0; 
    p2[0] = cos(120*PI/180) * i0_x[i]; p2[1] = i0_y[i]; p2[2] = sin(120*PI/180) * i0_x[i];
    p3[0] = cos(-120*PI/180) * i0_x[i]; p3[1] = i0_y[i]; p3[2] = sin(-120*PI/180) * i0_x[i];

    poly[0] = p1;
    poly[1] = p2;
    poly[2] = p3;
    
    //printf("level %i p1: (%f, %f, %f)\n", i, p1[0], p1[1], p1[2]);
    //printf("level %i p2: (%f, %f, %f)\n", i, p2[0], p2[1], p2[2]);
    //printf("level %i p3: (%f, %f, %f)\n", i, p3[0], p3[1], p3[2]);

    poly_list[i] = poly;
    num_draw_pts += 3;
  }
}

void draw3D(bool wire, bool points){

  int numOfLevels = pow(2, subdiv_v) * num_i0_pts;
  int numPointsInLevel = pow(2, subdiv_h) * 3;
  int totalNumPoints = numOfLevels * numPointsInLevel;
  //printf("curSubDivV : %i and subdiv_v : %i\n", curSubDivV, subdiv_v);
  //printf("curSubDivH : %i and subdiv_h : %i\n", curSubDivH, subdiv_h);  
  


  if (subdiv_v == 0 && subdiv_h == 0){
    genFirstPoints();
  }
  else
  {
    if (curSubDivV == (subdiv_v - 1))
    {
      // sub divide vertically

      
    }
    else if(curSubDivV != subdiv_v)
      printf("ERROR INVALID VALUES curSubDivV is %i and subdiv_v is %i\n", curSubDivV, subdiv_v);
    
    if (curSubDivH == (subdiv_h - 1))
    {
      //printf("SUBDIVIDING HORIZONTALLY\n");
      //printf("numPointsInLevel %i\n", numPointsInLevel);
      // sub divide horizontally
      for (int l = 0; l < numOfLevels; ++l)
      {
        //printf("SUBDIVIDING LEVEL %i\n", l);
        GLfloat** newLevel = new GLfloat*[numPointsInLevel];
        GLfloat** oldLevel = poly_list[l];
        // dont do anything for the zero case
        for (int p = 0; p < numPointsInLevel; ++p)
        {
          GLfloat* newP = new GLfloat[3];
          //printf("p %i\n", p);
          if ((p & 1) == 0)
          {
            int oldInd = p >> 1;
            int oldIntpo = (oldInd + 1) % (numPointsInLevel >> 1);
            int oldIntmo = (oldInd - 1) % (numPointsInLevel >> 1);
            
            //printf("before oldIntmo = %i\n", oldIntmo);
            if (oldIntmo < 0)
              oldIntmo += (numPointsInLevel >> 1);

            //printf("after oldIntmo = %i\n", oldIntmo);
            newP[0] = (oldLevel[oldIntmo][0] + 6 * oldLevel[oldInd][0] + oldLevel[oldIntpo][0]) / 8;
            newP[1] = (oldLevel[oldIntmo][1] + 6 * oldLevel[oldInd][1] + oldLevel[oldIntpo][1]) / 8;
            newP[2] = (oldLevel[oldIntmo][2] + 6 * oldLevel[oldInd][2] + oldLevel[oldIntpo][2]) / 8;
          } 
          else
          {
            int oldInd = (p - 1) / 2;
            int oldIntpo = (oldInd + 1) % (numPointsInLevel >> 1);
            newP[0] = (4 * oldLevel[oldInd][0] + 4 * oldLevel[oldIntpo][0]) / 8;
            newP[1] = (4 * oldLevel[oldInd][1] + 4 * oldLevel[oldIntpo][1]) / 8;
            newP[2] = (4 * oldLevel[oldInd][2] + 4 * oldLevel[oldIntpo][2]) / 8;
          }
          
          //printf("newP : (%f, %f, %f)\n", newP[0], newP[1], newP[2]);
          newLevel[p] = newP;
        }
        poly_list[l] = newLevel;
        for (int q = 0; q < numPointsInLevel >> 1; ++q)
          delete[] oldLevel[q];
        delete[] oldLevel;
      }
      ++curSubDivH;
    }
    else if(curSubDivH != subdiv_h)
      printf("ERROR INVALID VALUES curSubDivH is %i and subdiv_h is %i", curSubDivH, subdiv_h);
  }  

  
  glMatrixMode(GL_MODELVIEW);
  if (points)
  {
    glPointSize(2);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    for (int l = 0; l < numOfLevels; ++l)
      for (int p = 0; p < numPointsInLevel; ++p)
        glVertex3fv(poly_list[l][p]);
    glEnd();
  }
  else if (!wire)
  {
    // draw top tri
    for(int l = 0; l < numOfLevels - 1; ++l)
    {
    	glColor3f( (l%3 == 0 ? 1 : 0), (l%3 == 1 ? 1 : 0), (l%3 == 2 ? 1 : 0));
      for (int p = 0; p < numPointsInLevel; ++p)
      {
        int ppo = (p + 1) % numPointsInLevel;
        glBegin(GL_QUADS);
        glVertex3fv(poly_list[l][p]);
        glVertex3fv(poly_list[l + 1][p]);
        glVertex3fv(poly_list[l + 1][ppo]);
        glVertex3fv(poly_list[l][ppo]);
        glEnd();
      }
    }
    // draw bottom tri
  }
  else
  {
    // draw top tri
    for(int l = 0; l < numOfLevels - 1; ++l)
    {
    	glColor3f( (l%3 == 0 ? 1 : 0), (l%3 == 1 ? 1 : 0), (l%3 == 2 ? 1 : 0));
      for (int p = 0; p < numPointsInLevel; ++p)
      {
        int ppo = (p + 1) % numPointsInLevel;
        glBegin(GL_LINES);
        glVertex3fv(poly_list[l][p]);
        glVertex3fv(poly_list[l + 1][p]);
        glEnd();
        //printf("LINE BETWEEN level %i point %i and level %i point %i\n", l,p,l + 1,p);
        
        glBegin(GL_LINES);
        glVertex3fv(poly_list[l + 1][p]);
        glVertex3fv(poly_list[l + 1][ppo]);
        glEnd();
        //printf("LINE BETWEEN level %i point %i and level %i point %i\n", l + 1,p,l + 1,ppo);
        
        glBegin(GL_LINES);
        glVertex3fv(poly_list[l + 1][ppo]);
        glVertex3fv(poly_list[l][ppo]);
        glEnd();
        //printf("LINE BETWEEN level %i point %i and level %i point %i\n", l + 1,ppo,l,ppo);
        
        glBegin(GL_LINES);
        glVertex3fv(poly_list[l][ppo]);
        glVertex3fv(poly_list[l][p]);
        glEnd();
        //printf("LINE BETWEEN level %i point %i and level %i point %i\n\n", l,ppo,l,p);
      }
    }
  }
} // end draw3D

void generatePoints()
{
    
}

void draw2D (){

    
    glPointSize(2);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    for (int q = 0; q < num_i0_pts; ++q)
    {
        //printf("Drawing point (%f, %f)\n", i0_x[q], i0_y[q]);
        glVertex2f(i0_x[q], i0_y[q]);
    }

    glEnd();
    
    if (num_i0_pts > 1)
    {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        for (int q = 0; q < num_i0_pts - 1; ++q)
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
   return sqrt(a*a + b*b - 2*a*b* cos(theta));
}


/* end of drawing.cpp */
