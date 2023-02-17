#ifndef C3DSTRUCTURE_H
#define C3DSTRUCTURE_H

#include "../OpenGLHeader.h"
#include "CVector3.h"
#include "CVector2.h"
#include <vector>
#include <iostream>

// This holds all the information for our model/scene. 


// face structure used for indexing into the vertex  and texture coordinate arrays. 
struct SFace
{
	int triVertIndex[3];								// indicies for the verts that make up this triangle
	int texCoordIndex[3];								// indicies for the tex coords to texture this face
};

struct SMaterialInfo
{
	char  texPath[255];
	char  strName[255];									// The texture name
	char  strFile[255];									// The texture file name (If this is set it's a texture map)
	BYTE  color[3];										// The color of the object (R, G, B)
	int   texureId;										// the texture ID
	float uTile;										// u tiling of texture  
	float vTile;										// v tiling of texture	
	float uOffset;										// u offset of texture	
	float vOffset;										// v offset of texture	
} ;

struct S3DObject 
{
	unsigned int *indices;									// The straight face indices in a row, used for vertex arrays
	CVector3  *verts;									// The object's vertices
	CVector3  *normals;									// The object's normals
	CVector2  *texVerts;								// The texture's UV coordinates
	SFace *faces;										// The faces information of the object
	int  numOfVerts;									// The number of verts in the model
	int  numOfFaces;									// The number of faces in the model
	int  numTexVertex;									// The number of texture coordinates
	int  materialID;									// The texture ID to use, which is the index into our texture array
	bool bHasTexture;									// This is TRUE if there is a texture map for this object
	char strName[255];									// The name of the object
};
	
class C3DStructure
{
public:
	~C3DStructure()
	{
		std::cout<<"\nDeleting Stuff......"<<objVector.size()<<std::endl;
		for(unsigned int i=0; i<objVector.size(); ++i)
		{
			delete[] objVector[i].verts;
			delete[] objVector[i].faces;
			delete[] objVector[i].indices;
			delete[] objVector[i].normals;
			delete[] objVector[i].texVerts;
		}
		objVector.clear();
		matVector.clear();
		textureArray.clear();
	}
	int numOfObjects;									// The number of objects in the model
	int numOfMaterials;									// The number of materials for the model
	std::vector<S3DObject>		objVector;					// The object list for our model	
	std::vector<SMaterialInfo>	matVector;					// The list of material information (Textures and colors)
	std::vector<unsigned int>	textureArray;
};
#endif