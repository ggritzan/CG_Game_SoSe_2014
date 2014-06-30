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

    this->UpdateColliPoints();



}


double Cube::RotateCube(double x, double y, double z, int selector){

	double ergX;
	double ergY;
	double ergZ;

	// Rotation um die Y-Achse
	double c = cos(this->rotY);
	double s = sin(this->rotY);

	ergX = (x*c) - (z*s);
	ergY = y;
	ergZ = (x*s) + (z*c);

//	ergX = (x * c) - (s * z);
//	ergY = y;
//	ergZ = ( x * s) + (z * c);

//	std::cout << "OrgiX:" << x << std::endl;
//	std::cout << "NeuX:" <<ergX << std::endl;
//	std::cout << "OrgiZ:" <<z << std::endl;
//	std::cout << "NeuZ:" <<ergZ << std::endl;

	if(selector == 0){
		return ergX;
	}else if(selector == 1){
		return ergY;
	}else if (selector == 2){
		return ergZ;
	}


}


//Funktion zum aktualisieren der Kollisionspunkte
void Cube::UpdateColliPoints() {
		//Links
		this->newCubeDotLeft = Vec3((this->cubeDotLeft.p[0] + this->RotateCube(this->cubeDotLeft.p[0], this->cubeDotLeft.p[1], this->cubeDotLeft.p[2], 0) ), (this->cubeDotLeft.p[1] + this->RotateCube(this->cubeDotLeft.p[0], this->cubeDotLeft.p[1], this->cubeDotLeft.p[2], 1) ), (this->cubeDotLeft.p[2] + this->RotateCube(this->cubeDotLeft.p[0], this->cubeDotLeft.p[1], this->cubeDotLeft.p[2], 2) ));
		// NormVecLinks
		this->newCubeDotLeftNormVec = Vec3(this->RotateCube(this->cubeDotLeftNormVec.p[0],this->cubeDotLeftNormVec.p[1],this->cubeDotLeftNormVec.p[2],0), this->RotateCube(this->cubeDotLeftNormVec.p[0],this->cubeDotLeftNormVec.p[1],this->cubeDotLeftNormVec.p[2],1), this->RotateCube(this->cubeDotLeftNormVec.p[0],this->cubeDotLeftNormVec.p[1],this->cubeDotLeftNormVec.p[2],2));

		//Rechts
		this->newCubeDotRight = Vec3((this->cubeDotRight.p[0] + this->RotateCube(this->cubeDotRight.p[0], this->cubeDotRight.p[1], this->cubeDotRight.p[2], 0) ), (this->cubeDotRight.p[1] + this->RotateCube(this->cubeDotRight.p[0], this->cubeDotRight.p[1], this->cubeDotRight.p[2], 1) ), (this->cubeDotRight.p[2] + this->RotateCube(this->cubeDotRight.p[0], this->cubeDotRight.p[1], this->cubeDotRight.p[2], 2) ));
		// NormVecRechts
		this->newCubeDotRightNormVec = Vec3(this->RotateCube(this->cubeDotRightNormVec.p[0],this->cubeDotRightNormVec.p[1],this->cubeDotRightNormVec.p[2],0), this->RotateCube(this->cubeDotRightNormVec.p[0],this->cubeDotRightNormVec.p[1],this->cubeDotRightNormVec.p[2],1), this->RotateCube(this->cubeDotRightNormVec.p[0],this->cubeDotRightNormVec.p[1],this->cubeDotRightNormVec.p[2],2));

		//Vorne
		this->newCubeDotFront = Vec3((this->cubeDotFront.p[0] + this->RotateCube(this->cubeDotFront.p[0], this->cubeDotFront.p[1], this->cubeDotFront.p[2], 0) ), (this->cubeDotFront.p[1] + this->RotateCube(this->cubeDotFront.p[0], this->cubeDotFront.p[1], this->cubeDotFront.p[2], 1) ), (this->cubeDotFront.p[2] + this->RotateCube(this->cubeDotFront.p[0], this->cubeDotFront.p[1], this->cubeDotFront.p[2], 2) ));
		// NormVecFront
		this->newCubeDotFrontNormVec = Vec3(this->RotateCube(this->cubeDotFrontNormVec.p[0],this->cubeDotFrontNormVec.p[1],this->cubeDotFrontNormVec.p[2],0), this->RotateCube(this->cubeDotFrontNormVec.p[0],this->cubeDotFrontNormVec.p[1],this->cubeDotFrontNormVec.p[2],1), this->RotateCube(this->cubeDotFrontNormVec.p[0],this->cubeDotFrontNormVec.p[1],this->cubeDotFrontNormVec.p[2],2));

		///Hinten
		this->newCubeDotLeft = Vec3((this->cubeDotLeft.p[0] + this->RotateCube(this->cubeDotLeft.p[0], this->cubeDotLeft.p[1], this->cubeDotLeft.p[2], 0) ), (this->cubeDotLeft.p[1] + this->RotateCube(this->cubeDotLeft.p[0], this->cubeDotLeft.p[1], this->cubeDotLeft.p[2], 1) ), (this->cubeDotLeft.p[2] + this->RotateCube(this->cubeDotLeft.p[0], this->cubeDotLeft.p[1], this->cubeDotLeft.p[2], 2) ));
		// NormVecHinten
		this->newCubeDotBackNormVec = Vec3(this->RotateCube(this->cubeDotBackNormVec.p[0],this->cubeDotBackNormVec.p[1],this->cubeDotBackNormVec.p[2],0), this->RotateCube(this->cubeDotBackNormVec.p[0],this->cubeDotBackNormVec.p[1],this->cubeDotBackNormVec.p[2],1), this->RotateCube(this->cubeDotBackNormVec.p[0],this->cubeDotBackNormVec.p[1],this->cubeDotBackNormVec.p[2],2));
}
