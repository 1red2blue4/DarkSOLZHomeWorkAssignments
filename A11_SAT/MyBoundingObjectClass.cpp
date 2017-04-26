#include "MyBoundingObjectClass.h"


void MyBoundingObjectClass::SetCorners(std::vector<vector3> corners)
{
	m_mySAT->SetCorners(corners);
}
void MyBoundingObjectClass::SetCollidingBox(bool input) 
{ 
	m_myBox->SetColliding(input);
}
void MyBoundingObjectClass::SetCollidingSphere(bool input)
{
	m_mySphere->SetColliding(input);
}
void MyBoundingObjectClass::SetCenterLocal(vector3 input) 
{ 
	m_myBox->SetCenterLocal(input);
	m_mySphere->SetCenterLocal(input);
}
void MyBoundingObjectClass::SetCenterGlobal(vector3 input) 
{ 
	m_myBox->SetCenterGlobal(input); 
	m_mySphere->SetCenterGlobal(input);
}
void MyBoundingObjectClass::SetModelMatrix(matrix4 m_matrix)
{
	m_myBox->SetModelMatrix(m_matrix);
	m_mySphere->SetModelMatrix(m_matrix);
}
bool MyBoundingObjectClass::GetCollidingBox(void) 
{ 
	return m_myBox->GetColliding(); 
}
bool MyBoundingObjectClass::GetCollidingSphere(void)
{
	return m_mySphere->GetColliding();
}
vector3 MyBoundingObjectClass::GetCenterLocal(void) 
{ 
	return m_myBox->GetCenterLocal(); 
}
vector3 MyBoundingObjectClass::GetCenterGlobal(void) 
{ 
	return m_myBox->GetCenterGlobal(); 
}
matrix4 MyBoundingObjectClass::GetModelMatrix(void) 
{ 
	return m_myBox->GetModelMatrix(); 
}
MyBoundingBoxClass* MyBoundingObjectClass::GetBoundingBox(void)
{
	return m_myBox;
}
MyBoundingSphereClass* MyBoundingObjectClass::GetBoundingSphere(void)
{
	return m_mySphere;
}
SATClass* MyBoundingObjectClass::GetSAT(void)
{
	return m_mySAT;
}

void MyBoundingObjectClass::SetSphereColor(vector3 color) 
{ 
	m_mySphere->SetColor(color);
}

void MyBoundingObjectClass::SetBoxColor(vector3 color)
{
	m_myBox->SetColor(color);
}

void MyBoundingObjectClass::ToggleBoxVisibility()
{
	m_myBox->SetVisibility(!m_myBox->GetVisibility());
}

void MyBoundingObjectClass::ToggleSphereVisibility()
{
	m_mySphere->SetVisibility(!m_mySphere->GetVisibility());
}

void MyBoundingObjectClass::SetVisibility(bool input)
{
	m_mySphere->SetVisibility(input);
	m_myBox->SetVisibility(input);
}


MyBoundingObjectClass::MyBoundingObjectClass(std::vector<vector3> vertexList)
{
	m_myBox = new MyBoundingBoxClass(vertexList);
	m_mySphere = new MyBoundingSphereClass(vertexList);
	m_mySAT = new SATClass();
}



void MyBoundingObjectClass::Render() 
{
	m_myBox->Render();
	m_mySphere->RenderSphere();
}

bool MyBoundingObjectClass::IsBoxColliding(MyBoundingBoxClass* other)
{

	return m_myBox->IsColliding(other);
}

bool MyBoundingObjectClass::IsSimpleBoxColliding(MyBoundingBoxClass* other)
{

	return m_myBox->IsCollidingSimple(other);
}

bool MyBoundingObjectClass::IsSphereColliding(MyBoundingSphereClass* other)
{
	return m_mySphere->IsColliding(other);
}

bool MyBoundingObjectClass::IsSATColliding(SATClass* other)
{
	return m_mySAT->TestSAT(m_mySAT, other);
}

MyBoundingObjectClass::~MyBoundingObjectClass()
{
	delete m_myBox;
	delete m_mySphere;
	delete m_mySAT;
}