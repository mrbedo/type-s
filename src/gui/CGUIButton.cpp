///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUIButton.cpp
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
//  This class provides windows bitmap loading and displaying a bitmap rectangle
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CGUIButton.h"

using namespace std;

//Default constructor
CGUIButton::CGUIButton()
{
//	cout<<"Button Created!"<<endl;
}

CGUIButton::CGUIButton(const CGUIButton& b)
{
	*this=b;
	/*
	t=b.t;											//text object

	width=b.width;
	height=b.height;
	
	for(int i=0; i<3; ++i)
	{
		position[i]=		b.position[i];									//size of button
		scale[i]=			b.scale[i];										//scale of button
		color[i]=			b.color[i];										//color of actual button
		opacity[i]=			b.opacity[i];										//opacity of button
		borderColor[i]=		b.borderColor[i];
		borderThickness[i]=	b.borderThickness[i];
		text[i]=			b.text[i];
		label[i]=			b.label[i];
		textShadowColor[i]=	b.textShadowColor[i];
		textPosition[i]=	b.textPosition[i];
		textColor[i]=		b.textColor[i];
		textShadowColor[i]= b.textShadowColor[i];
		textPosition[i]=	b.textPosition[i];
		toolTip[i]=			b.toolTip[i];
		withTextShadow[i]=	b.withTextShadow[i];
		withImage[i]=		b.withImage[i];										//if the button is defined with bitmaps or just plain color
		withText[i]=		b.withText[i];										//if the button will have text in it
		withToolTip[i]=		b.withToolTip[i];
		withBorder[i]=		b.withBorder[i];
	}
	labelPosition= b.labelPosition;									 //label position
	state= b.state;
	withLabel= b.withLabel;
		*/
}
 
//Default destructor
CGUIButton::~CGUIButton()
{
//	cout<<"Button Destroyed!"<<endl;
}


//Initalized button
void CGUIButton::Init()
{
//	cout<<"Button Initalized!"<<endl;
	t.SetFont(4);

	CVector3 defColor[3]=		{CVector3( 0.4f, 0.4f, 0.4f),
	        					 CVector3( 0.6f, 0.6f, 0.6f),
								 CVector3( 0.2f, 0.2f, 0.2f)};

	CVector3 defBorderColor[3]=	{CVector3( 0.2f, 0.2f, 0.2f),
	        					 CVector3( 0.4f, 0.4f, 0.4f),
								 CVector3( 0.6f, 0.6f, 0.6f)};

	SetSize				(1.0f, 1.0f);
	SetPosition			(CVector3( 0.0f, 0.0f,-1.0f));
	SetScale			(CVector3( 0.06f, 0.03f, 0.03f));
	SetColor			(defColor);
	SetBorderColor		(defBorderColor);

	SetTextColor		(CVector3( 1.0f, 1.0f, 1.0f));
	SetTextPosition		(CVector3( 0.0f, 0.0f, 0.0f));

	SetTextShadowColor  (CVector3( 0.0f, 0.0f, 0.0f));

	state=0;												//button is up

	for(int i=0; i<3; ++i)
	{
		borderThickness[i]=3.0f;
		opacity[i]=0.0f;
		text[i]="";
		withTextShadow[i]=false;
		withImage[i]=false;										//no default image
		withText[i]=false;											//with no text
		withBorder[i]=true;
	}
}

//Cleans up initalized stuff
int CGUIButton::CleanUp()
{
//	cout<<"\n CGUIButtonCleanUp()...";
//	cout<<"[done]"<<endl;
	return true;
}

//set the size of the box
void CGUIButton::SetSize(float w, float h)
{
	width=w;
	height=h;
}

//Sets up button position
void CGUIButton::SetPosition(CVector3 *p)
{
	for(int i=0; i<3; ++i)
		position[i]=p[i];
}

//Sets up button position
void CGUIButton::SetPosition(const CVector3& p)
{
	for(int i=0; i<3; ++i)
		position[i]=p;
}

//Sets up button scale
void CGUIButton::SetScale(CVector3 *s)
{
	for(int i=0; i<3; ++i)
		scale[i]=s[i];
}

//Sets up button scale
void CGUIButton::SetScale(const CVector3& s)
{
	for(int i=0; i<3; ++i)
		scale[i]=s;
}

//Sets up the text that will go into the button

void CGUIButton::SetText(string *t)
{
	for(int i=0; i<3; ++i)
	{
		text[i]= t[i];
		withText[i]=true;
	}
}


//Sets up the text that will go into the button
void CGUIButton::SetText(string t)
{
	for(int i=0; i<3; ++i)
	{
		text[i]= t;
		withText[i]=true;
	}
}

string CGUIButton::GetText()
{
	return text[state];
}

//Sets up the text that will be the label for a button
void CGUIButton::SetLabel(const CVector3& position, string text)
{
	labelPosition=position;
	label= text;
	withLabel=true;
}

string CGUIButton::GetLabel()
{
	return label;
}

//Sets up the text that will go into the button
void CGUIButton::SetTextShadowColor(const CVector3& c)
{
	for(int i=0; i<3; ++i)
	{
		textShadowColor[i]=c;
	}
}

//Sets up the color of the button
void CGUIButton::SetColor(const CVector3 *c)
{
	for(int i=0; i<3; ++i)
		color[i]=c[i];
}

//Sets up the color of the button
void CGUIButton::SetColor(const CVector3& c)
{
	for(int i=0; i<3; ++i)
		color[i]=c;
}

//Sets up the text padding
void CGUIButton::SetTextPosition(CVector3 *p)
{
	for(int i=0; i<3; ++i)
		textPosition[i]=p[i];
}

//Sets up the text padding
void CGUIButton::SetTextPosition(const CVector3& p)
{
	for(int i=0; i<3; ++i)
		textPosition[i]=p;
}

//Sets up the border color of the button
void CGUIButton::SetBorderColor(CVector3 *c)
{
	for(int i=0; i<3; ++i)
		borderColor[i]=c[i];
}

//Sets up the border color of the button
void CGUIButton::SetBorderColor(const CVector3& c)
{
	for(int i=0; i<3; ++i)
		borderColor[i]=c;
}

//Sets the button opacity
void CGUIButton::SetBorderThickness(float *b)
{
	for(int i=0; i<3; ++i)
	{
		if(b[i]<1)
		{
			withBorder[i]=false;
		}
		else
		{
			withBorder[i]=true;
			borderThickness[i]=b[i];
		}
	}
}

//Sets the button opacity
void CGUIButton::SetBorderThickness(float b)
{
	if(b<1)
	{
		for(int i=0; i<3; ++i)
			withBorder[i]=false;
	}
	else
	{
		for(int i=0; i<3; ++i)
		{
			withBorder[i]=true;
			borderThickness[i]=b;
		}
	}
}

//Sets the button's text color
void CGUIButton::SetTextColor(CVector3 *c)
{
	for(int i=0; i<3; ++i)
		textColor[i]=c[i];
}

//Sets the button's text color
void CGUIButton::SetTextColor(const CVector3& c)
{
	for(int i=0; i<3; ++i)
		textColor[i]=c;
}

//Sets the button opacity
void CGUIButton::SetOpacity(float *o)
{
	for(int i=0; i<3; ++i)
		opacity[i]=o[i];
}

//Sets the button opacity
void CGUIButton::SetOpacity(float o)
{
	for(int i=0; i<3; ++i)
		opacity[i]=o;
}

//Sets the button's current state 0=up, 1=over, 2=down
void CGUIButton::SetState(short s)
{
	state=s;
}

//Return the buttons current state
short CGUIButton::GetState()
{
	return state;
}

//Draws a button rectangle
void CGUIButton::DrawRectangle()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glColor4f(color[state].x,
				  color[state].y, 
				  color[state].z,
				  opacity[state]);
		
				glBegin(GL_QUADS);								
					glVertex3f( 0.0f,   0.0f,   0.0f);
					glVertex3f( 0.0f,   height, 0.0f);
					glVertex3f( width,  height, 0.0f);
					glVertex3f( width,  0.0f,   0.0f);
				glEnd();
	glPopAttrib();
}

//Draws a button border
void CGUIButton::DrawBorder()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glLineWidth(borderThickness[state]);

		glColor4f(borderColor[state].x,
				  borderColor[state].y, 
				  borderColor[state].z,
				  opacity[state]);
					glBegin(GL_LINE_LOOP);								
						glVertex3f( 0.0f,   0.0f,   0.0f);
						glVertex3f( 0.0f,   height, 0.0f);
						glVertex3f( width,  height, 0.0f);
						glVertex3f( width,  0.0f,   0.0f);
					glEnd();
	glPopAttrib();
}

//Draws a button in its respective state
void CGUIButton::DisplayInState(short state)
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

		glTranslatef(position[state].x, position[state].y, position[state].z);
		glPushMatrix();											//render button
			glScalef(scale[state].x, scale[state].y, scale[state].z);
			DrawRectangle();
			if(withBorder[state])
				DrawBorder();
		glPopMatrix();
																//render button text, but do not scale with button
		if(withText[state])
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
//			cout<<"DisplayInState: 6"<<endl;
			//cout<<text[state]<<endl;
				glColor3f(textColor[state].x,
						  textColor[state].y,
						  textColor[state].z);
				t.Print(textPosition[state].x,
						textPosition[state].y,
						text[state]);
//		cout<<"DisplayInState: 7"<<endl;
			glPopMatrix();
			glPopAttrib();
		}

		if(withLabel)
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
				glColor3f(borderColor[state].x,
						  borderColor[state].y,
						  borderColor[state].z);
				t.Print(labelPosition.x,
						labelPosition.y,
						label);
			glPopMatrix();
			glPopAttrib();
		}	
	glPopAttrib();
	glPopMatrix();
	//	cout<<"DisplayInState: 8"<<endl;

}

//Displays the button based on its attributes
void CGUIButton::SelectionDisplay()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
		glTranslatef(position[state].x, position[state].y, position[state].z);
		glPushMatrix();											//render button
			glScalef(scale[state].x, scale[state].y, scale[state].z);
				glBegin(GL_QUADS);								
					glVertex3f( 0.0f,   0.0f,   0.0f);
					glVertex3f( 0.0f,   height, 0.0f);
					glVertex3f( width,  height, 0.0f);
					glVertex3f( width,  0.0f,   0.0f);
				glEnd();
		glPopMatrix();
	//		cout<<"SelectionDisplay: 7"<<endl;
	glPopMatrix();
	glPopAttrib();
}

//Displays the button based on its attributes
void CGUIButton::Display()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glPushMatrix();

	DisplayInState(state);		//button is down

	glPopMatrix();
	glEnable(GL_LIGHTING);
	glPopAttrib();
}

//Sets the font for the button texts
void CGUIButton::SetTextFont(int f)
{
	t.SetFont(f);
}

CGUIButton& CGUIButton::operator=(const CGUIButton &b)
{
	t=b.t;											//text object

	width=b.width;
	height=b.height;
	
	for(int i=0; i<3; ++i)
	{
		position[i]=		b.position[i];									//size of button
		scale[i]=			b.scale[i];										//scale of button
		color[i]=			b.color[i];										//color of actual button
		opacity[i]=			b.opacity[i];										//opacity of button
		borderColor[i]=		b.borderColor[i];
		borderThickness[i]=	b.borderThickness[i];
		text[i]=			b.text[i];
		label[i]=			b.label[i];
		textShadowColor[i]=	b.textShadowColor[i];
		textPosition[i]=	b.textPosition[i];
		textColor[i]=		b.textColor[i];
		textShadowColor[i]= b.textShadowColor[i];
		textPosition[i]=	b.textPosition[i];
		toolTip[i]=			b.toolTip[i];
		withTextShadow[i]=	b.withTextShadow[i];
		withImage[i]=		b.withImage[i];										//if the button is defined with bitmaps or just plain color
		withText[i]=		b.withText[i];										//if the button will have text in it
		withToolTip[i]=		b.withToolTip[i];
		withBorder[i]=		b.withBorder[i];
	}
	labelPosition= b.labelPosition;									 //label position
	state= b.state;
	withLabel= b.withLabel;
	return *this;
}
