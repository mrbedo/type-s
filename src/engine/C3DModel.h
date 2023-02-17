#ifndef C3DMODEL_H
#define C3DMODEL_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  C3DModel.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2005
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  Manages a complete 3D model loaded from a file and provides
//	functionality for rendering that model in various ways
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "C3DSFile.h"
#include "COctree.h"
#include "CTexture.h"
#include "CBitmap.h"

// Creates and manages 3D file formats and applies various optimization algorithms
class C3DModel
{
private:
	PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;
	PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;

	CBitmap			reflectionTexture;									// Texture which holds relfection
	CTexture		textureManager;

	C3DStructure	struct3D;
	C3DSFile		model3DS;
	COctree			octree;
	CDebug			octreeDebug;

public:
	C3DModel();
	//Constructor

	~C3DModel();
	//Destructor

	void Load3DSFile(const std::string &fileName, const std::string &texturePath);
	//Loads a .3ds file format

	void CreateOctree();
	//Initializes the octree for the environment

	void DrawOctree();
	//Draws the 3D model using the octree

	void DrawLODModel(float level);
	//Draws a 3D model in realtime based on level of detail

	void Draw3DModel();
	//Draws the 3D model as loaded from file

	void Draw3DModelWithEnviMap();
	//Draws the 3D model and applies an environment map texture
	//to make the model appear reflective

};
#endif