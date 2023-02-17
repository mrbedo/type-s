///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUIPanel.cpp
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

using namespace std;



//default constructor
CGUIPanel::CGUIPanel():	pickingAllowed(true), selectedItem( -1)
{
//	cout<<"Panel Created!"<<endl;
	textLine.clear();
	textXPos.clear();
	textYPos.clear();
}

//default destructor
CGUIPanel::~CGUIPanel()
{
//	cout<<"Panel Destroyed!"<<endl;
}
void CGUIPanel::SetButtonLabel(unsigned int buttonIndex, const CVector3& position, string text)
{
	selectionGroup.SetButtonLabel(buttonIndex, position, text);
}

void CGUIPanel::SetTextBoxLabel(unsigned int buttonIndex, const CVector3& position, string text)
{
	selectionGroup.SetTextBoxLabel(buttonIndex, position, text);
}

void CGUIPanel::SetListBoxLabel(unsigned int buttonIndex, const CVector3& position, string text)
{
	selectionGroup.SetListBoxLabel(buttonIndex, position, text);
}

void CGUIPanel::AddButton(const CVector3& position, string text, CGUITheme& theme)
{
	selectionGroup.AddButton(position, text, theme);
}

void CGUIPanel::AddTextBox(const CVector3& position, int maxChars, bool digitOnly, CGUITheme& theme)
{
	selectionGroup.AddTextBox(position, maxChars, digitOnly, theme);
}

void CGUIPanel::AddListBox(const CVector3& position, float width, float height, vector<string> buttonNames, CGUITheme& theme)
{
	selectionGroup.AddListBox(position, width, height, buttonNames, theme);
}

void CGUIPanel::RenderContents()
{
	glPushMatrix();
		glTranslatef(position[state].x, position[state].y, position[state].z);
		selectionGroup.DisplaySelection();
		//if(pickingAllowed)
			selectionGroup.MouseSelect();
	glPopMatrix();
}

void CGUIPanel::SelectItem(short state)
{
	switch(state)
	{
	case 0:		
		selectionGroup.DepressChoice(true);					//makes button appear depressed
		break;
	case 1:													//select button
		selectionGroup.DepressChoice(false);				//makes button appear released
		selectedItem= selectionGroup.GetChoice();		
		break;
	}
}

int CGUIPanel::GetSelectedItem()
{
	return selectedItem;
}

int CGUIPanel::GetSelectedButton()
{
	return selectedItem; //make only to reflect buttons
}

int CGUIPanel::GetListBoxValue(unsigned int index)
{
	return selectionGroup.GetListBoxValue(index);
}

float CGUIPanel::GetTextBoxFloatValue(unsigned int index)
{
	return selectionGroup.GetTextBoxFloatValue(index);
}

string CGUIPanel::GetTextBoxStringValue(unsigned int index)
{
	return selectionGroup.GetTextBoxStringValue(index);
}


//set the value of the string
void CGUIPanel::PrintLine(float x, float y, string l)
{
	textXPos.push_back(x);
	textYPos.push_back(y);
	textLine.push_back(l);
}

//sets the selected flag
void CGUIPanel::RemoveLine(int index)
{
	textXPos.pop_back();
	textYPos.pop_back();
	textLine.pop_back();
}

//set the value of the string

//display all the text in the box
void CGUIPanel::DisplayText()
{
	glPushMatrix();

	glTranslatef(position[state].x, position[state].y, position[state].z);
	glScalef(scale[state].x, scale[state].y, scale[state].z);

	for(unsigned int i=0; i<textLine.size(); ++i)
	{
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glColor3f(textColor[state].x,
				  textColor[state].y,
				  textColor[state].z);
		t.Print(textXPos[i], textYPos[i], 0.0f,textLine[i]);
		glPopAttrib();
		glPopMatrix();
	}
	glPopMatrix();
}

bool CGUIPanel::IsPickingDisabled()
{
	return selectionGroup.IsSelectionDisabled();
}

void CGUIPanel::SetPicking(bool s)
{
	pickingAllowed=s;
}