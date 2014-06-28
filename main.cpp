
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "BilliardBall.h"
#include "BilliardTable.h"
#include "Cube.h"
#include "Goal.h"
#include "Cylinder.h"
#include "vec3.hpp"

static double alpha_ = 35;		// Winkel für Y Achse
static double beta_ = 25;		// Winkel für X Achse

static float scale = 1.0;		// Skalierungswert
static float size = 1.0;		// Größe des Würfels entspricht immer der Hälfte der Würfel Größe
static float cubeVerschiebungX = 0.0;	// Verschiebung des Würfels auf der X-Achse
static float cubeVerschiebungY = 0.0;	// Verscheibung des Würfels auf der Y-Achse

static double sphereSize = 0.286;
static double sphereX = 5;
static double sphereY = sphereSize;
static double sphereZ = 0;

static bool selectionMode = true;

// Größe des OpenGL Fensters
static double window_width_ = 1024;
static double window_height_ = 768;

static int bs = 1;
static int cs = 0;

BilliardTable* table = new BilliardTable(size);

Cube* cube = new Cube(size, -5.0, 0.5, 5.0);

BilliardBall* whiteBall = new BilliardBall(sphereX, sphereY, sphereZ, sphereSize, 0.978, 1.0, 1.0, 1.0);

//Deklaration der Objektarrays
std::vector<BilliardBall*> ballVector;
//std::vector<Cube*> cubeVector;
std::vector<Cylinder*> cylinderVector;
Cube* cubeVector[5] = {};
//Cylinder* cylinderVector[5] = {};

void resetBalls() {
	for (int i = 0; i<ballVector.size(); i++) {
		ballVector.at(i)->collision = false;
	}
}

void checkBallsandWalls() {

	for (int i = 0; i<ballVector.size(); i++) {

	  if((!ballVector.at(i)->wallBack) && ballVector.at(i)->wallCollisionDetection(table->wallDotBack, table->wallDotBackNormVec)){
		  ballVector.at(i)->wallBack = true;
		  ballVector.at(i)->wallFront = false;
		  ballVector.at(i)->wallLeft = false;
		  ballVector.at(i)->wallRight = false;
		  ballVector.at(i)->wallObst = false;
		  ballVector.at(i)->speedZ = ballVector.at(i)->speedZ * -1;
	  }

	  if ((!ballVector.at(i)->wallFront) && ballVector.at(i)->wallCollisionDetection(table->wallDotFront, table->wallDotFrontNormVec)) {
		  ballVector.at(i)->wallBack = false;
		  ballVector.at(i)->wallFront = true;
		  ballVector.at(i)->wallLeft = false;
		  ballVector.at(i)->wallRight = false;
		  ballVector.at(i)->wallObst = false;
		  ballVector.at(i)->speedZ = ballVector.at(i)->speedZ * -1;
	  }

	  if ((!ballVector.at(i)->wallLeft) && ballVector.at(i)->wallCollisionDetection(table->wallDotLeft, table->wallDotLeftNormVec)) {
		  ballVector.at(i)->wallBack = false;
		  ballVector.at(i)->wallFront = false;
		  ballVector.at(i)->wallLeft = true;
		  ballVector.at(i)->wallRight = false;
		  ballVector.at(i)->wallObst = false;
		  ballVector.at(i)->speedX = ballVector.at(i)->speedX * -1;
	  }

	  if ((!ballVector.at(i)->wallRight) && ballVector.at(i)->wallCollisionDetection(table->wallDotRight, table->wallDotRightNormVec)) {
		  ballVector.at(i)->wallBack = false;
		  ballVector.at(i)->wallFront = false;
		  ballVector.at(i)->wallLeft = false;
		  ballVector.at(i)->wallRight = true;
		  ballVector.at(i)->wallObst = false;
		  ballVector.at(i)->speedX = ballVector.at(i)->speedX * -1;
	  }
	  if (table->obstacle) {
		  if ((!ballVector.at(i)->wallObst) && ballVector.at(i)->wallCollisionDetection(table->wallDotObs, table->WallDotObsNormVec)) {

			  ballVector.at(i)->wallBack = false;
			  ballVector.at(i)->wallFront = false;
			  ballVector.at(i)->wallLeft = false;
			  ballVector.at(i)->wallRight = false;
			  ballVector.at(i)->wallObst = true;

			  double amalV = ( (table->WallDotObsNormVec.p[0] * ballVector.at(i)->speedX) + (table->WallDotObsNormVec.p[1] * ballVector.at(i)->speedY) + (table->WallDotObsNormVec.p[2] * ballVector.at(i)->speedZ) );
			  double betragA = (sqrt((table->WallDotObsNormVec.p[0]*table->WallDotObsNormVec.p[0]) + (table->WallDotObsNormVec.p[1]*table->WallDotObsNormVec.p[1]) + (table->WallDotObsNormVec.p[2]*table->WallDotObsNormVec.p[2]))) * (sqrt((table->WallDotObsNormVec.p[0]*table->WallDotObsNormVec.p[0]) + (table->WallDotObsNormVec.p[1]*table->WallDotObsNormVec.p[1]) + (table->WallDotObsNormVec.p[2]*table->WallDotObsNormVec.p[2])));
			  double aNeuX = (2 * amalV / betragA) * table->WallDotObsNormVec.p[0];
			  double aNeuZ = (2 * amalV / betragA) * table->WallDotObsNormVec.p[2];

			  ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
			  ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;
		  }
	  }
	}

}

void checkBallsandCube() {

	for (int i = 0; i<ballVector.size(); i++) {

	  if((!ballVector.at(i)->cubeBack) && ballVector.at(i)->cubeCollisionDetection(cube->cubeDotBack, cube->cubeDotBackNormVec)){

		  ballVector.at(i)->cubeBack = true;
		  ballVector.at(i)->cubeFront = false;
		  ballVector.at(i)->cubeLeft = false;
		  ballVector.at(i)->cubeRight = false;

		  double amalV = ( (cube->cubeDotBackNormVec.p[0] * ballVector.at(i)->speedX) + (cube->cubeDotBackNormVec.p[1] * ballVector.at(i)->speedY) + (cube->cubeDotBackNormVec.p[2] * ballVector.at(i)->speedZ) );
		  double betragA = (sqrt((cube->cubeDotBackNormVec.p[0]*cube->cubeDotBackNormVec.p[0]) + (cube->cubeDotBackNormVec.p[1]*cube->cubeDotBackNormVec.p[1]) + (cube->cubeDotBackNormVec.p[2]*cube->cubeDotBackNormVec.p[2]))) * (sqrt((cube->cubeDotBackNormVec.p[0]*cube->cubeDotBackNormVec.p[0]) + (cube->cubeDotBackNormVec.p[1]*cube->cubeDotBackNormVec.p[1]) + (cube->cubeDotBackNormVec.p[2]*cube->cubeDotBackNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * cube->cubeDotBackNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * cube->cubeDotBackNormVec.p[2];

		  ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
		  ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;
	  }

	  if((!ballVector.at(i)->cubeFront) && ballVector.at(i)->cubeCollisionDetection(cube->cubeDotFront, cube->cubeDotFrontNormVec)){

		  ballVector.at(i)->cubeBack = false;
		  ballVector.at(i)->cubeFront = true;
		  ballVector.at(i)->cubeLeft = false;
		  ballVector.at(i)->cubeRight = false;

		  double amalV = ( (cube->cubeDotFrontNormVec.p[0] * ballVector.at(i)->speedX) + (cube->cubeDotFrontNormVec.p[1] * ballVector.at(i)->speedY) + (cube->cubeDotFrontNormVec.p[2] * ballVector.at(i)->speedZ) );
		  double betragA = (sqrt((cube->cubeDotFrontNormVec.p[0]*cube->cubeDotFrontNormVec.p[0]) + (cube->cubeDotFrontNormVec.p[1]*cube->cubeDotFrontNormVec.p[1]) + (cube->cubeDotFrontNormVec.p[2]*cube->cubeDotFrontNormVec.p[2]))) * (sqrt((cube->cubeDotFrontNormVec.p[0]*cube->cubeDotFrontNormVec.p[0]) + (cube->cubeDotFrontNormVec.p[1]*cube->cubeDotFrontNormVec.p[1]) + (cube->cubeDotFrontNormVec.p[2]*cube->cubeDotFrontNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * cube->cubeDotFrontNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * cube->cubeDotFrontNormVec.p[2];

		  ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
		  ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;
	  }

	  if((!ballVector.at(i)->cubeLeft) && ballVector.at(i)->cubeCollisionDetection(cube->cubeDotLeft, cube->cubeDotLeftNormVec)){
		  ballVector.at(i)->cubeBack = false;
		  ballVector.at(i)->cubeFront = false;
		  ballVector.at(i)->cubeLeft = true;
		  ballVector.at(i)->cubeRight = false;

		  double amalV = ( (cube->cubeDotLeftNormVec.p[0] * ballVector.at(i)->speedX) + (cube->cubeDotLeftNormVec.p[1] * ballVector.at(i)->speedY) + (cube->cubeDotLeftNormVec.p[2] * ballVector.at(i)->speedZ) );
		  double betragA = (sqrt((cube->cubeDotLeftNormVec.p[0]*cube->cubeDotLeftNormVec.p[0]) + (cube->cubeDotLeftNormVec.p[1]*cube->cubeDotLeftNormVec.p[1]) + (cube->cubeDotLeftNormVec.p[2]*cube->cubeDotLeftNormVec.p[2]))) * (sqrt((cube->cubeDotLeftNormVec.p[0]*cube->cubeDotLeftNormVec.p[0]) + (cube->cubeDotLeftNormVec.p[1]*cube->cubeDotLeftNormVec.p[1]) + (cube->cubeDotLeftNormVec.p[2]*cube->cubeDotLeftNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * cube->cubeDotLeftNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * cube->cubeDotLeftNormVec.p[2];

		  ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
		  ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;
	  }

	  if((!ballVector.at(i)->cubeRight) && ballVector.at(i)->cubeCollisionDetection(cube->cubeDotRight, cube->cubeDotRightNormVec)){
		  ballVector.at(i)->cubeBack = false;
		  ballVector.at(i)->cubeFront = false;
		  ballVector.at(i)->cubeLeft = false;
		  ballVector.at(i)->cubeRight = true;

		  double amalV = ( (cube->cubeDotRightNormVec.p[0] * ballVector.at(i)->speedX) + (cube->cubeDotRightNormVec.p[1] * ballVector.at(i)->speedY) + (cube->cubeDotRightNormVec.p[2] * ballVector.at(i)->speedZ) );
		  double betragA = (sqrt((cube->cubeDotRightNormVec.p[0]*cube->cubeDotRightNormVec.p[0]) + (cube->cubeDotRightNormVec.p[1]*cube->cubeDotRightNormVec.p[1]) + (cube->cubeDotRightNormVec.p[2]*cube->cubeDotRightNormVec.p[2]))) * (sqrt((cube->cubeDotRightNormVec.p[0]*cube->cubeDotRightNormVec.p[0]) + (cube->cubeDotRightNormVec.p[1]*cube->cubeDotRightNormVec.p[1]) + (cube->cubeDotRightNormVec.p[2]*cube->cubeDotRightNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * cube->cubeDotRightNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * cube->cubeDotRightNormVec.p[2];

		  ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
		  ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;
	  }
	}
}

void checkBallsandCylinder() {

	for (int i = 0; i<ballVector.size(); i++) {
		for (int j = 0; j<cylinderVector.size(); j++) {
			if(ballVector.at(i)->detectCollision(*cylinderVector.at(j)) && !(ballVector.at(i)->collision)){
				if (ballVector.at(i)->checkCollisionType(*ballVector.at(i), *cylinderVector.at(j))){
					//gerade Kugelkollision

					std::cout << "Gerade Kollision" << std::endl;

					ballVector.at(i)->collision = true;

					ballVector.at(i)->speedX = ballVector.at(i)->speedX * (-1);
					ballVector.at(i)->speedY = ballVector.at(i)->speedY * (-1);
					ballVector.at(i)->speedZ = ballVector.at(i)->speedZ * (-1);


				} else {
					ballVector.at(i)->collision = true;

					Vec3 n;
					n.p[0] = ( (ballVector.at(i)->posX - cylinderVector.at(j)->posX));
					n.p[1] = ( (ballVector.at(i)->posY - cylinderVector.at(j)->posY));
					n.p[2] = ( (ballVector.at(i)->posZ - cylinderVector.at(j)->posZ));

					Vec3 v1maln;
					v1maln.p[0] = ( cylinderVector.at(j)->posX * n.p[0] );
					v1maln.p[1] = ( cylinderVector.at(j)->posY * n.p[1] );
					v1maln.p[2] = ( cylinderVector.at(j)->posZ * n.p[2] );

					Vec3 v2maln;
					v2maln.p[0] = ( ballVector.at(i)->posX * n.p[0] );
					v2maln.p[1] = ( ballVector.at(i)->posY * n.p[1] );
					v2maln.p[2] = ( ballVector.at(i)->posZ * n.p[2] );

					Vec3 vplusn;
					vplusn.p[0] = ( ( v1maln.p[0] + v2maln.p[0] ) / 2.0 );
					vplusn.p[1] = ( ( v1maln.p[1] + v2maln.p[1] ) / 2.0 );
					vplusn.p[2] = ( ( v1maln.p[2] + v2maln.p[2] ) / 2.0 );

					Vec3 v1neu;
					v1neu.p[0] = ( ballVector.at(i)->speedX + ( 2.0 * ( vplusn.p[0] - v1maln.p[0] ) * n.p[0]) );
					v1neu.p[1] = ( ballVector.at(i)->speedY + ( 2.0 * ( vplusn.p[1] - v1maln.p[1] ) * n.p[1]) );
					v1neu.p[2] = ( ballVector.at(i)->speedZ + ( 2.0 * ( vplusn.p[2] - v1maln.p[2] ) * n.p[2]) );

					Vec3 v2neu;
					v2neu.p[0] = ( cylinderVector.at(j)->speedX + ( 2.0 * ( vplusn.p[0] - v2maln.p[0] ) * n.p[0]) );
					v2neu.p[1] = ( cylinderVector.at(j)->speedY + ( 2.0 * ( vplusn.p[1] - v2maln.p[1] ) * n.p[1]) );
					v2neu.p[2] = ( cylinderVector.at(j)->speedZ + ( 2.0 * ( vplusn.p[2] - v2maln.p[2] ) * n.p[2]) );

					ballVector.at(i)->speedX = v1neu.p[0];
					ballVector.at(i)->speedY = v1neu.p[1];
					ballVector.at(i)->speedZ = v1neu.p[2];
				}
			}
		}
	}
}

void checkBalls() {
	for (int i = 0; i<ballVector.size();i++) {
		for (int j = 0; j<ballVector.size(); j++) {
			if (i != j && ballVector.at(i)->detectCollision(*ballVector.at(j)) && !(ballVector.at(i)->collision && ballVector.at(j)->collision)) {
				ballVector.at(i)->wallBack = false;
				ballVector.at(i)->wallFront = false;
				ballVector.at(i)->wallLeft = false;
				ballVector.at(i)->wallRight = false;
				ballVector.at(i)->wallObst = false;
				ballVector.at(j)->wallBack = false;
				ballVector.at(j)->wallFront = false;
				ballVector.at(j)->wallLeft = false;
				ballVector.at(j)->wallRight = false;
				ballVector.at(j)->wallObst = false;

				if (ballVector.at(i)->checkCollisionType(*ballVector.at(i), *ballVector.at(j))){
					//gerade Kugelkollision

					std::cout << "Gerade Kollision" << std::endl;

					ballVector.at(j)->collision = true;
					ballVector.at(i)->collision = true;

					Vec3 temp;
					temp.p[0] = ballVector.at(i)->speedX;
					temp.p[1] = ballVector.at(i)->speedY;
					temp.p[2] = ballVector.at(i)->speedZ;

					ballVector.at(i)->speedX = ballVector.at(j)->speedX;
					ballVector.at(i)->speedY = ballVector.at(j)->speedY;
					ballVector.at(i)->speedZ = ballVector.at(j)->speedZ;

					ballVector.at(j)->speedX = temp.p[0];
					ballVector.at(j)->speedY= temp.p[1];
					ballVector.at(j)->speedZ = temp.p[2];

				} else {

					ballVector.at(j)->collision = true;
					ballVector.at(i)->collision = true;

//					double betragMittelpunkte = sqrt( ( (bList[j]->posX - bList[i]->posX) * (bList[j]->posX - bList[i]->posX) ) + ( (bList[j]->posY - bList[i]->posY) * (bList[j]->posY - bList[i]->posY) ) + ( (bList[j]->posZ - bList[i]->posZ) * (bList[j]->posZ - bList[i]->posZ) ) );

					Vec3 n;
					n.p[0] = ( (ballVector.at(j)->posX - ballVector.at(i)->posX));
					n.p[1] = ( (ballVector.at(j)->posY - ballVector.at(i)->posY));
					n.p[2] = ( (ballVector.at(j)->posZ - ballVector.at(i)->posZ));

					Vec3 v1maln;
					v1maln.p[0] = ( ballVector.at(i)->posX * n.p[0] );
					v1maln.p[1] = ( ballVector.at(i)->posY * n.p[1] );
					v1maln.p[2] = ( ballVector.at(i)->posZ * n.p[2] );

					Vec3 v2maln;
					v2maln.p[0] = ( ballVector.at(j)->posX * n.p[0] );
					v2maln.p[1] = ( ballVector.at(j)->posY * n.p[1] );
					v2maln.p[2] = ( ballVector.at(j)->posZ * n.p[2] );

					Vec3 vplusn;
					vplusn.p[0] = ( ( v1maln.p[0] + v2maln.p[0] ) / 2.0 );
					vplusn.p[1] = ( ( v1maln.p[1] + v2maln.p[1] ) / 2.0 );
					vplusn.p[2] = ( ( v1maln.p[2] + v2maln.p[2] ) / 2.0 );
//					Vec3 vplusn = (v1maln.operator +=(v2maln)) / 2.0;

					Vec3 v1neu;
					v1neu.p[0] = ( ballVector.at(j)->speedX + ( 2.0 * ( vplusn.p[0] - v1maln.p[0] ) * n.p[0]) );
					v1neu.p[1] = ( ballVector.at(j)->speedY + ( 2.0 * ( vplusn.p[1] - v1maln.p[1] ) * n.p[1]) );
					v1neu.p[2] = ( ballVector.at(j)->speedZ + ( 2.0 * ( vplusn.p[2] - v1maln.p[2] ) * n.p[2]) );

					Vec3 v2neu;
					v2neu.p[0] = ( ballVector.at(i)->speedX + ( 2.0 * ( vplusn.p[0] - v2maln.p[0] ) * n.p[0]) );
					v2neu.p[1] = ( ballVector.at(i)->speedY + ( 2.0 * ( vplusn.p[1] - v2maln.p[1] ) * n.p[1]) );
					v2neu.p[2] = ( ballVector.at(i)->speedZ + ( 2.0 * ( vplusn.p[2] - v2maln.p[2] ) * n.p[2]) );

					ballVector.at(j)->speedX = v1neu.p[0];
					ballVector.at(j)->speedY = v1neu.p[1];
					ballVector.at(j)->speedZ = v1neu.p[2];

					ballVector.at(i)->speedX = v2neu.p[0];
					ballVector.at(i)->speedY = v2neu.p[1];
					ballVector.at(i)->speedZ = v2neu.p[2];

				}
			}
		}
	}
}

// Setzen der Materialfarbe
void SetMaterialColor(int side, double r, double g, double b) {
  float	amb[4], dif[4], spe[4];
  int mat;

  dif[0] = r;
  dif[1] = g;
  dif[2] = b;

  for(int i = 0; i < 3; i++) {
    amb[i] = .1 * dif[i];
    spe[i] = .5;
  }
  amb[3] = dif[3] = spe[3] = 1.0;

  switch(side){
    case 1:	mat = GL_FRONT;
      break;
    case 2:	mat = GL_BACK;
      break;
    default: mat = GL_FRONT_AND_BACK;
  }

  glMaterialfv(mat, GL_AMBIENT, amb);
  glMaterialfv(mat, GL_DIFFUSE, dif);
  glMaterialfv(mat, GL_SPECULAR, spe);
  glMaterialf( mat, GL_SHININESS, 20);
}

void drawVectorBalls(){
	for (int i = 0; i<ballVector.size();i++) {
		if (bs == i && selectionMode) {
			SetMaterialColor(1, 0.0, 1.0, 1.0);
		} else {
			SetMaterialColor(1, ballVector.at(i)->colourR, ballVector.at(i)->colourG, ballVector.at(i)->colourB);
		}
		ballVector.at(i)->DrawBall(Vec3( ballVector.at(i)->posX, ballVector.at(i)->posY,ballVector.at(i)->posZ), ballVector.at(i)->ballSize);
	}
}
void drawVectorCylinders() {
	for (int i = 0; i<cylinderVector.size();i++) {
		if (cs == i && selectionMode) {
			SetMaterialColor(1, 0.0, 1.0, 1.0);
		} else {
	  	  SetMaterialColor(1, 0.0, 0.0, 0.0);
		}
	  	  cylinderVector.at(i)->DrawCylinder(cylinderVector.at(i)->posX, cylinderVector.at(i)->posY, cylinderVector.at(i)->posZ, cylinderVector.at(i)->cylinderSize);
	}
}
// Erstellung der Beleuchtung
void InitLighting() {
  GLfloat lp1[4]  = { 10,  5,  10,  0};
  GLfloat lp2[4]  = { -5,  5, -10,  0};
  GLfloat red[4]  = {1, 0.8,  0.8,  1};		//Beleuchtungsfarbe einstellen
  GLfloat blue[4] = { .8, .8, 1.0,  1};

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glShadeModel(GL_SMOOTH);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
  glEnable(GL_LIGHTING);

  glLightfv(GL_LIGHT1, GL_POSITION, lp1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  red);
  glLightfv(GL_LIGHT1, GL_SPECULAR, red);
  glEnable(GL_LIGHT1);

  glLightfv(GL_LIGHT2, GL_POSITION, lp2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE,  blue);
  glLightfv(GL_LIGHT2, GL_SPECULAR, blue);
  glEnable(GL_LIGHT2);

  glClearColor(1, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // init viewport to canvassize
  glViewport(0, 0, window_width_, window_height_);

  // init coordinate system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-15, 15, -10, 10, -20, 20);
//glFrustum(-15, 15, -10, 10, -20, 20);


 glScalef(scale, scale, scale );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Zeichnen der Szene
void Preview() {

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();				  // Reset The Current Modelview Matrix

  glPushMatrix();

  glTranslated(cubeVerschiebungX, cubeVerschiebungY, 0);

  glRotated(alpha_, 0 , 1, 0);	// Rotation um die Y Achse
  glRotated(beta_, 1 ,0 ,0);	// Rotation um die X Achse

  glScalef(scale, scale, scale );

  //BilliardTable colours inside and outside
  SetMaterialColor(2, 0.0, 1.0, 0.0);
  SetMaterialColor(1, 0.0, 1.0, 0.2);
  table->DrawTable();

  //Rotes Zielkreuz
  /* maxsize 1.5		size,   x,      y,   z    */
  Goal* goal = new Goal(1.25, -2.0, 0.0002, 0.0);
  goal->DrawGoal();
  /* GoalReached - decreases the speed of the billiardball to 0 when the area is reached */
  if(goal->GoalReached(ballVector.at(0)->posX, ballVector.at(0)->posY, ballVector.at(0)->posZ)) {
	  ballVector.at(0)->speedX=0;
	  ballVector.at(0)->speedZ=0;
  }

  checkBallsandWalls();

  checkBalls();

  //checkBallsandCylinder();
  //checkBallsandCube();
  resetBalls();

  for(int i =0; i<ballVector.size(); i++) {
	  ballVector.at(i)->updatePosition();
  }

  if (ballVector.size()>=1) {
	  drawVectorBalls();
  }
  if (cylinderVector.size()>=1) {
	  drawVectorCylinders();
  }
  //SetMaterialColor(1, 0.0, 1.0, 0.0);
  //cube->DrawCube();

  glPopMatrix();

}

// Key Callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
//Pfeiltasten moving the table
	// (<-)
	if (key == 263 && action == 2 && !(mods==GLFW_MOD_SHIFT || mods==GLFW_MOD_CONTROL)){
		cubeVerschiebungX -= 0.1;
	}

	// (->)
	if (key == 262 && action == 2 && !(mods==GLFW_MOD_SHIFT || mods==GLFW_MOD_CONTROL)){
		cubeVerschiebungX += 0.1;
	}

	// (^)
	if (key == 265 && action == 2 && !(mods==GLFW_MOD_SHIFT || mods==GLFW_MOD_CONTROL)){
		cubeVerschiebungY += 0.1;
	}

	// (v)
	if (key == 264 && action == 2 && !(mods==GLFW_MOD_SHIFT || mods==GLFW_MOD_CONTROL)){
		cubeVerschiebungY -= 0.1;
	}
	//WASD rotating the table
		// (W)
	if (key == 87 && action == 2){
		beta_ -= .9;
	}

		// (A)
	if (key == 65 && action == 2){
		alpha_ -= .9;
	}

		// (S)
	if (key == 83 && action == 2){
		beta_ += .9;
	}

		// (D)
	if (key == 68 && action == 2){
		alpha_ += .9;
	}

	if(selectionMode) {
		//+ - zum Iterieren durch die Auswahl
			//zum Iterieren durch die Kugelauswahl Shift und +/-
			if(key == 45 && action == 1 && mods==GLFW_MOD_SHIFT) {
				if (bs-1 <= 0) {
					bs = ballVector.size()-1;
				} else {
					bs--;
				}
			}
			if(key == 61 && action == 1 && mods==GLFW_MOD_SHIFT) {
				if (bs+1 >= ballVector.size()) {
					bs = 1;
				} else {
					bs++;
				}
			}
			//zum Iterieren durch die Zylinderauswahl strg und +/-
			if(key == 45 && action == 1 && mods==GLFW_MOD_CONTROL) {
				if (cs-1 < 0) {
					cs = cylinderVector.size()-1;
				} else {
					cs--;
				}
			}
			if(key == 61 && action == 1 && mods==GLFW_MOD_CONTROL) {
				if (cs+1 >= cylinderVector.size()) {
					cs = 0;
				} else {
					cs++;
				}
			}

			//BCZO Creating new Objects
			// (B) Creates a new Ball
			if (key == 66 && action == 1){
				ballVector.push_back(new BilliardBall(5, sphereY, -3,sphereSize, 0.978, 0.0, 0.0, 0.0));

			}
			// (C) Creates a new Cube
			if (key == 67 && action == 1){
				//

			}
			// (Z) Creates a new Cylinder
			if (key == 90 && action == 1){
				cylinderVector.push_back(new Cylinder(size, -2.5, 0.5, -2));

			}
			// (O) Creates a new Wall
			if (key == 79 && action == 1){
				table->obstacle=true;
			}

			//Bewegen der ausgewählten Kugel Shift + Pfeiltasten
			if (ballVector.size()>1) {
				// (<-)
				if ((key == 263 && action == 2 )&& mods==GLFW_MOD_SHIFT){
					ballVector.at(bs)->posX -= 0.1;
				}
				// (->)
				if (key == 262 && action == 2 && mods==GLFW_MOD_SHIFT){
					ballVector.at(bs)->posX += 0.1;
				}
				// (^)
				if (key == 265 && action == 2 && mods==GLFW_MOD_SHIFT){
					ballVector.at(bs)->posZ -= 0.1;
				}
				// (v)
				if (key == 264 && action == 2 && mods==GLFW_MOD_SHIFT){
					ballVector.at(bs)->posZ += 0.1;
				}
			}
			//Bewegen des ausgewählten Zylinders strg + Pfeiltasten
			if (cylinderVector.size()>0) {
				// (<-)
				if ((key == 263 && action == 2 )&& mods==GLFW_MOD_CONTROL){
					cylinderVector.at(cs)->posX -= 0.1;
				}
				// (->)
				if (key == 262 && action == 2 && mods==GLFW_MOD_CONTROL){
					cylinderVector.at(cs)->posX += 0.1;
				}
				// (^)
				if (key == 265 && action == 2 && mods==GLFW_MOD_CONTROL){
					cylinderVector.at(cs)->posZ -= 0.1;
				}
				// (v)
				if (key == 264 && action == 2 && mods==GLFW_MOD_CONTROL){
					cylinderVector.at(cs)->posZ += 0.1;
				}
			}
	}

	//Entertaste zum Starten des Spiels (Kugel rollt los)
	if (key == 257 && action == 1) {
		selectionMode = false;
		whiteBall->wallBack = false;
		whiteBall->wallFront = false;
		whiteBall->wallLeft = false;
		whiteBall->wallRight = false;
		whiteBall->wallObst = false;
		whiteBall->speedZ = 0.2;
		whiteBall->speedX = -0.3;
	}

	//Leertaste zum Zurücksetzen der Kugel auf die Startposition
	if (key == 32  && action == 1){
		selectionMode = true;
		whiteBall->posX = sphereX;
		whiteBall->posZ = sphereZ;
		whiteBall->speedZ = 0;
		whiteBall->speedX = 0;
	}
	if (key == 32  && action == 1 && mods==GLFW_MOD_SHIFT){
		selectionMode = true;
		whiteBall->posX = sphereX;
		whiteBall->posZ = sphereZ;
		whiteBall->speedZ = 0;
		whiteBall->speedX = 0;
		cylinderVector.clear();
		ballVector.clear();
		ballVector.push_back(whiteBall);
		table->obstacle=false;
	}

}

// Mouse Callback
void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
	//left click changes speed of ball
	if (button == 0 && action == GLFW_PRESS) {


	}

	if (button == 1 && action == GLFW_PRESS) {

	}

}

int main() {
	whiteBall->adventureBall= true;
	  ballVector.push_back(whiteBall);
	  GLFWwindow* window = NULL;

	  if(!glfwInit()){
		  return -1;
	  }

	  window = glfwCreateWindow(window_width_, window_height_,
								"CG_BallGame", NULL, NULL);
	  if(!window) {
		  glfwTerminate();
		  return -1;
	  }

	  glfwMakeContextCurrent(window);

	  while(!glfwWindowShouldClose(window)) {
			// switch on lighting (or you don't see anything)
			InitLighting();

			// set background color
			glClearColor(0.8, 0.8, 0.8, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw the scene
			Preview();

			// make it appear (before this, it's hidden in the rear buffer)
			glfwSwapBuffers(window);

			glfwPollEvents();

			// Callbacks for Keyboard, MouseButtons and MouseScrollWheel
			glfwSetKeyCallback(window, key_callback);
			glfwSetMouseButtonCallback(window, mouse_callback);

	  }

	  glfwTerminate();

	  printf("Goodbye!\n");

	  return 0;
}
