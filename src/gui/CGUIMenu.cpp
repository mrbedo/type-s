#include "CGUIMenu.h"
#include <iostream>

//menu class implementation
using namespace std;
//Constructor
CGUIMenu::CGUIMenu()
{
	panelMap.clear();
	tabPanelMap.clear();
	//cout<<"Menu Created!"<<endl;
}

CGUIMenu::~CGUIMenu()
{
	//cout<<"CGUIMenu::~CGUIMenu()..."<<endl;
	unsigned int i=0;

	for(panelIter= panelMap.begin(); panelIter != panelMap.end(); ++panelIter)
	{
		panelIter->second->CleanUp();
	}

	for(tabPanelIter= tabPanelMap.begin(); tabPanelIter != tabPanelMap.end(); ++tabPanelIter)
	{
		tabPanelIter->second->CleanUp();
	}
	
	panelMap.clear();
	tabPanelMap.clear();

	//cout<<"CGUIMenu::[done]"<<endl;
}

bool CGUIMenu::VerifyPanelID(const char* ID, const char* callerName)
{
	if(!ID || (panelMap.find(ID)== panelMap.end()))
	{
		cout<<"ERROR>> CGUIMenu::"<<callerName<<" - '"<<ID<< "' - undefined panel ID."<<endl;
		return false;
	}
	else
		return true;
}

bool CGUIMenu::VerifyTabPanelID(const char* ID, const char* callerName)
{
	if(!ID || (tabPanelMap.find(ID)== tabPanelMap.end()))
	{
		cout<<"ERROR>> CGUIMenu::"<<callerName<<" - '"<<ID<< "' - undefined tab panel ID."<<endl;
		return false;
	}
	else
		return true;
}
void CGUIMenu::SetPanelLabel(const char* panelID, const CVector3& position, string text)
{
	if(VerifyPanelID(panelID, "SetPanelLabel"))
		panelMap[panelID]->SetLabel(position, text);
}

void CGUIMenu::SetPanelButtonLabel(const char* panelID, unsigned int buttonIndex, const CVector3& position, string text)
{
	if(VerifyPanelID(panelID, "SetPanelButtonLabel"))
		panelMap[panelID]->SetButtonLabel(buttonIndex,position,text);
}

void CGUIMenu::SetPanelTextBoxLabel(const char* panelID, unsigned int buttonIndex, const CVector3& position, string text)
{
	if(VerifyPanelID(panelID, "SetPanelTextBoxLabel"))
		panelMap[panelID]->SetTextBoxLabel(buttonIndex,position,text);
}

void CGUIMenu::SetPanelListBoxLabel(const char* panelID, unsigned int buttonIndex, const CVector3& position, string text)
{
	if(VerifyPanelID(panelID, "SetPanelListBoxLabel"))
		panelMap[panelID]->SetListBoxLabel(buttonIndex,position,text);
}

void CGUIMenu::AddPanelButton(const char* panelID, const CVector3& position, string text, CGUITheme& theme)
{
	if(VerifyPanelID(panelID, "SetPanelListBoxLabel"))
		panelMap[panelID]->AddButton(position,text,theme);
}

void CGUIMenu::AddPanelTextBox(const char* panelID, const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme)
{
	if(VerifyPanelID(panelID, "AddPanelTextBox"))
		panelMap[panelID]->AddTextBox(position, maxChars, digitOnly, theme);
}

void CGUIMenu::AddPanelListBox(const char* panelID, const CVector3& position, float width, float height, vector<string> buttonNames, CGUITheme& theme)
{
	if(VerifyPanelID(panelID, "AddPanelListBox"))
		panelMap[panelID]->AddListBox(position, width, height, buttonNames, theme);
}

void CGUIMenu::AddTabPanelButton(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, string text, CGUITheme& theme)
{
	if(VerifyTabPanelID(tabPanelID, "AddTabPanelButton"))
		tabPanelMap[tabPanelID]->AddButton(tabNumber, position,text,theme);
}

void CGUIMenu::AddTabPanelTextBox(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme)
{
	if(VerifyTabPanelID(tabPanelID, "AddTabPanelTextBox"))
		tabPanelMap[tabPanelID]->AddTextBox(tabNumber, position, maxChars, digitOnly, theme);
}

void CGUIMenu::AddTabPanelListBox(const char* tabPanelID, unsigned int tabNumber, const CVector3& position, float width, float height, vector<string> buttonNames, CGUITheme& theme)
{
	if(VerifyTabPanelID(tabPanelID, "AddTabPanelListBox"))
		tabPanelMap[tabPanelID]->AddListBox(tabNumber, position, width, height, buttonNames, theme);
}

//Adds a button to the panelVector
void CGUIMenu::AddPanel(const char* ID, CGUIPanel* p)
{
	cout<<"aldfj"<<endl;
	//if(ID.c_str()!=0 && !ID.empty())
	if(ID !=0)
		panelMap[ID]=p;
	else
		cout<<"CGUIMenu::AddPanel>> Invalid ID, must have string value"<<endl;
}

//Adds a button to the panelVector
void CGUIMenu::AddTabPanel(const char* ID, CGUITabPanel* p)
{
	if(ID!=0)
		tabPanelMap[ID]=p;
	else
		cout<<"CGUIMenu::AddTabPanel>> Invalid ID, must have string value"<<endl;
}

//Removes a button from the button vector
void CGUIMenu::RemovePanel(const char* panelID)
{
	panelMap.erase(panelID);
}

//Removes a button from the button vector
void CGUIMenu::RemoveTabPanel(const char* tabPanelID)
{
	tabPanelMap.erase(tabPanelID);
}

//Display all the buttons
void CGUIMenu::DisplayMenu()
{
	for(tabPanelIter=tabPanelMap.begin(); tabPanelIter!=tabPanelMap.end(); ++tabPanelIter)
	{
		tabPanelIter->second->Display();
		tabPanelIter->second->DisplayText();
		tabPanelIter->second->RenderContents();
	} 
	for(panelIter=panelMap.begin(); panelIter!=panelMap.end(); ++panelIter)
	{
		panelIter->second->Display();
		panelIter->second->DisplayText();
		panelIter->second->RenderContents();
	}  
}
//process a main menu selection
void CGUIMenu::MakeMenuSelection(short state)
{
	const char* activeSelectionPanel= "";
	const char*  activeSelectionTabPanel= "";

	panelIter=panelMap.begin();
	while(panelIter != panelMap.end() && activeSelectionPanel == "") //check panels first
	{
		if(panelIter->second->GetSelectedItem()> -1)			//some panel was selected
			activeSelectionPanel= panelIter->first;
		++panelIter;
	}

	tabPanelIter=tabPanelMap.begin();
	if(activeSelectionPanel == "")
	{
		while(tabPanelIter != tabPanelMap.end() && activeSelectionTabPanel == "")
		{
			if(tabPanelIter->second->GetSelectedItem()> -1)	//some tab panel was selected
				activeSelectionTabPanel= tabPanelIter->first;
			++tabPanelIter;
		}
	}

	if(activeSelectionPanel != "")							//if a panel was activated
		panelMap[activeSelectionPanel]->SelectItem(state); //apply selection to only that item
	else
	{
		if(activeSelectionTabPanel == "")					//if no tab panel was selected, do free selection on panels 
		{
			for(panelIter= panelMap.begin(); panelIter!= panelMap.end(); ++panelIter)
				panelIter->second->SelectItem(state);
			for(tabPanelIter= tabPanelMap.begin(); tabPanelIter!= tabPanelMap.end(); ++tabPanelIter)
				tabPanelIter->second->SelectItem(state);
		}	
		else												//else a tab panel was selected, and apply only selection to it
			tabPanelMap[activeSelectionTabPanel]->SelectItem(state);

	}
}




int CGUIMenu::GetPanelSelection(const char* panelID)
{
	if(VerifyPanelID(panelID, "GetPanelSelection"))
		return panelMap[panelID]->GetSelectedItem();
	else
		return -9999;
}

int CGUIMenu::GetPanelSelectedButton(const char* panelID)
{
	if(VerifyPanelID(panelID, "GetPanelSelectedButton"))
		return panelMap[panelID]->GetSelectedButton();
	else
		return -9999;
}

int CGUIMenu::GetPanelListBoxValue(const char* panelID, unsigned int listBoxIndex)
{
 	if(VerifyPanelID(panelID, "GetPanelListBoxValue"))
		return panelMap[panelID]->GetListBoxValue(listBoxIndex);
	else
		return -9999;
}

float  CGUIMenu::GetPanelTextBoxFloatValue(const char* panelID, unsigned int textBoxIndex)
{
 	if(VerifyPanelID(panelID, "GetPanelTextBoxFloatValue"))
		return panelMap[panelID]->GetTextBoxFloatValue(textBoxIndex);
	else
		return -9999.0f;
}

string CGUIMenu::GetPanelTextBoxStringValue(const char* panelID, unsigned int textBoxIndex)
{
  	if(VerifyPanelID(panelID, "GetPanelTextBoxStringValue"))
        return panelMap[panelID]->GetTextBoxStringValue(textBoxIndex);
	else
		return "###INVALID_STRING###";
}

int CGUIMenu::GetTabPanelSelection(const char* tabPanelID)
{
  	if(VerifyTabPanelID(tabPanelID, "GetTabPanelSelection"))
		return tabPanelMap[tabPanelID]->GetSelectedItem();
	else
		return -9999;
}

int CGUIMenu::GetTabPanelSelectedButton(const char* tabPanelID, unsigned int tabNumber)
{
  	if(VerifyTabPanelID(tabPanelID, "GetTabPanelSelectedButton"))
		return tabPanelMap[tabPanelID]->GetSelectedButton(tabNumber);
	else
		return -9999;
}

int CGUIMenu::GetTabPanelListBoxValue(const char* tabPanelID, unsigned int tabNumber, unsigned int listBoxIndex)
{
  	if(VerifyTabPanelID(tabPanelID, "GetTabPanelListBoxValue"))
		return tabPanelMap[tabPanelID]->GetListBoxValue(tabNumber, listBoxIndex);
	else
		return -9999;
}
float  CGUIMenu::GetTabPanelTextBoxFloatValue(const char* tabPanelID, unsigned int tabNumber, unsigned int textBoxIndex)
{
  	if(VerifyTabPanelID(tabPanelID, "GetTabPanelTextBoxFloatValue"))
		return tabPanelMap[tabPanelID]->GetTextBoxFloatValue(tabNumber, textBoxIndex);
	else
		return -9999.0f;
}

string CGUIMenu::GetTabPanelTextBoxStringValue(const char* tabPanelID, unsigned int tabNumber, unsigned int textBoxIndex)
{
  	if(VerifyTabPanelID(tabPanelID, "GetTabPanelTextBoxStringValue"))
		return tabPanelMap[tabPanelID]->GetTextBoxStringValue(tabNumber, textBoxIndex);
	else
		return "###INVALID_STRING###";
}


void CGUIMenu::SetMenuTheme(float sizeX, float sizeY, CVector3* color, CVector3* borderColor, CVector3* scale,
							CVector3* textColor, CVector3* textPosition, CVector3* textBoxColor, float* borderThickness)
{
	themeSizeX=sizeX;
	themeSizeY=sizeY;
	for(unsigned int i=0; i<3; ++i)
	{
		themeColor[i]= color[i];
		themeBorderColor[i]=borderColor[i];
		themeScale[i]=scale[i];
		themeTextColor[i]=textColor[i];
		themeTextPosition[i]=textPosition[i];
		themeTextBoxColor[i]=textBoxColor[i];
		themeBorderThickness[i]=borderThickness[i];
	}
}

