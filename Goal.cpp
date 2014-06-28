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
#define degToRad( x ) ( x*0.0174532925 )


/* constructor */
Goal::Goal(double goalSize, double posX, double posY, double posZ) {
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

	/* draws the cross */
	glBegin(GL_QUADS);
			glNormal3f(0.0, 1.0, 0.0);
			/*										x     								y     						z                  */
			glVertex3d( (( -(this->goalSize / (this->goalSize*2))) + this->posX),	this->posY,	(-(this->goalSize/2.0)) + this->posZ);
			glVertex3d( (( -(this->goalSize + (this->goalSize/5.0))) + this->posX), this->posY,	(this->goalSize/3.0) + this->posZ);
			glVertex3d( (-this->goalSize) + this->posX, 							this->posY, (this->goalSize/2.0) + this->posZ);
			glVertex3d( (-(this->goalSize/5.0) + this->posX),						this->posY, (-(this->goalSize/3.0)) + this->posZ);

			glNormal3f(0.0, 1.0, 0.0);
			/*										x     								y     						z                  */
			glVertex3d((-this->goalSize) + this->posX,								this->posY, (-(this->goalSize/2.0)) + this->posZ);
			glVertex3d( (( -(this->goalSize + (this->goalSize/5.0))) + this->posX), this->posY, (-(this->goalSize/3.0)) + this->posZ);
			glVertex3d((( -(this->goalSize / (this->goalSize*2))) + this->posX), 	this->posY, (this->goalSize/2.0) + this->posZ);
			glVertex3d((-(this->goalSize/5.0)) + this->posX, 						this->posY, (this->goalSize/3.0) + this->posZ);
	glEnd();

	/* draws the circle */
	SetMaterialColor(3, 1.0, 1.0, 1.0);
	const double PI2 = 6.28272;
	double xm = (posX - (goalSize-(this->goalSize/3.4)));
	double ym = posY - 0.0001;
	double zm = posZ ;
	double r = goalSize/1.5;


	/* draws the circle */
	glBegin(GL_TRIANGLE_FAN);
		for( double i = 0.0; i < PI2; i += PI2 / 360.0 ) {
			float x = xm + ( cos( i ) * r );
			float y = zm + ( sin( i ) * r);
			glVertex3f( x, ym ,y );
		  }
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
bool Goal::GoalReached(double ballPosX, double ballPosY, double ballPosZ) {

	if (ballPosX >= ( -(this->goalSize + (this->goalSize/5.0))) + this->posX && ballPosX <= (-(this->goalSize/5.0)) + this->posX && ballPosZ >= 0 + this->posZ && ballPosZ <= ((this->goalSize/2.0) + this->posZ)) {
		return true;
	} else {
		return false;
	}
}
