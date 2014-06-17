/*
 * Cube.h
 *
 *  Created on: 03.06.2014
 *      Author: Kathrin
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "vec3.hpp"

class Cube {
public:
	double cubeSize;

    Vec3 cubeDotLeft;
    Vec3 cubeDotLeftNormVec;

    Vec3 cubeDotRight;
    Vec3 cubeDotRightNormVec;

    Vec3 cubeDotFront;
    Vec3 cubeDotFrontNormVec;

    Vec3 cubeDotBack;
    Vec3 cubeDotBackNormVec;

	Cube(double size);
	virtual ~Cube();

	void DrawCube();
};

#endif /* CUBE_H_ */
