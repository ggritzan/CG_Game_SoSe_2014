/*
 * Goal.h
 *
 *  Created on: 27.06.2014
 *      Author: Giacomo
 */

#ifndef GOAL_H_
#define GOAL_H_


#include "vec3.hpp"



class Goal {
public:
	/* attributes */
	float goalSize;
	double posX;
	double posY;
	double posZ;

	/* constructor */
	Goal(double goalSize, double posX, double posY, double posZ);

	/* destructor */
	virtual ~Goal();

	/* method to draw the goal */
	void DrawGoal();

	/* method to check if the goal is reached */
	bool GoalReached(double ballPosX, double ballPosY, double ballPosZ);

	/* set material color */
	void SetMaterialColor(int side, double r, double g, double b);
};



#endif /* GOAL_H_ */
