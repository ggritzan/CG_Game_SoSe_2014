#include <GL/gl.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "vec3.hpp"
#include "Table.h"

//Konstruktor
Table::Table(double size) {
    this->tableSize = size;

    this->wallDotLeft = Vec3(-tableSize*7, tableSize, tableSize*-5);
    this->wallDotLeftNormVec = Vec3(10, 0.0, 0.0);

    this->wallDotRight = Vec3(tableSize*7, 0, tableSize*5);
    this->wallDotRightNormVec = Vec3(10, 0.0, 0.0);

    this->wallDotFront = Vec3(-tableSize*7, tableSize, tableSize*5);
    this->wallDotFrontNormVec = Vec3(0.0, 0.0, 14);

    this->wallDotBack = Vec3(-tableSize*7, tableSize, tableSize*-5);
    this->wallDotBackNormVec = Vec3(0.0, 0.0, 14);

    this->wallDotObs = Vec3(tableSize * 3, tableSize, tableSize * 5);
    this->WallDotObsNormVec = Vec3(tableSize * 6, 0.0, 8.0);
}

//Destruktor
Table::~Table() {
    // TODO Auto-generated destructor stub
}

//Zeichnen des Tisches
void Table::DrawTable(){

    glBegin(GL_QUADS);

      //Boden
      glNormal3f(0.0, 0.0, tableSize);

      glVertex3f(-tableSize*7, 0, tableSize*5);
      glVertex3f(-tableSize*7, 0, tableSize*-5);
      glVertex3f(tableSize*7, 0, tableSize*-5);
      glVertex3f(tableSize*7, 0, tableSize*5);


      //Linke Bande

      glNormal3f(-tableSize, 0.0, 0.0);

      glVertex3f(-tableSize*7, tableSize, tableSize*-5);
      glVertex3f(-tableSize*7, tableSize, tableSize*5);
      glVertex3f(-tableSize*7, 0, tableSize*5);
      glVertex3f(-tableSize*7, 0, tableSize*-5);

      //Rechte Bande
      glNormal3f(0.0, 0.0, -tableSize);

      glVertex3f(tableSize*7, 0, tableSize*5);
      glVertex3f(tableSize*7, 0, tableSize*-5);
      glVertex3f(tableSize*7, tableSize, tableSize*-5);
      glVertex3f(tableSize*7, tableSize, tableSize*5);

      //Vordere Bande
      glNormal3f(-tableSize, 0.0, 0.0);

      glVertex3f(-tableSize*7, tableSize, tableSize*5);
      glVertex3f(-tableSize*7, 0, tableSize*5);
      glVertex3f(tableSize*7, 0, tableSize*5);
      glVertex3f(tableSize*7, tableSize, tableSize*5);

      //Hintere Bande
      glNormal3f(-tableSize, 0.0, 0.0);

      glVertex3f(-tableSize*7, tableSize, tableSize*-5);
      glVertex3f(-tableSize*7, 0, tableSize*-5);
      glVertex3f(tableSize*7, 0, tableSize*-5);
      glVertex3f(tableSize*7, tableSize, tableSize*-5);

    glEnd();

}
