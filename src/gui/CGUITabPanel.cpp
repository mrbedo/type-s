///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUITabPanel.cpp
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
#include "CGUITabPanel.h"
#include <iostream>

using namespace std;

//default constructor
CGUITabPanel::CGUITabPanel()
{
	tabsPanel= new CGUIPanel();			//CAUTION:  Does not get deleted since needs to stay in memory
										//			so that tabsPanel can call AddButtons without a 0 pointer
										//			error. Creating a regular object thew an exception as well
										//			future effects unknown.

	cout<<"TabPanel Created!"<<endl;
	currentTab= 0;
}

//default destructor
CGUITabPanel::~CGUITabPanel()
{
	cout<<"TabPanel Destroyed!"<<endl;
	//delete tabsPanel;
	cout<<"tabs panel deleted!"<<endl;
}

void CGUITabPanel::CreateTabs(int tabWidth, int tabHeight, vector<string> &tabNames)
{
		
	for(unsigned int j=0; j<3; j++)							//create a color theme
	{	
		tabTheme.themeBorderColor[j]= borderColor[j];
		tabTheme.themeBorderThickness[j]= borderThickness[j];
		tabTheme.themeScale[j]= scale[j];
		tabTheme.themeColor[j]= color[j];
		tabTheme.themeTextColor[j]= textColor[j];
		tabTheme.themeTextPosition[j]= textPosition[j];
	}
	tabTheme.themeSizeX= tabWidth;
	tabTheme.themeSizeY= tabHeight;
	
	tabsPanel->Init();
	tabsPanel->SetPosition(position);

	for(unsigned int i=0; i<tabNames.size(); ++i)			//initalize each panel for each tab name
	{
		CGUIPanel p;
		p.Init();
		p.SetSize(tabWidth, tabHeight);
		p.SetPosition(position);
		p.SetText(tabNames[i]);
		p.SetTextPosition(textPosition);
		p.SetTextColor(textColor);
		p.SetScale(scale);
		p.SetColor(color);
		p.SetBorderColor(borderColor);
		p.SetBorderThickness(borderThickness);
		panelVector.push_back(p);
		tabsPanel->AddButton(CVector3(i*tabWidth, -tabHeight, 0),tabNames[i], tabTheme);
	}

}

void CGUITabPanel::CreateTabBrowser(int tabWidth, int tabHeight, vector<string> &tabNames)
{
}

void CGUITabPanel::AddButton(unsigned int tabIndex, const CVector3& position, string text, CGUITheme& theme)
{
	if(tabIndex >= panelVector.size())
		cout<<"CGUITabPanel::AddButton>> Invalid tab Index, exceeds panelVector size"<<endl;
	else
		panelVector[tabIndex].AddButton(position, text, theme);
}

void CGUITabPanel::AddTextBox(unsigned int tabIndex, const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme)
{
	if(tabIndex >= panelVector.size())
		cout<<"CGUITabPanel::AddTextBox>> Invalid tab Index, exceeds panelVector size"<<endl;
	else
		panelVector[tabIndex].AddTextBox(position, maxChars, digitOnly, theme);
}

void CGUITabPanel::AddListBox(unsigned int tabIndex, const CVector3& position, float width, float height, vector<string> &buttonNames, CGUITheme& theme)
{
	if(tabIndex >= panelVector.size())
		cout<<"CGUITabPanel::AddListBox>> Invalid tab Index, exceeds panelVector size"<<endl;
	else
		panelVector[tabIndex].AddListBox(position, width, height, buttonNames, theme);
}

void CGUITabPanel::DrawTabSelectionMask()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glLineWidth(tabTheme.themeBorderThickness[state]);

	glColor3f(tabTheme.themeColor[0].x, tabTheme.themeColor[0].y, tabTheme.themeColor[0].z);
	glBegin(GL_LINES);
		glVertex2f(position[state].x+((currentTab)*tabTheme.themeSizeX)-1+tabTheme.themeBorderThickness[state], position[state].y);
		glVertex2f(position[state].x+((currentTab+1)*tabTheme.themeSizeX)-1, position[state].y);
	glEnd();
	glPopAttrib();
}

void CGUITabPanel::RenderContents()
{
	tabsPanel->RenderContents();
	panelVector[currentTab].RenderContents();
	DrawTabSelectionMask();
}

void CGUITabPanel::SelectItem(short state)
{
	tabsPanel->SelectItem(state);
	cout<<"tabsPanel selected="<<tabsPanel->GetSelectedItem()<<endl;
	if(tabsPanel->GetSelectedItem()> -1)
		currentTab=tabsPanel->GetSelectedItem();
	panelVector[currentTab].SelectItem(state);
}

int CGUITabPanel::GetSelectedItem()
{
	return panelVector[currentTab].GetSelectedItem();
}

int CGUITabPanel::GetSelectedButton(unsigned int tabIndex)
{
	return panelVector[tabIndex].GetSelectedButton(); //make only to reflect buttons
}

int CGUITabPanel::GetListBoxValue(unsigned int tabIndex, unsigned int index)
{
	return panelVector[tabIndex].GetListBoxValue(index);
}

float CGUITabPanel::GetTextBoxFloatValue(unsigned int tabIndex, unsigned int index)
{
	return panelVector[tabIndex].GetTextBoxFloatValue(index);
}

string CGUITabPanel::GetTextBoxStringValue(unsigned int tabIndex, unsigned int index)
{
	return panelVector[tabIndex].GetTextBoxStringValue(index);
}

//set the value of the string
void CGUITabPanel::PrintLine(float x, float y, string l)
{
	panelVector[currentTab].PrintLine(x,y,l);
}

//sets the selected flag
void CGUITabPanel::RemoveLine(int index)
{
	panelVector[currentTab].RemoveLine(index);
}

//set the value of the string

//display all the text in the box
void CGUITabPanel::DisplayText()
{
	panelVector[currentTab].DisplayText();
}
