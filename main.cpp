#include <GL/gl.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "vec3.hpp"
#include "BilliardBall.h"
#include "BilliardTable.h"

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

BilliardBall* whiteBall = new BilliardBall(sphereX, sphereY, sphereZ, sphereSize, 0.99);
BilliardBall* ball1 = new BilliardBall(5, sphereY, -3, sphereSize,  0.99);
BilliardBall* ball2 = new BilliardBall(3, sphereY, -1, sphereSize,  0.99);

BilliardBall* bList[3] = {whiteBall, ball1, ball2};

void resetBalls() {
	for (int i = 0; i<3; i++) {
		bList[i]->collision = false;
	}
}

void checkBallsandWalls() {

	for (int i = 0; i<3; i++) {

	  if((!bList[i]->wallBack) && bList[i]->wallCollisionDetection(table->wallDotBack, table->wallDotBackNormVec)){
		  bList[i]->wallBack = true;
		  bList[i]->wallFront = false;
		  bList[i]->wallLeft = false;
		  bList[i]->wallRight = false;
		  bList[i]->wallObst = false;
		  bList[i]->speedZ = bList[i]->speedZ * -1;
	  }

	  if ((!bList[i]->wallFront) && bList[i]->wallCollisionDetection(table->wallDotFront, table->wallDotFrontNormVec)) {
		  bList[i]->wallBack = false;
		  bList[i]->wallFront = true;
		  bList[i]->wallLeft = false;
		  bList[i]->wallRight = false;
		  bList[i]->wallObst = false;
		  bList[i]->speedZ = bList[i]->speedZ * -1;
	  }

	  if ((!bList[i]->wallLeft) &&bList[i]->wallCollisionDetection(table->wallDotLeft, table->wallDotLeftNormVec)) {
		  bList[i]->wallBack = false;
		  bList[i]->wallFront = false;
		  bList[i]->wallLeft = true;
		  bList[i]->wallRight = false;
		  bList[i]->wallObst = false;
		  bList[i]->speedX = bList[i]->speedX * -1;
	  }

	  if ((!bList[i]->wallRight) && bList[i]->wallCollisionDetection(table->wallDotRight, table->wallDotRightNormVec)) {
		  bList[i]->wallBack = false;
		  bList[i]->wallFront = false;
		  bList[i]->wallLeft = false;
		  bList[i]->wallRight = true;
		  bList[i]->wallObst = false;
		  bList[i]->speedX = bList[i]->speedX * -1;
	  }

	  if ((!bList[i]->wallObst) && bList[i]->wallCollisionDetection(table->wallDotObs, table->WallDotObsNormVec)) {

		  bList[i]->wallBack = false;
		  bList[i]->wallFront = false;
		  bList[i]->wallLeft = false;
		  bList[i]->wallRight = false;
		  bList[i]->wallObst = true;

		  double amalV = ( (table->WallDotObsNormVec.p[0] * bList[i]->speedX) + (table->WallDotObsNormVec.p[1] * bList[i]->speedY) + (table->WallDotObsNormVec.p[2] * bList[i]->speedZ) );
		  double betragA = (sqrt((table->WallDotObsNormVec.p[0]*table->WallDotObsNormVec.p[0]) + (table->WallDotObsNormVec.p[1]*table->WallDotObsNormVec.p[1]) + (table->WallDotObsNormVec.p[2]*table->WallDotObsNormVec.p[2]))) * (sqrt((table->WallDotObsNormVec.p[0]*table->WallDotObsNormVec.p[0]) + (table->WallDotObsNormVec.p[1]*table->WallDotObsNormVec.p[1]) + (table->WallDotObsNormVec.p[2]*table->WallDotObsNormVec.p[2])));
		  double aNeuX = (2 * amalV / betragA) * table->WallDotObsNormVec.p[0];
		  double aNeuZ = (2 * amalV / betragA) * table->WallDotObsNormVec.p[2];

		  bList[i]->speedX = bList[i]->speedX - aNeuX;
		  bList[i]->speedZ = bList[i]->speedZ - aNeuZ;
	  }
	}

}

void checkBalls() {
	for (int i = 0; i<3;i++) {
		for (int j = 0; j<3; j++) {
			/*if (i!= j) {
				std::cout << i << "     " << j << std::endl;
			}*/
			if (i != j && bList[i]->detectCollision(*bList[j]) && !(bList[i]->collision && bList[j]->collision)) {
				bList[i]->wallBack = false;
				bList[i]->wallFront = false;
				bList[i]->wallLeft = false;
				bList[i]->wallRight = false;
				bList[i]->wallObst = false;
				bList[j]->wallBack = false;
				bList[j]->wallFront = false;
				bList[j]->wallLeft = false;
				bList[j]->wallRight = false;
				bList[j]->wallObst = false;

				if (bList[i]->checkCollisionType(*bList[i], *bList[j])){
					//gerade Kugelkollision

					std::cout << "Gerade Kollision" << std::endl;

					bList[j]->collision = true;
					bList[i]->collision = true;

					Vec3 temp;
					temp.p[0] = bList[i]->speedX;
					temp.p[1] = bList[i]->speedY;
					temp.p[2] = bList[i]->speedZ;

					bList[i]->speedX = bList[j]->speedX;
					bList[i]->speedY = bList[j]->speedY;
					bList[i]->speedZ = bList[j]->speedZ;

					bList[j]->speedX = temp.p[0];
					bList[j]->speedY= temp.p[1];
					bList[j]->speedZ = temp.p[2];

				} else {

					bList[j]->collision = true;
					bList[i]->collision = true;

//					double betragMittelpunkte = sqrt( ( (bList[j]->posX - bList[i]->posX) * (bList[j]->posX - bList[i]->posX) ) + ( (bList[j]->posY - bList[i]->posY) * (bList[j]->posY - bList[i]->posY) ) + ( (bList[j]->posZ - bList[i]->posZ) * (bList[j]->posZ - bList[i]->posZ) ) );

					Vec3 n;
					n.p[0] = ( (bList[j]->posX - bList[i]->posX));
					n.p[1] = ( (bList[j]->posY - bList[i]->posY));
					n.p[2] = ( (bList[j]->posZ - bList[i]->posZ));

					Vec3 v1maln;
					v1maln.p[0] = ( bList[i]->posX * n.p[0] );
					v1maln.p[1] = ( bList[i]->posY * n.p[1] );
					v1maln.p[2] = ( bList[i]->posZ * n.p[2] );

					Vec3 v2maln;
					v2maln.p[0] = ( bList[j]->posX * n.p[0] );
					v2maln.p[1] = ( bList[j]->posY * n.p[1] );
					v2maln.p[2] = ( bList[j]->posZ * n.p[2] );

					Vec3 vplusn;
					vplusn.p[0] = ( ( v1maln.p[0] + v2maln.p[0] ) / 2.0 );
					vplusn.p[1] = ( ( v1maln.p[1] + v2maln.p[1] ) / 2.0 );
					vplusn.p[2] = ( ( v1maln.p[2] + v2maln.p[2] ) / 2.0 );
//					Vec3 vplusn = (v1maln.operator +=(v2maln)) / 2.0;

					Vec3 v1neu;
					v1neu.p[0] = ( bList[j]->speedX + ( 2.0 * ( vplusn.p[0] - v1maln.p[0] ) * n.p[0]) );
					v1neu.p[1] = ( bList[j]->speedY + ( 2.0 * ( vplusn.p[1] - v1maln.p[1] ) * n.p[1]) );
					v1neu.p[2] = ( bList[j]->speedZ + ( 2.0 * ( vplusn.p[2] - v1maln.p[2] ) * n.p[2]) );

					Vec3 v2neu;
					v2neu.p[0] = ( bList[i]->speedX + ( 2.0 * ( vplusn.p[0] - v2maln.p[0] ) * n.p[0]) );
					v2neu.p[1] = ( bList[i]->speedY + ( 2.0 * ( vplusn.p[1] - v2maln.p[1] ) * n.p[1]) );
					v2neu.p[2] = ( bList[i]->speedZ + ( 2.0 * ( vplusn.p[2] - v2maln.p[2] ) * n.p[2]) );

					bList[j]->speedX = v1neu.p[0];
					bList[j]->speedY = v1neu.p[1];
					bList[j]->speedZ = v1neu.p[2];

					bList[i]->speedX = v2neu.p[0];
					bList[i]->speedY = v2neu.p[1];
					bList[i]->speedZ = v2neu.p[2];

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

  checkBallsandWalls();
  checkBalls();
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
		if (whiteBall->speedZ == 0) {

			whiteBall->wallBack = false;
			whiteBall->wallFront = false;
			whiteBall->wallLeft = false;
			whiteBall->wallRight = false;
			whiteBall->wallObst = false;
			whiteBall->speedZ = -0.7;
			whiteBall->speedX = 0.1;

		}
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
		ball1->speedX = 0.8;
		ball1->speedZ = 0.6;

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
