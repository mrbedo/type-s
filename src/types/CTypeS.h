#ifndef CTYPES_H
#define CTYPES_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTypeS.h
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
//  The common game class descriptor, various game classes will inherit this class
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "../engine/CCamera.h"
#include "../engine/CTime.h"
#include "../gui/CGUIText.h"




class CTypeS
{
protected:
	HDC hDC;

	CCamera			c;											// Camera object
  	CGUIText		t;											// Text object
	CTime			time;
	float deltaTime;
	int menuSelection;
	float mouseSensitivity;
	bool showText;												//displays the text on the screen
	bool useMouse;
	bool loadingScreenUp;
	float textAlign;
	float scrollplace;

public:
	CTypeS();
	//Post- default constructor
	
	virtual ~CTypeS();
	//Post- default destructor
	
	virtual int Init()=0;
	//Pre-  data members exist uninitalized
	//Post- 3DS model is initalized
	
	virtual void RunScene()=0;
	//Pre-  initalization exists
	//Post- the free drive mode of the game where the user
	//		can freely expore the world driving the car without
	//		limitations

	virtual void LogicLoop()=0;
	//Pre-  initalization along with FreeDrive() exist
	//Post- performs game logic separate from render loop
	
	virtual int CleanUp()=0;
	//Pre-  new objects exist
	//Post- frees up any memory and deallocates the new objects
	
	virtual void DisplayText()=0;
	//Pre-  text is initialized
	//Post- builds openGL fonts and glutOpenGL fonts

	virtual void DisplayMenu()=0;
	//Pre-  menu is initalized with a number of available choices
	//Post- the main menu of the game
		
	virtual void InitMenu()=0;
	//Pre-  initalization in progress
	//Post- main menu is initalized
	
	virtual void MakeMenuSelection(short state)=0;
	//Pre-  main menu object exists
	//Post-	if state = 0, main menu selection is only depressed
	//		if state = 1, main menu selection is selected

	virtual int GetMenuSelection()=0;
	//Pre-  a menu selection exists
	//Post- returns the menu selection from the local mainMenu object

	void SetHDC(HDC hdc)
	{
		hDC=hdc;
	}
	
	void MoveCamera(CVector3 velocity);
	//Pre-  camera object exists
	//Post- camera velocity set with x y z vector

	void SetCameraSpeed(float speed);
	//Pre-  camera objec exists
	//Post- defines how fast the camera is moving

	float GetCameraSpeed();
	//Pre-  camera object exists
	//Post- returns camera speed variable

	void ResetCamera();
	//Pre-  camera object exists
	//Post- resets the camera

	void LoadingScreen();
	//Pre-  picture created
	//Post- draws a picture for a loading screen
		
	void ToggleOutputDisplay();
	//Pre-  showText flag exists
	//Post- toggle text display

	void ScrollOutputUp();
	//Pre-  output text coordinates exist
	//Post- scroll display data up
	
	void ScrollOutputDown();
	//Pre-  output text coordinates exist
	//Post- scroll display data down
	
	void MoveMouse(bool flag);
	//Pre-  mouse moving flag exists
	//Post- sets the mouse moving flag

	int TopCleanUp();
	//Pre-  global objects exists
	//Post- cleans up global objects

	void UpdateTime();
	//Pre-  delta time exists
	//Post- new delta time computed
};
#endif