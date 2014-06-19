#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "BilliardBall.h"
#include "BilliardTable.h"
#include "Cube.h"
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

// Größe des OpenGL Fensters
static double window_width_ = 1024;
static double window_height_ = 768;

BilliardTable* table = new BilliardTable(size);

Cube* cube = new Cube(size, -5.0, 0.5, 5.0);
Cylinder* cylinder = new Cylinder(size, 5.5, 0.5, -2);

BilliardBall* whiteBall = new BilliardBall(sphereX, sphereY, sphereZ, sphereSize, 0.978);
BilliardBall* ball1 = new BilliardBall(4, sphereY, -4, sphereSize,  0.978);
BilliardBall* ball2 = new BilliardBall(3, sphereY, -1, sphereSize,  0.978);

//BilliardBall* ball3 = new BilliardBall(6, sphereY, -1, sphereSize,  0.978);

//Deklaration der Objektarrays
//std::vector<BilliardBall*> ballVector;
//ballVector.push_back (whiteBall);
//ballVector.push_back (ball1);
//ballVector.push_back (ball2);

BilliardBall* ballArray[5] = {whiteBall, ball1, ball2};
Cube* cubeVector[5] = {};
Cylinder* cylinderVector[5] = {};

void resetBalls() {
	for (int i = 0; i<3; i++) {
		ballArray[i]->collision = false;
	}
}

void checkBallsandWalls() {

	for (int i = 0; i<3; i++) {

	  if((!ballArray[i]->wallBack) && ballArray[i]->wallCollisionDetection(table->wallDotBack, table->wallDotBackNormVec)){
		  ballArray[i]->wallBack = true;
		  ballArray[i]->wallFront = false;
		  ballArray[i]->wallLeft = false;
		  ballArray[i]->wallRight = false;
		  ballArray[i]->wallObst = false;
		  ballArray[i]->speedZ = ballArray[i]->speedZ * -1;
	  }

	  if ((!ballArray[i]->wallFront) && ballArray[i]->wallCollisionDetection(table->wallDotFront, table->wallDotFrontNormVec)) {
		  ballArray[i]->wallBack = false;
		  ballArray[i]->wallFront = true;
		  ballArray[i]->wallLeft = false;
		  ballArray[i]->wallRight = false;
		  ballArray[i]->wallObst = false;
		  ballArray[i]->speedZ = ballArray[i]->speedZ * -1;
	  }

	  if ((!ballArray[i]->wallLeft) &&ballArray[i]->wallCollisionDetection(table->wallDotLeft, table->wallDotLeftNormVec)) {
		  ballArray[i]->wallBack = false;
		  ballArray[i]->wallFront = false;
		  ballArray[i]->wallLeft = true;
		  ballArray[i]->wallRight = false;
		  ballArray[i]->wallObst = false;
		  ballArray[i]->speedX = ballArray[i]->speedX * -1;
	  }

	  if ((!ballArray[i]->wallRight) && ballArray[i]->wallCollisionDetection(table->wallDotRight, table->wallDotRightNormVec)) {
		  ballArray[i]->wallBack = false;
		  ballArray[i]->wallFront = false;
		  ballArray[i]->wallLeft = false;
		  ballArray[i]->wallRight = true;
		  ballArray[i]->wallObst = false;
		  ballArray[i]->speedX = ballArray[i]->speedX * -1;
	  }

	  if ((!ballArray[i]->wallObst) && ballArray[i]->wallCollisionDetection(table->wallDotObs, table->WallDotObsNormVec)) {

		  ballArray[i]->wallBack = false;
		  ballArray[i]->wallFront = false;
		  ballArray[i]->wallLeft = false;
		  ballArray[i]->wallRight = false;
		  ballArray[i]->wallObst = true;

		  double amalV = ( (table->WallDotObsNormVec.p[0] * ballArray[i]->speedX) + (table->WallDotObsNormVec.p[1] * ballArray[i]->speedY) + (table->WallDotObsNormVec.p[2] * ballArray[i]->speedZ) );
		  double betragA = (sqrt((table->WallDotObsNormVec.p[0]*table->WallDotObsNormVec.p[0]) + (table->WallDotObsNormVec.p[1]*table->WallDotObsNormVec.p[1]) + (table->WallDotObsNormVec.p[2]*table->WallDotObsNormVec.p[2]))) * (sqrt((table->WallDotObsNormVec.p[0]*table->WallDotObsNormVec.p[0]) + (table->WallDotObsNormVec.p[1]*table->WallDotObsNormVec.p[1]) + (table->WallDotObsNormVec.p[2]*table->WallDotObsNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * table->WallDotObsNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * table->WallDotObsNormVec.p[2];

		  ballArray[i]->speedX = ballArray[i]->speedX - aNeuX;
		  ballArray[i]->speedZ = ballArray[i]->speedZ - aNeuZ;
	  }
	}

}

void checkBallsandCube() {

	for (int i = 0; i<3; i++) {

	  if((!ballArray[i]->cubeBack) && ballArray[i]->cubeCollisionDetection(cube->cubeDotBack, cube->cubeDotBackNormVec)){

		  ballArray[i]->cubeBack = true;
		  ballArray[i]->cubeFront = false;
		  ballArray[i]->cubeLeft = false;
		  ballArray[i]->cubeRight = false;

		  double amalV = ( (cube->cubeDotBackNormVec.p[0] * ballArray[i]->speedX) + (cube->cubeDotBackNormVec.p[1] * ballArray[i]->speedY) + (cube->cubeDotBackNormVec.p[2] * ballArray[i]->speedZ) );
		  double betragA = (sqrt((cube->cubeDotBackNormVec.p[0]*cube->cubeDotBackNormVec.p[0]) + (cube->cubeDotBackNormVec.p[1]*cube->cubeDotBackNormVec.p[1]) + (cube->cubeDotBackNormVec.p[2]*cube->cubeDotBackNormVec.p[2]))) * (sqrt((cube->cubeDotBackNormVec.p[0]*cube->cubeDotBackNormVec.p[0]) + (cube->cubeDotBackNormVec.p[1]*cube->cubeDotBackNormVec.p[1]) + (cube->cubeDotBackNormVec.p[2]*cube->cubeDotBackNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * cube->cubeDotBackNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * cube->cubeDotBackNormVec.p[2];

		  ballArray[i]->speedX = ballArray[i]->speedX - aNeuX;
		  ballArray[i]->speedZ = ballArray[i]->speedZ - aNeuZ;
	  }

	  if((!ballArray[i]->cubeFront) && ballArray[i]->cubeCollisionDetection(cube->cubeDotFront, cube->cubeDotFrontNormVec)){

		  ballArray[i]->cubeBack = false;
		  ballArray[i]->cubeFront = true;
		  ballArray[i]->cubeLeft = false;
		  ballArray[i]->cubeRight = false;

		  double amalV = ( (cube->cubeDotFrontNormVec.p[0] * ballArray[i]->speedX) + (cube->cubeDotFrontNormVec.p[1] * ballArray[i]->speedY) + (cube->cubeDotFrontNormVec.p[2] * ballArray[i]->speedZ) );
		  double betragA = (sqrt((cube->cubeDotFrontNormVec.p[0]*cube->cubeDotFrontNormVec.p[0]) + (cube->cubeDotFrontNormVec.p[1]*cube->cubeDotFrontNormVec.p[1]) + (cube->cubeDotFrontNormVec.p[2]*cube->cubeDotFrontNormVec.p[2]))) * (sqrt((cube->cubeDotFrontNormVec.p[0]*cube->cubeDotFrontNormVec.p[0]) + (cube->cubeDotFrontNormVec.p[1]*cube->cubeDotFrontNormVec.p[1]) + (cube->cubeDotFrontNormVec.p[2]*cube->cubeDotFrontNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * cube->cubeDotFrontNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * cube->cubeDotFrontNormVec.p[2];

		  ballArray[i]->speedX = ballArray[i]->speedX - aNeuX;
		  ballArray[i]->speedZ = ballArray[i]->speedZ - aNeuZ;
	  }

	  if((!ballArray[i]->cubeLeft) && ballArray[i]->cubeCollisionDetection(cube->cubeDotLeft, cube->cubeDotLeftNormVec)){
		  ballArray[i]->cubeBack = false;
		  ballArray[i]->cubeFront = false;
		  ballArray[i]->cubeLeft = true;
		  ballArray[i]->cubeRight = false;

		  double amalV = ( (cube->cubeDotLeftNormVec.p[0] * ballArray[i]->speedX) + (cube->cubeDotLeftNormVec.p[1] * ballArray[i]->speedY) + (cube->cubeDotLeftNormVec.p[2] * ballArray[i]->speedZ) );
		  double betragA = (sqrt((cube->cubeDotLeftNormVec.p[0]*cube->cubeDotLeftNormVec.p[0]) + (cube->cubeDotLeftNormVec.p[1]*cube->cubeDotLeftNormVec.p[1]) + (cube->cubeDotLeftNormVec.p[2]*cube->cubeDotLeftNormVec.p[2]))) * (sqrt((cube->cubeDotLeftNormVec.p[0]*cube->cubeDotLeftNormVec.p[0]) + (cube->cubeDotLeftNormVec.p[1]*cube->cubeDotLeftNormVec.p[1]) + (cube->cubeDotLeftNormVec.p[2]*cube->cubeDotLeftNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * cube->cubeDotLeftNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * cube->cubeDotLeftNormVec.p[2];

		  ballArray[i]->speedX = ballArray[i]->speedX - aNeuX;
		  ballArray[i]->speedZ = ballArray[i]->speedZ - aNeuZ;
	  }

	  if((!ballArray[i]->cubeRight) && ballArray[i]->cubeCollisionDetection(cube->cubeDotRight, cube->cubeDotRightNormVec)){
		  ballArray[i]->cubeBack = false;
		  ballArray[i]->cubeFront = false;
		  ballArray[i]->cubeLeft = false;
		  ballArray[i]->cubeRight = true;

		  double amalV = ( (cube->cubeDotRightNormVec.p[0] * ballArray[i]->speedX) + (cube->cubeDotRightNormVec.p[1] * ballArray[i]->speedY) + (cube->cubeDotRightNormVec.p[2] * ballArray[i]->speedZ) );
		  double betragA = (sqrt((cube->cubeDotRightNormVec.p[0]*cube->cubeDotRightNormVec.p[0]) + (cube->cubeDotRightNormVec.p[1]*cube->cubeDotRightNormVec.p[1]) + (cube->cubeDotRightNormVec.p[2]*cube->cubeDotRightNormVec.p[2]))) * (sqrt((cube->cubeDotRightNormVec.p[0]*cube->cubeDotRightNormVec.p[0]) + (cube->cubeDotRightNormVec.p[1]*cube->cubeDotRightNormVec.p[1]) + (cube->cubeDotRightNormVec.p[2]*cube->cubeDotRightNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * cube->cubeDotRightNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * cube->cubeDotRightNormVec.p[2];

		  ballArray[i]->speedX = ballArray[i]->speedX - aNeuX;
		  ballArray[i]->speedZ = ballArray[i]->speedZ - aNeuZ;
	  }
	}
}

void checkBallsandCylinder() {

	for (int i = 0; i<3; i++) {
		if(ballArray[i]->detectCollision(*cylinder) && !(ballArray[i]->collision)){
			if (ballArray[i]->checkCollisionType(*ballArray[i], *cylinder)){
				//gerade Kugelkollision

				std::cout << "Gerade Kollision" << std::endl;

				ballArray[i]->collision = true;

				ballArray[i]->speedX = ballArray[i]->speedX * (-1);
				ballArray[i]->speedY = ballArray[i]->speedY * (-1);
				ballArray[i]->speedZ = ballArray[i]->speedZ * (-1);


			} else {
				ballArray[i]->collision = true;

				Vec3 n;
				n.p[0] = ( (ballArray[i]->posX - cylinder->posX));
				n.p[1] = ( (ballArray[i]->posY - cylinder->posY));
				n.p[2] = ( (ballArray[i]->posZ - cylinder->posZ));

				Vec3 v1maln;
				v1maln.p[0] = ( cylinder->posX * n.p[0] );
				v1maln.p[1] = ( cylinder->posY * n.p[1] );
				v1maln.p[2] = ( cylinder->posZ * n.p[2] );

				Vec3 v2maln;
				v2maln.p[0] = ( ballArray[i]->posX * n.p[0] );
				v2maln.p[1] = ( ballArray[i]->posY * n.p[1] );
				v2maln.p[2] = ( ballArray[i]->posZ * n.p[2] );

				Vec3 vplusn;
				vplusn.p[0] = ( ( v1maln.p[0] + v2maln.p[0] ) / 2.0 );
				vplusn.p[1] = ( ( v1maln.p[1] + v2maln.p[1] ) / 2.0 );
				vplusn.p[2] = ( ( v1maln.p[2] + v2maln.p[2] ) / 2.0 );

				Vec3 v1neu;
				v1neu.p[0] = ( ballArray[i]->speedX + ( 2.0 * ( vplusn.p[0] - v1maln.p[0] ) * n.p[0]) );
				v1neu.p[1] = ( ballArray[i]->speedY + ( 2.0 * ( vplusn.p[1] - v1maln.p[1] ) * n.p[1]) );
				v1neu.p[2] = ( ballArray[i]->speedZ + ( 2.0 * ( vplusn.p[2] - v1maln.p[2] ) * n.p[2]) );

				Vec3 v2neu;
				v2neu.p[0] = ( cylinder->speedX + ( 2.0 * ( vplusn.p[0] - v2maln.p[0] ) * n.p[0]) );
				v2neu.p[1] = ( cylinder->speedY + ( 2.0 * ( vplusn.p[1] - v2maln.p[1] ) * n.p[1]) );
				v2neu.p[2] = ( cylinder->speedZ + ( 2.0 * ( vplusn.p[2] - v2maln.p[2] ) * n.p[2]) );

				ballArray[i]->speedX = v1neu.p[0];
				ballArray[i]->speedY = v1neu.p[1];
				ballArray[i]->speedZ = v1neu.p[2];
			}
		}
	}
}

void checkBalls() {
	for (int i = 0; i<3;i++) {
		for (int j = 0; j<3; j++) {
			/*if (i!= j) {
				std::cout << i << "     " << j << std::endl;
			}*/
			if (i != j && ballArray[i]->detectCollision(*ballArray[j]) && !(ballArray[i]->collision && ballArray[j]->collision)) {
				ballArray[i]->wallBack = false;
				ballArray[i]->wallFront = false;
				ballArray[i]->wallLeft = false;
				ballArray[i]->wallRight = false;
				ballArray[i]->wallObst = false;
				ballArray[j]->wallBack = false;
				ballArray[j]->wallFront = false;
				ballArray[j]->wallLeft = false;
				ballArray[j]->wallRight = false;
				ballArray[j]->wallObst = false;

				if (ballArray[i]->checkCollisionType(*ballArray[i], *ballArray[j])){
					//gerade Kugelkollision

					std::cout << "Gerade Kollision" << std::endl;

					ballArray[j]->collision = true;
					ballArray[i]->collision = true;

					Vec3 temp;
					temp.p[0] = ballArray[i]->speedX;
					temp.p[1] = ballArray[i]->speedY;
					temp.p[2] = ballArray[i]->speedZ;

					ballArray[i]->speedX = ballArray[j]->speedX;
					ballArray[i]->speedY = ballArray[j]->speedY;
					ballArray[i]->speedZ = ballArray[j]->speedZ;

					ballArray[j]->speedX = temp.p[0];
					ballArray[j]->speedY= temp.p[1];
					ballArray[j]->speedZ = temp.p[2];

				} else {

					ballArray[j]->collision = true;
					ballArray[i]->collision = true;

//					double betragMittelpunkte = sqrt( ( (bList[j]->posX - bList[i]->posX) * (bList[j]->posX - bList[i]->posX) ) + ( (bList[j]->posY - bList[i]->posY) * (bList[j]->posY - bList[i]->posY) ) + ( (bList[j]->posZ - bList[i]->posZ) * (bList[j]->posZ - bList[i]->posZ) ) );

					Vec3 n;
					n.p[0] = ( (ballArray[j]->posX - ballArray[i]->posX));
					n.p[1] = ( (ballArray[j]->posY - ballArray[i]->posY));
					n.p[2] = ( (ballArray[j]->posZ - ballArray[i]->posZ));

					Vec3 v1maln;
					v1maln.p[0] = ( ballArray[i]->posX * n.p[0] );
					v1maln.p[1] = ( ballArray[i]->posY * n.p[1] );
					v1maln.p[2] = ( ballArray[i]->posZ * n.p[2] );

					Vec3 v2maln;
					v2maln.p[0] = ( ballArray[j]->posX * n.p[0] );
					v2maln.p[1] = ( ballArray[j]->posY * n.p[1] );
					v2maln.p[2] = ( ballArray[j]->posZ * n.p[2] );

					Vec3 vplusn;
					vplusn.p[0] = ( ( v1maln.p[0] + v2maln.p[0] ) / 2.0 );
					vplusn.p[1] = ( ( v1maln.p[1] + v2maln.p[1] ) / 2.0 );
					vplusn.p[2] = ( ( v1maln.p[2] + v2maln.p[2] ) / 2.0 );
//					Vec3 vplusn = (v1maln.operator +=(v2maln)) / 2.0;

					Vec3 v1neu;
					v1neu.p[0] = ( ballArray[j]->speedX + ( 2.0 * ( vplusn.p[0] - v1maln.p[0] ) * n.p[0]) );
					v1neu.p[1] = ( ballArray[j]->speedY + ( 2.0 * ( vplusn.p[1] - v1maln.p[1] ) * n.p[1]) );
					v1neu.p[2] = ( ballArray[j]->speedZ + ( 2.0 * ( vplusn.p[2] - v1maln.p[2] ) * n.p[2]) );

					Vec3 v2neu;
					v2neu.p[0] = ( ballArray[i]->speedX + ( 2.0 * ( vplusn.p[0] - v2maln.p[0] ) * n.p[0]) );
					v2neu.p[1] = ( ballArray[i]->speedY + ( 2.0 * ( vplusn.p[1] - v2maln.p[1] ) * n.p[1]) );
					v2neu.p[2] = ( ballArray[i]->speedZ + ( 2.0 * ( vplusn.p[2] - v2maln.p[2] ) * n.p[2]) );

					ballArray[j]->speedX = v1neu.p[0];
					ballArray[j]->speedY = v1neu.p[1];
					ballArray[j]->speedZ = v1neu.p[2];

					ballArray[i]->speedX = v2neu.p[0];
					ballArray[i]->speedY = v2neu.p[1];
					ballArray[i]->speedZ = v2neu.p[2];

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
  SetMaterialColor(1, 1.0, 0.0, 0.0);
  SetMaterialColor(2, 1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
  	  glNormal3f(0.0, 1.0, 0.0);
	  glVertex3d(-0.5, 0.0001, 0);
  	  glVertex3d(-1.5, 0.0001, 1);
  	  glVertex3d(-1.25, 0.0001, 1.25);
  	  glVertex3d(-0.25, 0.0001, 0.25);

  	  glNormal3f(0.0, 1.0, 0.0);
	  glVertex3d(-1.25, 0.0001, 0.0);
  	  glVertex3d(-1.5, 0.0001, 0.25);
  	  glVertex3d(-0.5, 0.0001, 1.25);
  	  glVertex3d(-0.25, 0.0001, 1.0);
  glEnd();

  checkBallsandWalls();

  checkBalls();

  checkBallsandCylinder();
//  checkBallsandCube();
  resetBalls();

  whiteBall->updatePosition();
  ball1->updatePosition();
  ball2->updatePosition();

  //BilliardBall colour
  SetMaterialColor(1, 1.0, 1.0, 1.0);
  whiteBall->DrawBall(Vec3( whiteBall->posX, whiteBall->posY, whiteBall->posZ), whiteBall->ballSize);

  SetMaterialColor(1, 0.0, 0.0, 0.0);
  ball1->DrawBall(Vec3( ball1->posX, ball1->posY, ball1->posZ), ball1->ballSize);

  SetMaterialColor(1, 1.0, 0.0, 0.0);
  ball2->DrawBall(Vec3( ball2->posX, ball2->posY, ball2->posZ), ball2->ballSize);

//  SetMaterialColor(2, 1.0, 1.0, 0.0);
//  SetMaterialColor(1, 0.0, 1.0, 0.0);
//  cube->DrawCube();
  SetMaterialColor(1, 0.0, 0.5, 0.9);
  cylinder->DrawCylinder(cylinder->posX, cylinder->posY, cylinder->posZ, cylinder->cylinderSize);

  glPopMatrix();

}

// Key Callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){

//WASD
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

//Pfeiltasten
	// (<-)
	if (key == 263 && action == 2){
		cubeVerschiebungX -= 0.1;
	}

	// (->)
	if (key == 262 && action == 2){
		cubeVerschiebungX += 0.1;
	}

	// (^)
	if (key == 265 && action == 2){
		cubeVerschiebungY += 0.1;
	}

	// (v)
	if (key == 264 && action == 2){
		cubeVerschiebungY -= 0.1;
	}

	//Entertaste zum Starten des Spiels (Kugel rollt los)
	if (key == 257 && action == 1) {
		whiteBall->wallBack = false;
		whiteBall->wallFront = false;
		whiteBall->wallLeft = false;
		whiteBall->wallRight = false;
		whiteBall->wallObst = false;
		whiteBall->speedZ = -0.7;
		whiteBall->speedX = 0.1;
	}

	//Leertaste zum Zurücksetzen der Kugel auf die Startposition
	if (key == 32  && action == 2){
		whiteBall->posX = sphereX;
		whiteBall->posZ = sphereZ;
		whiteBall->speedZ = 0;
		whiteBall->speedX = 0;
	}

}

// Mouse Callback
void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
	//left click changes speed of ball
	if (button == 0 && action == GLFW_PRESS) {
		ball1->wallBack = false;
		ball1->wallFront = false;
		ball1->wallLeft = false;
		ball1->wallRight = false;
		ball1->wallObst = false;
		ball1->speedX = -1.0;
		ball1->speedZ = 1.0;

	}

	if (button == 1 && action == GLFW_PRESS) {

		ball2->wallBack = false;
		ball2->wallFront = false;
		ball2->wallLeft = false;
		ball2->wallRight = false;
		ball2->wallObst = false;
		ball2->speedX = -0.2;
		ball2->speedZ = 0.7;
	}

}

int main() {
  GLFWwindow* window = NULL;

  printf("Here we go!\n");

  if(!glfwInit()){
    return -1;
  }

  window = glfwCreateWindow(window_width_, window_height_,
                            "CG_Wuerfel", NULL, NULL);
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
