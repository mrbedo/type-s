///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  C3DModel.cpp
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

#include "C3DModel.h"
#include <iostream>
#include "../OpenGLHeader.h"

using namespace std;

C3DModel::C3DModel()
{
	struct3D.numOfMaterials=0;
	struct3D.numOfObjects=0;
}

C3DModel::~C3DModel()
{
}

void C3DModel::Load3DSFile(const string &fileName, const string &texturePath)
{
//	cout<<"Creating .3DS model : "<<fileName<<"...";

	string textureFileName="";

	reflectionTexture.LoadGLTexture("models/cars/envRef.bmp");
	glActiveTextureARB	 = (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)	wglGetProcAddress("glMultiTexCoord2fARB");
	

	if(model3DS.Import3DS(&struct3D, fileName))				// Here we load the world from a .3ds file
	{
//		cout<<"\n-------MODEL INFO-------"<<endl;
//		cout<<"#materials: "<<struct3D.matVector.size()<<endl;
//		cout<<"#materials(v):"<<struct3D.numOfMaterials<<endl;
//		cout<<"#numOfObjects(v):"<<struct3D.numOfObjects<<endl;
//		cout<<"#numOfObjects:"<<struct3D.objVector.size()<<endl;
//		cout<<"#numberOfTextures:"<<struct3D.textureArray.size()<<endl;
////		cout<<"[ok] \nCreating Materials and Textures : "<<fileName<<endl;

		for(int i = 0; i < struct3D.numOfMaterials; ++i)
		{
			if(strlen(struct3D.matVector[i].strFile) > 0)
			{
				textureFileName=texturePath;
				textureFileName.append(struct3D.matVector[i].strFile);
				struct3D.textureArray.resize(i+1);
				textureManager.CreateTexture(struct3D.textureArray, (char*)textureFileName.c_str(), struct3D.textureArray.size());
			}
			struct3D.matVector[i].texureId = struct3D.textureArray.size();
		}

	}
	else
	{
		cout<<"[FAILED]  No file was loaded"<<endl;
	}
}

void C3DModel::CreateOctree()
{
//	cout<<"Creating Octree for 3DModel...";
	octree.GetSceneDimensions(&struct3D);

	int TotalTriangleCount = octree.GetSceneTriangleCount(&struct3D);
	
	octree.CreateNode(&struct3D, TotalTriangleCount, octree.GetCenter(), octree.GetWidth());
	octree.CreateDisplayList(&octree, &struct3D, octree.GetDisplayListID());
}

//draws 3d model using an OCTree optimization algorithm
void C3DModel::DrawOctree()
{
	octree.DrawOctree(&octree, &struct3D);
	//octreeDebug.RenderDebugLines();

}

void C3DModel::DrawLODModel(float level)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

	for(int i = 0; i < struct3D.numOfObjects; ++i)			// Since we know how many objects our model has, go through each of them.
	{
		if(struct3D.objVector.size() <= 0)					// Make sure we have valid objects just in case. (size() is in the vector class)
		 	return;	

		S3DObject *o = &struct3D.objVector[i];				// Get the current object that we are displaying

		if(o->bHasTexture) 									// Check to see if this object has a texture map, if so bind the texture to it.
		{
			glEnable(GL_TEXTURE_2D);						// Turn on texture mapping and turn off color
			glBindTexture(GL_TEXTURE_2D, struct3D.textureArray[o->materialID]);	// Bind the texture map to the object by it's materialID
		}
		else 
		{
			glDisable(GL_TEXTURE_2D);						// Turn off texture mapping and turn on color
		}

		glBegin(GL_TRIANGLES);	
		for(int j = 0; j < o->numOfFaces; ++j)
		{
			for(int k = 0; k < 3; ++k)						// Go through each corner of the triangle and draw it.
			{				
				int n = o->faces[j].triVertIndex[k];		// Get the index for each point of the face
				glNormal3f(o->normals[n].x, o->normals[n].y, o->normals[n].z);
				if(o->bHasTexture && o->texVerts) 							// If the object has a texture associated with it, give it a texture coordinate.
				{
					//if(o->texVerts)							// Make sure there was a UVW map applied to the object or else it won't have tex coords.
						glTexCoord2f(o->texVerts[n].x, o->texVerts[n].y);
				} 
				glVertex3f(o->verts[n].x, o->verts[n].y, o->verts[n].z);
			}		
		}
		glEnd();
		//glFlush();
	}
	glPopMatrix();
	glPopAttrib();

}

//draws the model by itself
void C3DModel::Draw3DModel()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

	for(int i = 0; i < struct3D.numOfObjects; ++i)			// Since we know how many objects our model has, go through each of them.
	{
		if(struct3D.objVector.size() <= 0)					// Make sure we have valid objects just in case. (size() is in the vector class)
		 	return;	

		S3DObject *o = &struct3D.objVector[i];				// Get the current object that we are displaying

		if(o->bHasTexture) 									// Check to see if this object has a texture map, if so bind the texture to it.
		{
			glEnable(GL_TEXTURE_2D);						// Turn on texture mapping and turn off color
			glBindTexture(GL_TEXTURE_2D, struct3D.textureArray[o->materialID]);	// Bind the texture map to the object by it's materialID
		}
		else 
			glDisable(GL_TEXTURE_2D);						// Turn off texture mapping and turn on color

		glBegin(GL_TRIANGLES);	
		for(int j = 0; j < o->numOfFaces; j++)
		{
			for(int k = 0; k < 3; k++)						// Go through each corner of the triangle and draw it.
			{				
				int n = o->faces[j].triVertIndex[k];		// Get the index for each point of the face
				
				glNormal3f(o->normals[n].x, o->normals[n].y, o->normals[n].z);
			
				if(o->bHasTexture) 							// If the object has a texture associated with it, give it a texture coordinate.
				{
					if(o->texVerts)							// Make sure there was a UVW map applied to the object or else it won't have tex coords.
						glTexCoord2f(o->texVerts[n].x, o->texVerts[n].y);
				} 
				glVertex3f(o->verts[n].x, o->verts[n].y, o->verts[n].z);
			}		
		}
		glEnd();
		//glFlush();
	}
	glPopMatrix();
	glPopAttrib();
}

void C3DModel::Draw3DModelWithEnviMap()
{
	//cout<<"REFLECTION!!!!!!"<<endl;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

	for(int i = 0; i < struct3D.numOfObjects; ++i)			// Since we know how many objects our model has, go through each of them.
	{
		if(struct3D.objVector.size() <= 0)return;			// Make sure we have valid objects just in case. (size() is in the vector class)
 
		S3DObject *pObject = &struct3D.objVector[i];		// Get the current object that we are displaying

		if(pObject->bHasTexture) 							// Check to see if this object has a texture map, if so bind the texture to it.
		{
			glEnable(GL_TEXTURE_2D);						// Turn on texture mapping and turn off color
			glActiveTextureARB(GL_TEXTURE1_ARB);			// 0'th ID for multitexture
			glBindTexture(GL_TEXTURE_2D, struct3D.textureArray[pObject->materialID]);	// Bind the texture map to the object by it's materialID
			
			glEnable(GL_TEXTURE_2D);						// Turn on texture mapping and turn off color
			glActiveTextureARB(GL_TEXTURE0_ARB);			// 1'th ID for multitexture
			
			
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glBindTexture(GL_TEXTURE_2D, reflectionTexture.GetTexture());	// Bind the texture map to the object by it's materialID
			
			//glBlendFunc(GL_DST_COLOR,GL_ZERO);
			//glColor4f(1.0f, 1.0f, 0.0f, 0.9f);
			//glBlendFunc(GL_ONE, GL_ONE);					// Copy Image 2 Color To The Screen
		
		}
		else 
		{
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_2D);						// Turn off texture mapping and turn on color
		}

		glBegin(GL_TRIANGLES);


		for(int j = 0; j < pObject->numOfFaces; ++j)
		{
			for(int whichVertex = 0; whichVertex < 3; ++whichVertex)		// Go through each corner of the triangle and draw it.
			{				
				int index = pObject->faces[j].triVertIndex[whichVertex];	// Get the index for each point of the face
				glNormal3f(pObject->normals[ index ].x, 
						   pObject->normals[ index ].y, 
						   pObject->normals[ index ].z);
			
				if(pObject->bHasTexture) 				// If the object has a texture associated with it, give it a texture coordinate.
				{
					if(pObject->texVerts)				// Make sure there was a UVW map applied to the object or else it won't have tex coords.
					{
						glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pObject->texVerts[ index ].x, pObject->texVerts[ index ].y);
						glMultiTexCoord2fARB(GL_TEXTURE0_ARB,pObject->texVerts[ index ].x, pObject->texVerts[ index ].y);
					}
				} 
				glVertex3f(pObject->verts[index].x,
						   pObject->verts[index].y,
						   pObject->verts[index].z);
			}		
		}
		glEnd();

		//glFlush();
	}
	glPopMatrix();
	glPopAttrib();
}