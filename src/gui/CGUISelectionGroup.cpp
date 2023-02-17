#include "CGUISelectionGroup.h"
#include <iostream>
using namespace std;

//menu class implementation

//Constructor
CGUISelectionGroup::CGUISelectionGroup():
screen(CScreenManager::Instance()),
input(CInput::Instance()),
currentMenuChoice(0),
depressedMenuChoice( -1),
buttonHits( 0),
disableSelection(false)
{
	buttonVector.clear();
	listBoxVector.clear();
	textBoxVector.clear();

	DeselectAllListBoxes();
	DeselectAllTextBoxes();
}

CGUISelectionGroup::~CGUISelectionGroup()
{
	unsigned int i=0;

	for(i=0; i<buttonVector.size(); ++i)
		buttonVector[i].CleanUp();

	for(i=0; i<listBoxVector.size(); ++i)
		listBoxVector[i].CleanUp();
	
	for(i=0; i<textBoxVector.size(); ++i)
		textBoxVector[i].CleanUp();

	buttonVector.clear();
	listBoxVector.clear();
	textBoxVector.clear();
}

void CGUISelectionGroup::SetButtonLabel(unsigned int buttonIndex, const CVector3& position, string text)
{
	if(buttonIndex>=0 && buttonIndex<buttonVector.size())
		buttonVector[buttonIndex].SetLabel(position, text);
	else
		cout<<"Error:: CGUISelectionGroup::SetButtonLabel>> buttonIndex out of range!"<<endl;
}

void CGUISelectionGroup::SetTextBoxLabel(unsigned int buttonIndex, const CVector3& position, string text)
{
	if(buttonIndex>=0 && buttonIndex<textBoxVector.size())
		textBoxVector[buttonIndex].SetLabel(position, text);
	else
		cout<<"Error:: CGUISelectionGroup::SetTextBoxLabel>> buttonIndex out of range!"<<endl;
}

void CGUISelectionGroup::SetListBoxLabel(unsigned int buttonIndex, const CVector3& position, string text)
{
	if(buttonIndex>=0 && buttonIndex<listBoxVector.size())
		listBoxVector[buttonIndex].SetLabel(position, text);
	else
		cout<<"Error:: CGUISelectionGroup::SetListBoxLabel>> buttonIndex out of range!"<<buttonIndex<<endl;
}
//Set a new menu choice
void CGUISelectionGroup::SetChoice(int c)
{
	currentMenuChoice=c;
}

float CGUISelectionGroup::GetTextBoxFloatValue(unsigned int index)
{
	if(index < textBoxVector.size())
		return textBoxVector[index].GetFloatValue();
	else
	{
		cout<<"CGUISelectionGroup::Invalid textBox index: trying to access index "<<index<<" for textBox with max index of "<<(int)textBoxVector.size()-1<<endl;
		return -9999;
	}
}

string CGUISelectionGroup::GetTextBoxStringValue(unsigned int index)
{
	if(index < textBoxVector.size())
		return textBoxVector[index].GetStringValue();
	else
	{
		cout<<"CGUISelectionGroup::Invalid textBox index: trying to access index "<<index<<" for textBox with max index of "<<(int)textBoxVector.size()-1<<endl;
		return "INVALID STRING";
	}
}

int CGUISelectionGroup::GetListBoxValue(unsigned int index)
{
	if(index < listBoxVector.size())
		return listBoxVector[index].GetValue();
	else
	{
		cout<<"CGUISelectionGroup::Invalid listBox index: trying to access index "<<index<<" for textBox with max index of "<<(int)listBoxVector.size()-1<<endl;
		return -9999;
	}
}

//Make the selected menu choice depressed
void CGUISelectionGroup::DepressChoice(bool depress)
{

	if(depress)												//On button down PRESS
	{
		depressedMenuChoice=currentMenuChoice;
		DeselectAllTextBoxes();
	}
	else													//On button up RELEASE
	{
		DeselectAllListBoxes();	
		if(depressedMenuChoice > -1)						//if an item was selected
		{
			//cout<<"depressedMenuChoice="<<depressedMenuChoice<<endl;
			if(depressedMenuChoice < (int)buttonVector.size())
			{
				buttonVector[depressedMenuChoice].SetState(2);
			}
			else if(depressedMenuChoice < (int)(buttonVector.size()+textBoxVector.size()))
			{
				textBoxVector[depressedMenuChoice-buttonVector.size()].SetState(2);
				DeselectAllTextBoxes();
				textBoxVector[depressedMenuChoice-buttonVector.size()].SetActive(true);
			}
			else if(depressedMenuChoice < (int)(buttonVector.size()+textBoxVector.size()+listBoxVector.size()))
			{
				listBoxVector[depressedMenuChoice-(buttonVector.size()+textBoxVector.size())].DepressItem(depress);
				listBoxVector[depressedMenuChoice-(buttonVector.size()+textBoxVector.size())].SetState(2);
				DeselectAllListBoxes();	
				listBoxVector[depressedMenuChoice-(buttonVector.size()+textBoxVector.size())].SetActive(true);
			}
			depressedMenuChoice= -1;
		}
	}
}

//Returns the current selected choice
int CGUISelectionGroup::GetChoice()
{
	return currentMenuChoice;
}

//Adds a button to the buttonVector
void CGUISelectionGroup::AddButton(const CVector3& position, const string& text, CGUITheme& theme)
{
	CGUIButton b;
	b.Init();
	b.SetPosition(position);
	b.SetText(text);
	b.SetColor(theme.themeColor);
	b.SetBorderColor(theme.themeBorderColor);
	b.SetSize(theme.themeSizeX, theme.themeSizeY);
	b.SetScale(theme.themeScale);
	b.SetTextColor(theme.themeTextColor);
	b.SetTextPosition(theme.themeTextPosition);
	b.SetBorderThickness(theme.themeBorderThickness);
	b.SetTextFont(theme.themeTextFont);

	buttonVector.push_back(b);
}

//Adds a button to the buttonVector
void CGUISelectionGroup::AddListBox(const CVector3& position, float width, float height, vector<string> &buttonNames, CGUITheme& theme)
{
	CGUIListBox b;
	b.Init();
	b.SetPosition(position);
	b.SetSize(width, height);
	b.SetColor(theme.themeColor);
	b.SetBorderColor(theme.themeBorderColor);
	b.SetScale(theme.themeScale);
	b.SetTextColor(theme.themeTextColor);
	b.SetTextPosition(theme.themeTextPosition);
	b.SetBorderThickness(theme.themeBorderThickness);
	b.SetTextFont(theme.themeTextFont);

	for(unsigned int i=0; i<buttonNames.size(); ++i)
	{
		//cout<<"SelectionGroup:: adding to listbox..."<<buttonNames[i]<<endl;
		b.AddButton(buttonNames[i]);
	}
	listBoxVector.push_back(b);
}

//Adds a button to the buttonVector
void CGUISelectionGroup::AddTextBox(const CVector3 &position, int maxChars, bool digitOnly, CGUITheme& theme)
{
	CGUITextBox b;
	b.Init();
	b.SetPosition(position);
	b.SetMaxChars(maxChars);
	b.SetDigitOnly(digitOnly);
	b.SetColor(theme.themeColor);
	b.SetBorderColor(theme.themeBorderColor);
	b.SetSize(maxChars*10, theme.themeSizeY);
	b.SetScale(theme.themeScale);
	b.SetTextColor(theme.themeTextColor);
	b.SetTextPosition(theme.themeTextPosition);
	b.SetBorderThickness(theme.themeBorderThickness);
	b.SetTextFont(theme.themeTextFont);
    textBoxVector.push_back(b);
}


//Removes a button from the button vector
void CGUISelectionGroup::RemoveButton()
{
	if(buttonVector.size()>0)
		buttonVector.pop_back();
	else
		cout<<"\nCGUIButton>> Error: buttonVector.size() <=0"<<endl;
}

//Display all the buttons
void CGUISelectionGroup::DisplaySelection()
{
	unsigned int btnIndexEnd= (unsigned int)buttonVector.size();
	unsigned int txtIndexEnd= btnIndexEnd + textBoxVector.size();
	unsigned int lstIndexEnd= txtIndexEnd + listBoxVector.size();
	unsigned int i=0;

	for(i=0; i<btnIndexEnd; ++i)							//selection display for buttonVector
	{
		if(i != currentMenuChoice || buttonHits <= 0)		//unselected button
			buttonVector[i].SetState(0);					
		else if(i == depressedMenuChoice)					//pressed button
			buttonVector[i].SetState(2);
		else
			buttonVector[i].SetState(1);					//mouse over state

		buttonVector[i].Display();
	}

	for(i=btnIndexEnd; i<txtIndexEnd; ++i)					//selection display for textBoxVector
	{
		if(i != currentMenuChoice || buttonHits <= 0)		//unselected button
			textBoxVector[i-btnIndexEnd].SetState(0);					
		else if(i == depressedMenuChoice)					//pressed button
			textBoxVector[i-btnIndexEnd].SetState(2);
		else
			textBoxVector[i-btnIndexEnd].SetState(1);		//mouse over state

		textBoxVector[i-btnIndexEnd].Display();
		textBoxVector[i-btnIndexEnd].SetInput();
	}

	bool activeListBox= false;
	for(i=txtIndexEnd; i<lstIndexEnd; ++i)					//selection display for listBoxVector
	{
		if(listBoxVector[i-txtIndexEnd].IsActive())
			activeListBox=true;								//set flag if any list boxes are active

		if(i != currentMenuChoice || buttonHits <= 0)		//unselected button
			listBoxVector[i-txtIndexEnd].SetState(0);	
		else if(i == depressedMenuChoice)					//pressed button
			listBoxVector[i-txtIndexEnd].SetState(2);	
		else
			listBoxVector[i-txtIndexEnd].SetState(1);		//mouse over state
		listBoxVector[i-txtIndexEnd].DisplayListBox();
		listBoxVector[i-txtIndexEnd].SetInput();
	}

	if(activeListBox)										//if there an active list box
		disableSelection=true;								//disable selection processing for entire group
	else													//otherwise
		disableSelection=false;								//enable selction processing for entire group
}

//sets the active flags for all text boxes false
void CGUISelectionGroup::DeselectAllTextBoxes()
{
	for(unsigned int j=0; j<textBoxVector.size(); j++)				//deselect all other text fields
		textBoxVector[j].SetActive(false);
}

void CGUISelectionGroup::DeselectAllListBoxes()
{
	for(unsigned int j=0; j<listBoxVector.size(); j++)				//deselect all other text fields
			listBoxVector[j].SetActive(false);
}

bool CGUISelectionGroup::IsSelectionDisabled()
{
	return disableSelection;
}

void CGUISelectionGroup::DisableSelection(bool ds)
{
	disableSelection=ds;
}

//mouse selection
void CGUISelectionGroup::MouseSelect()
{
	GLuint	buffer[32];										// Set Up A Selection Buffer
	GLint	viewport[4];									// The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>

	glGetIntegerv(GL_VIEWPORT, viewport);					// This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
	glSelectBuffer(32, buffer);							// Tell OpenGL To Use Our Array For Selection
	glRenderMode(GL_SELECT);								// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.

	glInitNames();											// Initializes The Name Stack

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLdouble x,y,w,h;										//Viewport coordinate 0,0 is bottom left
	x= (input->GetMouseX()) - (screen->screenPosX);
	y= (viewport[3])        - (input->GetMouseY()) +screen->screenPosY;
	w= 1.0f;
	h= 1.0f;

	gluPickMatrix(x,y,w,h, viewport);						// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.

	gluOrtho2D (0, screen->GetScreenX(), screen->GetScreenY(),0 );	//window position(0,0) is top left corner

	glMatrixMode(GL_MODELVIEW);				

	int btnIndexEnd= (int)buttonVector.size();
	int txtIndexEnd= btnIndexEnd+(int)textBoxVector.size();
	int lstIndexEnd= txtIndexEnd+(int)listBoxVector.size();
	int i=0;

	for(i=btnIndexEnd-1; i>=0; i--)		//render last drawn button first, so top most gets pushed on the stack first
	{
		glPushName(i);
			buttonVector[i].SelectionDisplay();
		glPopName();
	}

	for(i=txtIndexEnd-1; i>=btnIndexEnd; i--)
	{
		glPushName(i);
			textBoxVector[i-btnIndexEnd].SelectionDisplay();
		glPopName();
	}

	for(i=lstIndexEnd-1; i>=txtIndexEnd; i--)
	{
		glPushName(i);
			listBoxVector[i-txtIndexEnd].SelectionDisplay();
		glPopName();
	}

	glMatrixMode(GL_PROJECTION);							
	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);
	//glFlush();

	buttonHits=glRenderMode(GL_RENDER);							// Switch To Render Mode, Find Out How Many

	if (buttonHits > 0 && !disableSelection)
	{
		GLuint *ptr;
		ptr = (GLuint *) buffer;								//buffer[0]- number of names buffer[1]- first z buffer, buffer[2] - second z buffer, buffer[3..N]- objects pushed on
		SetChoice(*(ptr+3));									//go directly to the first stored buffer object
	} 
	else
	{
		SetChoice(-1);
		depressedMenuChoice= -1;
	}
}