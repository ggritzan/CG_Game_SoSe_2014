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
    this->obstacle = false;

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

      if (this->obstacle) {
		  //Hinderniswand
		  glNormal3f(billiardTableSize * 6, 0.0, 8.0);

		  glVertex3f(billiardTableSize * 3, billiardTableSize, billiardTableSize * 5);
		  glVertex3f(billiardTableSize * 7, billiardTableSize, billiardTableSize * 2);
		  glVertex3f(billiardTableSize * 7, 0, billiardTableSize * 2);
		  glVertex3f(billiardTableSize * 3, 0, billiardTableSize * 5);
      }
    glEnd();

//    // Beine des Tisches
//    const double PI2 = 6.28272;
//
//    //Zeichnen des Deckels
//    for(double j = 0.0001; j < 2.0; j += 0.01){
//
//		glBegin(GL_TRIANGLE_FAN);
//		for( double i = 0.0; i < PI2; i += PI2 / 360.0 ) {
//
//			float x = 7 + ( cos( i ) * (billiardTableSize/3)) -(billiardTableSize/6);
//			float y = j -2.0;
//			float z = 5 + ( sin( i ) * (billiardTableSize/3)) -(billiardTableSize/6);
//			glVertex3f( x, y ,z );
//
//		  }
//		glEnd();
//    }

}
