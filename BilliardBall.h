

#ifndef BILLIARDBALL_H_
#define BILLIARDBALL_H_

class BilliardBall {
public:

	float ballSize;
	double speedX;
	double speedY;
	double speedZ;
	double posX;
	double posY;
	double posZ;
	double friction;
	bool collision;
	bool wallBack;
	bool wallFront;
	bool wallLeft;
	bool wallRight;
	bool wallObst;

	BilliardBall(double posX, double posY, double posZ, float size, double friction);
	virtual ~BilliardBall();

	//bool detectCollision(BilliardBall b);

	void updatePosition();

	bool wallCollisionDetection(Vec3& wallDot, Vec3& wallNormVec);
	bool detectCollision(BilliardBall b);

	bool checkCollisionType(BilliardBall a, BilliardBall b);

	void DrawBall(const Vec3& ctr, double r);

};

#endif /* BILLIARDBALL_H_ */
