#ifndef BILLIARDTABLE_H_
#define BILLIARDTABLE_H_
#include "vec3.hpp"

//Tischklass
class Table {

public:
	//Attribute
    double tableSize;
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

    //Funktionen
    Table(double size);
    virtual ~Table();
    void DrawTable();

};

#endif /* BILLIARDTABLE_H_ */
