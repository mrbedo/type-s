#ifndef CTYPESCAREDITOR_H
#define CTYPESCAREDITOR_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTypeSCarEditor.h
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
//  This class will provides an interface to edit cars by modifying properties
//	of the CModel described objects
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTypeS.h"
#include "CCarManager.h"
#include "CTerrainManager.h"
#include "../gui/CGUIMenu.h"
#include <vector>

//Car Shop class for editing car parts of a specified car object
class CTypeSCarEditor: public CTypeS
{
private:
	CCarManager		*carMan;
	CTerrainManager *terrainMan;
	CCar			*currentCar;
	CTerrain		*currentTerrain;
	CGUIMenu		*carShopMenu;										//menu in car shop
//	CGUIButton carShopMenuButtons[7];							//Buttons to be placed in garage menu
//	CGUITextField tf[2];

	short partOperation;										//0-move 1-scale 2- rotate	
	std::vector<int> selectedCarParts;								//holds the currently selected car parts
	int selectedCarPart;										//used for representing the car part index

public:
	CTypeSCarEditor();
	//Post- default constructor
	
	virtual ~CTypeSCarEditor();
	//Post- default destructor
	
	virtual int Init();
	//Pre-  data members exist uninitalized
	//Post- 3DS model is initalized
	
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
		
	void AdjustPart(char axis, float amount);
	//Pre-  a selection of car parts exists in a vector
	//Post- applies the selected operation along the selected axis with the specified amount
	//		to all the parts in the selectedCarParts vector
	
	void SelectPart(bool selectUp, bool addToSelection);
	//Pre-  a selectedCarParts vector exists
	//Post- if selects up, increments the value in the selectedCarParts, else decrements
	//		if addToSelection, pushes on the selectedUp, or selectedDown part index

};
#endif