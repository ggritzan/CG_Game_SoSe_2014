#ifndef BILLIARDTABLE_H_
#define BILLIARDTABLE_H_
#include "vec3.hpp"


class BilliardTable {

public:
    double billiardTableSize;

    Vec3 wallDotLeft;
    Vec3 wallDotLeftNormVec;

    Vec3 wallDotRight;
    Vec3 wallDotRightNormVec;

    Vec3 wallDotFront;
    Vec3 wallDotFrontNormVec;

    Vec3 wallDotBack;
    Vec3 wallDotBackNormVec;

    Vec3 wallDotObs;
    Vec3 WallDotObsNormVec;

	bool obstacle;

    BilliardTable(double size);
    virtual ~BilliardTable();

    void DrawTable();

};

#endif /* BILLIARDTABLE_H_ */
