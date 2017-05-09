#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Octree"); // Window Name 

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 2.5f, 150.0f), vector3(0.0f, 2.5f, 11.0f), REAXISY);

	m_pBOMngr = MyBOManager::GetInstance();

	m_nInstances = 3500;
	int nSquare = static_cast<int>(std::sqrt(m_nInstances));
	m_nInstances = nSquare * nSquare;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			sInstances.push_back("Cube_" + std::to_string(i) + "_" + std::to_string(j));
			matrix4 m4Positions = glm::translate(static_cast<float>(i - nSquare / 2.0f), static_cast<float>(j), 0.0f);
			m4Positions = glm::translate(vector3(glm::sphericalRand(35.0f)));
			m_pMeshMngr->LoadModel("Portal\\CompanionCube.bto", sInstances[i*nSquare + j], false, m4Positions);
			m_pMeshMngr->SetModelMatrix(m4Positions, sInstances[i*nSquare + j]);
			m_pBOMngr->AddObject(m_pMeshMngr->GetVertexList(sInstances[i*nSquare + j]), sInstances[i*nSquare + j]);
			m_pBOMngr->SetModelMatrix(m4Positions, sInstances[i*nSquare + j]);
		}
	}

	for (int i = 0; i < sInstances.size(); i++)
	{
		
		matrix4 temp = m_pMeshMngr->GetModelMatrix(sInstances[i]);
		allM4.push_back(temp);
		allBO.push_back(m_pBOMngr->GetBoundingObject(i));
	}

	myOctree = new Octree(allM4, allBO, 3);

	usingOctree = true;
	calcOctree = true;
	renderOctree = true;
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

	ArcBall();
	
	if (usingOctree)
	{
		if (renderOctree)
		{
			myOctree->Render();
		}

		if (calcOctree)
		{
			myOctree->Update();
		}
	}
	else
	{
		m_pBOMngr->Update();
	}


	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	m_pMeshMngr->PrintLine("");
	//Print info on the screen
	std::vector<int> list = m_pBOMngr->GetCollidingVector(0);
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);

	m_pMeshMngr->PrintLine("Toggles: ");
	m_pMeshMngr->Print("  Octree: O ("); if (usingOctree) { m_pMeshMngr->Print("On"); }	else { m_pMeshMngr->Print("Off"); } m_pMeshMngr->PrintLine(")");
	m_pMeshMngr->Print("  Render: R ("); if (renderOctree) { m_pMeshMngr->Print("On"); } else { m_pMeshMngr->Print("Off"); } m_pMeshMngr->PrintLine(")");
	m_pMeshMngr->Print("  Calculate: C ("); if (calcOctree) { m_pMeshMngr->Print("On"); } else { m_pMeshMngr->Print("Off"); } m_pMeshMngr->PrintLine(")");
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
	MyBOManager::ReleaseInstance();
	delete myOctree;
	for (int i = 0; i < allBO.size(); i++)
	{
		if (allBO[i] != nullptr)
		{
			allBO[i] = nullptr;
		}
		delete allBO[i];
	}
}