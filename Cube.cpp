/*
 * Cube.cpp
 *
 *  Created on: 03.06.2014
 *      Author: Kathrin
 */

#include "Cube.h"

#include <GL/gl.h>
#include <iostream>
#include "vec3.hpp"

Cube::Cube(double size, double posX, double posY, double posZ) {
	this->cubeSize = size;


	this->speedX = 0.0;
	this->speedY = 0.0;
	this->speedZ = 0.0;
	this->rotX = 0.0;
	this->rotY = 0.0;
	this->rotZ = 0.0;
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;

    this->cubeDotLeft = Vec3(-cubeSize, cubeSize, -cubeSize);
    this->cubeDotLeftNormVec = Vec3(-1, 0.0, 0.0);

    this->newCubeDotLeft = Vec3(-cubeSize, cubeSize, -cubeSize);
    this->newCubeDotLeftNormVec = Vec3(-1, 0.0, 0.0);


    this->cubeDotRight = Vec3(cubeSize, 0.0, cubeSize);
    this->cubeDotRightNormVec = Vec3(10, 0.0, 0.0);

    this->newCubeDotRight = Vec3(cubeSize, 0.0, cubeSize);
    this->newCubeDotRightNormVec = Vec3(10, 0.0, 0.0);


    this->cubeDotFront = Vec3(-cubeSize, cubeSize, cubeSize);
    this->cubeDotFrontNormVec = Vec3(0.0, 0.0, 14);

    this->newCubeDotFront = Vec3(-cubeSize, cubeSize, cubeSize);
    this->newCubeDotFrontNormVec = Vec3(0.0, 0.0, 14);


    this->cubeDotBack = Vec3(-cubeSize, cubeSize, -cubeSize);
    this->cubeDotBackNormVec = Vec3(0.0, 0.0, 14);

    this->newCubeDotBack = Vec3(-cubeSize, cubeSize, -cubeSize);
    this->newCubeDotBackNormVec = Vec3(0.0, 0.0, 14);

}

Cube::~Cube() {
	// TODO Auto-generated destructor stub
}

// Zeichnen des Würfels
void Cube::DrawCube() {


   glPushMatrix();
//    glTranslated(posX, posY, posZ);
//      glRotated(rotX, 1, 0, 0);
//      glRotated(rotY, 0, 1, 0);// Rotation um die Y Achse
//      glRotated(rotZ, 0 ,0 ,1);
      glBegin(GL_QUADS);
      //ground
      glNormal3f(0.0, 0.0, cubeSize);

      glVertex3f(-cubeSize+posX, 0, cubeSize+posZ);
      glVertex3f(-cubeSize+posX, 0, -cubeSize+posZ);
      glVertex3f(cubeSize+posX, 0, -cubeSize+posZ);
      glVertex3f(cubeSize+posX, 0, cubeSize+posZ);

      //Bande left

      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize+posX, cubeSize, -cubeSize+posZ);
      glVertex3f(-cubeSize+posX, cubeSize, cubeSize+posZ);
      glVertex3f(-cubeSize+posX, 0, cubeSize+posZ);
      glVertex3f(-cubeSize+posX, 0, -cubeSize+posZ);

      //Bande right
      glNormal3f(0.0, 0.0, -cubeSize);

      glVertex3f(cubeSize+posX, 0, cubeSize+posZ);
      glVertex3f(cubeSize+posX, 0, -cubeSize+posZ);
      glVertex3f(cubeSize+posX, cubeSize, -cubeSize+posZ);
      glVertex3f(cubeSize+posX, cubeSize, cubeSize+posZ);

      //Bande front
      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize+posX, cubeSize, cubeSize+posZ);
      glVertex3f(-cubeSize+posX, 0, cubeSize+posZ);
      glVertex3f(cubeSize+posX, 0, cubeSize+posZ);
      glVertex3f(cubeSize+posX, cubeSize, cubeSize+posZ);

      //Bande back
      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize+posX, cubeSize, -cubeSize+posZ);
      glVertex3f(-cubeSize+posX, 0, -cubeSize+posZ);
      glVertex3f(cubeSize+posX, 0, -cubeSize+posZ);
      glVertex3f(cubeSize+posX, cubeSize, -cubeSize+posZ);

      //top
      glNormal3f(0.0, 0.0, -cubeSize);

      glVertex3f(-cubeSize+posX, cubeSize, cubeSize+posZ);
      glVertex3f(-cubeSize+posX, cubeSize, -cubeSize+posZ);
      glVertex3f(cubeSize+posX, cubeSize, -cubeSize+posZ);
      glVertex3f(cubeSize+posX, cubeSize, cubeSize+posZ);
      glEnd();
    glPopMatrix();

    this->UpdateColliPoints();
}

// Translate der Collisionswerte
void Cube::UpdateColliPoints() {


		/* Left */
		this->newCubeDotLeft = Vec3((this->cubeDotLeft.p[0] + posX ), (this->cubeDotLeft.p[1] + posY ), (this->cubeDotLeft.p[2] + posZ ));
		//this->newCubeDotLeftNormVec = Vec3((this->cubeDotLeftNormVec.p[0] + posX), (this->cubeDotLeftNormVec.p[1] + posY), (this->cubeDotLeftNormVec.p[2] + posZ));


		std::cout <<  this->cubeDotLeft.p[0] << this->cubeDotLeft.p[1] << this->cubeDotLeft.p[2]  << std::endl;
		std::cout << this->newCubeDotLeft.p[0] << this->newCubeDotLeft.p[1] << this->newCubeDotLeft.p[2]  << std::endl;

		/* Right */
		this->newCubeDotRight = Vec3((this->cubeDotRight.p[0] + posX ), (this->cubeDotRight.p[1] + posY ), (this->cubeDotRight.p[2] + posZ ));

		//this->newCubeDotRightNormVec = Vec3((this->cubeDotRightNormVec.p[0] + posX), (this->cubeDotRightNormVec.p[1] + posY), (this->cubeDotRightNormVec.p[2] + posZ));


		/* Front */
		this->newCubeDotFront = Vec3((this->cubeDotFront.p[0] + posX ), (this->cubeDotFront.p[1] + posY ), (this->cubeDotFront.p[2] + posZ ));

		//this->newCubeDotFrontNormVec = Vec3((this->cubeDotFrontNormVec.p[0] + posX), (this->cubeDotFrontNormVec.p[1] + posY), (this->cubeDotFrontNormVec.p[2] + posZ));


		/* Back */
		this->newCubeDotBack = Vec3((this->cubeDotBack.p[0] + posX ), (this->cubeDotBack.p[1] + posY  ), (this->cubeDotBack.p[2] +posZ ));

		//this->newCubeDotBackNormVec = Vec3((this->cubeDotBackNormVec.p[0] + posX), (this->cubeDotBackNormVec.p[1] + posY), (this->cubeDotBackNormVec.p[2] + posZ));

}
