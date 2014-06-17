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

//#include <GL/gl.h>

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

void Cylinder::DrawCylinder(const Vec3& ctr, double r) {

	  int     i, j,
	          n1 = 12, n2 = 24;
	  Vec3    normal, v1;
	  double  a1, a1d = M_PI / n1,
	          a2, a2d = M_PI / n2,
	          s1, s2,
	          c1, c2;

	  glShadeModel(GL_SMOOTH);
	  for(i = 0; i < n1; i++){
	    a1 = i * a1d;

	    glBegin(GL_TRIANGLE_STRIP);
	    for(j = 0; j <= n2; j++){
	      a2 = (j + .5 * (i % 2)) * 2 * a2d;

	      s1 = sin(a1);
	      c1 = cos(a1);
	      s2 = sin(a2);
	      c2 = cos(a2);
	      normal = c1 * XVec3 + s1 * (c2 * YVec3 + s2 * ZVec3);
	      v1 = ctr + r * normal;
	      glNormal3dv(normal.p);
	      glVertex3dv(v1.p);

	      s1 = sin(a1 + a1d);
	      c1 = cos(a1 + a1d);
	      s2 = sin(a2 + a2d);
	      c2 = cos(a2 + a2d);
	      normal = c1 * XVec3 + s1 * (c2 * YVec3 + s2 * ZVec3);
	      v1 = ctr + r * normal;
	      glNormal3dv(normal.p);
	      glVertex3dv(v1.p);
	    }
	    glEnd();
	  }

//	double size = this->cylinderSize;
//	double halfLength = size*2;
//	int slices = 10;
//	for(int i=0; i<slices; i++) {
//		float theta = ((float)i)*2.0*M_PI;
//		float nextTheta = ((float)i+1)*2.0*M_PI;
//		glBegin(GL_TRIANGLE_STRIP);
//			/*vertex at middle of end */
//			glVertex3f(0.0, halfLength, 0.0);
//			/*vertices at edges of circle*/
//			glVertex3f(size*cos(theta), halfLength, size*sin(theta));
//			glVertex3f (size*cos(nextTheta), halfLength, size*sin(nextTheta));
//			/* the same vertices at the bottom of the cylinder*/
//			glVertex3f (size*cos(nextTheta), -halfLength, size*sin(nextTheta));
//			glVertex3f(size*cos(theta), -halfLength, size*sin(theta));
//			glVertex3f(0.0, -halfLength, 0.0);
//		glEnd();
//	}
}

