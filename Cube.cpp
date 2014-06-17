/*
 * Cube.cpp
 *
 *  Created on: 03.06.2014
 *      Author: Kathrin
 */

#include "Cube.h"

#include <GL/gl.h>

#include "vec3.hpp"

Cube::Cube(double size) {
	this->cubeSize = size;

    this->cubeDotLeft = Vec3(-cubeSize, cubeSize, -cubeSize);
    this->cubeDotLeftNormVec = Vec3(-1, 0.0, 0.0);

    this->cubeDotRight = Vec3(0.0, 0.0, -1);
    this->cubeDotRightNormVec = Vec3(10, 0.0, 0.0);

    this->cubeDotFront = Vec3(-1, 0.0, 0.0);
    this->cubeDotFrontNormVec = Vec3(0.0, 0.0, 14);

    this->cubeDotBack = Vec3(-1, 0.0, 0.0);
    this->cubeDotBackNormVec = Vec3(0.0, 0.0, 14);

}

Cube::~Cube() {
	// TODO Auto-generated destructor stub
}

// Zeichnen des Würfels
void Cube::DrawCube() {
    glBegin(GL_QUADS);

      //ground
      glNormal3f(0.0, 0.0, cubeSize);

      glVertex3f(-cubeSize, 0, cubeSize);
      glVertex3f(-cubeSize, 0, -cubeSize);
      glVertex3f(cubeSize, 0, -cubeSize);
      glVertex3f(cubeSize, 0, cubeSize);

      //Bande left

      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize, cubeSize, -cubeSize);
      glVertex3f(-cubeSize, cubeSize, cubeSize);
      glVertex3f(-cubeSize, 0, cubeSize);
      glVertex3f(-cubeSize, 0, -cubeSize);

      //Bande right
      glNormal3f(0.0, 0.0, -cubeSize);

      glVertex3f(cubeSize, 0, cubeSize);
      glVertex3f(cubeSize, 0, -cubeSize);
      glVertex3f(cubeSize, cubeSize, -cubeSize);
      glVertex3f(cubeSize, cubeSize, cubeSize);

      //Bande front
      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize, cubeSize, cubeSize);
      glVertex3f(-cubeSize, 0, cubeSize);
      glVertex3f(cubeSize, 0, cubeSize);
      glVertex3f(cubeSize, cubeSize, cubeSize);

      //Bande back
      glNormal3f(-cubeSize, 0.0, 0.0);

      glVertex3f(-cubeSize, cubeSize, -cubeSize);
      glVertex3f(-cubeSize, 0, -cubeSize);
      glVertex3f(cubeSize, 0, -cubeSize);
      glVertex3f(cubeSize, cubeSize, -cubeSize);

      //top
      glNormal3f(0.0, 0.0, -cubeSize);

      glVertex3f(-cubeSize, cubeSize, cubeSize);
      glVertex3f(-cubeSize, cubeSize, -cubeSize);
      glVertex3f(cubeSize, cubeSize, -cubeSize);
      glVertex3f(cubeSize, cubeSize, cubeSize);
    glEnd();
}

