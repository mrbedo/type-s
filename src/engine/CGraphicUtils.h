///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGraphicUtils.h
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
#ifndef CGRAPHICUTILS_H
#define CGRAPHICUTILS_H

class CGraphicUtils
{
private:

public:
	CGraphicUtils();
	// default constructor

	~CGraphicUtils();
	// destructor

	void DrawAxisLines(const int axisSize);
	// axis size is passed in
	// draws a graphical indication of the XYZ coordinates in differnt colors
	
	void DrawGridLines(const int gridSize);
	// grid size is passed in
	// draws grid lines in increments of gridsize of size gridsize

	void DrawBox(const float width, const float length, const float height);
	// width length and height are passed in to draw a box with the center
	// in the middle of the box

	void DrawCylinder(const float radius, const float length);
	// draws a cylinder with a radius and length
	// using openGL quadric opbjects

	void DrawRawQuad(const float width, const float	length, const int axis);
	// draws a simple quad with a normal along the specified axis,
	//does not save any attributes

	void DrawCarShadowPolygon(const float width, const float	length, const int axis);
	// draws a flat car shaped polygon (for shadow)
};
#endif