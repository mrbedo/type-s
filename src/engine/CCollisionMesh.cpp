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
#include "CCollisionMesh.h"
#include "C3DStructure.h"
#include "C3DSFile.h"
#include <iostream>

CCollisionMesh::CCollisionMesh()
{
}

CCollisionMesh::~CCollisionMesh()
{
	delete[] vertexArray;
}

void CCollisionMesh::Load(const char* fileName)
{
	C3DStructure *s= new C3DStructure();
	C3DSFile model3DS;
	std::cout<<"CollisionMesh::Load>> Loading model..."<<std::endl;
	model3DS.Import3DS(s, fileName, false);

	unsigned int i, j, objIndex;


	vertexCount=0;
	indexCount=0;
	
	for(objIndex=0; objIndex < s->numOfObjects; ++objIndex)
	{
		indexCount+= s->objVector[objIndex].numOfFaces;
	}
	indexCount*=3;
	std::cout<<"CollisionMesh::Load>> Allocating indices memory..."<<std::endl;
	indexArray= new unsigned int[indexCount];
	memset(indexArray,0,sizeof(unsigned int)*indexCount);

	std::cout<<"CollisionMesh::Load>> Allocating indices memory..."<<std::endl;
	std::cout<<"CollisionMesh::Load>> Copying indices..."<<std::endl;

	for(objIndex=0,	j=0; objIndex<s->numOfObjects; ++objIndex)
	{
		indexCount+= s->objVector[objIndex].numOfFaces*3;

		for(i=0; i<s->objVector[objIndex].numOfFaces; ++i, ++(++(++j))) //flatten vertex data into array
		{
				indexArray[j+0]=s->objVector[objIndex].faces[i].triVertIndex[0];
				indexArray[j+1]=s->objVector[objIndex].faces[i].triVertIndex[1];				
				indexArray[j+2]=s->objVector[objIndex].faces[i].triVertIndex[2];		
		}

		vertexCount+= s->objVector[objIndex].numOfVerts;
	}
	std::cout<<"Objects= "<<s->numOfObjects<<"..."<<std::endl;

	std::cout<<"CollisionMesh::Load>> Allocating vertex memory..."<<std::endl; 
	vertexCount*=3;
	vertexArray= new float[vertexCount];					//allocate memory for verteces
	memset(vertexArray,0,sizeof(float)*vertexCount);		//initalize array elements to 0

	std::cout<<"CollisionMesh::Load>> Extracting vertex data..."<<std::endl;

	for(objIndex=0,	j=0; objIndex< s->numOfObjects; ++objIndex)
	{
		for(i=0; i<s->objVector[objIndex].numOfVerts; ++i, ++(++(++j))) //flatten vertex data into array
		{
			vertexArray[j+0]=s->objVector[objIndex].verts[i].x;
			vertexArray[j+1]=s->objVector[objIndex].verts[i].y;
			vertexArray[j+2]=s->objVector[objIndex].verts[i].z;
		}
	}

	std::cout<<"CollisionMesh::Load>> Vertex Count: "<<vertexCount<<std::endl;
	std::cout<<"CollisionMesh::Load>> Index Count: "<<indexCount<<std::endl;
	delete s;

/*
(dTriMeshDataID g,
 const void* Vertices,
 int VertexStride, 
 int VertexCount,
 const void* Indices, 
 int IndexCount,
 int TriStride, 
 const void* Normals);
}*/
}

void CCollisionMesh::Draw()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor3f(1.0f, 1.0f, 0.0f);
	glPushMatrix();
		glBegin(GL_TRIANGLE_STRIP);

		for(unsigned int i=0; i<vertexCount; ++(++(++i)))
			glVertex3f(vertexArray[i],vertexArray[i+1],vertexArray[i+2]);
		glEnd();
	glPopMatrix();
	glPopAttrib();
}

