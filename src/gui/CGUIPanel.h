#ifndef CGUIPanel_H
#define CGUIPanel_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUIPanel.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2004
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  This class provides a panel to which text and buttons could be added
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CGUISelectionGroup.h"
#include "CGUITheme.h"

//text class
class CGUIPanel: public CGUIButton
{
public:
	CGUIPanel();
	//Post- Constructor initalizes member variables

	~CGUIPanel();
	//Post- Destructor
	void SetButtonLabel(unsigned int buttonIndex, const CVector3& position, std::string text);
	void SetTextBoxLabel(unsigned int buttonIndex, const CVector3& position, std::string text);
	void SetListBoxLabel(unsigned int buttonIndex, const CVector3& position, std::string text);

	void AddButton(const CVector3& position, std::string text, CGUITheme& theme);
	void AddTextBox(const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme);
	void AddListBox(const CVector3& position, float width, float height, std::vector<std::string> buttonNames, CGUITheme& theme);

	void RemoveButton();
	void RemoveListBox();
	void RemoveTextBox();

	int GetSelectedButton();
	int GetListBoxValue(unsigned int index);
	float GetTextBoxFloatValue(unsigned int index);
	std::string GetTextBoxStringValue(unsigned int index);

	void RenderContents();

	void SelectItem(short state);

	int GetSelectedItem();

	void PrintLine(float x, float y, std::string l);
	//Pre-  a vector of strings exists
	//Post- another string is pushed on, each string is a line

	void RemoveLine(int index);
	//Pre-  a vector of strings exists
	//Post- a string at a specified index is removed

	void SetLineSpacing(float l);
	//Pre-  default line spacing exists
	//Post- updates lineSpacing with a new value

	float GetLineSpacing();
	//Pre-  lineSpacing exists
	//Post- returns lineSpacing

	void DisplayText();
	//Pre-  a vector of strings exist
	//Post- each string is displayed as a line at a specified line spacing
	//		from a position of the top left corner

	bool IsPickingDisabled();
	void SetPicking(bool s);

	CGUISelectionGroup selectionGroup;
private:
	bool pickingAllowed;
	int selectedItem;
	std::vector<std::string> textLine;								//holds the text in the text field
	std::vector<float> textXPos;
	std::vector<float> textYPos;
};
#endif