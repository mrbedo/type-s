///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CShadow.cpp
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

#include "CShadow.h"
#include "../OpenGLHeader.h"

//Constructor
CShadow::CShadow():shadowIntensity(1)
{
}

//Destructor
CShadow::~CShadow()
{
}

//Opens up necessary initialization for drawing shadows 
//after function call
void CShadow::ShadowProjection(const CVector3 &l, const CVector3 &e, const CVector3 &n)
{
	float d, c;

	// These are c and d (corresponding to the tutorial)
	d = n.x*l.x + n.y*l.y + n.z*l.z;
	c = e.x*n.x + e.y*n.y + e.z*n.z - d;

	// Create the matrix. OpenGL uses column by column
	// ordering

	mat[0]  = l.x*n.x+c; 
	mat[4]  = n.y*l.x; 
	mat[8]  = n.z*l.x; 
	mat[12] = -l.x*c-l.x*d;
	  
	mat[1]  = n.x*l.y;        
	mat[5]  = l.y*n.y+c;
	mat[9]  = n.z*l.y; 
	mat[13] = -l.y*c-l.y*d;
	  
	mat[2]  = n.x*l.z;        
	mat[6]  = n.y*l.z; 
	mat[10] = l.z*n.z+c; 
	mat[14] = -l.z*c-l.z*d;
	  
	mat[3]  = n.x;        
	mat[7]  = n.y; 
	mat[11] = n.z; 
	mat[15] = -d;
/*   

	float mat[16];
	for (int i=0; i<16; ++i) mat[i] = 0;
	mat[0]=1;
	mat[5]=1;
	mat[8]=-lightPos.x;
	mat[9]=-lightPos.y;
	mat[15]=1;
  
	glRotatef(-90,1,0,0);
	*/
	// Finally multiply the matrices together *plonk*
	glMultMatrixf(mat);
}