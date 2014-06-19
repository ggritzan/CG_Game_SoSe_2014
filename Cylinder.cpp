/*
 * Cylinder.cpp
 *
 *  Created on: 14.06.2014
 *      Author: Kathrin
 */

#include "Cylinder.h"

#include <GL/gl.h>
#include <cmath>

#include "vec3.hpp"

#include <GL/gl.h>

Cylinder::Cylinder(float size, double posX, double posY, double posZ) {
	this->cylinderSize = size;
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;
	this->speedX = 0.0;
	this->speedY = 0.0;
	this->speedZ = 0.0;

}

Cylinder::~Cylinder() {
	// TODO Auto-generated destructor stub
}

void Cylinder::DrawCylinder(double xm, double ym, double zm, double r) {

	//Zeichnen des Deckels
	glBegin(GL_LINE_LOOP);
	for(double j = 0; j < r; j = j + 0.005){
		for(int i = 0; i <= 360; i++){
			double angle = 2 * M_PI * i / 360;
			double x = j * cos(angle) + xm;
			double z = j * sin(angle) + zm;
			double y = 2.0;
			glVertex3d(x, y, z);
		}
	}
	glEnd();


	//Zeichnen des Mantels
	glBegin(GL_LINE_LOOP);
		for(double j = 0; j < 2; j = j + 0.005){
			for(int i = 0; i <= 360; i++){
				double angle = 2 * M_PI * i / 360;
				double x = cos(angle) + xm;
				double z = sin(angle) + zm;
				double y = j;
				glVertex3d(x, y, z);
			}
		}
	glEnd();

	//Zeichnen des Deckels
	glBegin(GL_LINE_LOOP);
	for(double j = 0; j < r; j = j + 0.005){
		for(int i = 0; i <= 360; i++){
			double angle = 2 * M_PI * i / 360;
			double x = j * cos(angle) + xm;
			double z = j * sin(angle) + zm;
			double y = 0.0;
			glVertex3d(x, y, z);
		}
	}
	glEnd();
}

