#include <GL/gl.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "vec3.hpp"
#include "BilliardTable.h"

BilliardTable::BilliardTable(double size) {
    this->billiardTableSize = size;

    this->wallDotLeft = Vec3(-billiardTableSize*7, billiardTableSize, billiardTableSize*-5);
    this->wallDotLeftNormVec = Vec3(10, 0.0, 0.0);

    this->wallDotRight = Vec3(billiardTableSize*7, 0, billiardTableSize*5);
    this->wallDotRightNormVec = Vec3(10, 0.0, 0.0);

    this->wallDotFront = Vec3(-billiardTableSize*7, billiardTableSize, billiardTableSize*5);
    this->wallDotFrontNormVec = Vec3(0.0, 0.0, 14);

    this->wallDotBack = Vec3(-billiardTableSize*7, billiardTableSize, billiardTableSize*-5);
    this->wallDotBackNormVec = Vec3(0.0, 0.0, 14);

    this->wallDotObs = Vec3(billiardTableSize * 3, billiardTableSize, billiardTableSize * 5);
    this->WallDotObsNormVec = Vec3(billiardTableSize * 6, 0.0, 8.0);
}

BilliardTable::~BilliardTable() {
    // TODO Auto-generated destructor stub
}

// Zeichnen des Würfels
void BilliardTable::DrawTable(){

    glBegin(GL_QUADS);

      //ground
      glNormal3f(0.0, 0.0, billiardTableSize);

      glVertex3f(-billiardTableSize*7, 0, billiardTableSize*5);
      glVertex3f(-billiardTableSize*7, 0, billiardTableSize*-5);
      glVertex3f(billiardTableSize*7, 0, billiardTableSize*-5);
      glVertex3f(billiardTableSize*7, 0, billiardTableSize*5);


      //Bande left

      glNormal3f(-billiardTableSize, 0.0, 0.0);

      glVertex3f(-billiardTableSize*7, billiardTableSize, billiardTableSize*-5);
      glVertex3f(-billiardTableSize*7, billiardTableSize, billiardTableSize*5);
      glVertex3f(-billiardTableSize*7, 0, billiardTableSize*5);
      glVertex3f(-billiardTableSize*7, 0, billiardTableSize*-5);

      //Bande right
      glNormal3f(0.0, 0.0, -billiardTableSize);

      glVertex3f(billiardTableSize*7, 0, billiardTableSize*5);
      glVertex3f(billiardTableSize*7, 0, billiardTableSize*-5);
      glVertex3f(billiardTableSize*7, billiardTableSize, billiardTableSize*-5);
      glVertex3f(billiardTableSize*7, billiardTableSize, billiardTableSize*5);

      //Bande front
      glNormal3f(-billiardTableSize, 0.0, 0.0);

      glVertex3f(-billiardTableSize*7, billiardTableSize, billiardTableSize*5);
      glVertex3f(-billiardTableSize*7, 0, billiardTableSize*5);
      glVertex3f(billiardTableSize*7, 0, billiardTableSize*5);
      glVertex3f(billiardTableSize*7, billiardTableSize, billiardTableSize*5);

      //Bande back
      glNormal3f(-billiardTableSize, 0.0, 0.0);

      glVertex3f(-billiardTableSize*7, billiardTableSize, billiardTableSize*-5);
      glVertex3f(-billiardTableSize*7, 0, billiardTableSize*-5);
      glVertex3f(billiardTableSize*7, 0, billiardTableSize*-5);
      glVertex3f(billiardTableSize*7, billiardTableSize, billiardTableSize*-5);

      //Hinderniswand
      glNormal3f(billiardTableSize * 6, 0.0, 8.0);

      glVertex3f(billiardTableSize * 3, billiardTableSize, billiardTableSize * 5);
      glVertex3f(billiardTableSize * 7, billiardTableSize, billiardTableSize * 2);
      glVertex3f(billiardTableSize * 7, 0, billiardTableSize * 2);
      glVertex3f(billiardTableSize * 3, 0, billiardTableSize * 5);
    glEnd();
}
