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


int oldNumOfLevels = 0;

GLfloat magnitVec(GLfloat* v){
 return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
}

GLfloat* normCrossProduct(GLfloat* x, GLfloat* y){
     GLfloat* result = new GLfloat[3];
     result[0] = (x[1] * y[2]) - (x[2] * y[1]);
     result[1] = (x[2] * y[0]) - (x[0] * y[2]);
     result[2] = (x[0] * y[1]) - (x[1] * y[0]);

     GLfloat mag = magnitVec(result);
     result[0] /= mag;
     result[1] /= mag;
     result[2] /= mag;

     return result;
}

GLfloat* compNormal(GLfloat* a, GLfloat* b, GLfloat* c){
        GLfloat v1[3] = {b[0]-a[0], b[1]-a[1], b[2]-a[2]};
        GLfloat v2[3] = {c[0]-a[0], c[1]-a[1], c[2]-a[2]};
        
        GLfloat* ncp = normCrossProduct(v1, v2);
        //ncp[0] *= -1;
        //ncp[1] *= -1;
        //ncp[2] *= -1;
        return ncp;
}

void drawNormal(GLfloat* point, GLfloat* n)
{
  glBegin(GL_LINES);
  glVertex3fv(point);
  glVertex3f(point[0] + -50 * n[0], point[1] + -50 * n[1], point[2] + -50 * n[2]);
  glEnd();
}


int calcNumOfLevels(int subDivDepth)
{
  if (subDivDepth == 0)
    return num_i0_pts;
  else
  {
    return calcNumOfLevels(subDivDepth - 1) * 2 - 1;
  }
}

int calcNumPointsInLevel (int subDivDepth)
{
  return pow(2, subDivDepth) * 3;
}

void computeGNormals()
{

  int numOfLevels = calcNumOfLevels(subdiv_v);
  int numPointsInLevel = calcNumPointsInLevel(subdiv_h);
  vert_normals = new GLfloat**[numOfLevels];
  
  //int*** numNormals[numOfLevels][numPointsInLevel][3];
  
  // draw top tri
    for (int l = 0; l < numOfLevels; ++l)
    {
      GLfloat ** normLevels = new GLfloat*[numPointsInLevel];
      for (int p = 0; p < numPointsInLevel; ++p)
      {
        printf("l : %i, p : %i\n", l, p);
        GLfloat* norm = new GLfloat[3];

        int ppo = (p + 1);
        int pmo = (p - 1);
        if (ppo >= numPointsInLevel)
          ppo -= numPointsInLevel;
        if (pmo < 0)
          pmo += numPointsInLevel;

        int lpo = (l + 1);
        int lmo = (l - 1);
        if (lpo >= numOfLevels)
          lpo -= numOfLevels;
        if (lmo < 0)
          lmo += numOfLevels;
          
        printf("lmo : %i, lpo : %i\n", lmo, lpo);
        printf("pmo : %i, ppo : %i\n", pmo, ppo);
          
        if (l == 0)
        {
          norm[0] = 0.0;
          norm[1] = 0.0;
          norm[2] = 0.0;
          normLevels[p] = norm;
        }
        else if (l == numOfLevels - 1)
        {
          norm[0] = 0.0;
          norm[1] = 0.0;
          norm[2] = 0.0;
          normLevels[p] = norm;
        }
        else
        {
          GLfloat* n0 = compNormal(poly_list[l][p], poly_list[l][ppo], poly_list[lmo][p]);
          GLfloat* n1 = compNormal(poly_list[l][p], poly_list[lmo][p], poly_list[l][pmo]);
          GLfloat* n2 = compNormal(poly_list[l][p], poly_list[lpo][p], poly_list[l][ppo]);
          GLfloat* n3 = compNormal(poly_list[l][p], poly_list[l][pmo], poly_list[lpo][p]);
          
          //drawNormal(poly_list[l][p], n0);
          //drawNormal(poly_list[l][p], n1);
          //drawNormal(poly_list[l][p], n2);
          //drawNormal(poly_list[l][p], n3);
          

          norm[0] = (n0[0] + n1[0] + n2[0] + n3[0]) / 4;
          norm[1] = (n0[1] + n1[1] + n2[1] + n3[1]) / 4;
          norm[2] = (n0[2] + n1[2] + n2[2] + n3[2]) / 4;
          
          drawNormal(poly_list[l][p], norm);
          normLevels[p] = norm;
                               
          delete[] n0;
          delete[] n1;
          delete[] n2;
          delete[] n3;
        }
        //drawNormal(poly_list[l][p], trueN);
        
        //glBegin(GL_QUADS);
        //glVertex3fv(poly_list[l][p]);
        //glVertex3fv(poly_list[l + 1][p]);
        //glVertex3fv(poly_list[l + 1][ppo]);
        //glVertex3fv(poly_list[l][ppo]);
        //glEnd();
      }
      vert_normals[l] = normLevels;
    }
    printf("gent norm nol : %i , npl : %i\n", numOfLevels, numPointsInLevel);
}

/* The parameter list may need to be changed for the functions in this file */
DISP_MODE disp = DRAW2D;
void drawShape(bool wire, bool points) 
{
  
  // glEnable(GL_NORMALIZE);
  glShadeModel(GL_SMOOTH);

  GLfloat diffuse0[] = {0.0, 1.0, 0.0, 1.0};
  GLfloat ambient0[] = {0.0, 1.0, 0.0, 1.0};
  GLfloat specular0[] = {0.0, 1.0, 0.0, 1.0};
  GLfloat light0_pos[] = {1.0, 2.0, 4.0, 1.0};

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
  //GLfloat attenu = 0.8;
  //glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, attenu);

  glEnable(GL_COLOR_MATERIAL);
  GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat diffuse[] = {1.0, 0.8, 0.0, 1.0};
  GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  //GLfloat shine = 20.0;
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, shine);


  glMaterialfv(GL_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_BACK, GL_SPECULAR, specular);
  //glMaterialfv(GL_BACK, GL_SHININESS, shine);

  GLfloat emission[] = {0.0, 0.3, 0.3, 1.0};
  glMaterialfv(GL_FRONT, GL_EMISSION, emission);

  int numOfLevels = calcNumOfLevels(subdiv_v);
  int numPointsInLevel = calcNumPointsInLevel(subdiv_h);
  //int totalNumPoints = numOfLevels * numPointsInLevel;
  
  printf("numOfLevels %i\n", numOfLevels);
  printf("numPointsInLevel %i\n", numPointsInLevel);
  
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
    	//glColor3f(0.0, 0.0, 1.0);
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
    	//glColor3f( (l%3 == 0 ? 1 : 0), (l%3 == 1 ? 1 : 0), (l%3 == 2 ? 1 : 0));
    	glColor3f(0.0, 0.0, 1.0);
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
}

void genFirstPoints()
{
  curSubDivV = 0;
  curSubDivH = 0;
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
  oldNumOfLevels = 0;
}

void subdivideH()
{
  int numOfLevels = calcNumOfLevels(subdiv_v);
  int numPointsInLevel = calcNumPointsInLevel(curSubDivH + 1);
  //int totalNumPoints = numOfLevels * numPointsInLevel;
  printf("SUBDIVIDING HORIZONTALLY\n");
  printf("numPointsInLevel %i\n", numPointsInLevel);
  // sub divide horizontally
  for (int l = 0; l < numOfLevels; ++l)
  {
    printf("SUBDIVIDING LEVEL %i\n", l);
    GLfloat** newLevel = new GLfloat*[numPointsInLevel];
    GLfloat** oldLevel = poly_list[l];
    
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
}


void subdivideV()
{
  
  int numOfLevels = calcNumOfLevels(curSubDivV + 1);
  int numPointsInLevel = calcNumPointsInLevel(subdiv_h);
  int oldNumOfLevels = calcNumOfLevels(curSubDivV);
  //int totalNumPoints = numOfLevels * numPointsInLevel;
  printf("SUBDIVIDING VERTICALLY\n");
  printf("numOfLevels %i\n", numOfLevels);
  printf("oldNumOfLevels %i\n", oldNumOfLevels);
  // sub divide VERTICALLY
  GLfloat*** newPolyList = new GLfloat**[numOfLevels]; //new GLfloat[3][numPointsInLevel][numOfLevels];
  
  for (int l = 0; l < numOfLevels; ++l)
  {
    printf("SUBDIVIDING in LEVEL %i\n", l);
    GLfloat** newLevel = new GLfloat*[numPointsInLevel];
    if (l == 0 || l == numOfLevels - 1)
    {
      int oldInd = l;
      if (l == numOfLevels - 1)
        oldInd = oldNumOfLevels - 1;
        
      for (int p = 0; p < numPointsInLevel; ++p)
      {
        GLfloat* newP = new GLfloat[3];
        for (int i = 0; i < 3; ++i)
          newP[i] = poly_list[oldInd][p][i];
        newLevel[p] = newP;
      }
    }
    else if ((l & 1) == 0)
    {
      // these are indices of levels
      int oldInd = l >> 1;
      int oldIntpo = (oldInd + 1) % oldNumOfLevels;
      int oldIntmo = (oldInd - 1) % oldNumOfLevels;
      
      //printf("before oldIntmo = %i\n", oldIntmo);
      if (oldIntmo < 0)
        oldIntmo += oldNumOfLevels;
        
      printf("oldInd : %i\n", oldInd);
      printf("oldIntpo : %i\n", oldIntpo);
      printf("oldIntmo : %i\n", oldIntmo);
      
      for (int p = 0; p < numPointsInLevel; ++p)
      {
        printf("point %i\n", p);
        GLfloat* newP = new GLfloat[3];
        newP[0] = (poly_list[oldIntmo][p][0] + 6 * poly_list[oldInd][p][0] + poly_list[oldIntpo][p][0]) / 8;
        newP[1] = (poly_list[oldIntmo][p][1] + 6 * poly_list[oldInd][p][1] + poly_list[oldIntpo][p][1]) / 8;
        newP[2] = (poly_list[oldIntmo][p][2] + 6 * poly_list[oldInd][p][2] + poly_list[oldIntpo][p][2]) / 8;
        
        newLevel[p] = newP;
      }
    }
    else
    {
      int oldInd = (l - 1) / 2;
      int oldIntpo = (oldInd + 1) % oldNumOfLevels;
      
      for (int p = 0; p < numPointsInLevel; ++p)
      {
        GLfloat* newP = new GLfloat[3];
        newP[0] = (4 * poly_list[oldInd][p][0] + 4 * poly_list[oldIntpo][p][0]) / 8;
        newP[1] = (4 * poly_list[oldInd][p][1] + 4 * poly_list[oldIntpo][p][1]) / 8;
        newP[2] = (4 * poly_list[oldInd][p][2] + 4 * poly_list[oldIntpo][p][2]) / 8;
        
        newLevel[p] = newP;
      }
    }
    newPolyList[l] = newLevel;
  }
  for (int l = 0; l < oldNumOfLevels; ++l)
  {
    for (int p = 0; p < numPointsInLevel; ++p)
      delete[] poly_list[l][p];
      
    delete[] poly_list[l];
  }
  delete[] poly_list;
  poly_list = newPolyList;
}

void draw3D(bool wire, bool points)
{
  printf("curSubDivV : %i and subdiv_v : %i\n", curSubDivV, subdiv_v);
  printf("curSubDivH : %i and subdiv_h : %i\n", curSubDivH, subdiv_h);  

  if (subdiv_v == 0 && subdiv_h == 0){
    // if this is the first time we need to generate the points
    genFirstPoints();
  }
  else
  {
    // other wise perform the subdivisions if needed
    while (curSubDivV < subdiv_v)
    {
      // sub divide vertically
      subdivideV();
      ++curSubDivV;
    }
    if(curSubDivV > subdiv_v)
      printf("ERROR INVALID VALUES curSubDivV is %i and subdiv_v is %i\n", curSubDivV, subdiv_v);
    
    while (curSubDivH < subdiv_h)
    {
      subdivideH();
      ++curSubDivH;
    }
    if(curSubDivH > subdiv_h)
      printf("ERROR INVALID VALUES curSubDivH is %i and subdiv_h is %i\n", curSubDivH, subdiv_h);
  }  
  
  computeGNormals();
  drawShape(wire, points);
  
  int numOfLevels = calcNumOfLevels(subdiv_v);
  int numPointsLevels = calcNumPointsInLevel(subdiv_h);
  
  printf("nol : %i , npl : %i\n", numOfLevels, numPointsLevels);
  #if 1
  for (int l = 0; l < numOfLevels; ++l)
  {
    for (int p = 0; p < numPointsLevels; ++p){
      delete[] vert_normals[l][p];
      }
      
    delete[] vert_normals[l];
  }
  delete[] vert_normals;
    #endif
} // end draw3D

void draw2D ()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    
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

void cleanPolyList()
{
  for (int l = 0; l < calcNumOfLevels(subdiv_v); ++l)
  {
    for (int p = 0; p < calcNumPointsInLevel(subdiv_h); ++p)
      delete[] poly_list[l][p];
      
    delete[] poly_list[l];
  }
  delete[] poly_list;
}


/* end of drawing.cpp */
