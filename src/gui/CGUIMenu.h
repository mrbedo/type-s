#ifndef CGUIMenu_H
#define CGUIMenu_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUIMenu.h
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

#include "CGUITabPanel.h"
#include "CGUITheme.h"
#include <map>

class CGUIMenu
{
public:
enum PANEL_ID {PANEL, TAB_PANEL};
	CGUIMenu();
	//Post- Constructor initalizes the number of menu items

	~CGUIMenu();
	//Post- Default destructor, empties out the button vector

	void	AddPanel(const char* ID, CGUIPanel* p);
	void	SetPanelLabel		(const char* panelID, const CVector3& position, std::string text);
	void	SetPanelButtonLabel	(const char* panelID, unsigned int buttonIndex, const CVector3& position, std::string text);
	void	SetPanelTextBoxLabel(const char* panelID, unsigned int buttonIndex, const CVector3& position, std::string text);
	void	SetPanelListBoxLabel(const char* panelID, unsigned int buttonIndex, const CVector3& position, std::string text);
	void	AddPanelButton		(const char* panelID, const CVector3& position, std::string text, CGUITheme& theme);
	void	AddPanelTextBox		(const char* panelID, const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme);
	void	AddPanelListBox		(const char* panelID, const CVector3& position, float width, float height, std::vector<std::string> buttonNames, CGUITheme& theme);

	void	AddTabPanel(const char* ID, CGUITabPanel *p);
	void	AddTabPanelLabel		(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, std::string text);
	void	AddTabPanelButtonLabel	(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, std::string text);
	void	AddTabPanelTextBoxLabel	(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, std::string text);
	void	AddTabPanelListBoxLabel	(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, std::string text);
	void	AddTabPanelButton		(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, std::string text, CGUITheme& theme);
	void	AddTabPanelTextBox		(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme);
	void	AddTabPanelListBox		(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, float width, float height, std::vector<std::string> buttonNames, CGUITheme& theme);

	int		GetPanelSelection			(const char* panelID);
	int		GetPanelSelectedButton		(const char* panelID);
	int		GetPanelListBoxValue		(const char* panelID, unsigned int listBoxIndex);
	float	GetPanelTextBoxFloatValue	(const char* panelID, unsigned int textBoxIndex);
	std::string	GetPanelTextBoxStringValue	(const char* panelID, unsigned int textBoxIndex);

	int		GetTabPanelSelection			(const char* tabPanelID);
	int		GetTabPanelSelectedButton		(const char* tabPanelID, unsigned int tabNumber);
	int		GetTabPanelListBoxValue			(const char* tabPanelID, unsigned int tabNumber, unsigned int listBoxIndex);
	float	GetTabPanelTextBoxFloatValue	(const char* tabPanelID, unsigned int tabNumber, unsigned int textBoxIndex);
	std::string	GetTabPanelTextBoxStringValue	(const char* tabPanelID, unsigned int tabNumber, unsigned int textBoxIndex);


	void RemovePanel(const char* panelID);
	//Post- removes a panel from the vector
	void RemoveTabPanel(const char* tabPanelID);

	void DisplayMenu();
	//Pre-  button vector exists
	//Post- calls the display function for each button

	//process a main menu selection
	void MakeMenuSelection(short state);


	
	void SetMenuTheme(float sizeX, float sizeY, CVector3* color, CVector3* borderColor, CVector3* scale,
					  CVector3* textColor, CVector3* textPosition, CVector3* textBoxColor, float* borderThickness);
	//Pre-  menu initalized button panel and textfield objects with default parameters
	//Post- sets attributes through defined tsiFile for all menu components
private:
	std::map<const char*, CGUIPanel*> panelMap;
	std::map<const char*, CGUIPanel*>::iterator panelIter;
	std::map<const char*, CGUITabPanel*> tabPanelMap;
	std::map<const char*, CGUITabPanel*>::iterator tabPanelIter;


	//vector<CGUIPanel*>    panelVector;
	//vector<CGUITabPanel*> tabPanelVector;


	float themeSizeX;
	float themeSizeY;
	CVector3 themeColor[3];
	CVector3 themeBorderColor[3];
	CVector3 themeScale[3];
	CVector3 themeTextColor[3];
	CVector3 themeTextPosition[3];
	CVector3 themeTextBoxColor[3];
	float themeBorderThickness[3];
	bool VerifyPanelID(const char* ID, const char* msg);
	bool VerifyTabPanelID(const char* ID, const char* msg);
};
#endif