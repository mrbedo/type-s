#ifndef CGUIListBox_H
#define CGUIListBox_H

#include "../engine/CInput.h"
#include "../engine/CScreenManager.h"
#include "CGUIButton.h"
#include <vector>


class CGUIListBox: public CGUIButton
{
public:
	CGUIListBox();
	//Post- Constructor initalizes member variables

	~CGUIListBox();
	//Post- Destructor

	void AddButton(std::string name);

	void DisplayListBox();

	void SetInput();

	/*
	void DisplayListBoxItem(unsigned int index);
	void SelectionDisplayListBoxItem(unsigned int index);
	void SetItemState(unsigned int index, short state);
*/
	unsigned int GetListBoxSize();

	int GetValue();

	void SetItemChoice(int c);

	void DepressItem(bool depress);

	bool IsActive();
	//Pre-  active flag exists
	//Post- returns the active flag

	void SetActive(bool a);
	//Pre-  active flag exists
	//Post- sets the active flag
private:
	CInput *input;
	CScreenManager *screen;

	bool listBoxOpened;											//if selected you can type into the text box
	int currentItemChoice,									//stores the current menu selection index
		depressedItemChoice,								//stores the menu item for which the mouse button is pressed down on
		itemHits;											//number of rendred buttons picked during openGL picking
	std::vector<CGUIButton> buttonItems;


};
#endif