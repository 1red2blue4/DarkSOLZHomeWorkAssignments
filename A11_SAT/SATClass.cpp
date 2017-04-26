#include "SATClass.h"

SATClass::SATClass()
{
	allCorners = std::vector<vector3>();
}

std::vector<vector3> SATClass::GetCornerNormals(SATClass* obj)
{
	std::vector<vector3> normals;


	//right
	vector3 u(obj->allCorners[0] - obj->allCorners[1]);
	vector3 v(obj->allCorners[2] - obj->allCorners[0]);

	//find the cross-product between u and v, and normalize it;then add it to the vector
	normals.push_back(glm::normalize(vector3((u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), (u.x * v.y) - (u.y * v.x))));

	//down
	u = vector3(obj->allCorners[2] - obj->allCorners[3]);
	v = vector3(obj->allCorners[2] - obj->allCorners[6]);
	normals.push_back(glm::normalize(vector3((u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), (u.x * v.y) - (u.y * v.x))));

	//forward
	u = vector3(obj->allCorners[3] - obj->allCorners[7]);
	v = vector3(obj->allCorners[5] - obj->allCorners[7]);
	normals.push_back(glm::normalize(vector3((u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), (u.x * v.y) - (u.y * v.x))));

	return normals;
}

std::vector<vector3> SATClass::GetEdgeNormals(SATClass* obj1, SATClass* obj2)
{
	std::vector<vector3> normals;

	vector3 firstEdges[3];
	vector3 secondEdges[3];

	//find all the edges leading out of a corner
	firstEdges[0] = obj1->allCorners[1] - obj1->allCorners[0];
	firstEdges[1] = obj1->allCorners[2] - obj1->allCorners[0];
	firstEdges[2] = obj1->allCorners[4] - obj1->allCorners[0];

	//....for both objects
	secondEdges[0] = obj2->allCorners[4] - obj2->allCorners[0];
	secondEdges[1] = obj2->allCorners[1] - obj2->allCorners[0];
	secondEdges[2] = obj2->allCorners[2] - obj2->allCorners[0];

	//find a unique set of normals
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == j) {
				vector3 cross = glm::cross(firstEdges[i], secondEdges[j]);
				vector3 normCross = glm::normalize(cross);
				normals.push_back(normCross);
			}
		}
	}

	return normals;
}

void SATClass::GetMinMax(SATClass* obj, vector3 axis, float& min, float& max)
{
	//To get a projection along a vector, you take the dot product of the vertex and the axis vector.
	//Since we're looking for min and max, we start with the first and go from there.
	min = glm::dot(obj->allCorners[0], axis);
	max = min;

	for (int i = 1; i < 8; i++) {

		//Find the projection for the current point
		float currProjection = glm::dot(obj->allCorners[i], axis);

		//If the projection is smaller than our minimum projection, the minimum becomes this
		if (min > currProjection) {
			min = currProjection;
		}

		//If the projection is greater than our maximum projection, the maximum becomes this
		if (currProjection > max) {
			max = currProjection;
		}
	}
}

bool SATClass::TestSAT(SATClass* a, SATClass* b) {

	std::vector<vector3> aNormals = GetCornerNormals(a);
	std::vector<vector3> bNormals = GetCornerNormals(b);
	std::vector<vector3> edgeNormals = GetEdgeNormals(a, b);

	bool isSeparated = false;

	//test the 8 corners of the first object
	for (int i = 0; i < aNormals.size(); i++) {
		
		float aMin, aMax;
		GetMinMax(a, aNormals[i], aMin, aMax);

		float bMin, bMax;
		GetMinMax(b, aNormals[i], bMin, bMax);

		isSeparated = aMax < bMin || bMax < aMin;
		if (isSeparated) break;
	}

	//test the 8 corners of the second object
	if (!isSeparated) {
		
		for (int i = 0; i < bNormals.size(); i++) {
			float aMin, aMax;
			GetMinMax(a, bNormals[i], aMin, aMax);

			float bMin, bMax;
			GetMinMax(b, bNormals[i], bMin, bMax);

			isSeparated = aMax < bMin || bMax < aMin;
			if (isSeparated) break;
		}

	}
	//test the edges of the objects
	if (!isSeparated) {
		for (int i = 0; i < edgeNormals.size(); i++) {
			float aMin, aMax;
			GetMinMax(a, edgeNormals[i], aMin, aMax);

			float bMin, bMax;
			GetMinMax(b, edgeNormals[i], bMin, bMax);

			isSeparated = aMax < bMin || bMax < aMin;
			if (isSeparated) break;
		}
	}

	return !isSeparated;
}

void SATClass::SetCorners(std::vector<vector3> corners) { allCorners = corners; }
