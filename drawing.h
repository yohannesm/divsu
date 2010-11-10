#ifndef _DRAWING_H_
#define _DRAWING_H_

/* Globals declared in drawing.cpp */
extern int subdiv_v; // The user-specified subdivision level, vertical
extern int subdiv_h; // The user-specified subdivision level, horizontal

enum DISP_MODE{ DRAW2D, DRAW3D };
/* Functions implemented in drawing.cpp */
void drawSurface(void);

void draw2D ();
void draw3D (bool,bool);
GLfloat cosLaw(GLfloat, GLfloat, GLfloat);
int calcNumPointsInLevel (int subDivDepth);
int calcNumOfLevels(int subDivDepth);

void cleanPolyList();

extern DISP_MODE disp;
extern GLfloat*** poly_list;
extern GLfloat*** vert_normals;
#define PI 3.14159265
#endif	/* _DRAWING_H_ */
