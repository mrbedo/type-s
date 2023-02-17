#include"CTypeSManager.h"
#include <iostream>
using namespace std;

//Initalize objects and member variables
void CTypeSManager::Init()					// All Setup For OpenGL Goes Here
{
	exit=false;
	input= CInput::Instance();
	manager= 0;

	GoToScene(MENU);
}	

//Initalize input object
void CTypeSManager::SetInput()
{
}

void CTypeSManager::ProcessInputs()
{
	int key=input->GetKey();
	int mouse=input->GetMouseButton();

	switch(currentSceneState)
	{
	case MENU:

		if(mouse == L_BUTTON_DOWN)
		{
			input->SetMouseButton(mouse, false);
			manager->MakeMenuSelection(0);
		}

		if(mouse == L_BUTTON_UP)
		{
			input->SetMouseButton(mouse, false);
			
			manager->MakeMenuSelection(1);
			switch(manager->GetMenuSelection())
			{
			case 0:	GoToScene(GAME);		break;
			//case 1: GoToScene(CAR_EDITOR);	break;
			case 3: exit=true;				break;
			}
		}		

		switch(key)
		{
		case 'O':
			input->SetKey('O',false);
			GoToScene(GAME);
			break;
		case 'P':
			input->SetKey('P',false);
			GoToScene(CAR_EDITOR);
			break;

		case VK_ESCAPE:
			input->SetKey(VK_ESCAPE,false);
			exit=true;
			break;

		case VK_UP:
			input->SetKey(VK_UP,false);
			((CTypeSMenu*) manager)->SelectCarUp();			//select car up
			break;
		case VK_DOWN:
			input->SetKey(VK_DOWN,false);
			((CTypeSMenu*) manager)->SelectCarDown();		//select car up
			break;
		}

		if(input->Key(VK_F11))
		{
			input->SetKey(VK_F11,false);
			ToggleWireFrame();
		}

		break;

	case GAME:
		if(mouse == L_BUTTON_DRAG)
			manager->MoveMouse(true);
		else
			manager->MoveMouse(false);
		
		if(input->Key(VK_ESCAPE))
		{
			input->SetKey(VK_ESCAPE,false);
			GoToScene(MENU);
		}
		else
		{

			if(input->Key('1'))
				manager->SetCameraSpeed(0.1f);

			if(input->Key('2'))
				manager->SetCameraSpeed(-0.1f);


			if(input->Key('V'))
			{
				input->SetKey('V',false);
				((CTypeSGame*) manager)->ChangeCarCamera();
			}

			if(input->Key('A'))
				manager->MoveCamera(CVector3(0.0f,0.0f,-manager->GetCameraSpeed()));

			if(input->Key('S'))
				manager->MoveCamera(CVector3(-manager->GetCameraSpeed(),0.0f,0.0f));

			if(input->Key('D'))
				manager->MoveCamera(CVector3(0.0f,0.0f,manager->GetCameraSpeed()));

			if(input->Key('W'))
				manager->MoveCamera(CVector3(manager->GetCameraSpeed(),0.0f,0.0f));

			if(input->Key('E'))
				manager->MoveCamera(CVector3(0.0f,manager->GetCameraSpeed(),0.0f));

			if(input->Key('Q'))
				manager->MoveCamera(CVector3(0.0f,-manager->GetCameraSpeed(),0.0f));
			
			if(input->Key(VK_LEFT))
			{
				((CTypeSGame*) manager)->SteerLeft();
			}
			else if(input->Key(VK_RIGHT))
			{
				((CTypeSGame*) manager)->SteerRight();
			}
			else
			{
				((CTypeSGame*) manager)->StraightenSteering();
			}

			if(input->Key(VK_UP))
			{
				((CTypeSGame*) manager)->Throttle();
			}
			else if(input->Key(VK_DOWN))
			{
				((CTypeSGame*) manager)->Brake();
			}
			else
			{
				((CTypeSGame*) manager)->Decel();
			}

			if(input->Key(VK_SPACE))
			{
				//input->SetKey(VK_SPACE,false);
				//((CTypeSGame*) manager)->Decel();
			}

			if(input->Key('R'))
			{
				input->SetKey('R',false);
				((CTypeSGame*) manager)->ResetCarPosition();
			}

			if(input->Key(VK_PRIOR))
				manager->ScrollOutputDown();

			if(input->Key(VK_NEXT))
				manager->ScrollOutputUp();

			if(input->Key( VK_F11))
			{
				input->SetKey(VK_F11,false);
				ToggleWireFrame();
			}

			if(input->Key( VK_F2))
			{
				input->SetKey(VK_F2,false);
				manager->ToggleOutputDisplay();
			}
		}
		break;

	case CAR_EDITOR:
		if(input->Key(VK_ESCAPE))
		{
			input->SetKey(VK_ESCAPE,false);
			GoToScene(MENU);
		}

		if(input->Key(VK_CONTROL))
			manager->MoveMouse(true);
		else
			manager->MoveMouse(false);

		if(input->Key('V'))
		{
			input->SetKey('V',false);
			((CTypeSCarEditor*) manager)->ChangeCarCamera();
		}

		if(input->Key('A'))
		{
			manager->MoveCamera(CVector3(0.0f,0.0f,-manager->GetCameraSpeed()));
		}

		if(input->Key('S'))
			manager->MoveCamera(CVector3(-manager->GetCameraSpeed(),0.0f,0.0f));

		if(input->Key('D'))
			manager->MoveCamera(CVector3(0.0f,0.0f,manager->GetCameraSpeed()));

		if(input->Key('W'))
			manager->MoveCamera(CVector3(manager->GetCameraSpeed(),0.0f,0.0f));

		if(input->Key('E'))
			manager->MoveCamera(CVector3(0.0f,manager->GetCameraSpeed(),0.0f));

		if(input->Key('Q'))
			manager->MoveCamera(CVector3(0.0f,-manager->GetCameraSpeed(),0.0f));

		if(input->Key( VK_F11))
		{
			input->SetKey(VK_F11,false);
			ToggleWireFrame();
		}
		break;
	}
}

void CTypeSManager::RunScene()
{
	POINT mousePos;
	GetCursorPos(&mousePos);								// Get the mouse's current X,Y position
	input->SetMouseCoords(mousePos.x, mousePos.y);

	manager->UpdateTime();
	ProcessInputs();
	manager->RunScene();
	manager->LogicLoop();
}

//Manages the current scene selection
void CTypeSManager::GoToScene(int scene)
{
	currentSceneState=scene;

	manager=0;

	switch(scene)
	{
	case MENU:
		manager= &typeSMenu;
		break;
	case GAME:
		manager= &typeSGame;
		break;
	case CAR_EDITOR:
		manager= &typeSCarEditor;
		break;
	}
	manager->SetHDC(hDC);
	manager->LoadingScreen();
	manager->Init();
}

//returns the current mode of the game
int CTypeSManager::GetCurrentSceneState()
{
	return currentSceneState;
}

//delete unused data
int CTypeSManager::CleanUp()
{
	cout<<"\nCleanUP - CTypeSManager...";

	typeSGame.CleanUp();
	typeSMenu.CleanUp();
	typeSCarEditor.CleanUp();

	manager= 0;
	cout<<"\n          [done]";
	return true;
}

//flag to exit to operating system
bool CTypeSManager::Quit()
{
	if(exit)
	{
		CleanUp();
		return true;
	}
	else
		return false;
}

//toggle between wireframe and solid line drawing mode
void CTypeSManager::ToggleWireFrame()
{
	static bool drawWire=false;

	glDisable(GL_LIGHTING);
	drawWire= !drawWire;
	if(drawWire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
}