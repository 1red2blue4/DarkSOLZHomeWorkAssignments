#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Bounding Spheres DEMO");
}

void AppClass::InitVariables(void)
{
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie");
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Cow.obj", "Cow");

	m_boMngr = BoundingObjectManager::GetInstance();

	m_boMngr->MakeNewBoundingObject(m_pMeshMngr->GetVertexList("Zombie"));
	m_boMngr->MakeNewBoundingObject(m_pMeshMngr->GetVertexList("Steve"));
	m_boMngr->MakeNewBoundingObject(m_pMeshMngr->GetVertexList("Cow"));

	m_boMngr->ToggleSphereVisibility(0);
	m_boMngr->ToggleSphereVisibility(1);
	m_boMngr->ToggleSphereVisibility(2);

	//GOALS:
	//get corners of object in global space
	//project dist(maximumObj1, centerObj1) onto axis, and project dist(centerObj2, minimumObj2) to axis; compare to dist(centerObj1, centerObj2)
	//if center to maximum + center to minimum > center to center, they are colliding
	//if center to minimum + center to maximum > center to center, they are colliding
	//must make those checks accross the following axes:
	//object 1 x
	//object 1 y
	//object 1 z
	//object 2 x
	//object 2 y
	//object 2 z
	//object 1 x across object 2 x
	//object 1 x across object 2 y
	//object 1 x across object 2 z
	//object 1 y across object 2 x
	//object 1 y across object 2 y
	//object 1 y across object 2 z
	//object 1 z across object 2 x
	//object 1 z across object 2 y
	//object 1 z across object 2 z
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();

	matrix4 m4Position = glm::translate(vector3(3.0f, 0.0f, 0.0f));
	m_pMeshMngr->SetModelMatrix(m4Position, "Steve");
	//m_pBS1->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));

	matrix4 m4Position2 = glm::translate(vector3(7.0f, 0.0f, 0.0f));
	m_pMeshMngr->SetModelMatrix(m4Position2, "Cow");
	//m_pBS2->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Cow"));

	//Object Movement
	static float fTimer = 0.0f;
	static int nClock = m_pSystem->GenClock();
	float fDeltaTime = static_cast<float>(m_pSystem->LapClock(nClock));
	fTimer += fDeltaTime;
	static vector3 v3Start = vector3(3.0, 0.0, 0.0);
	static vector3 v3End = vector3(5.0, 0.0, 0.0);
	float fPercentage = MapValue(fTimer, 0.0f, 3.0f, 0.0f, 1.0f);
	vector3 v3Current = glm::lerp(v3Start, v3End, fPercentage);
	matrix4 mTranslation = glm::translate(v3Current);

	//set the translate to create the transform matrix
	matrix4 m4Transform = glm::translate(m_v3Position) * ToMatrix4(m_qArcBall);
	matrix4 m4JustRotation = ToMatrix4(m_qArcBall);
	matrix4 m4NoRotTransform = glm::translate(m_v3Position);
	m_pMeshMngr->SetModelMatrix(m4Transform, "Zombie"); //set the matrix to the model

		
	//set matrices and render
	m_pMeshMngr->SetModelMatrix(mTranslation, "Steve");

	//bounce back and forth
	if (fPercentage > 1.0f)
	{
		fTimer = 0.0f;
		std::swap(v3Start, v3End);
	}

	

	//render the bounding boxes
	m_boMngr->SetAllMatrices({ m_pMeshMngr->GetModelMatrix("Zombie"), m_pMeshMngr->GetModelMatrix("Steve"), m_pMeshMngr->GetModelMatrix("Cow") });
	m_boMngr->CheckAllCollisions();
	m_boMngr->CheckRotation(m_qArcBall, 0);
	m_boMngr->RenderAll();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
	m_boMngr->ReleaseInstance();
	
}