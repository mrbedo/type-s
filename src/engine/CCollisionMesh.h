///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CCollisionMesh.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2005
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//	Creates a collision mesh out of a specified 3D Model type
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <vector>

class CCollisionMesh
{
public:
	unsigned int vertexCount;
	unsigned int indexCount;
	float* vertexArray;
	unsigned int* indexArray;


	CCollisionMesh();
	~CCollisionMesh();
	
	void Load(const char* fileName);
	//Loads a temporary 3D file into a temporary structure
	//Copies the vertex content of the structure into the verteces vector

	void Draw();
	float* GetRawVertices(int objectIndex);
	//Returns a pointer to a flat array of vertices for a particular
	//object index in the 3D model
};