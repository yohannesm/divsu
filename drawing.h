#ifndef _DRAWING_H_
#define _DRAWING_H_

/* Globals declared in drawing.cpp */
extern int subdiv_v; // The user-specified subdivision level, vertical
extern int subdiv_h; // The user-specified subdivision level, horizontal

/* Functions implemented in drawing.cpp */
void drawSurface(void);

void draw2D ();
void draw3D ();

#endif	/* _DRAWING_H_ */
