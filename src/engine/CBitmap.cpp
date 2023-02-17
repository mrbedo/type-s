///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CBitmap.cpp
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
#include "CBitmap.h"
#include<gl/gl.h>
#include<gl/glaux.h>

using namespace std;

//constructor
CBitmap::CBitmap():bitmapOpacity(1.0f)
{
}

//destructor
CBitmap::~CBitmap()
{
	glDeleteTextures(1, texture);
}

//sets up the texture from the bmp
int CBitmap::LoadGLTexture(const char* bmpFileName)				// Load Bitmaps And Convert To Textures
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glLoadIdentity();

	memset(bmpImage,0,sizeof(void *)*1);           		// Set The Pointer To NULL

	
	if(!bmpFileName)										// Make Sure A Filename Was Given
	{
		bmpFileName="core/default.bmp";						// If Not Return NULL
	}

	FILE *file = fopen(bmpFileName,"r");						// Check To See If The File Exists
	if(file)												// Does The File Exist?
	{
		fclose(file);										// Close The Handle
		bmpImage[0]= auxDIBImageLoad(bmpFileName);					// Load The Bitmap And Return A Pointer
	}
	else
	{
		return false;
	}
	

	
	if(bmpImage[0])									// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	{
		glGenTextures(1, &texture[0]);						// Create The Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);			// Typical Texture Generation Using Data From The Bitmap
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 
					 bmpImage[0]->sizeX, 
					 bmpImage[0]->sizeY,
					 0,
					 GL_RGB,
					 GL_UNSIGNED_BYTE,
					 bmpImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glPopAttrib();
		glPopMatrix();

		if (bmpImage[0]->data)							// If Texture Image Exists
		{
			free(bmpImage[0]->data);					// Free The Texture Image Memory
		}
		free(bmpImage[0]);								// Free The Image Structure
		bmpImage[0]=0;
	}
	else
		return false;

	return true;											// Return The Status to see if image was loaded
}

//Build a polygon and map the texture to it
void CBitmap::DrawBMPTexture(const CVector3 &position, const CVector3 &scale) const
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		if(bitmapOpacity > 0)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
		glColor4f(1.0f, 1.0f, 1.0f, bitmapOpacity);
		
		glTranslatef(position.x, position.y, position.z);
		glScalef(scale.x, scale.y, scale.z);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	glPopMatrix();
	//glFlush();
}

//Build a polygon and map the texture to it
void CBitmap::DrawBMPImage(const CVector3 &position, const CVector3 &scale) const
{
	if(bmpImage[0])
	{
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
			if(bitmapOpacity > 0)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
			}
			else
			{
				glDisable(GL_BLEND);
			}
			glColor4f(1.0f, 1.0f, 1.0f, bitmapOpacity);
			
			glTranslatef(position.x, position.y, position.z);
			glScalef(scale.x, scale.y, scale.z);

			glDrawPixels(bmpImage[0]->sizeX, 
						 bmpImage[0]->sizeY, 
						 GL_RGB, 
						 GL_UNSIGNED_BYTE, 
						 bmpImage[0]->data);

		glPopAttrib();
		glPopMatrix();
		//glFlush();
	}
}

GLuint CBitmap::GetTexture() const
{
	return texture[0];
}

//sets the opaquness value for a button from 0-1, 0 being completely transparent
void CBitmap::SetBitmapOpacity(const float &opacity)
{
	bitmapOpacity=opacity;
}

//returns the opaqueness value
float CBitmap::GetBitmapOpacity() const
{
	return bitmapOpacity;
}
