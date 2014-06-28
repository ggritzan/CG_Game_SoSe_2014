

#ifndef BILLIARDBALL_H_
#define BILLIARDBALL_H_
class Wall;

class Cylinder;


class Vec3;

class BilliardBall {
public:

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
	bool adventureBall;
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

	BilliardBall(double posX, double posY, double posZ, float size, double friction, double colourR, double colourG, double colourB);
	virtual ~BilliardBall();

	//bool detectCollision(BilliardBall b);

	void updatePosition();

	bool wallCollisionDetection(Vec3& wallDot, Vec3& wallNormVec);
	bool cubeCollisionDetection(Vec3& wallDot, Vec3& wallNormVec);
	bool detectCollision(BilliardBall b);

	bool detectCollision(Wall w);
	bool detectCollision(Cylinder c);
	bool detectCollision(double posX, double posY, double posZ, double size);

	bool checkCollisionType(BilliardBall a, BilliardBall b);
	bool checkCollisionType(BilliardBall a, Cylinder c);


	void DrawBall(const Vec3& ctr, double r);

};

#endif /* BILLIARDBALL_H_ */
