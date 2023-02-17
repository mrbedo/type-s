#ifndef _3DS_H
#define _3DS_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  C3DSFile.h
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
//  Loads an autodesk .3DS file type
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "C3DStructure.h"
// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.






// This class handles all of the loading code
class C3DSFile
{
private:




	/////FIX : make them enums





		//generic 3d model structure that stores 3d model information

	struct tIndices 	// Here is our structure for our 3DS indicies (since .3DS stores 4 unsigned shorts)
	{							
		unsigned short a, b, c, bVisible;		// This will hold point1, 2, and 3 index's into the vertex array plus a visible flag
	};

	struct tChunk	// This holds the chunk info
	{
		unsigned short int	ID;					// The chunk's ID		
		unsigned int		length;					// The length of the chunk
		unsigned int		bytesRead;					// The amount of bytes read within that chunk
	};

	//>------ Primary Chunk, at the beginning of each file
	#define PRIMARY       0x4D4D

	//>------ Main Chunks
	#define OBJECTINFO    0x3D3D				// This gives the version of the mesh and is found right before the material and object information
	#define VERSION       0x0002				// This gives the version of the .3ds file
	#define EDITKEYFRAME  0xB000				// This is the header for all of the key frame info

	//>------ sub defines of OBJECTINFO
	#define MATERIAL	  0xAFFF				// This stored the texture info
	#define OBJECT		  0x4000				// This stores the faces, vertices, etc...

	//>------ sub defines of MATERIAL
	#define MATNAME       0xA000				// This holds the material name
	#define MATDIFFUSE    0xA020				// This holds the color of the object/material
	#define MATMAP        0xA200				// This is a header for a new material
	#define MATMAPFILE    0xA300				// This holds the file name of the texture

	#define OBJECT_MESH   0x4100				// This lets us know that we are reading a new object

	//>------ sub defines of OBJECT_MESH
	#define OBJECT_VERTICES     0x4110			// The objects vertices
	#define OBJECT_FACES		0x4120			// The objects faces
	#define OBJECT_MATERIAL		0x4130			// This is found if the object has a material, either texture map or color
	#define OBJECT_UV			0x4140			// The UV texture coordinates

	#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))
	// This computes the magnitude of a normal.   (magnitude = sqrt(x^2 + y^2 + z^2)

	FILE *filePtr;
	
	// These are used through the loading process to hold the chunk information
	tChunk *currentChunk;
	tChunk *tempChunk;
	bool loadMaterialInfo;

	//t3DModel *pModel; 
	//unsigned int textureArray[100];	// This holds the texture info, referenced by an ID
	

	int GetString(char *);
	// This reads in a string and saves it in the char array passed in

	void ReadChunk(tChunk *);
	// This reads the next chunk

	void ProcessNextChunk(C3DStructure *s3D, tChunk *);
	// This function reads the main sections of the .3DS file, then dives deeper with recursion
	//
	// Below we check our chunk ID each time we read a new chunk.  Then, if
	// we want to extract the information from that chunk, we do so.
	// If we don't want a chunk, we just read past it.  
	//
	// Continue to read the sub chunks until we have reached the length.
	// After we read ANYTHING we add the bytes read to the chunk and then check
	// check against the length.

	void ProcessNextObjectChunk(C3DStructure *s3D, S3DObject *obj, tChunk *);
	// This reads the object chunks

	void ProcessNextMaterialChunk(C3DStructure *s3D, tChunk *);
	// This reads the material chunks

	void ReadColorChunk(SMaterialInfo *mat, tChunk *);
	// This reads the RGB value for the object's color

	void ReadVertices(S3DObject *obj, tChunk *);
	// This reads the objects vertices

	void ReadVertexIndices(S3DObject *obj, tChunk *);
	// This reads the objects face information

	void ReadUVCoordinates(S3DObject *obj, tChunk *);
	// This reads the texture coodinates of the object

	void ReadObjectMaterial(C3DStructure *s3D, S3DObject *obj, tChunk *);
	// This reads in the material name assigned to the object and sets the materialID
	
	void ComputeNormals(C3DStructure *s3D);
	// This computes the vertex normals for the object (used for lighting)

	CVector3 Cross(CVector3 &vVector1, CVector3 &vVector2);
	// This returns the cross product between 2 vectors

	float Magnitude(CVector3 &vNormal);
	// This returns the magnitude of a vector

	CVector3 Normalize(CVector3 &vVector);
	// This returns a normalized vector

	CVector3 DivideVectorByScaler(const CVector3 &vVector1, const float Scaler);
	// This divides a vector by a single number (scalar) and returns the result


public:
	C3DSFile();								// This inits the data members
	~C3DSFile();

	bool Import3DS(C3DStructure *s3D, std::string strFileName, bool loadTextures=true);
	// This is the function that you call to load the 3DS

	void CleanUp();
	// This frees memory and closes the file


};


#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
// 
// This file added the model structures to it, and added indicies to our t3DModel
// structure.  This is used for vertex arrays.  Nothing else was added to this file.
//
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//