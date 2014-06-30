/*
 * Cube.h
 *
 *  Created on: 03.06.2014
 *      Author: Kathrin
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "vec3.hpp"

//Würfelklasse
class Cube {
public:
	//Attribute
	double cubeSize;
	double speedX;
	double speedY;
	double speedZ;
	double rotX;
	double rotY;
	double rotZ;
	double posX;
	double posY;
	double posZ;
    Vec3 cubeDotLeft;
    Vec3 cubeDotLeftNormVec;
    Vec3 newCubeDotLeft;
    Vec3 newCubeDotLeftNormVec;
    Vec3 cubeDotRight;
    Vec3 cubeDotRightNormVec;
    Vec3 newCubeDotRight;
    Vec3 newCubeDotRightNormVec;
    Vec3 cubeDotFront;
    Vec3 cubeDotFrontNormVec;
    Vec3 newCubeDotFront;
    Vec3 newCubeDotFrontNormVec;
    Vec3 cubeDotBack;
    Vec3 cubeDotBackNormVec;
    Vec3 newCubeDotBack;
    Vec3 newCubeDotBackNormVec;

    //Funktionen
	Cube(double size, double posX, double posY, double posZ);
	virtual ~Cube();
	void DrawCube();
	void UpdateColliPoints();
};

#endif /* CUBE_H_ */
