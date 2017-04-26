#pragma once
#include "MyBoundingBoxClass.h"
#include "MyBoundingSphereClass.h"
#include "RE\ReEng.h"
#include "SATClass.h"

class MyBoundingObjectClass
{
private:
	MyBoundingBoxClass* m_myBox;
	MyBoundingSphereClass* m_mySphere;
	SATClass* m_mySAT;
public:
	MyBoundingObjectClass(std::vector<vector3> vertexList);
	~MyBoundingObjectClass();

	//set visibility
	void ToggleBoxVisibility();
	void ToggleSphereVisibility();
	void SetVisibility(bool input);

	/*
	Sets Colliding
	*/
	void SetCollidingBox(bool input);
	void SetCollidingSphere(bool input);
	/*
	Sets Center of the sphere in local space
	*/
	void SetCenterLocal(vector3 input);
	/*
	Sets Center of the sphere in global space
	*/
	void SetCenterGlobal(vector3 input);
	/*
	Set the corners of the SAT
	*/
	void SetCorners(std::vector<vector3> corners);

	/*
	Get the box or the sphere
	*/
	MyBoundingBoxClass* GetBoundingBox(void);
	MyBoundingSphereClass* GetBoundingSphere(void);
	SATClass* GetSAT(void);

	/*
	Gets Colliding
	*/
	bool GetCollidingBox(void);
	bool GetCollidingSphere(void);
	/*
	Gets center of the sphere in local space
	*/
	vector3 GetCenterLocal(void);
	/*
	Gets center of the sphere in global space
	*/
	vector3 GetCenterGlobal(void);
	/*
	Gets model to world matrix of the sphere
	*/
	matrix4 GetModelMatrix(void);
	/*
	Sets model to world matrix of the sphere
	*/
	void SetModelMatrix(matrix4 m_matrix);
	/*
	Sets the color of the Bounding Object
	*/
	void SetSphereColor(vector3 color);
	void SetBoxColor(vector3 color);
	/*
	Renders the sphere based on the radius and the center in global space
	*/
	void Render();
	/*
	Will check the collision with another object using boxes
	*/
	bool IsBoxColliding(MyBoundingBoxClass* other);
	/*
	Will check the collision with another object using boxes
	*/
	bool IsSimpleBoxColliding(MyBoundingBoxClass* other);
	/*
	Will check the collision with another object using spheres
	*/
	bool IsSphereColliding(MyBoundingSphereClass* other);
	/*
	Checks if two SAT objects are colliding
	*/
	bool IsSATColliding(SATClass* other);
};