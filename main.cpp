
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Ball.h"
#include "Table.h"
#include "Wall.h"
#include "Cube.h"
#include "Goal.h"
#include "Cylinder.h"
#include "vec3.hpp"

using namespace::std;

static double alpha_ = 35; //Winkel f�r Y Achse der Szenarie
static double beta_ = 25; //Winkel f�r X Achse der Szenarie

static float scale = 1.0; // Skalierungswert
static float size = 1.0; //Gr��e des W�rfels entspricht immer der H�lfte der W�rfel Gr��e

static double wallSize = 2.0; //Gr��e der Wand
static float moveX = 0.0; //Verschiebung des W�rfels auf der X-Achse
static float moveY = 0.0; //Verscheibung des W�rfels auf der Y-Achse

static double sphereSize = 0.286; //Durchmesser des Balls
static double sphereX = 5.3; //X Position des Balls
static double sphereY = sphereSize; //Y Position des Balls
static double sphereZ = 0; //Z Position des Balls

static bool selectionMode = true; //Bestimmt ob Gegenst�nde hinzugef�gt und bewegt werden k�nnen

static std::string selectedObject = "balls"; //Ausgew�hlte Objektgruppe

// Gr��e des OpenGL Fensters
static double window_width_ = 1024;
static double window_height_ = 768;

//Z�hler f�r die Objektauswahl innerhalb der Vektoren
static int bs = 1;
static int cs = 0;
static int cus = 0;
static int os = 0;

//Initialisierung von Tischobjekt und der wei�en Kugel
Table* table = new Table(size);
Ball* whiteBall = new Ball(sphereX, sphereY, sphereZ, sphereSize, 0.99, 1.0, 1.0, 1.0);

//Deklaration der Objekvektoren
std::vector<Ball*> ballVector;
std::vector<Cube*> cubeVector;
std::vector<Cylinder*> cylinderVector;
std::vector<Wall*> obstacleVector;

//Setzt Kollision der B�lle zur�ck
void resetBalls() {
	for (int i = 0; i<ballVector.size(); i++) {
		ballVector.at(i)->collision = false;
	}
}

//�berpr�ft Kollision zwischen Kugeln und W�nden
void checkBallsandWalls() {
	for (int i = 0; i<ballVector.size(); i++) {
		//�berpr�fung der hinteren Wand
		if((!ballVector.at(i)->wallBack) && ballVector.at(i)->wallCollisionDetection(table->wallDotBack, table->wallDotBackNormVec)){
			ballVector.at(i)->wallBack = true;
			ballVector.at(i)->wallFront = false;
			ballVector.at(i)->wallLeft = false;
			ballVector.at(i)->wallRight = false;
			ballVector.at(i)->wallObst = false;
			ballVector.at(i)->speedZ = ballVector.at(i)->speedZ * -1;
		}
		//�berpr�fung der vorderen Wand
		if ((!ballVector.at(i)->wallFront) && ballVector.at(i)->wallCollisionDetection(table->wallDotFront, table->wallDotFrontNormVec)) {
			ballVector.at(i)->wallBack = false;
			ballVector.at(i)->wallFront = true;
			ballVector.at(i)->wallLeft = false;
			ballVector.at(i)->wallRight = false;
			ballVector.at(i)->wallObst = false;
			ballVector.at(i)->speedZ = ballVector.at(i)->speedZ * -1;
		}
		//�berpr�fung der linken Wand
		if ((!ballVector.at(i)->wallLeft) && ballVector.at(i)->wallCollisionDetection(table->wallDotLeft, table->wallDotLeftNormVec)) {
			ballVector.at(i)->wallBack = false;
			ballVector.at(i)->wallFront = false;
			ballVector.at(i)->wallLeft = true;
			ballVector.at(i)->wallRight = false;
			ballVector.at(i)->wallObst = false;
			ballVector.at(i)->speedX = ballVector.at(i)->speedX * -1;
		}
		//�berpr�fung der rechten Wand
		if ((!ballVector.at(i)->wallRight) && ballVector.at(i)->wallCollisionDetection(table->wallDotRight, table->wallDotRightNormVec)) {
			ballVector.at(i)->wallBack = false;
			ballVector.at(i)->wallFront = false;
			ballVector.at(i)->wallLeft = false;
			ballVector.at(i)->wallRight = true;
			ballVector.at(i)->wallObst = false;
			ballVector.at(i)->speedX = ballVector.at(i)->speedX * -1;
		}
	}
}

//Pr�ft Kollision zwischen Kugeln und platzierbaren Hindernisw�nden
void checkBallsandObstacle() {
	for (int i = 0; i<ballVector.size(); i++) {
		for(int j = 0; j<obstacleVector.size();j++) {
			if ((!ballVector.at(i)->wallObst) && ballVector.at(i)->wallCollisionDetection(obstacleVector.at(j)->newWallDotObs, obstacleVector.at(j)->WallDotObsNormVec) && ballVector.at(i)->detectCollision(*obstacleVector.at(j))) {
				ballVector.at(i)->wallBack = false;
				ballVector.at(i)->wallFront = false;
				ballVector.at(i)->wallLeft = false;
				ballVector.at(i)->wallRight = false;
				ballVector.at(i)->wallObst = true;

				double amalV = ( (obstacleVector.at(j)->WallDotObsNormVec.p[0] * ballVector.at(i)->speedX) + (obstacleVector.at(j)->WallDotObsNormVec.p[1] * ballVector.at(i)->speedY) + (obstacleVector.at(j)->WallDotObsNormVec.p[2] * ballVector.at(i)->speedZ) );
				double betragA = (sqrt((obstacleVector.at(j)->WallDotObsNormVec.p[0]*obstacleVector.at(j)->WallDotObsNormVec.p[0]) + (obstacleVector.at(j)->WallDotObsNormVec.p[1]*obstacleVector.at(j)->WallDotObsNormVec.p[1]) + (obstacleVector.at(j)->WallDotObsNormVec.p[2]*obstacleVector.at(j)->WallDotObsNormVec.p[2]))) * (sqrt((obstacleVector.at(j)->WallDotObsNormVec.p[0]*obstacleVector.at(j)->WallDotObsNormVec.p[0]) + (obstacleVector.at(j)->WallDotObsNormVec.p[1]*obstacleVector.at(j)->WallDotObsNormVec.p[1]) + (obstacleVector.at(j)->WallDotObsNormVec.p[2]*obstacleVector.at(j)->WallDotObsNormVec.p[2])));
				double aNeuX = (2 * amalV / betragA) * obstacleVector.at(j)->WallDotObsNormVec.p[0];
				double aNeuZ = (2 * amalV / betragA) * obstacleVector.at(j)->WallDotObsNormVec.p[2];
				//Anpassung der Geschwindigkeit der Kugel
				ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
				ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;
				//Zur�cksetzen der Kollisionen
				ballVector.at(i)->wallBack = false;
				ballVector.at(i)->wallFront = false;
				ballVector.at(i)->wallLeft = false;
				ballVector.at(i)->wallRight = false;
				ballVector.at(i)->cubeBack = false;
				ballVector.at(i)->cubeFront = false;
				ballVector.at(i)->cubeLeft = false;
				ballVector.at(i)->cubeRight = false;
				ballVector.at(i)->collision = false;
				ballVector.at(i)->cylinderCol = false;
			}
		}
	}
}

//�berpr�fung der Kollisionen zwishen Kugeln und platzierbaren W�rfeln
void checkBallsandCube() {
	for (int i = 0; i<ballVector.size(); i++) {
		for(int j =0; j<cubeVector.size();j++) {
			if( ( (!ballVector.at(i)->cubeBack) && ballVector.at(i)->detectCollision(*cubeVector.at(j)) ) && ballVector.at(i)->cubeCollisionDetection(cubeVector.at(j)->newCubeDotBack, cubeVector.at(j)->newCubeDotBackNormVec) ) {
				ballVector.at(i)->cubeBack = true;
				ballVector.at(i)->cubeFront = false;
				ballVector.at(i)->cubeLeft = false;
				ballVector.at(i)->cubeRight = false;

				double amalV = ( (cubeVector.at(j)->newCubeDotBackNormVec.p[0] * ballVector.at(i)->speedX) + (cubeVector.at(j)->newCubeDotBackNormVec.p[1] * ballVector.at(i)->speedY) + (cubeVector.at(j)->newCubeDotBackNormVec.p[2] * ballVector.at(i)->speedZ) );
				double betragA = (sqrt((cubeVector.at(j)->newCubeDotBackNormVec.p[0]*cubeVector.at(j)->newCubeDotBackNormVec.p[0]) + (cubeVector.at(j)->newCubeDotBackNormVec.p[1]*cubeVector.at(j)->newCubeDotBackNormVec.p[1]) + (cubeVector.at(j)->newCubeDotBackNormVec.p[2]*cubeVector.at(j)->newCubeDotBackNormVec.p[2]))) * (sqrt((cubeVector.at(j)->newCubeDotBackNormVec.p[0]*cubeVector.at(j)->newCubeDotBackNormVec.p[0]) + (cubeVector.at(j)->newCubeDotBackNormVec.p[1]*cubeVector.at(j)->newCubeDotBackNormVec.p[1]) + (cubeVector.at(j)->newCubeDotBackNormVec.p[2]*cubeVector.at(j)->newCubeDotBackNormVec.p[2])));
				double aNeuX = (2 * amalV / betragA) * cubeVector.at(j)->newCubeDotBackNormVec.p[0];
				double aNeuZ = (2 * amalV / betragA) * cubeVector.at(j)->newCubeDotBackNormVec.p[2];

				ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
				ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;

				ballVector.at(i)->wallBack = false;
				ballVector.at(i)->wallFront = false;
				ballVector.at(i)->wallLeft = false;
				ballVector.at(i)->wallRight = false;
				ballVector.at(i)->wallObst = false;
				ballVector.at(i)->cubeBack = false;
				ballVector.at(i)->cubeFront = false;
				ballVector.at(i)->cubeLeft = false;
				ballVector.at(i)->cubeRight = false;
				ballVector.at(i)->collision = false;
				ballVector.at(i)->cylinderCol = false;
			}

			if( ( (!ballVector.at(i)->cubeFront)  && ballVector.at(i)->detectCollision(*cubeVector.at(j)) ) && ballVector.at(i)->cubeCollisionDetection(cubeVector.at(j)->newCubeDotFront, cubeVector.at(j)->newCubeDotFrontNormVec)){

				ballVector.at(i)->cubeBack = false;
				ballVector.at(i)->cubeFront = true;
				ballVector.at(i)->cubeLeft = false;
				ballVector.at(i)->cubeRight = false;

				double amalV = ( (cubeVector.at(j)->newCubeDotFrontNormVec.p[0] * ballVector.at(i)->speedX) + (cubeVector.at(j)->newCubeDotFrontNormVec.p[1] * ballVector.at(i)->speedY) + (cubeVector.at(j)->newCubeDotFrontNormVec.p[2] * ballVector.at(i)->speedZ) );
				double betragA = (sqrt((cubeVector.at(j)->newCubeDotFrontNormVec.p[0]*cubeVector.at(j)->newCubeDotFrontNormVec.p[0]) + (cubeVector.at(j)->newCubeDotFrontNormVec.p[1]*cubeVector.at(j)->newCubeDotFrontNormVec.p[1]) + (cubeVector.at(j)->newCubeDotFrontNormVec.p[2]*cubeVector.at(j)->newCubeDotFrontNormVec.p[2]))) * (sqrt((cubeVector.at(j)->newCubeDotFrontNormVec.p[0]*cubeVector.at(j)->newCubeDotFrontNormVec.p[0]) + (cubeVector.at(j)->newCubeDotFrontNormVec.p[1]*cubeVector.at(j)->newCubeDotFrontNormVec.p[1]) + (cubeVector.at(j)->newCubeDotFrontNormVec.p[2]*cubeVector.at(j)->newCubeDotFrontNormVec.p[2])));
				double aNeuX = (2 * amalV / betragA) * cubeVector.at(j)->newCubeDotFrontNormVec.p[0];
				double aNeuZ = (2 * amalV / betragA) * cubeVector.at(j)->newCubeDotFrontNormVec.p[2];

				ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
				ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;

				ballVector.at(i)->wallBack = false;
				ballVector.at(i)->wallFront = false;
				ballVector.at(i)->wallLeft = false;
				ballVector.at(i)->wallRight = false;
				ballVector.at(i)->wallObst = false;
				ballVector.at(i)->cubeBack = false;
				ballVector.at(i)->cubeFront = false;
				ballVector.at(i)->cubeLeft = false;
				ballVector.at(i)->cubeRight = false;
				ballVector.at(i)->collision = false;
				ballVector.at(i)->cylinderCol = false;
			}

			if( ( (!ballVector.at(i)->cubeLeft)  && ballVector.at(i)->detectCollision(*cubeVector.at(j)) ) && ballVector.at(i)->cubeCollisionDetection(cubeVector.at(j)->newCubeDotLeft, cubeVector.at(j)->newCubeDotLeftNormVec) ){
				ballVector.at(i)->cubeBack = false;
				ballVector.at(i)->cubeFront = false;
				ballVector.at(i)->cubeLeft = true;
				ballVector.at(i)->cubeRight = false;

				double amalV = ( (cubeVector.at(j)->newCubeDotLeftNormVec.p[0] * ballVector.at(i)->speedX) + (cubeVector.at(j)->newCubeDotLeftNormVec.p[1] * ballVector.at(i)->speedY) + (cubeVector.at(j)->newCubeDotLeftNormVec.p[2] * ballVector.at(i)->speedZ) );
				double betragA = (sqrt((cubeVector.at(j)->newCubeDotLeftNormVec.p[0]*cubeVector.at(j)->newCubeDotLeftNormVec.p[0]) + (cubeVector.at(j)->newCubeDotLeftNormVec.p[1]*cubeVector.at(j)->newCubeDotLeftNormVec.p[1]) + (cubeVector.at(j)->newCubeDotLeftNormVec.p[2]*cubeVector.at(j)->newCubeDotLeftNormVec.p[2]))) * (sqrt((cubeVector.at(j)->newCubeDotLeftNormVec.p[0]*cubeVector.at(j)->newCubeDotLeftNormVec.p[0]) + (cubeVector.at(j)->newCubeDotLeftNormVec.p[1]*cubeVector.at(j)->newCubeDotLeftNormVec.p[1]) + (cubeVector.at(j)->newCubeDotLeftNormVec.p[2]*cubeVector.at(j)->newCubeDotLeftNormVec.p[2])));
				double aNeuX = (2 * amalV / betragA) * cubeVector.at(j)->newCubeDotLeftNormVec.p[0];
				double aNeuZ = (2 * amalV / betragA) * cubeVector.at(j)->newCubeDotLeftNormVec.p[2];

				ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
				ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;

				ballVector.at(i)->wallBack = false;
				ballVector.at(i)->wallFront = false;
				ballVector.at(i)->wallLeft = false;
				ballVector.at(i)->wallRight = false;
				ballVector.at(i)->wallObst = false;
				ballVector.at(i)->cubeBack = false;
				ballVector.at(i)->cubeFront = false;
				ballVector.at(i)->cubeLeft = false;
				ballVector.at(i)->cubeRight = false;
				ballVector.at(i)->collision = false;
				ballVector.at(i)->cylinderCol = false;
			}

			if( ( (!ballVector.at(i)->cubeRight)  && ballVector.at(i)->detectCollision(*cubeVector.at(j)) ) && ballVector.at(i)->cubeCollisionDetection(cubeVector.at(j)->newCubeDotRight, cubeVector.at(j)->newCubeDotRightNormVec) ){
				ballVector.at(i)->cubeBack = false;
				ballVector.at(i)->cubeFront = false;
				ballVector.at(i)->cubeLeft = false;
				ballVector.at(i)->cubeRight = true;

				double amalV = ( (cubeVector.at(j)->newCubeDotRightNormVec.p[0] * ballVector.at(i)->speedX) + (cubeVector.at(j)->newCubeDotRightNormVec.p[1] * ballVector.at(i)->speedY) + (cubeVector.at(j)->newCubeDotRightNormVec.p[2] * ballVector.at(i)->speedZ) );
				double betragA = (sqrt((cubeVector.at(j)->newCubeDotRightNormVec.p[0]*cubeVector.at(j)->newCubeDotRightNormVec.p[0]) + (cubeVector.at(j)->newCubeDotRightNormVec.p[1]*cubeVector.at(j)->newCubeDotRightNormVec.p[1]) + (cubeVector.at(j)->newCubeDotRightNormVec.p[2]*cubeVector.at(j)->newCubeDotRightNormVec.p[2]))) * (sqrt((cubeVector.at(j)->newCubeDotRightNormVec.p[0]*cubeVector.at(j)->newCubeDotRightNormVec.p[0]) + (cubeVector.at(j)->newCubeDotRightNormVec.p[1]*cubeVector.at(j)->newCubeDotRightNormVec.p[1]) + (cubeVector.at(j)->newCubeDotRightNormVec.p[2]*cubeVector.at(j)->newCubeDotRightNormVec.p[2])));
				double aNeuX = (2 * amalV / betragA) * cubeVector.at(j)->newCubeDotRightNormVec.p[0];
				double aNeuZ = (2 * amalV / betragA) * cubeVector.at(j)->newCubeDotRightNormVec.p[2];

				ballVector.at(i)->speedX = ballVector.at(i)->speedX - aNeuX;
				ballVector.at(i)->speedZ = ballVector.at(i)->speedZ - aNeuZ;

				ballVector.at(i)->wallBack = false;
				ballVector.at(i)->wallFront = false;
				ballVector.at(i)->wallLeft = false;
				ballVector.at(i)->wallRight = false;
				ballVector.at(i)->wallObst = false;
				ballVector.at(i)->cubeBack = false;
				ballVector.at(i)->cubeFront = false;
				ballVector.at(i)->cubeLeft = false;
				ballVector.at(i)->cubeRight = false;
				ballVector.at(i)->collision = false;
				ballVector.at(i)->cylinderCol = false;
			}
		}
	}
}

//�berpr�fung der Kollision zwischen Kugeln und platzierbaren Zylindern
void checkBallsandCylinder() {

	for (int i = 0; i<ballVector.size(); i++) {
		for (int j = 0; j<cylinderVector.size(); j++) {
			if(ballVector.at(i)->detectCollision(*cylinderVector.at(j)) && !(ballVector.at(i)->collision)){
				if (ballVector.at(i)->checkCollisionType(*ballVector.at(i), *cylinderVector.at(j))){
					//gerade Kugelkollision

					ballVector.at(i)->collision = true;

					ballVector.at(i)->speedX = ballVector.at(i)->speedX * (-1);
					ballVector.at(i)->speedY = ballVector.at(i)->speedY * (-1);
					ballVector.at(i)->speedZ = ballVector.at(i)->speedZ * (-1);

					ballVector.at(i)->wallBack = false;
					ballVector.at(i)->wallFront = false;
					ballVector.at(i)->wallLeft = false;
					ballVector.at(i)->wallRight = false;
					ballVector.at(i)->wallObst = false;
					ballVector.at(i)->cubeBack = false;
					ballVector.at(i)->cubeFront = false;
					ballVector.at(i)->cubeLeft = false;
					ballVector.at(i)->cubeRight = false;
					ballVector.at(i)->collision = false;
					ballVector.at(i)->cylinderCol = false;

				} else {
					ballVector.at(i)->collision = true;

					double betragMittelpunkte = sqrt( ( (cylinderVector.at(j)->posX - ballVector.at(i)->posX) * (cylinderVector.at(j)->posX - ballVector.at(i)->posX) ) + ( (cylinderVector.at(j)->posY - ballVector.at(i)->posY) * (cylinderVector.at(j)->posY - ballVector.at(i)->posY) ) + ( (cylinderVector.at(j)->posZ - ballVector.at(i)->posZ) * (cylinderVector.at(j)->posZ - ballVector.at(i)->posZ) ) );

					Vec3 n;
					n.p[0] = ( (ballVector.at(i)->posX - cylinderVector.at(j)->posX) / betragMittelpunkte);
					n.p[1] = ( (ballVector.at(i)->posY - cylinderVector.at(j)->posY) / betragMittelpunkte);
					n.p[2] = ( (ballVector.at(i)->posZ - cylinderVector.at(j)->posZ) / betragMittelpunkte);

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

					ballVector.at(i)->wallBack = false;
					ballVector.at(i)->wallFront = false;
					ballVector.at(i)->wallLeft = false;
					ballVector.at(i)->wallRight = false;
					ballVector.at(i)->wallObst = false;
					ballVector.at(i)->cubeBack = false;
					ballVector.at(i)->cubeFront = false;
					ballVector.at(i)->cubeLeft = false;
					ballVector.at(i)->cubeRight = false;
					ballVector.at(i)->collision = false;
					ballVector.at(i)->cylinderCol = false;
				}
			}
		}
	}
}

//�berpr�ft Kollisionen zwischen Kugeln
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

					ballVector.at(i)->wallBack = false;
					ballVector.at(i)->wallFront = false;
					ballVector.at(i)->wallLeft = false;
					ballVector.at(i)->wallRight = false;
					ballVector.at(i)->cubeBack = false;
					ballVector.at(i)->cubeFront = false;
					ballVector.at(i)->cubeLeft = false;
					ballVector.at(i)->cubeRight = false;
					ballVector.at(i)->collision = false;
					ballVector.at(i)->cylinderCol = false;

				} else {

					ballVector.at(j)->collision = true;
					ballVector.at(i)->collision = true;

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

					ballVector.at(i)->wallBack = false;
					ballVector.at(i)->wallFront = false;
					ballVector.at(i)->wallLeft = false;
					ballVector.at(i)->wallRight = false;
					ballVector.at(i)->cubeBack = false;
					ballVector.at(i)->cubeFront = false;
					ballVector.at(i)->cubeLeft = false;
					ballVector.at(i)->cubeRight = false;
					ballVector.at(i)->collision = false;
					ballVector.at(i)->cylinderCol = false;
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

//Zeichnen der Kugeln
void drawVectorBalls(){
	for (int i = 0; i<ballVector.size();i++) {
		if (bs == i && selectionMode && selectedObject == "balls") {
			//Wenn Auswahlmodus und Objektgruppe der B�lle ausgew�hlt ist
			SetMaterialColor(1, 0.0, 1.0, 1.0);
		} else {
			SetMaterialColor(1, ballVector.at(i)->colourR, ballVector.at(i)->colourG, ballVector.at(i)->colourB);
		}
		ballVector.at(i)->DrawBall(Vec3( ballVector.at(i)->posX, ballVector.at(i)->posY,ballVector.at(i)->posZ), ballVector.at(i)->ballSize);
	}
}
//Zeichnen der Zylinder
void drawVectorCylinders() {
	for (int i = 0; i<cylinderVector.size();i++) {
		if (cs == i && selectionMode && selectedObject == "cylinders") {
			//Wenn Auswahlmodus und Objektgruppe der Zylinder ausgew�hlt ist
			SetMaterialColor(1, 0.0, 1.0, 1.0);
			SetMaterialColor(2, 0.0, 1.0, 1.0);
		} else {
	  	  SetMaterialColor(1, 0.0, 0.0, 0.0);
	  	  SetMaterialColor(2, 0.0, 0.0, 0.0);
		}
	  	  cylinderVector.at(i)->DrawCylinder(cylinderVector.at(i)->posX, cylinderVector.at(i)->posY, cylinderVector.at(i)->posZ, cylinderVector.at(i)->cylinderSize);
	}
}
//Zeichnen der W�rfel
void drawCubes() {
	for(int i =0;i<cubeVector.size();i++){
		if(cus==i && selectionMode && selectedObject == "cubes"){
			//Wenn Auswahlmodus und Objektgruppe der W�rfel ausgew�hlt ist
			SetMaterialColor(2, 0.0, 1.0, 1.0);
		}else{
			SetMaterialColor(2 ,0.0, 0.0, 0.0);
		}
		cubeVector.at(i)->DrawCube();
	}
}
//Zeichnen der Hindernisw�nde
void drawObstacles() {
	for(int i =0;i<obstacleVector.size();i++){
		if(os==i && selectionMode && selectedObject == "obstacles"){
			//Wenn Auswahlmodus und Objektgruppe der Hindernisw�nde ausgew�hlt ist
			SetMaterialColor(1, 0.0, 1.0, 1.0);
			SetMaterialColor(2, 0.0, 1.0, 1.0);
		}else{
			SetMaterialColor(1 ,0.0, 0.0, 0.0);
			SetMaterialColor(2 ,0.0, 0.0, 0.0);
		}
		obstacleVector.at(i)->DrawWall();
	}
}

#pragma mark Fremder Code
// Frame-Counter adapted from http://r3dux.org/2012/07/a-simple-glfw-fps-counter/
double calcFPS(GLFWwindow* window, double theTimeInterval = 1.0, std::string theWindowTitle = "NONE")
{
	// Static values which only get initialised the first time the function runs
	static double t0Value       = glfwGetTime(); // Set the initial time to now
	static int    fpsFrameCount = 0;             // Set the initial FPS frame count to 0
	static double fps           = 0.0;           // Set the initial FPS value to 0.0

	// Get the current time in seconds since the program started (non-static, so executed every time)
	double currentTime = glfwGetTime();

	// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
	// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
	if (theTimeInterval < 0.1)
	{
		theTimeInterval = 0.1;
	}
	if (theTimeInterval > 10.0)
	{
		theTimeInterval = 10.0;
	}

	// Calculate and display the FPS every specified time interval
	if ((currentTime - t0Value) > theTimeInterval)
	{
		// Calculate the FPS as the number of frames divided by the interval in seconds
		fps = (double)fpsFrameCount / (currentTime - t0Value);

		// If the user specified a window title to append the FPS value to...
		if (theWindowTitle != "NONE")
		{
			// Convert the fps value into a string using an output stringstream
            ostringstream stream;
			stream << fps;
			string fpsString = stream.str();

			// Append the FPS value to the window title details
			theWindowTitle += " | FPS: " + fpsString;

			// Convert the new window title to a c_str and set it
			const char* pszConstString = theWindowTitle.c_str();
			glfwSetWindowTitle(window, pszConstString);

		}
		else // If the user didn't specify a window to append the FPS to then output the FPS to the console
		{
			cout << "FPS: " << fps << std::endl;
		}

		// Reset the FPS frame counter and set the initial time to be now
		fpsFrameCount = 0;
		t0Value = glfwGetTime();
	}
	else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
	{
		fpsFrameCount++;
	}

	// Return the current FPS - doesn't have to be used if you don't want it!
	return fps;
}


//Erstellung der Beleuchtung
void InitLighting() {
  GLfloat lp1[4]  = { 10,  5,  10,  0};
  GLfloat lp2[4]  = { -5,  5, -10,  0};
  GLfloat red[4]  = {1, 0.8,  0.8,  1}; //Beleuchtungsfarbe einstellen
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

  glScalef(scale, scale, scale );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Zeichnen der Szene
void Preview() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();				  // Reset The Current Modelview Matrix

	glPushMatrix();

	glTranslated(moveX, moveY, 0);

	glRotated(alpha_, 0 , 1, 0);	// Rotation um die Y Achse
	glRotated(beta_, 1 ,0 ,0);	// Rotation um die X Achse

	glScalef(scale, scale, scale );


	//BilliardTable colours inside and outside
	SetMaterialColor(2, 0.0, 1.0, 0.0);
	SetMaterialColor(1, 0.0, 1.0, 0.2);
	table->DrawTable();

	//Impulsweiser
	if(selectionMode) {
		glBegin(GL_TRIANGLES);
			glVertex3f( 4.0, 0.0001, 1.5);
			glVertex3f( 6.0, 0.0001, 0.0);
			glVertex3f( 5.0, 0.0001, -0.5);
			glEnd();
	}

	Goal* goal = new Goal(1.25, -2.0, 0.0002, 0.0);
	goal->DrawGoal();
	//GoalReached - decreases the speed of the Ball to 0 when the area is reached
	if(goal->GoalReached(ballVector.at(0)->posX, ballVector.at(0)->posY, ballVector.at(0)->posZ)) {
		ballVector.at(0)->speedX=0;
		ballVector.at(0)->speedZ=0;
	}
	//Verarbeiten der Kollisionen
	checkBallsandWalls();
	checkBallsandObstacle();
	checkBalls();
	checkBallsandCylinder();
	checkBallsandCube();
	resetBalls();
	//Bewegen der B�lle
	for(int i =0; i<ballVector.size(); i++) {
		ballVector.at(i)->updatePosition();
	}
	//Zeichnen der Objekte
	if (ballVector.size()>=1) {
	  drawVectorBalls();
	}
	if (cylinderVector.size()>=1) {
		drawVectorCylinders();
	}
	if (obstacleVector.size()>=1) {
		drawObstacles();
	}
	if (cubeVector.size()>=1) {
		drawCubes();
	}

  glPopMatrix();

}

//Key Callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
//Pfeiltasten um den Tisch zu bewegen
	// (<-)
	if (key == 263 && action == 2 && !(mods==GLFW_MOD_SHIFT)){
		moveX -= 0.1;
	}
	// (->)
	if (key == 262 && action == 2 && !(mods==GLFW_MOD_SHIFT)){
		moveX += 0.1;
	}
	// (^)
	if (key == 265 && action == 2 && !(mods==GLFW_MOD_SHIFT)){
		moveY += 0.1;
	}
	// (v)
	if (key == 264 && action == 2 && !(mods==GLFW_MOD_SHIFT)){
		moveY -= 0.1;
	}
//WASD um den Tisch zu drehen
	// (W)
	if (key == 87 && action == 2 && !(mods==GLFW_MOD_SHIFT)){
		beta_ -= .9;
	}
	// (A)
	if (key == 65 && action == 2&& !(mods==GLFW_MOD_SHIFT)){
		alpha_ -= .9;
	}
	// (S)
	if (key == 83 && action == 2&& !(mods==GLFW_MOD_SHIFT)){
		beta_ += .9;
	}
	// (D)
	if (key == 68 && action == 2&& !(mods==GLFW_MOD_SHIFT)){
		alpha_ += .9;
	}
	//Auswahl der Objektgruppe �ndern
	if(key == 45 && action == 1 && !(mods==GLFW_MOD_SHIFT)) {
		if(selectedObject=="balls"){
			selectedObject="cylinders";
			printf("cylinders");
		}else if (selectedObject=="cylinders"){
			selectedObject="cubes";
			printf("cubes");
		}else if(selectedObject=="cubes"){
			selectedObject="obstacles";
			printf("obstacles");
		}else if(selectedObject=="obstacles"){
			selectedObject="balls";
			printf("balls");
		}
	}
	if(key == 61 && action == 1 && !(mods==GLFW_MOD_SHIFT)) {
		if(selectedObject=="balls"){
			selectedObject="obstacles";
			printf("obstacles");
		}else if (selectedObject=="cylinders"){
			selectedObject="balls";
			printf("balls");
		}else if(selectedObject=="cubes"){
			selectedObject="cylinders";
			printf("cylinders");
		}else if(selectedObject=="obstacles"){
			selectedObject="cubes";
			printf("cubes");
		}
	}
	//Im Auswahlmodus
	if(selectionMode) {
		//Entertaste zum Starten des Spiels (Kugel rollt los)
		if (key == 257 && action == 1) {
			selectionMode = false;
			whiteBall->wallBack = false;
			whiteBall->wallFront = false;
			whiteBall->wallLeft = false;
			whiteBall->wallRight = false;
			whiteBall->wallObst = false;
			whiteBall->speedZ = 1.2;
			whiteBall->speedX = -1.6;
		}
		//BCZO Um neue Objekte hinzuzuf�gen
		// (B) Erstellt einen neuen Ball
		if (key == 66 && action == 1){
			ballVector.push_back(new Ball(5, sphereY, -3,sphereSize, 0.978, 0.0, 0.0, 0.0));

		}
		// (C) Erstellt einen neuen W�rfwl
		if (key == 67 && action == 1){
			cubeVector.push_back(new Cube(size, 0.0, 0.0, 0.0));
		}
		// (Z) Erstellt einen neen Zylinder
		if (key == 90 && action == 1){
			cylinderVector.push_back(new Cylinder(size, -2.5, 0.5, -2));
		}
		// (O) Erstellt eine neue Hinderniswand
		if (key == 79 && action == 1){
			obstacleVector.push_back(new Wall(wallSize, 1.0, 0.0, -1.0));
		}
		//Wenn Ballgruppe ausgew�hlt ist
		if (selectedObject == "balls") {
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
			//Bewegen der ausgew�hlten Kugel Shift + Pfeiltasten
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
		//Wenn Zylindergrupppe ausgew�hlt ist
		}else if(selectedObject=="cylinders"){
			//zum Iterieren durch die Zylinderauswahl shift und +/-
			if(key == 45 && action == 1 && mods==GLFW_MOD_SHIFT) {
				if (cs-1 < 0) {
					cs = cylinderVector.size()-1;
				} else {
					cs--;
				}
			}
			if(key == 61 && action == 1 && mods==GLFW_MOD_SHIFT) {
				if (cs+1 >= cylinderVector.size()) {
					cs = 0;
				} else {
					cs++;
				}
			}
			//Bewegen des ausgew�hlten Zylinders shift + Pfeiltasten
			if (cylinderVector.size()>0) {
				// (<-)
				if ((key == 263 && action == 2 )&& mods==GLFW_MOD_SHIFT){
					cylinderVector.at(cs)->posX -= 0.1;
				}
				// (->)
				if (key == 262 && action == 2 && mods==GLFW_MOD_SHIFT){
					cylinderVector.at(cs)->posX += 0.1;
				}
				// (^)
				if (key == 265 && action == 2 && mods==GLFW_MOD_SHIFT){
					cylinderVector.at(cs)->posZ -= 0.1;
				}
				// (v)
				if (key == 264 && action == 2 && mods==GLFW_MOD_SHIFT){
					cylinderVector.at(cs)->posZ += 0.1;
				}
			}
		//Wenn Objektgruppe der W�rfel ausgew�hlt ist
		}else if(selectedObject=="cubes") {
			//Zum Iterieren durch die W�rfelauswahl
			if(key == 45 && action == 1 && mods==GLFW_MOD_SHIFT) {
				if (cus-1 < 0) {
					cus = cubeVector.size()-1;
				} else {
					cus--;
				}
			}
			if(key == 61 && action == 1 && mods==GLFW_MOD_SHIFT) {
				if (cus+1 >= cubeVector.size()) {
					cus = 0;
				} else {
					cus++;
				}
			}
			//Bewegen des ausgew�hlten W�rfels Shift + Pfeiltasten
			if (cubeVector.size()>0) {
				// (<-)
				if ((key == 263 && action == 2 )&& mods==GLFW_MOD_SHIFT){
					cubeVector.at(cus)->posX -= 0.1;
				}
				// (->)
				if (key == 262 && action == 2 && mods==GLFW_MOD_SHIFT){
					cubeVector.at(cus)->posX += 0.1;
				}
				// (^)
				if (key == 265 && action == 2 && mods==GLFW_MOD_SHIFT){
					cubeVector.at(cus)->posZ -= 0.1;
				}
				// (v)
				if (key == 264 && action == 2 && mods==GLFW_MOD_SHIFT){
					cubeVector.at(cus)->posZ += 0.1;
				}
			}
			//AD rotieren des ausgew�hlten W�rfels
			if (cubeVector.size()>0) {
			    // (A)
				if (key == 65 && action == 2 && mods==GLFW_MOD_SHIFT){
					cubeVector.at(cus)->rotY-=0.9;
				}
				// (D)
				if (key == 68 && action == 2 && mods==GLFW_MOD_SHIFT){
					cubeVector.at(cus)->rotY+=0.9;
				}
			}
		//Wenn Gruppe der Hindernisw�nde ausgew�hlt ist
		}else if(selectedObject=="obstacles") {
			//Zum Iterieren durch die Wandauswahl
			if(key == 45 && action == 1 && mods==GLFW_MOD_SHIFT) {
				if (os-1 < 0) {
					os = obstacleVector.size()-1;
				} else {
					os--;
				}
			}
			if(key == 61 && action == 1 && mods==GLFW_MOD_SHIFT) {
				if (os+1 >= obstacleVector.size()) {
					os = 0;
				} else {
					os++;
				}
			}
			//AD rotieren der ausgew�hlten Wand
			if (obstacleVector.size()>0) {
			    // (A)
				if (key == 65 && action == 2 && mods==GLFW_MOD_SHIFT){
					obstacleVector.at(os)->rotY-=0.9;
				}
				// (D)
				if (key == 68 && action == 2 && mods==GLFW_MOD_SHIFT){
					obstacleVector.at(os)->rotY+=0.9;
				}
			}
			//bewegen der ausgew�hlten Wand
			if (obstacleVector.size()>0) {
				// (<-)
				if ((key == 263 && action == 2 )&& mods==GLFW_MOD_SHIFT){
					obstacleVector.at(os)->posX -= 0.1;
				}
				// (->)
				if (key == 262 && action == 2 && mods==GLFW_MOD_SHIFT){
					obstacleVector.at(os)->posX += 0.1;
				}
				// (^)
				if (key == 265 && action == 2 && mods==GLFW_MOD_SHIFT){
					obstacleVector.at(os)->posZ -= 0.1;
				}
				// (v)
				if (key == 264 && action == 2 && mods==GLFW_MOD_SHIFT){
					obstacleVector.at(os)->posZ += 0.1;
				}
			}
		}
	}
	//Leertaste zum Zur�cksetzen der Kugel auf die Startposition
	if (key == 32  && action == 1){
		selectionMode = true;
		selectedObject = "balls";
		whiteBall->posX = sphereX;
		whiteBall->posZ = sphereZ;
		whiteBall->speedZ = 0;
		whiteBall->speedX = 0;
	}
	//Leertaste + Shift zum Zur�cksetzen des gesamten Spiels
	if (key == 32  && action == 1 && mods==GLFW_MOD_SHIFT){
		selectionMode = true;
		selectedObject = "balls";
		whiteBall->posX = sphereX;
		whiteBall->posZ = sphereZ;
		whiteBall->speedZ = 0;
		whiteBall->speedX = 0;
		bs = 1;
		cs = 0;
		cus = 0;
		os = 0;
		cylinderVector.clear();
		cubeVector.clear();
		obstacleVector.clear();
		ballVector.clear();
		ballVector.push_back(whiteBall);
	}

}

int main() {
	//Hinzuf�gend er wei�en Kugel zum Kugelvektor
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

		// Callbacks for Keyboard
		glfwSetKeyCallback(window, key_callback);

		calcFPS(window,1.0,"Billiard Game");

	}



	glfwTerminate();

	printf("Goodbye!\n");

	return 0;
}
