/*
 * Goal.cpp
 *
 *  Created on: 27.06.2014
 *      Author: Giacomo
 */
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Goal.h"
#include <stdlib.h>
#include "vec3.hpp"


/* constructor */
Goal::Goal(int goalSize, double posX, double posY, double posZ) {
	this->goalSize = goalSize;
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;
}

/* destructor */
Goal::~Goal() {

}

/* method to draw the goal */
void Goal::DrawGoal() {

	SetMaterialColor(1, 1.0, 0.0, 0.0);
	SetMaterialColor(2, 1.0, 0.0, 0.0);

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3d(-0.5, 0.0001, 0);
	  	glVertex3d(-1.5, 0.0001, 1);
	  	glVertex3d(-1.25, 0.0001, 1.25);
	  	glVertex3d(-0.25, 0.0001, 0.25);

	  	glNormal3f(0.0, 1.0, 0.0);
		glVertex3d(-1.25, 0.0001, 0.0);
	  	glVertex3d(-1.5, 0.0001, 0.25);
	  	glVertex3d(-0.5, 0.0001, 1.25);
	  	glVertex3d(-0.25, 0.0001, 1.0);
	 glEnd();

}

/* set material color */
void Goal::SetMaterialColor(int side, double r, double g, double b) {
  float	amb[4], dif[4], spe[4];
  int mat;

  dif[0] = r;
  dif[1] = g;
  dif[2] = b;

  for(int i = 0; i < 3; i++) {
    amb[i] = .1 * dif[i];
    spe[i] = .5;
  }
  amb[3] = dif[3] = spe[3] = 1.0;

  switch(side){
    case 1:	mat = GL_FRONT;
      break;
    case 2:	mat = GL_BACK;
      break;
    default: mat = GL_FRONT_AND_BACK;
  }

  glMaterialfv(mat, GL_AMBIENT, amb);
  glMaterialfv(mat, GL_DIFFUSE, dif);
  glMaterialfv(mat, GL_SPECULAR, spe);
  glMaterialf( mat, GL_SHININESS, 20);
}
/* method to check if the goal is reached */

bool Goal::GoalReached() {
 return true;
}
