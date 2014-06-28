#ifndef WALL_H_
#define WALL_H_

#include "vec3.hpp"

class Wall {
public:

    double wallSize;
	double posX;
	double posY;
	double posZ;

    Vec3 wallDotObs;
    Vec3 WallDotObsNormVec;

	bool obstacle;

	Wall(double size, double posX, double posY, double posZ);
	virtual ~Wall();

    void DrawWall();
};

#endif /* WALL_H_ */
