
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUIListBox.cpp
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
//  This class provides a text field box in which you can type into and get a sting
//	value out of it
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "CGUIListBox.h"

using namespace std;


//default constructor
CGUIListBox::CGUIListBox():
input(CInput::Instance()),
screen(CScreenManager::Instance()),
itemHits(0),
depressedItemChoice( -1),
currentItemChoice( -1),
listBoxOpened(false)
{
	buttonItems.clear();
	SetText("+");
}

//default destructor
CGUIListBox::~CGUIListBox()
{
//	cout<<"ListBox Destroyed!"<<endl;
}

//return the selected flag value
bool CGUIListBox::IsActive()
{
	return listBoxOpened;
}

//sets the selected flag
void CGUIListBox::SetActive(bool a)
{
	listBoxOpened=a;
}


void CGUIListBox::AddButton(string name)
{
	CGUIButton btn;
	btn.Init();
	btn.SetSize(width, height);
	btn.SetPosition(CVector3(position[state].x, position[state].y+((buttonItems.size()+1)*height), position[state].z));
	btn.SetText(name);
	btn.SetColor(color);
	btn.SetBorderColor(borderColor);
	btn.SetScale(scale);
	btn.SetTextColor(textColor);
	btn.SetTextPosition(textPosition);
	btn.SetBorderThickness(borderThickness);
	buttonItems.push_back(btn);
}

//Set a new menu choice
void CGUIListBox::SetItemChoice(int c)
{
	currentItemChoice=c;
}

//Make the selected menu choice depressed
void CGUIListBox::DepressItem(bool depress)
{
	if(!depress)
		depressedItemChoice=currentItemChoice;
	else
		depressedItemChoice= -1;

	for(unsigned int i=0; i<buttonItems.size(); ++i)							//selection display for buttonVector
	{
		if(i == depressedItemChoice)					//pressed button
			buttonItems[i].SetState(2);
	}
}

//Returns the current selected choice
int CGUIListBox::GetValue()
{
	if(currentItemChoice> -1)
		SetText(buttonItems[currentItemChoice].GetText());

	return currentItemChoice;
}

void CGUIListBox::DisplayListBox()
{
	Display();
	glPushMatrix();
		glTranslatef(position[state].x+width, position[state].y, position[state].z);
		glScalef(-1.0f, 1.0f, 1.0f);
		glColor3f(borderColor[2].x,borderColor[2].y ,borderColor[2].z);
		glBegin(GL_TRIANGLES);
			glVertex2d(12, 8);
			glVertex2d(7, 14);
			glVertex2d(2, 8);
		glEnd();
	glPopMatrix();

	if(listBoxOpened)
		for(unsigned int i=0; i<buttonItems.size(); ++i)
		{
			if(i != currentItemChoice || itemHits <= 0)		//unselected button
				buttonItems[i].SetState(0);					
			else if(i == depressedItemChoice)					//pressed button
				buttonItems[i].SetState(2);
			else
				buttonItems[i].SetState(1);					//mouse over state

			buttonItems[i].Display();
		}
}

//set the value of the string
void CGUIListBox::SetInput()
{
	if(listBoxOpened)
	{
		//cout<<"Hello...I'm an active listbox"<<endl;
		GLuint	buffer[32];										// Set Up A Selection Buffer
		GLint	viewport[4];									// The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
		glGetIntegerv(GL_VIEWPORT, viewport);					// This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
		glSelectBuffer(32, buffer);							// Tell OpenGL To Use Our Array For Selection
		glRenderMode(GL_SELECT);								// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
		glInitNames();											// Initializes The Name Stack
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
		gluPickMatrix(input->GetMouseX(), viewport[3]-input->GetMouseY(), 5.0f, 5.0f, viewport);
		gluOrtho2D (0, screen->GetScreenX(), screen->GetScreenY(), 0 );	//position(0,0) is top left corner
		glMatrixMode(GL_MODELVIEW);				

		for(int i= (int)buttonItems.size()-1; i>=0; i--)
		{
			glPushName(i);
				buttonItems[i].SelectionDisplay();
			glPopName();
		}

		glMatrixMode(GL_PROJECTION);							
		glPopMatrix();	
		glMatrixMode(GL_MODELVIEW);
		//glFlush();

		itemHits=glRenderMode(GL_RENDER);							// Switch To Render Mode, Find Out How Many

		if (itemHits > 0)
		{
			GLuint *ptr;
			ptr = (GLuint *) buffer;								//buffer[0]- number of names buffer[1]- first z buffer, buffer[2] - second z buffer, buffer[3..N]- objects pushed on
			SetItemChoice(*(ptr+3));									//go directly to the first stored buffer object
		} 
		else
		{
			SetItemChoice(-1);
			depressedItemChoice= -1;
		}
	}
}
