/*
 * Cube.cpp
 *
 *  Created on: 03.06.2014
 *      Author: Kathrin, Giacomo, Nina
 */

#include "Cube.h"
#include <GL/gl.h>
#include <iostream>s
#include "vec3.hpp"

//Konstruktor
Cube::Cube(double size, double posX, double posY, double posZ) {
	this->cubeSize = size;
	this->speedX = 0.0;
	this->speedY = 0.0;
	this->speedZ = 0.0;
	this->rotX = 0.0;
	this->rotY = 0.0;
	this->rotZ = 0.0;
	this->posXOriginal = posX;
	this->posYOriginal = posY;
	this->posZOriginal = posZ;
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

//Destruktor
Cube::~Cube() {
}

//Zeichnen des Würfels
void Cube::DrawCube() {
	this->RotateCube();
	this->UpdateColliPoints();

   glPushMatrix();


   glRotated(rotY, 0, 1, 0);



      glBegin(GL_QUADS);
      //Boden
      glNormal3f(0.0, 0.0, cubeSize);

      glVertex3f(-cubeSize+posXOriginal, 0, cubeSize+posZOriginal);
      glVertex3f(-cubeSize+posXOriginal, 0, -cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, 0, -cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, 0, cubeSize+posZOriginal);

      //Linke Wand
      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize+posXOriginal, cubeSize, -cubeSize+posZOriginal);
      glVertex3f(-cubeSize+posXOriginal, cubeSize, cubeSize+posZOriginal);
      glVertex3f(-cubeSize+posXOriginal, 0, cubeSize+posZOriginal);
      glVertex3f(-cubeSize+posXOriginal, 0, -cubeSize+posZOriginal);

      //Rechte Wand
      glNormal3f(0.0, 0.0, -cubeSize);

      glVertex3f(cubeSize+posXOriginal, 0, cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, 0, -cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, cubeSize, -cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, cubeSize, cubeSize+posZOriginal);

      //Vordere Wand
      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize+posXOriginal, cubeSize, cubeSize+posZOriginal);
      glVertex3f(-cubeSize+posXOriginal, 0, cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, 0, cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, cubeSize, cubeSize+posZOriginal);

      //BHintere Wand
      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize+posXOriginal, cubeSize, -cubeSize+posZOriginal);
      glVertex3f(-cubeSize+posXOriginal, 0, -cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, 0, -cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, cubeSize, -cubeSize+posZOriginal);

      //Deckel
      glNormal3f(0.0, 0.0, -cubeSize);

      glVertex3f(-cubeSize+posXOriginal, cubeSize, cubeSize+posZOriginal);
      glVertex3f(-cubeSize+posXOriginal, cubeSize, -cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, cubeSize, -cubeSize+posZOriginal);
      glVertex3f(cubeSize+posXOriginal, cubeSize, cubeSize+posZOriginal);
      glEnd();


    glPopMatrix();



    //Update der Kollisionspunkte

}


void Cube::RotateCube(){

	double c = cos(rotY);
	double s = sin(rotY);

	this->posX = (this->posXOriginal * c) - (s*this->posZOriginal);
	this->posY = this->posYOriginal;
	this->posZ = ( this->posXOriginal * s) + (this->posZOriginal*c);


}


//Funktion zum aktualisieren der Kollisionspunkte
void Cube::UpdateColliPoints() {
		//Links
		this->newCubeDotLeft = Vec3((this->cubeDotLeft.p[0] + posX ), (this->cubeDotLeft.p[1] + posY ), (this->cubeDotLeft.p[2] + posZ ));

		//Rechts
		this->newCubeDotRight = Vec3((this->cubeDotRight.p[0] + posX ), (this->cubeDotRight.p[1] + posY ), (this->cubeDotRight.p[2] + posZ ));

		//Vorne
		this->newCubeDotFront = Vec3((this->cubeDotFront.p[0] + posX ), (this->cubeDotFront.p[1] + posY ), (this->cubeDotFront.p[2] + posZ ));

		///Hinten
		this->newCubeDotBack = Vec3((this->cubeDotBack.p[0] + posX ), (this->cubeDotBack.p[1] + posY  ), (this->cubeDotBack.p[2] +posZ ));
}
