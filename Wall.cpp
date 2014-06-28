#include "Wall.h"

#include <GL/gl.h>
#include <cmath>

#include "vec3.hpp"

Wall::Wall(double size, double posX, double posY, double posZ) {
	this->wallSize = size;
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;
	this->rotX = 0.0;
	this->rotY = 0.0;
	this->rotZ = 0.0;

    this->obstacle = false;

	double a = (wallSize * (1 / sqrt(2)));

    this->wallDotObs = Vec3(posX - a, 1.0, posZ + a);
    this->WallDotObsNormVec = Vec3(-2 * a, 0.0, -2 * a);

}

Wall::~Wall() {
	// TODO Auto-generated destructor stub
}


void Wall::DrawWall(){

	double a = (wallSize * (1 / sqrt(2)));


    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotated(rotX, 1, 0, 0);
    glRotated(rotY, 0, 1, 0);// Rotation um die Y Achse
    glRotated(rotZ, 0 ,0 ,1);
		glBegin(GL_QUADS);
			  //Hinderniswand
			  glNormal3f(wallSize * 6, 0.0, 8.0);

			  glVertex3f(posX - a, 1.0, posZ + a);
			  glVertex3f(posX + a, 1.0, posZ - a);
			  glVertex3f(posX + a, 0.0, posZ - a);
			  glVertex3f(posX - a, 0.0, posZ + a);
		glEnd();
	glPopMatrix();
}
