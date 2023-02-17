//***********************************************************************//
//																		 //
//		- "Talk to me like I'm a 3 year old!" Programming Lessons -		 //
//                                                                       //
//		$Author:		DigiBen		digiben@gametutorials.com			 //
//																		 //
//		$Program:		Octree3	 										 //
//																		 //
//		$Description:	A working octree with a .3ds file format scene   //
//																		 //
//		$Date:			2/16/02											 //
//																		 //
//***********************************************************************//
//		Modified: Bedros Magardichian

#include "CFrustum.h"
#include "../OpenGLHeader.h"
#include <math.h>

///////////////////////////////// NORMALIZE PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This normalizes a plane (A side) from a given frustum.
/////
///////////////////////////////// NORMALIZE PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
// Here we calculate the magnitude of the normal to the plane (point A B C)
// Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
// To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
// Then we divide the plane's values by it's magnitude.
// This makes it easier to work with.
void NormalizePlane(float frustum[6][4], int side)
{
	float magnitude = (float)sqrt( frustum[side][0] * frustum[side][0] + 
								   frustum[side][1] * frustum[side][1] + 
								   frustum[side][2] * frustum[side][2] );

	frustum[side][0] /= magnitude;
	frustum[side][1] /= magnitude;
	frustum[side][2] /= magnitude;
	frustum[side][3] /= magnitude; 
}


///////////////////////////////// CALCULATE FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This extracts our frustum from the projection and modelview matrix.
/////
///////////////////////////////// CALCULATE FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
// glGetFloatv() is used to extract information about our OpenGL world.
// Below, we pass in GL_PROJECTION_MATRIX to abstract our projection matrix.
// It then stores the matrix into an array of [16].
// By passing in GL_MODELVIEW_MATRIX, we can abstract our model view matrix.
// This also stores it in an array of [16].
// Now that we have our modelview and projection matrix, if we combine these 2 matrices,
// it will give us our clipping planes.  To combine 2 matrices, we multiply them.
// Now we actually want to get the sides of the frustum.  To do this we take
// the clipping planes we received above and extract the sides from them.

// This will extract the RIGHT side of the frustum
// Now that we have a normal (A,B,C) and a distance (D) to the plane,
// we want to normalize that normal and distance.

// Extract sides
// Normalize the sides
void CFrustum::CalculateFrustum()
{    
	float   proj[16];								// This will hold our projection matrix
	float   modl[16];								// This will hold our modelview matrix
	float   clip[16];								// This will hold the clipping planes

	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
	

	m_Frustum[RIGHT][X] = clip[ 3] - clip[ 0];
	m_Frustum[RIGHT][Y] = clip[ 7] - clip[ 4];
	m_Frustum[RIGHT][Z] = clip[11] - clip[ 8];
	m_Frustum[RIGHT][D] = clip[15] - clip[12];
	NormalizePlane(m_Frustum, RIGHT);

	m_Frustum[LEFT][X] = clip[ 3] + clip[ 0];
	m_Frustum[LEFT][Y] = clip[ 7] + clip[ 4];
	m_Frustum[LEFT][Z] = clip[11] + clip[ 8];
	m_Frustum[LEFT][D] = clip[15] + clip[12];
	NormalizePlane(m_Frustum, LEFT);

	m_Frustum[BOTTOM][X] = clip[ 3] + clip[ 1];
	m_Frustum[BOTTOM][Y] = clip[ 7] + clip[ 5];
	m_Frustum[BOTTOM][Z] = clip[11] + clip[ 9];
	m_Frustum[BOTTOM][D] = clip[15] + clip[13];
	NormalizePlane(m_Frustum, BOTTOM);

	m_Frustum[TOP][X] = clip[ 3] - clip[ 1];
	m_Frustum[TOP][Y] = clip[ 7] - clip[ 5];
	m_Frustum[TOP][Z] = clip[11] - clip[ 9];
	m_Frustum[TOP][D] = clip[15] - clip[13];
	NormalizePlane(m_Frustum, TOP);

	m_Frustum[BACK][X] = clip[ 3] - clip[ 2];
	m_Frustum[BACK][Y] = clip[ 7] - clip[ 6];
	m_Frustum[BACK][Z] = clip[11] - clip[10];
	m_Frustum[BACK][D] = clip[15] - clip[14];
	NormalizePlane(m_Frustum, BACK);

	m_Frustum[FRONT][X] = clip[ 3] + clip[ 2];
	m_Frustum[FRONT][Y] = clip[ 7] + clip[ 6];
	m_Frustum[FRONT][Z] = clip[11] + clip[10];
	m_Frustum[FRONT][D] = clip[15] + clip[14];
	NormalizePlane(m_Frustum, FRONT);
}

// The code below will allow us to make checks within the frustum.  For example,
// if we want to see if a point, a sphere, or a cube lies inside of the frustum.
// Because all of our planes point INWARDS (The normals are all pointing inside the frustum)
// we then can assume that if a point is in FRONT of all of the planes, it's inside.

///////////////////////////////// POINT IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This determines if a point is inside of the frustum
/////
///////////////////////////////// POINT IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
// Go through all the sides of the frustum
// Calculate the plane equation and check if the point is behind a side of the frustum
// If The point was behind a side, so it ISN'T in the frustum, return false
// else The point was inside of the frustum (In front of ALL the sides of the frustum)
bool CFrustum::PointInFrustum( float x, float y, float z )
{
	for(int i = 0; i < 6; ++i )	
	{
		if(m_Frustum[i][X] * x + 
		   m_Frustum[i][Y] * y + 
		   m_Frustum[i][Z] * z + 
		   m_Frustum[i][D]  <= 0)
		   return false;			
	}
	return true;
}


///////////////////////////////// SPHERE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This determines if a sphere is inside of our frustum by it's center and radius.
/////
///////////////////////////////// SPHERE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
// Go through all the sides of the frustum
// If the center of the sphere is farther away from the plane than the radius
// if The distance was greater than the radius so the sphere is outside of the frustum
// else The sphere was inside of the frustum!

bool CFrustum::SphereInFrustum( float x, float y, float z, float radius )
{
	for(int i = 0; i < 6; ++i )	
	{
		if( m_Frustum[i][X] * x + 
			m_Frustum[i][Y] * y + 
			m_Frustum[i][Z] * z + 
			m_Frustum[i][D] <= -radius )
			return false;
	}
	return true;
}


///////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This determines if a cube is in or around our frustum by it's center and 1/2 it's length
/////
///////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
// Basically, what is going on is, that we are given the center of the cube,
// and half the length.  Think of it like a radius.  Then we checking each point
// in the cube and seeing if it is inside the frustum.  If a point is found in front
// of a side, then we skip to the next side.  If we get to a plane that does NOT have
// a point in front of it, then it will return false.

// *Note* - This will sometimes say that a cube is inside the frustum when it isn't.
// This happens when all the corners of the bounding box are not behind any one plane.
// This is rare and shouldn't effect the overall rendering speed.
// If get to end, it isn't in the frustum

bool CFrustum::CubeInFrustum( float x, float y, float z, float size )
{
	for(int i = 0; i < 6; ++i )
	{
		if( m_Frustum[i][X] * (x - size) + 
		    m_Frustum[i][Y] * (y - size) + 
		    m_Frustum[i][Z] * (z - size) + 
		    m_Frustum[i][D] > 0) continue;

		if( m_Frustum[i][X] * (x + size) + 
		    m_Frustum[i][Y] * (y - size) + 
		    m_Frustum[i][Z] * (z - size) + 
		    m_Frustum[i][D] > 0) continue;

		if( m_Frustum[i][X] * (x - size) + 
		    m_Frustum[i][Y] * (y + size) + 
		    m_Frustum[i][Z] * (z - size) + 
		    m_Frustum[i][D] > 0) continue;

		if( m_Frustum[i][X] * (x + size) + 
			m_Frustum[i][Y] * (y + size) + 
			m_Frustum[i][Z] * (z - size) + 
			m_Frustum[i][D] > 0) continue;

		if( m_Frustum[i][X] * (x - size) + 
			m_Frustum[i][Y] * (y - size) + 
			m_Frustum[i][Z] * (z + size) + 
			m_Frustum[i][D] > 0) continue;

		if( m_Frustum[i][X] * (x + size) + 
			m_Frustum[i][Y] * (y - size) + 
			m_Frustum[i][Z] * (z + size) + 
			m_Frustum[i][D] > 0) continue;

		if( m_Frustum[i][X] * (x - size) + 
			m_Frustum[i][Y] * (y + size) + 
			m_Frustum[i][Z] * (z + size) + 
			m_Frustum[i][D] > 0) continue;

		if( m_Frustum[i][X] * (x + size) + 
			m_Frustum[i][Y] * (y + size) + 
			m_Frustum[i][Z] * (z + size) + 
			m_Frustum[i][D] > 0) continue;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This code was taken directly from the frustum tutorial located at www.GameTutorials.com.
// Most of the large block of comments were taken out.  If you want to learn more about
// this frustum code, visit our site.  Though we don't use the PointInFrustum() or 
// SphereInFrustum() code I decided to leave it in, just so you don't have to paste
// it in from the frustum tutorial if you include this file in your application/game.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// www.GameTutorials.com
//
//


