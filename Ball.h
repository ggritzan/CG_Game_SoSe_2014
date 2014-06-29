

#ifndef BILLIARDBALL_H_
#define BILLIARDBALL_H_
class Cube;
class Wall;
class Cylinder;
class Vec3;

//Kugelklasse
class Ball {
public:
	//Attribute
	float ballSize;
	double speedX;
	double speedY;
	double speedZ;
	double colourR;
	double colourG;
	double colourB;
	double posX;
	double posY;
	double posZ;
	double friction;
	bool collision;
	bool cylinderCol;
	bool wallBack;
	bool wallFront;
	bool wallLeft;
	bool wallRight;
	bool wallObst;
	bool cubeBack;
	bool cubeFront;
	bool cubeLeft;
	bool cubeRight;

	//Funktionen
	Ball(double posX, double posY, double posZ, float size, double friction, double colourR, double colourG, double colourB);
	virtual ~Ball();
	void updatePosition();
	bool wallCollisionDetection(Vec3& wallDot, Vec3& wallNormVec);
	bool cubeCollisionDetection(Vec3& wallDot, Vec3& wallNormVec);
	bool detectCollision(Ball b);
	bool detectCollision(Wall w);
	bool detectCollision(Cylinder c);
	bool detectCollision(Cube c);
	bool detectCollision(double posX, double posY, double posZ, double size);
	bool checkCollisionType(Ball a, Ball b);
	bool checkCollisionType(Ball a, Cylinder c);
	void DrawBall(const Vec3& ctr, double r);
};

#endif /* BILLIARDBALL_H_ */
