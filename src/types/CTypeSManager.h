#ifndef CTYPESMANAGER_H
#define CTYPESMANAGER_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTypeSManager.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2003
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  The main game class
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTypeSGame.h"
#include "CTypeSMenu.h"
#include "CTypeSCarEditor.h"

using namespace std;


class CTypeSManager
{
private:
	HDC hDC;
	CInput *input;

	CTypeSGame typeSGame;
	CTypeSMenu typeSMenu;
	CTypeSCarEditor typeSCarEditor;

	CTypeS *manager;

	enum TYPES_SCENES {GAME, MENU, CAR_EDITOR, CREDITS};
	int currentSceneState;

	int resolutionX;
	int resolutionY;
	bool exit;

public:
	CTypeSManager(){}
	//Post- default constructor
	
	~CTypeSManager(){}
	//Post- default destructor

	void SetHDC(HDC hhDC)
	{
		hDC=hhDC;
	}

	
	void Init();
	//Pre-  data members exist uninitalized
	//Post- 3DS model is initalized

	void SetInput();
	//Pre-  a blank input pointer exists
	//Post- an input pointer to an existing object is assigned
	
	void GoToScene(int scene);
	//Pre-	scene objects exist
	//Post- based on the scene selection, manager object is assigned new scene object

	void RunScene();
	//Pre-  the main loop
	//Post- exectutes all infinite calling functions

	void ProcessInputs();
	//Pre-  input object exists
	//Post- based on current scene state, process necessary inputs
	
	int GetCurrentSceneState();
	//Pre-  some currentSceneState exists
	//Post- returns currentSceneState
			
	void SetCurrentKeyPressed(int key, bool down);
	//Pre-  keyboard input exists into variable
	//Post- new keyboard input is assigned
	
	int CleanUp();
	//Pre-  new objects exist
	//Post- frees up any memory and deallocates the new objects
	
	bool Quit();
	//Pre-  quit flag exists
	//Post- returns the quit flag.

	void ToggleWireFrame();
	//Pre-  openGL is initalized
	//Post- sets polygon drawing mode to line drawing mode
};
#endif