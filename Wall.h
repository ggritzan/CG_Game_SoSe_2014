#ifndef WALL_H_
#define WALL_H_

#include "vec3.hpp"

class Wall {
public:

    double wallSize;
	double posX;
	double posY;
	double posZ;
	double rotX;
	double rotY;
	double rotZ;

    Vec3 wallDotObs;
    Vec3 WallDotObsNormVec;
    Vec3 newWallDotObs;

	bool obstacle;

	Wall(double size, double posX, double posY, double posZ);
	virtual ~Wall();

    void DrawWall();
    void UpdateColliPoints();
};

#endif /* WALL_H_ */
