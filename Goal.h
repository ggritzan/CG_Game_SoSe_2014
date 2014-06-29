/*
 * Goal.h
 *
 *  Created on: 27.06.2014
 *      Author: Giacomo
 */

#ifndef GOAL_H_
#define GOAL_H_

#include "vec3.hpp"

//Zielbereich Klasse
class Goal {
public:
	//Attribute
	float goalSize;
	double posX;
	double posY;
	double posZ;

	//Funktionen
	Goal(double goalSize, double posX, double posY, double posZ);
	virtual ~Goal();
	void DrawGoal();
	bool GoalReached(double ballPosX, double ballPosY, double ballPosZ);
	void SetMaterialColor(int side, double r, double g, double b);
};

#endif /* GOAL_H_ */
