#include "CTexture.h"
#include"../OpenGLHeader.h"
//#include <iostream>

using namespace std;

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}

void CTexture::CreateTexture(vector<unsigned int> &textureArray, const char* strFileName, int textureID) const
{
	//cout<<"creating texture"<<endl;
	if(!strFileName)									// Return from the function if no file name was passed in
		return;

	AUX_RGBImageRec *pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	
	if(!pBitmap)									// If we can't load the file, quit!
		return;

	glGenTextures(1, &textureArray.back());	// Generate a texture with the associative texture ID stored in the array
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);	// This sets the alignment requirements for the start of each pixel row in memory.
	glBindTexture(GL_TEXTURE_2D, textureArray.back());	// Bind the texture to the texture arrays index and init the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);	// Build Mipmaps (builds different versions of the picture for distances - looks better)

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.  

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if(pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
       	free(pBitmap);									// Free the bitmap structure
	}
}

