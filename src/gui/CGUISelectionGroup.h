#ifndef CGUISelectionGroup_H
#define CGUISelectionGroup_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUISelectionGroup.h
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
//  This class provides ability to track menu items and current selections on the menu
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../engine/CInput.h"
#include "../engine/CScreenManager.h"
#include "CGUIButton.h"
#include "CGUITextBox.h"
#include "CGUIListBox.h"
#include "CGUITheme.h"

class CGUISelectionGroup
{
public:

	CGUISelectionGroup();
	//Post- Constructor initalizes the number of menu items

	~CGUISelectionGroup();
	//Post- Default destructor, empties out the button vector

	void SetButtonLabel(unsigned int buttonIndex, const CVector3& position, std::string text);
	void SetTextBoxLabel(unsigned int buttonIndex, const CVector3& position, std::string text);
	void SetListBoxLabel(unsigned int buttonIndex, const CVector3& position, std::string text);

	void SetChoice(int c);
	//Pre-	A choice exists
	//Post-	A new choice is assigned

	void DepressChoice(bool depress);
	//Pre-  A choice exists
	//Post- If depress = true depressedButton is assigned the current button
	//		else depressedButton is nothing and is assigned -1

	void NextChoice();
	//Pre-	A menu exists
	//Post-	The next choice in the menu is selected

	void PreviousChoice();
	//Pre-	A menu exists
	//Post- The previous choice in the menu is selected

	int GetChoice();
	//Pre-	A menu choice exists
	//Post- Returns the current menu choice

	int GetNumberOfChoices();
	//Pre-  A set number of choices exists
	//Post- Returns numberOfMenuChoices

	void AddButton(const CVector3& position, const std::string& text, CGUITheme& theme);
	//Pre-  button vector exists
	//Post- adds a button to the vector at specified position with specified text

	void RemoveButton();
	//Pre-  button vector exists
	//Post- removes a button from the vector

	void AddTextBox(const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme);
	//Pre-  button vector exists
	//Post- adds a button to the vector

	float GetTextBoxFloatValue(unsigned int index);
	std::string GetTextBoxStringValue(unsigned int index);

	void RemoveTextBox();
	//Pre-  button vector exists
	//Post- removes a button from the vector

	void AddListBox(const CVector3& position, float width, float height, std::vector<std::string> &buttonNames, CGUITheme& theme);
	//Pre-  panel vector exists
	//Post- adds a panel to the vector

	int GetListBoxValue(unsigned int index);

	void RemoveListBox();
	//Pre-  panel vector exists
	//Post- removes a panel from the vector

	void DisplaySelection();
	//Pre-  button vector exists
	//Post- calls the display function for each button
	
	void MouseSelect();					
	//Pre-  draw a menu with mouse selection capabilities
	//Post- menu drawn where mouse coordinates are gotten and menu choice is selected
	
	void DeselectAllTextBoxes();
	//Pre-  some text boxes may be active
	//Post- sets the active flags for all present text boxes to false

	void DeselectAllListBoxes();

	bool IsSelectionDisabled();
	//returns disableselected flag so other instances can see if listboxes are active somewhere
	void DisableSelection(bool ds);
	//sets the disableSelection flag

private:
	CInput *input;
	CScreenManager *screen;

	bool disableSelection;
	int activeTextBox,
		activeListBox;

	int currentMenuChoice,									//stores the current menu selection index
		depressedMenuChoice,								//stores the menu item for which the mouse button is pressed down on
		buttonHits;											//number of rendred buttons picked during openGL picking

	std::vector<CGUIButton> buttonVector;
	std::vector<CGUITextBox> textBoxVector;
	std::vector<CGUIListBox> listBoxVector;

};
#endif