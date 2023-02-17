#ifndef CGUITabPanel_H
#define CGUITabPanel_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUITabPanel.h
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

#include "CGUIPanel.h"

//text class
class CGUITabPanel: public CGUIButton
{
public:
	CGUITabPanel();
	//Post- Constructor initalizes member variables

	~CGUITabPanel();
	//Post- Destructor

	void CreateTabs(int tabWidht, int tabHeight, std::vector<std::string> &tabNames);
	void CreateTabBrowser(int tabWidht, int tabHeight, std::vector<std::string> &tabNames);
	void AddButton(unsigned int tabIndex, const CVector3& position, std::string text, CGUITheme& theme);
	void AddTextBox(unsigned int tabIndex, const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme);
	void AddListBox(unsigned int tabIndex, const CVector3& position, float width, float height, std::vector<std::string> &buttonNames, CGUITheme& theme);

	void RemoveButton();
	void RemoveListBox();
	void RemoveTextBox();

	int GetSelectedButton(unsigned int tabIndex);
	int GetListBoxValue(unsigned int tabIndex, unsigned int index);
	float GetTextBoxFloatValue(unsigned int tabIndex, unsigned int index);
	std::string GetTextBoxStringValue(unsigned int tabIndex, unsigned int index);

	void RenderContents();

	void SelectItem(short state);

	int GetSelectedItem();

	int GetSelectedTab();

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

	void DrawTabSelectionMask();
	//draws a line of the border thickness of the panel color at the bottom of the tab
	//to make it seem as though the tab is joined with the panel

private:
	std::vector<CGUIPanel> panelVector;
	CGUIPanel* tabsPanel;
	CGUITheme tabTheme;

	int currentTab;
};
#endif