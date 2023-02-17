#include "CGUIText.h"

using namespace std;


// Constructor. Initalizes currentFontType to GLUT_BITMAP_8_BY_13
CGUIText::CGUIText():currentFontType(GLUT_BITMAP_HELVETICA_10)
{}

CGUIText::CGUIText(const CGUIText& t)
{
	*this=t;
}

// Destructor.
CGUIText::~CGUIText()
{
}

// Sets the glut bitmap font type
//    fontType id corresponds the following way
//
//	  value      glut defined type
//		0 ....... GLUT_BITMAP_8_BY_13
//		1 ....... GLUT_BITMAP_9_BY_15
//		2 ....... GLUT_BITMAP_TIMES_ROMAN_10
//		3 ....... GLUT_BITMAP_TIMES_ROMAN_24
//		4 ....... GLUT_BITMAP_HELVETICA_10
//		5 ....... GLUT_BITMAP_HELVETICA_12
//		6 ....... GLUT_BITMAP_HELVETICA_18
void CGUIText::SetFont(int fontType)
{
	switch(fontType)
	{
	case 0:	currentFontType=GLUT_BITMAP_8_BY_13; break;
	case 1:	currentFontType=GLUT_BITMAP_9_BY_15; break;
	case 2:	currentFontType=GLUT_BITMAP_TIMES_ROMAN_10; break;
	case 3:	currentFontType=GLUT_BITMAP_TIMES_ROMAN_24; break;
	case 4:	currentFontType=GLUT_BITMAP_HELVETICA_10; break;
	case 5:	currentFontType=GLUT_BITMAP_HELVETICA_12; break;
	case 6:	currentFontType=GLUT_BITMAP_HELVETICA_18; break;
	default: currentFontType=GLUT_BITMAP_8_BY_13; break;
	}
}

// Prints a string at a specified x y position
void CGUIText::Print(float x, float y, const string& str) const
{
	glRasterPos2d(x, y);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(float x, float y, float z, const string& str) const
{
	glRasterPos3d(x, y, z);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(const CVector2& pt, const string& str) const
{
	glRasterPos2d(pt.x, pt.y);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(const CVector3& pt, const string& str) const
{
	glRasterPos3d(pt.x, pt.y, pt.z);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
//With Integer variables
void CGUIText::Print(float x, float y, string str, int num)
{
	char buf[10];
	itoa(num,buf,10);
	str+=buf;

	glRasterPos2d(x, y);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(float x, float y, float z, string str, int num)
{
	char buf[10];
	itoa(num,buf,10);
	str+=buf;

	glRasterPos3d(x, y, z);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(const CVector2& pt, string str, int num)
{
	char buf[10];
	itoa(num,buf,10);
	str+=buf;

	glRasterPos2d(pt.x, pt.y);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(const CVector3& pt, string str, int num)
{
	char buf[10];
	itoa(num,buf,10);
	str+=buf;

	glRasterPos3d(pt.x, pt.y, pt.z);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}

//With Floating point variables
void CGUIText::Print(float x, float y, string str, float num)
{
	char buf[20];
	gcvt(num,4,buf);
    str+= buf;

	glRasterPos2d(x, y);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(float x, float y, float z, string str, float num)
{
	char buf[20];
	gcvt(num,4,buf);
    str+= buf;

	glRasterPos3d(x, y, z);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(const CVector2& pt, string str, float num)
{
	char buf[20];
	gcvt(num,4,buf);
    str+= buf;

	glRasterPos2d(pt.x, pt.y);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}
void CGUIText::Print(const CVector3& pt, string str, float num)
{
	char buf[20];
	gcvt(num,4,buf);
    str+= buf;

	glRasterPos3d(pt.x, pt.y, pt.z);
	for(unsigned int i=0; i < str.length(); ++i) 
	{
		glutBitmapCharacter(currentFontType, str[i]);
	}
}

CGUIText& CGUIText::operator=(const CGUIText& t)
{
	currentFontType=t.currentFontType;
	return *this;
}