#include "Wall.h"

#include <GL/gl.h>
#include <cmath>

#include "vec3.hpp"

//Konstruktor
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
    this->newWallDotObs = Vec3(posX - a, 1.0, posZ + a);

}

//Destruktor
Wall::~Wall() {
	// TODO Auto-generated destructor stub
}

//Zeichnen der Hinderniswand
void Wall::DrawWall(){

	double a = (wallSize * (1 / sqrt(2)));

    glPushMatrix();
		glBegin(GL_QUADS);
			  glNormal3f(wallSize * 6, 0.0, 8.0);

			  glVertex3f(posX - a, 1.0, posZ + a);
			  glVertex3f(posX + a, 1.0, posZ - a);
			  glVertex3f(posX + a, 0.0, posZ - a);
			  glVertex3f(posX - a, 0.0, posZ + a);
		glEnd();
	glPopMatrix();

	//Kollisionspunkt updaten
    this->UpdateColliPoints();
}

//Neubelegeung des Kollisionspunktes
void Wall::UpdateColliPoints() {
	this->newWallDotObs = Vec3((this->wallDotObs.p[0] + posX ), (this->wallDotObs.p[1] + posY ), (this->wallDotObs.p[2] + posZ ));
}
