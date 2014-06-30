#define GLFW_DLL

#include <GL/gl.h>
#include "Cube.h"
#include "Cylinder.h"
#include "Wall.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "vec3.hpp"
#include "Ball.h"

//Konstruktor
Ball::Ball(double posX, double posY, double posZ, float size, double friction, double colourR, double colourG, double colourB) {

	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;

	this->ballSize = size;

	this->speedX = 0.0;
	this->speedY = 0.0;
	this->speedZ = 0.0;
	this->friction = friction;

	this->colourR = colourR;
	this->colourG = colourG;
	this->colourB = colourB;

	this->collision = false;
	this->cylinderCol = false;

	this->wallBack = false;
	this->wallFront = false;
	this->wallLeft = false;
	this->wallRight = false;
	this->wallObst = false;

	this->cubeBack = false;
	this->cubeFront = false;
	this->cubeLeft = false;
	this->cubeRight = false;
}

//Destruktor
Ball::~Ball() {
	// TODO Auto-generated destructor stub
}

//Überprüfung der Kollision zwischen zwei Kugeln
bool Ball::detectCollision(Ball b) {
    //Geschwindigkeit
    double dvX = this->speedX - b.speedX;
    double dvY = this->speedY - b.speedY;
    double dvZ = this->speedZ - b.speedZ;
    //Distanz
    double dpX = this->posX - b.posX;
    double dpY = this->posY - b.posY;
    double dpZ = this->posZ - b.posZ;
    //minimal Distanz zum Quadrat
    double r = this->ballSize + b.ballSize;
    double pp = dpX * dpX + dpY * dpY + dpZ * dpZ - r*r;
    //berühren sich bereits -> Kollision
    if (pp < 0) {
        return true;
    }
    double pv = dpX * dvX + dpY * dvY + dpZ * dvZ;
    //bewegen sich voneinander weg -> Keine Kollision
    if (pv >= 0) {
        return false;
    }
    double vv = dvX * dvX + dvY * dvY + dvZ * dvZ;
    //Könnten sich im nächsten Frame berühren? -> Keine Kollision
    if ( (pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0 ) {
        return false;
    }
    //Diskriminante
    double D = pv * pv - pp * vv;
    if (D < 0) {
    	D = D * -1;
    }
    //Keine Kollision
    if(D > 0) {
        return false;
    //Kollision
    } else {
        return true;
    }
}

//Kollisionserkennung zwischen Kugel und Wand
bool Ball::detectCollision(Wall w) {

    //Distanzvektor
    double dvX = this->posX - w.posX;
    double dvY = this->posY - w.posY;
    double dvZ = this->posZ - w.posZ;

    double distance = sqrt( dvX*dvX + dvY*dvY + dvZ*dvZ );

    double minDistance = w.wallSize + this->ballSize;

    if(distance < minDistance) {
    	return true;
    } else {
    	return false;
    }
}

//Kollisionsüberprüfung zwischen Kugel und Zylinder
bool Ball::detectCollision(Cylinder c) {

    //Geschwindigkeit
    double dvX = this->speedX - c.speedX;
    double dvY = this->speedY - c.speedY;
    double dvZ = this->speedZ - c.speedZ;
    //Distanz
    double dpX = this->posX - c.posX;
    double dpY = this->posY - c.posY;
    double dpZ = this->posZ - c.posZ;
    //Minimal Distanz zum Quadrat
    double r = this->ballSize + c.cylinderSize;
    double pp = dpX * dpX + dpY * dpY + dpZ * dpZ - r*r;
    //berühren sich bereits -> Kollision
    if (pp < 0) {
        return true;
    }
    double pv = dpX * dvX + dpY * dvY + dpZ * dvZ;
    //bewegen sich voneinander weg -> Keine Kollision
    if (pv >= 0) {
        return false;
    }
    double vv = dvX * dvX + dvY * dvY + dvZ * dvZ;
    //Könnten sich im nächsten Frame berühren? -> Keine Kollision
    if ( (pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0 ) {
        return false;
    }
    //Diskriminante
    double D = pv * pv - pp * vv;
    if (D < 0) {
    	D = D * -1;
    }
    //Keine Kollision
    if(D > 0) {
        return false;
    //Kollision
    } else {
    	std::cout << "collision" <<std::endl;
        return true;
    }
}

//Kollision zwischen Kugel und Würfel
bool Ball::detectCollision(Cube c) {

	double a = 2 * c.cubeSize * (1 / sqrt(2));

	//Geschwindigkeit
    double dvX = this->speedX - c.speedX;
    double dvY = this->speedY - c.speedY;
    double dvZ = this->speedZ - c.speedZ;
    //Distanz
    double dpX = this->posX - c.posX;
    double dpY = this->posY - c.posY;
    double dpZ = this->posZ - c.posZ;
    //Minimaldistanz zum Quadrat
    double r = this->ballSize + a;
    double pp = dpX * dpX + dpY * dpY + dpZ * dpZ - r*r;
    //berühren sich bereits -> Kollision
    if (pp < 0) {
        return true;
    }
    double pv = dpX * dvX + dpY * dvY + dpZ * dvZ;
    //bewegen sich voneinander weg -> Keine Kollision
    if (pv >= 0) {
        return false;
    }
    double vv = dvX * dvX + dvY * dvY + dvZ * dvZ;
    //Berühren sich im nächsten Frame? -> Keine Kollision
    if ( (pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0 ) {
        return false;
    }
    //Diskriminante
    double D = pv * pv - pp * vv;
    if (D < 0) {
    	D = D * -1;
    }



    //Keine Kollision
    if(D > 0) {
        return false;
    //Kollision
    } else {
    	std::cout << "collision" <<std::endl;
        return true;
    }
}

//Liefert true wenn eine gerade Ball/Ball Kollision vorliegt
bool Ball::checkCollisionType(Ball a, Ball b) {
	Vec3  t;

	t.p[0] = ( (b.posX - a.posX) / a.speedX );
	t.p[1] = ( (b.posY - a.posY) / a.speedY );
	t.p[2] = ( (b.posZ - a.posZ) / a.speedZ );

	if( (t.p[0] == t.p[1]) && (t.p[0] == t.p[2]) ) {
		return true;
	} else {
		return false;
	}
}

//Liefert true wenn eine gerade Ball/Zylinder Kollision vorliegt
bool Ball::checkCollisionType(Ball a, Cylinder c) {
	Vec3  t;

	t.p[0] = ( (c.posX - a.posX) / a.speedX );
	t.p[1] = ( (c.posY - a.posY) / a.speedY );
	t.p[2] = ( (c.posZ - a.posZ) / a.speedZ );

	if( (t.p[0] == t.p[1]) && (t.p[0] == t.p[2]) ) {
		return true;
	} else {
		return false;
	}
}

// Updatet die Position der Kugel
void Ball::updatePosition(){
		if (speedZ == 0 && speedX == 0) {
			//Zurücksetzejn der Kollision
			this->collision = false;
			this->wallBack = false;
			this->wallFront = false;
			this->wallLeft = false;
			this->wallRight = false;
			this->wallObst = false;
		}
		//Bearbeiten von Position und Geschwindigkeit
		posX = posX + (speedX * 0.1);
		posZ = posZ + (speedZ * 0.1);
		speedX = (speedX * friction);
		speedZ = (speedZ * friction);
}

// Kollision mit einer Bande
bool Ball::wallCollisionDetection(Vec3& wallDot, Vec3& wallNormVec){

    double amalb = (wallDot.p[0]*wallNormVec.p[0]) + (wallDot.p[1]*wallNormVec.p[1]) + (wallDot.p[2]*wallNormVec.p[2]);
    double amaly = (posX*wallNormVec.p[0]) + (posY*wallNormVec.p[1]) + (posZ*wallNormVec.p[2]);
    double betraga = sqrt((wallNormVec.p[0]*wallNormVec.p[0]) + (wallNormVec.p[1]*wallNormVec.p[1]) + (wallNormVec.p[2]*wallNormVec.p[2]));
    double D = ((amalb - amaly) / betraga);

    if(D < 0) {
        D = D * (-1);
    }

    if(D <= ballSize){
        return true;
    } else {
        return false;
    }

}

// Kollision mit einem Würfel
bool Ball::cubeCollisionDetection(Vec3& wallDot, Vec3& wallNormVec){

    double amalb = (wallDot.p[0]*wallNormVec.p[0]) + (wallDot.p[1]*wallNormVec.p[1]) + (wallDot.p[2]*wallNormVec.p[2]);
    double amaly = (posX*wallNormVec.p[0]) + (posY*wallNormVec.p[1]) + (posZ*wallNormVec.p[2]);
    double betraga = sqrt((wallNormVec.p[0]*wallNormVec.p[0]) + (wallNormVec.p[1]*wallNormVec.p[1]) + (wallNormVec.p[2]*wallNormVec.p[2]));
    double D = ((amalb - amaly) / betraga);

    //std::cout << "D: " <<  D <<std::endl;

    if(D < 0) {
        D = D * (-1);
    }

    if(D <= ballSize){
        return true;
    } else {
        return false;
    }

}

//Zeichnen der Kugel
void Ball::DrawBall(const Vec3& ctr, double r){
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
}
