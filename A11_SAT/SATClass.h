#pragma once
#include "RE\ReEng.h"

class SATClass
{
private:
	std::vector<vector3> allCorners;

public:
	SATClass();
	//gets the normals of every corner
	std::vector<vector3> GetCornerNormals(SATClass* obj);
	//gets the normals of every connection between corners
	std::vector<vector3> GetEdgeNormals(SATClass* obj1, SATClass* obj2);
	//sets the minimum and maximum values given an axis
	void GetMinMax(SATClass* obj, vector3 axis, float& min, float& max);
	//test the collision: returns true for yes and false for no
	bool TestSAT(SATClass* obj1, SATClass* obj2);

	void SetCorners(std::vector<vector3> corners);
};