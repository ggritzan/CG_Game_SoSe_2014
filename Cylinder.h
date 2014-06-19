/*
 * Cylinder.h
 *
 *  Created on: 14.06.2014
 *      Author: Kathrin
 */

#ifndef CYLINDER_H_
#define CYLINDER_H_
class Vec3;

class Cylinder {
public:

	float cylinderSize;
	double speedX;
	double speedY;
	double speedZ;
	double posX;
	double posY;
	double posZ;

	Cylinder(float size, double posX, double posY, double posZ);
	virtual ~Cylinder();

	void DrawCylinder(double x, double y, double z, double r);
};

#endif /* CYLINDER_H_ */
