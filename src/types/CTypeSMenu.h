#ifndef CTYPESMENU_H
#define CTYPESMENU_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTypeSMenu.h
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
//  Manages the main menu screen of the game
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTypeS.h"
#include "CCarManager.h"
#include "CTerrainManager.h"
#include "../gui/CGUIMenu.h"

using namespace std;

class CTypeSMenu: public CTypeS
{
private:
	CScreenManager  *screen;

	CCarManager		*carMan;
	CTerrainManager *terrainMan;
	CCar			*currentCar;
	CTerrain		*currentTerrain;

	CGUIMenu *guiMenu;
	CGUIPanel *panel;
	CGUITheme *btnTheme;
	CGUITheme *btnThemeDisabled;



	int mainMenuSelection;

//	CGUIButton mainMenuButtons[4];									// Buttons to be placed in menu
	CBitmap menuBMP;
	CVector3 btnScale;

	float textAlign;
	float scrollplace;
	bool quit;													//holds value which singals termination of program

	vector<CBitmap> carDescriptionPic;

public:
	CTypeSMenu(){}
	//Post- default constructor
	
	virtual ~CTypeSMenu(){}
	//Post- default destructor
	
	virtual int Init();
	//Pre-  initalization in progress
	//Post- main menu is initalized
	
	virtual void InitMenu();
	//Pre-  initalization in progress
	//Post- main menu is initalized
	
	virtual void MakeMenuSelection(short state);
	//Pre-  main menu object exists
	//Post-	if state = 0, main menu selection is only depressed
	//		if state = 1, main menu selection is selected

	virtual int GetMenuSelection();
	//Pre-  a menu selection exists
	//Post- returns the menu selection from the local mainMenu object
	

	virtual void RunScene();
	//Pre-  initalization exists
	//Post- the free drive mode of the game where the user
	//		can freely expore the world driving the car without
	//		limitations

	virtual void LogicLoop();
	//Pre-  initalization along with FreeDrive() exist
	//Post- performs game logic separate from render loop
	
	virtual int CleanUp();
	//Pre-  new objects exist
	//Post- frees up any memory and deallocates the new objects
	
	virtual void DisplayText();
	//Pre-  text is initialized
	//Post- builds openGL fonts and glutOpenGL fonts

	virtual void DisplayMenu();
	//Pre-  menu is initalized with a number of available choices
	//Post- the main menu of the game

	virtual void ChangeCarCamera();
	//Pre-  a car's camera index exists
	//Post- the next camera index is set
		
	virtual void ContextSpecificFunction(int id);
	//Pre-  most scenes involve some sort of specific functions 
	//		i.e. controlling direction
	//Post- provides an abstraction for delegating specific tasks to each
	//		inherited class, where the inmplementation of this function will 
	//		be context specific based on the specified id. 
	//		i.e. (1,2,3,4) for game scene may be used to call directional up, down,
	//		left, right to control the car, but in garage mode to call functions for
	//		controlling parts, etc.
	
	void SelectCarUp();
	//Pre-  a vector of cars exists
	//Post-	selects the next car in the vector
	//selects the previous car in the vector
	
	void SelectCarDown();
	//Pre-  a vector of cars exists
	//Post-	the current car selection is the previous vector item
	
	void NextMainMenuChoice();
	//Pre-	a menu choice exists
	//Post-	current menu choice becomes the next in the menu choices
	//selects the previous menu choice
	
	void PreviousMainMenuChoice();
	//Pre-  a menu choice exists
	//Post-	current menu choice becomes the previous in the menuchoices
	
	void ScrollOutputUp();
	//Pre-;
	//Post- scroll display data up
	
	void ScrollOutputDown();
	//Pre-
	//Post- scroll display data down
};
#endif