#ifndef CSHADOW_H
#define CSHADOW_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CShadow.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2001-2005
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  This class provides the rendering of shadows primitives for objects
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CVector3.h"

class CShadow
{
private:
	float shadowIntensity;									//intensity of the shadow
	float mat[16];								//shadow transformation matrix

public:
	CShadow();
	//Constructor

	~CShadow();
	//Destructor

	void ShadowProjection(const CVector3 &lightPos, const CVector3 &shadowPos, const CVector3 &shadowPlaneNormal);
	//Creates a shadow projection matrix based on the position of the
	//light source, the point where the shadow is located, and the normal
	//of the plane on which the shadow is projected on
	//then multiplies the openGL projection matrix by it

};
#endif