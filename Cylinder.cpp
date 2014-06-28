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


	const double PI2 = 6.28272;

	//Zeichnen des Deckels
	glBegin(GL_TRIANGLE_FAN);
		for( double i = 0.0; i < PI2; i += PI2 / 360.0 ) {
			float x = xm + ( cos( i ) * r );
			float z = zm + ( sin( i ) * r);
			glVertex3f( x, 0.0001 ,z );
		  }
	glEnd();


		for(double j = 0.0001; j < 2.0; j += 0.1){

			glBegin(GL_TRIANGLE_FAN);
			for( double i = 0.0; i < PI2; i += PI2 / 360.0 ) {

				float x = xm + ( cos( i ) * r );
				float y = j;
				float z = zm + ( sin( i ) * r);
				glVertex3f( x, y ,z );

			  }
			glEnd();
		}


	//Zeichnen des Boden
	glBegin(GL_TRIANGLE_FAN);
			for( double i = 0.0; i < PI2; i += PI2 / 360.0 ) {
				float x = xm + ( cos( i ) * r );
				float z = zm + ( sin( i ) * r);
				glVertex3f( x, 2.0 ,z );
			  }
	glEnd();
}

