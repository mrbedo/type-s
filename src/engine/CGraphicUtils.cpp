///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGraphicUtils.cpp
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
//  Provides general OpenGL based drawing utilities and functionalities
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "CGraphicUtils.h"
#include "../OpenGLHeader.h"
#include "../gui/CGUIText.h"
//destructor
CGraphicUtils::CGraphicUtils()
{
}

//constructor
CGraphicUtils::~CGraphicUtils()
{
}

//draws axis lines
void CGraphicUtils::DrawAxisLines(const int axisSize)
{
 	CGUIText t;													// Text object
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
        glLineWidth(2);
	    glBegin(GL_LINES);
		    glColor3f(1.0f, 0.0f, 0.0f);					//x axis marker
		    glVertex3f(0,0,0);
		    glVertex3f(axisSize,0,0);
		    glColor3f(0.0f, 1.0f, 0.0f);					//y axis marker
		    glVertex3f(0,0,0);
		    glVertex3f(0,axisSize,0);
		    glColor3f(0.0f, 0.0f, 1.0f);					//z axis marker
		    glVertex3f(0,0,0);
		    glVertex3f(0,0,axisSize);
	    glEnd();
	    glColor3f(1.0f, 0.0f, 0.0f);						//x axis marker
	    t.Print(axisSize,0.0f,0.0f,"x",0);
	    glColor3f(0.0f, 1.0f, 0.0f);						//y axis marker
	    t.Print(0.0f,axisSize,0.0f,"y",0);
	    glColor3f(0.0f, 0.0f, 1.0f);						//z axis marker
	    t.Print(0.0f,0.0f,axisSize,"z",0);
    glPopMatrix();
    glPopAttrib();
}

//Draws grid lines along the XZ Plane
void CGraphicUtils::DrawGridLines(const int gridSize)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
	int gS= gridSize>>1;
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glColor3f(0.4f, 0.4f, 0.4f);
		for(int i = -gS; i<=gS; i+=10)
		{
			if(i)
				glColor3f(0.4f, 0.4f, 0.4f);
			else
				glColor3f(0.6f, 0.6f, 0.6f);				//draw the axis line brighter

			glBegin(GL_LINES);
				glVertex3f(i, 0, gS);
				glVertex3f(i, 0, -gS);
				glVertex3f(-gS,0, i);
				glVertex3f( gS,0, i);
			glEnd();
			//glFlush();
		}
	glPopMatrix();
    glPopAttrib();
	glLineWidth(1);
}

//render bounding box
void CGraphicUtils::DrawBox(const float width, const float length, const float height)
{
	float bBox[3]={width/2, length/2, height/2};

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
		glDisable(GL_LIGHTING);
		//glLineWidth(3);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
		glColor4f(0.3f, 1.0f, 0.0f,0.5f);
		glBegin(GL_QUADS);
			glVertex3f( bBox[0], bBox[1],-bBox[2]);
			glVertex3f(-bBox[0], bBox[1],-bBox[2]);
			glVertex3f(-bBox[0], bBox[1], bBox[2]);
			glVertex3f( bBox[0], bBox[1], bBox[2]);

			//bottom of box
			glVertex3f( bBox[0],-bBox[1], bBox[2]);
			glVertex3f(-bBox[0],-bBox[1], bBox[2]);
			glVertex3f(-bBox[0],-bBox[1],-bBox[2]);
			glVertex3f( bBox[0],-bBox[1],-bBox[2]);

			glVertex3f( bBox[0], bBox[1], bBox[2]);
			glVertex3f(-bBox[0], bBox[1], bBox[2]);
			glVertex3f(-bBox[0],-bBox[1], bBox[2]);
			glVertex3f( bBox[0],-bBox[1], bBox[2]);

			glVertex3f( bBox[0],-bBox[1],-bBox[2]);
			glVertex3f(-bBox[0],-bBox[1],-bBox[2]);
			glVertex3f(-bBox[0], bBox[1],-bBox[2]);
			glVertex3f( bBox[0], bBox[1],-bBox[2]);

			glVertex3f(-bBox[0], bBox[1], bBox[2]);
			glVertex3f(-bBox[0], bBox[1],-bBox[2]);
			glVertex3f(-bBox[0],-bBox[1],-bBox[2]);
			glVertex3f(-bBox[0],-bBox[1], bBox[2]);

			glVertex3f( bBox[0], bBox[1],-bBox[2]);
			glVertex3f( bBox[0], bBox[1], bBox[2]);
			glVertex3f( bBox[0],-bBox[1], bBox[2]);
			glVertex3f( bBox[0],-bBox[1],-bBox[2]);
		glEnd();

		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPopAttrib();
	//glFlush();
}

//render bounding cylinder
void CGraphicUtils::DrawCylinder(const float radius, const float length)
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glLineWidth(3);
			glTranslatef(0.0f, 0.0f, -length/2);
			glColor3f(0.2f, 1.0f, 0.2f);							//light blue
			GLUquadricObj* quadObj = gluNewQuadric();
			gluQuadricDrawStyle (quadObj, GLU_FILL);
 			gluCylinder(quadObj, radius, radius, length, 20, 4);       
			gluDeleteQuadric(quadObj);
	glPopAttrib();
	glPopMatrix();
	//glFlush();
}

//render a simple quad
void CGraphicUtils::DrawRawQuad(const float width, const float length, const int axis)
{
	float w=width/2, l=length/2;

	switch(axis)
	{
	case 0: //X axis
			glBegin(GL_POLYGON);
			glVertex3f(0, -w, -l);
			glVertex3f(0, -w,  l);
			glVertex3f(0,  w,  l);
			glVertex3f(0,  w, -l);
			glEnd();
			break;
	case 1: //Y axis
			glBegin(GL_POLYGON);
			glVertex3f(-w, 0, -l);
			glVertex3f(-w, 0,  l);
			glVertex3f( w, 0,  l);
			glVertex3f( w, 0, -l);
			glEnd();
			break;
	case 2: //Z axis
			glBegin(GL_POLYGON);
			glVertex3f(-w, -l, 0);
			glVertex3f(-w,  l, 0);
			glVertex3f( w,  l, 0);
			glVertex3f( w, -l, 0);
			glEnd();
			break;
	}
}

//render a car shaped shadow polygon
void CGraphicUtils::DrawCarShadowPolygon(const float width, const float length, const int axis)
{
	float w=width/2, l=length/2;

	switch(axis)
	{
	case 0: //X axis
			glBegin(GL_POLYGON);
			glVertex3f(0,  w,  l);
			glVertex3f(0,  w, -l);
			glVertex3f(0, -w, -l);
			glVertex3f(0, -w,  l);
			glEnd();
			break;
	case 1: //Y axis
			glBegin(GL_POLYGON);
			
			glVertex3f( l-(0.02 * l),  0, -w+(0.55 * w));
			glVertex3f( l-(0.10 * l),  0, -w+(0.25 * w));
			glVertex3f( l-(0.20 * l),  0, -w+(0.10 * w));
			glVertex3f( l-(0.30 * l),  0, -w+(0.05 * w));

			glVertex3f( -l+(0.20 * l), 0,  -w);
			glVertex3f( -l+(0.10 * l), 0,  -w+(0.09 * w));
			glVertex3f( -l+(0.05 * l), 0,  -w+(0.20 * w));
			glVertex3f( -l+(0.02 * l), 0,  -w+(0.45 * w));
		
			glVertex3f( -l+(0.02 * l), 0, w-(0.45 * w));
			glVertex3f( -l+(0.05 * l), 0, w-(0.20 * w));
			glVertex3f( -l+(0.10 * l), 0, w-(0.09 * w));
			glVertex3f( -l+(0.20 * l), 0, w);

			glVertex3f( l-(0.30 * l),  0, w-(0.05 * w));
			glVertex3f( l-(0.20 * l),  0, w-(0.10 * w));
			glVertex3f( l-(0.10 * l),  0, w-(0.25 * w));
			glVertex3f( l-(0.02 * l),  0, w-(0.55 * w));
			
		
			glEnd();
			break;
	case 2: //Z axis
			glBegin(GL_POLYGON);
			glVertex3f( w-0.25,  l-0.02, 0);
			glVertex3f( w-0.15,  l-0.15, 0);
			glVertex3f( w-0.10,  l-0.20, 0);
			glVertex3f( w-0.05,  l-0.25, 0);
			
			glVertex3f( w-0.00,  -l+0.25, 0);
			glVertex3f( w-0.05,  -l+0.20, 0);
			glVertex3f( w-0.10,  -l+0.15, 0);
			glVertex3f( w-0.20,  -l+0.02, 0);

			glVertex3f( -w+0.20,  -l+0.02, 0);
			glVertex3f( -w+0.10,  -l+0.15, 0);
			glVertex3f( -w+0.05,  -l+0.20, 0);
			glVertex3f( -w+0.00,  -l+0.25, 0);

			glVertex3f( -w+0.05,  l-0.25, 0);
			glVertex3f( -w+0.10,  l-0.20, 0);
			glVertex3f( -w+0.15,  l-0.15, 0);
			glVertex3f( -w+0.25,  l-0.02, 0);

			glEnd();
			break;
	}
}