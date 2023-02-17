#ifndef CBITMAP_H
#define CBITMAP_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CBitmap.h
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
#include<vector>
#include"CVector3.h"
#include"../OpenGLHeader.h"

class CBitmap
{
private:
	float bitmapOpacity;
	GLuint	texture[1];										// Storage For One Texture
	AUX_RGBImageRec *bmpImage[1];						// Create Storage Space For The Texture
public:

	CBitmap();
	//Pre-	no bitmap font exists
	//Post- constructor 
	~CBitmap();
	//Pre-
	//Post- destructor
	int LoadGLTexture(const char* bmpFileName);
	//Pre-
	//Post- loads a specific bitmap and sets up the openGL 
	//		texturing and loads the bitmap with LoadBMP
	void DrawBMPTexture(const CVector3 &position , const CVector3 &scale) const;
	// builds a polygon with the bmp texture and displays it
	// at the specified poisition with the specified scale with a 
	// specified opacity 0-1 being completely transparent, 1 being opaque

	void DrawBMPImage(const CVector3 &position, const CVector3 &scale) const;
	// uses glDrawPixel to draw the bmpImage data
	// this method is much faster than using a textured polygon
	// it should be used for staic 2d background images

	void SetBitmapOpacity(const float &opacity);
	//Pre-  button opacity exists
	//Post- value updated
	float GetBitmapOpacity() const;
	//Pre-  opacity exists
	//Post- returns the opacity value
	GLuint GetTexture() const;
	//Pre-  an array of textures exists
	//Post- returns that array
};
#endif