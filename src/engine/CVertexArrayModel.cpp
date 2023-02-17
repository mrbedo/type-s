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

// Include our associated .h file
#include "CVertexArrayModel.h"
#include <iostream>

using namespace std;

// This is the final version of our octree code.  It will allow you to take  
// a 3D model, regardless of the file format (as long as it conforms to the C3DStructure struct).
// The difference from the last 2 tutorials is that we store face indices, rather
// than vertices.  Check out the top of Main.cpp for a explanation of why we chose this way.
// 
// Here is a list of the important new functions added to our CVertexArrayModel class:
//
//	//This returns the number of polygons in our entire scene
//	int GetSceneTriangleCount(C3DStructure *pWorld);
//
//  // This adds the current object index to our object list
//	void AddObjectIndexToList(int index);
//
//	// This recursively creates a display list ID for every end node in the octree
//	void CreateDisplayList(CVertexArrayModel *pNode, C3DStructure *pRootWorld, int displayListOffset);
//
// Since we need to know the total triangle count for each node that we will
// be potentially splitting, GetSceneTriangleCount() was created to go through
// all of the objects in the model and add up their triangles to a total number.
// This number is then returned and used for the root node to pass in.
//
// Instead of going through every object in the model's object list when drawing
// each end node, we store a list of indices into the object list that are in the end node.  
// This way we don't do unnecessary looping if the triangles in that end node aren't in 
// those objects.  The AddObjectIndexToList() function adds the passed in index to our 
// index list, if it's not already there.
//
// To get a greater efficiency out of the drawing of our octree, display lists are used.
// CreateDisplayList() recursively goes through each node and creates a display list ID for each.
// This is only done once, then we can use the display ID to render that end node liquid fast.
//
// Keep in mind, that we do NOT split the triangles over the node's planes.  Instead, we
// are just storing the face indices for each object. You might wonder how this works, since 
// our C3DStructure structure has multiple objects, with multiple face index arrays.  Simple,
// we just create a pointer to a C3DStructure structure for every end node (m_pWorld).  Then,
// we only store the objects that are in that end node.  Of course, there is no need to
// store anything but the face indices, number of faces and objects.  We will then use
// the face indices stored in our m_pWorld pointer to pass into the original world model's
// structure to draw it.  Remember, we don't store all the objects from the original
// model's structure, just the ones that are in our node's dimensions.
//
// There is so much that is going on in this tutorial, that I suggest having a second
// window open to look at the Octree2 project, so you can contrast between what is changed
// and what isn't.  This was necessary even for me when creating this tutorial.  Also,
// I think this helps you to understand the new code if you understand what the simplified
// octree code was doing.  Follow along each function with the old code and the new code.
// The same concepts are being coded, but with full world data, not just vertices.
//
//


// This holds the current amount of subdivisions we are currently at.
// This is used to make sure we don't go over the max amount




//-------------------------------------------------------------------------\\ 



///////////////////////////////// OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	The CVertexArrayModel contstructor which calls our init function
/////
///////////////////////////////// OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// Notice that we got rid of our InitOctree() function and just stuck the
	// initialization code in our constructor.  This is because we no longer need
	// to create the octree in real-time.

	// Initialize our world data to NULL.  This stores all the object's
	// face indices that need to be drawn for this node.  

CVertexArrayModel::CVertexArrayModel():m_TriangleCount(0),m_pWorld(0)
{}


///////////////////////////////// ~OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	The CVertexArrayModel destructor which calls our destroy function
/////
///////////////////////////////// ~OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// Notice that we got rid of DestroyOctree() from that last tutorials.  Like
	// the InitOctree(), we didn't need them any more because we will not be able
	// to change the octree restrictions in real-time.

CVertexArrayModel::~CVertexArrayModel()
{
	if( m_pWorld )	
	{
		for(int i = 0; i < m_pWorld->numOfObjects; ++i)
		{
			if( m_pWorld->objVector[i].faces )
			{
				delete [] m_pWorld->objVector[i].faces;	
				m_pWorld->objVector[i].faces = 0;
			}		

			if( m_pWorld->objVector[i].indices )
			{
				delete [] m_pWorld->objVector[i].indices;	
				m_pWorld->objVector[i].indices = 0;
			}
		}
		delete m_pWorld;
		m_pWorld = NULL;
	}
}




//////////////////////////// GET SCENE TRIANGLE COUNT \\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the total number of polygons in our scene
/////
//////////////////////////// GET SCENE TRIANGLE COUNT \\\\\\\\\\\\\\\\\\\\\\\\\\*
	// This function is only called once, right before we create our first root node.
	// Basically, we just go through all of the objects in our scene and add up their triangles.


int CVertexArrayModel::GetSceneTriangleCount(C3DStructure *pWorld)
{
	int numberOfTriangles = 0;									// Initialize a variable to hold the total amount of polygons in the scene

	for(int i = 0; i < pWorld->numOfObjects; ++i)				// Go through all the objects and add up their polygon count
		numberOfTriangles += pWorld->objVector[i].numOfFaces;		// Increase the total polygon count

	return numberOfTriangles;
}





///////////////////////////////// CREATE NEW NODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This figures out the new node information and then passes it into CreateNode()
/////
///////////////////////////////// CREATE NEW NODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/*
void CVertexArrayModel::CreateNewNode(C3DStructure *pWorld, vector<tFaceList> pList, int triangleCount,
					  	    CVector3 vCenter, float width,			   int nodeID)
{
	// This function is used as our helper function to partition the world data
	// to pass into the subdivided nodes.  The same things go on as in the previous
	// tutorials, but it's dealing with more than just vertices.  We are given
	// the world data that needs to be partitioned, the list of faces that are in
	// the new node about to be created, the triangle count, the parent node's center
	// and width, along with the enum ID that tells us which new node is being created.
	//
	// The tFaceList structure stores a vector of booleans, which tell us if that face
	// index is in our end node (true) or not (false).  It also contains a integer
	// to tell us how many of those faces (triangles) are "true", or in other words, 
	// are in our node that is being created.  

	// Check if the first node found some triangles in it, if not we don't continue
	if(!triangleCount) return;
	
	// Here we create the temporary partitioned data model, which will contain
	// all the objects and triangles in this end node.
	C3DStructure *pTempWorld = new C3DStructure;

	// Intialize the temp model data and assign the object count to it
	memset(pTempWorld, 0, sizeof(C3DStructure));
	pTempWorld->numOfObjects = pWorld->numOfObjects;
	
	// Go through all of the objects in the current partition passed in
	for(int i = 0; i < pWorld->numOfObjects; ++i)
	{
		// Create a new object, initialize it, then add it to our temp partition
		S3DObject newObject;
		memset(&newObject, 0, sizeof(S3DObject));
		pTempWorld->objVector.push_back(newObject);

		// Assign the new node's face count, material ID, texture boolean and 
		// vertices to the new object.  Notice that it's not that obj's face
		// count, but the pList's.  Also, we are just assigning the pointer to the
		// vertices, not copying them.
		pTempWorld->objVector[i].numOfFaces  = pList[i].totalFaceCount;
		pTempWorld->objVector[i].materialID  = pWorld->objVector[i].materialID;
		pTempWorld->objVector[i].bHasTexture = pWorld->objVector[i].bHasTexture;
		pTempWorld->objVector[i].verts		 = pWorld->objVector[i].verts;

		// Allocate memory for the new face list
		pTempWorld->objVector[i].faces = new SFace[pTempWorld->objVector[i].numOfFaces];

		// Create a counter to count the current index of the new node vertices
		int index = 0;

		// Go through all of the current object's faces and only take the ones in this new node
		for(int j = 0; j < pWorld->objVector[i].numOfFaces; j++)
		{
			// If this current triangle is in the node, assign it's index to our new face list
			if(pList[i].pFaceList[j])	
			{
				pTempWorld->objVector[i].faces[index] = pWorld->objVector[i].faces[j];
				index++;
			}
		}
	}



	// Now comes the initialization of the node.  First we allocate memory for
	// our node and then get it's center point.  Depending on the nodeID, 
	// GetNewNodeCenter() knows which center point to pass back (TOP_LEFT_FRONT, etc..)

	// Allocate a new node for this octree
	m_pOctreeNodes[nodeID] = new CVertexArrayModel;

	// Get the new node's center point depending on the nodexIndex (which of the 8 subdivided cubes).
	CVector3 vNodeCenter = GetNewNodeCenter(vCenter, width, nodeID);
		
	// Below, before and after we recurse further down into the tree, we keep track
	// of the level of subdivision that we are in.  This way we can restrict it.

	// Increase the current level of subdivision
	g_CurrentSubdivision++;




	// This chance is just that we pass in the temp partitioned world for this node,
	// instead of passing in just straight vertices.

	m_pOctreeNodes[nodeID]->CreateNode(pTempWorld, triangleCount, vNodeCenter, width / 2);	// Recurse through this node and subdivide it if necessary

	g_CurrentSubdivision--;	// Decrease the current level of subdivision





	// To free the temporary partition, we just go through all of it's objects and
	// free the faces.  The rest of the dynamic data was just being pointed too and
	// does not to be deleted.  Finally, we delete the allocated pTempWorld.
	for(i = 0; i < pWorld->numOfObjects; ++i)
	{
		if(pTempWorld->objVector[i].faces)
			delete [] pTempWorld->objVector[i].faces;
	}
	delete pTempWorld;

}*/



//////////////////////////// ADD OBJECT INDEX TO LIST \\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This adds the index into the model's object list to our object index list
/////
//////////////////////////// ADD OBJECT INDEX TO LIST \\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CVertexArrayModel::AddObjectIndexToList(int index)
{
	// To eliminate the need to loop through all of the objects in the original
	// model, when drawing the end nodes, we create an instance of our C3DStructure
	// structure to hold only the objects that lie in the child node's 3D space.

	// Go through all of the objects in our face index list
	for(unsigned int i = 0; i < objectList.size(); ++i)
	{
		// If we already have this index stored in our object index list, don't add it.
		if(objectList[i] == index)
			return;
	}

	// Add this index to our object index list, which indexes into the root world object list
	objectList.push_back(index);
}


//////////////////////////// ASSIGN TRIANGLES TO NODE \\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This allocates memory for the face indices to assign to the current end node
/////
//////////////////////////// ASSIGN TRIANGLES TO NODE \\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// We take our pWorld partition and then copy it into our member variable
	// face list, m_pWorld.  This holds the face indices that need to be rendered.
	// Since we are using vertex arrays, we can't use the tFace structure for the
	// indices, so we need to create an array that has all the face indices in a row.
	// This will be stored in our indices array, which is of type unsigned int.
	// Remember, it must be unsigned int for vertex arrays to register it.

void CVertexArrayModel::AssignTrianglesToNode(C3DStructure *pWorld, int numberOfTriangles)
{
	m_TriangleCount = numberOfTriangles;					// Initialize the triangle count of this end node 

	m_pWorld = new C3DStructure;							// Create and init an instance of our model structure to store the face index information
	memset(m_pWorld, 0, sizeof(C3DStructure));
	m_pWorld->numOfObjects = pWorld->numOfObjects;			// Assign the number of objects to our face index list


	for(int i = 0; i < m_pWorld->numOfObjects; ++i)			// Go through all of the objects in the partition that was passed in
	{
		S3DObject *obj = &(pWorld->objVector[i]);			// Create a pointer to the current object
		S3DObject newObject;								// Create and init a new object to hold the face index information

		memset(&newObject, 0, sizeof(S3DObject));

		if(obj->numOfFaces)									// If this object has face information, add it's index to our object index list
			AddObjectIndexToList(i);

		m_pWorld->objVector.push_back(newObject);			// Add our new object to our face index list


		int numOfFaces = obj->numOfFaces;					// Store the number of faces in a local variable
		m_pWorld->objVector[i].numOfFaces = numOfFaces;		// Assign the number of faces to this current face list
		m_pWorld->objVector[i].faces = new SFace [numOfFaces];			// Allocate memory for the face indices.  Remember, we also have faces indices
		m_pWorld->objVector[i].indices = new UINT [numOfFaces * 3];		// in a row, indices, which can be used to pass in for vertex arrays.  

		memset(m_pWorld->objVector[i].indices, 0, sizeof(UINT) * numOfFaces * 3);		// Initialize the face indices for vertex arrays (are copied below
		memcpy(m_pWorld->objVector[i].faces, obj->faces, sizeof(SFace) * numOfFaces);		// Copy the faces from the partition passed in to our end nodes face index list


		// Since we are using vertex arrays, we want to create a array with all of the
		// faces in a row.  That way we can pass it into glDrawElements().  We do this below.

		for(int j = 0; j < numOfFaces * 3; j += 3)			// Go through all the faces and assign them in a row to our indices array
		{
			m_pWorld->objVector[i].indices[j]     = m_pWorld->objVector[i].faces[j / 3].triVertIndex[0];
			m_pWorld->objVector[i].indices[j + 1] = m_pWorld->objVector[i].faces[j / 3].triVertIndex[1];
			m_pWorld->objVector[i].indices[j + 2] = m_pWorld->objVector[i].faces[j / 3].triVertIndex[2];
		}

		// We can now free the faces list if we want since it isn't going to be used from here
		// on out.  If you do NOT want to use vertex arrays, don't free the faces, and get
		// rid of the loop up above to store the indices.

		delete [] m_pWorld->objVector[i].faces;
		m_pWorld->objVector[i].faces = NULL;
	}

	m_DisplayListID++; 										// Increase the amount of end nodes created (Nodes with vertices stored)

}




//////////////////////////////// DRAW OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function recurses through all the nodes and draws the end node's vertices
/////
//////////////////////////////// DRAW OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*


	// To draw our octree, all that needs to be done is call our display list ID.
	// First we want to check if the current node is even in our frustum.  If it is,
	// we make sure that the node isn't subdivided.  We only can draw the end nodes.



void CVertexArrayModel::DrawOctree(CVertexArrayModel *pNode, C3DStructure *pRootWorld)
{
	if(!pNode) return;	// Make sure a valid node was passed in, otherwise go back to the last node

//	g_TotalNodesDrawn++;								// Increase the amount of nodes in our viewing frustum (camera's view)
	if(!pNode->m_pWorld) return;						// Make sure we have valid data assigned to this node
	glCallList(pNode->m_DisplayListID);					// Call the list with our end node's display list ID
}




//////////////////////////////// CREATE DISPLAY LIST \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function recurses through all the nodes and creates a display list for them
/////
//////////////////////////////// CREATE DISPLAY LIST \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CVertexArrayModel::CreateDisplayList(CVertexArrayModel *pNode, C3DStructure *pRootWorld, int displayListOffset)
{
	displayListOffset=glGenLists(5);
	cout<<"displayListOffset="<<displayListOffset<<endl;
	// This function handles our rendering code in the beginning and assigns it all
	// to a display list.  This increases our rendering speed, as long as we don't flood
	// the pipeline with a TON of data.  Display lists can actually be to bloated or too small.
	// Like our DrawOctree() function, we need to find the end nodes by recursing down to them.
	// We only create a display list for the end nodes and ignore the rest.  The 
	// displayListOffset is used to add to the end nodes current display list ID, in case
	// we created some display lists before creating the octree.  Usually it is just 1 otherwise.

	// Make sure a valid node was passed in, otherwise go back to the last node
	if(!pNode) return;

	// Check if this node is subdivided. If so, then we need to recurse down to it's nodes
	if(!pNode->m_pWorld) return;		// Make sure we have valid data assigned to this node

	pNode->m_DisplayListID += displayListOffset;		// Add our display list offset to our current display list ID

	glNewList(pNode->m_DisplayListID,GL_COMPILE);		// Start the display list and assign it to the end nodes ID

	int counter = 0;		// Create a temp counter for our while loop below to store the objects drawn		
	int objectCount = pNode->objectList.size();		// Store the object count and material count in some local variables for optimization
	int materialCount = pRootWorld->matVector.size();

	while(counter < objectCount)		// Go through all of the objects that are in our end node
	{
		int i = pNode->objectList[counter];			// Get the first object index into our root world
		S3DObject *obj     = &pNode->m_pWorld->objVector[i];			// Store pointers to the current face list and the root object 
		S3DObject *pRootObject = &pRootWorld->objVector[i];			// that holds all the data (verts, texture coordinates, normals, etc..)

		if(pRootObject->bHasTexture) 			// Check to see if this object has a texture map, if so, bind the texture to it.
		{
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			glBindTexture(GL_TEXTURE_2D, pRootWorld->textureArray[pRootObject->materialID]);
		} 
		else 
		{
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		if(materialCount && pRootObject->materialID >= 0) 
		{
			BYTE *pColor = pRootWorld->matVector[pRootObject->materialID].color;
			glColor3ub(pColor[0], pColor[1], pColor[2]);
		}

		// Now we get to the more unknown stuff, vertex arrays.  If you haven't
		// dealt with vertex arrays yet, let me give you a brief run down on them.
		// Instead of doing loops to go through and pass in each of the vertices
		// of a model, we can just pass in the array vertices, then an array of
		// indices that MUST be an unsigned int, which gives the indices into
		// the vertex array.  That means that we can send the vertices to the video
		// card with one call to glDrawElements().  There are a bunch of other
		// functions for vertex arrays that do different things, but I am just going
		// to mention this one.  Since texture coordinates, normals and colors are also
		// associated with vertices, we are able to point OpenGL to these arrays before
		// we draw the geometry.  It uses the same indices that we pass to glDrawElements()
		// for each of these arrays.  Below, we point OpenGL to our texture coordinates,
		// vertex and normal arrays.  This is done with calls to glTexCoordPointer(), 
		// glVertexPointer() and glNormalPointer().
		//
		// Before using any of these functions, we need to enable their states.  This is
		// done with glEnableClientState().  You just pass in the ID of the type of array 
		// you are wanting OpenGL to look for.  If you don't have data in those arrays,
		// the program will most likely crash.
		//
		// If you don't want to use vertex arrays, you can just render the world like normal.
		// That is why I saved the pFace information, as well as the indices info.  This
		// way you can use what ever method you are comfortable with.  I tried both, and
		// by FAR the vertex arrays are incredibly faster.  You decide :)

		// Make sure we have texture coordinates to render
		if(pRootObject->texVerts) 
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);				// Turn on the texture coordinate state

			// Point OpenGL to our texture coordinate array.
			// We have them in a pair of 2, of type float and 0 bytes of stride between them.
			glTexCoordPointer(2, GL_FLOAT, 0, pRootObject->texVerts);
		}

		if(pRootObject->verts)							// Make sure we have vertices to render
		{
			glEnableClientState(GL_VERTEX_ARRAY);		// Turn on the vertex array state

			// Point OpenGL to our vertex array.  We have our vertices stored in
			// 3 floats, with 0 stride between them in bytes.
			glVertexPointer(3, GL_FLOAT, 0, pRootObject->verts);
		}

		if(pRootObject->normals)			// Make sure we have normals to render
		{
			glEnableClientState(GL_NORMAL_ARRAY);				// Turn on the normals state
			glNormalPointer(GL_FLOAT, 0, pRootObject->normals);				// Point OpenGL to our normals array.  We have our normals
															// stored as floats, with a stride of 0 between.

		}

		// Here we pass in the indices that need to be rendered.  We want to
		// render them in triangles, with numOfFaces * 3 for indice count,
		// and the indices are of type UINT (important).
		glDrawElements(GL_TRIANGLES,    obj->numOfFaces * 3, 
					   GL_UNSIGNED_INT, obj->indices);

		counter++;										// Increase the current object count rendered
	}
	glEndList();										// End the display list for this ID
}




/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Holy cow!  That was a ton of technical garbage to sift through.  This is probably
// one of the LEAST intuitive tutorials that I have created, but hey, there is a lot
// to creating octrees.  The basics are easy to understand, but implementing it with
// a model structure can be tricky to do.  If you find any blaring errors or obvious
// notable optimizations to this code, please let me know.  It's 4am as I am finishing
// this, so I could have left out something important :)  Let's go over a brief overview
// of what was changed.
//
// The most important thing that was change in this tutorial was that we stored face
// indices instead of vertices.  In order to do this, we had to create a helper structure
// called tFaceList to partition the nodes in CreateNode().  We also created a instance
// of the C3DStructure structure in our octree class.  This holds the face indices for
// each object that is in the end node.  Only the end nodes allocate an instance of
// this structure, and they do not store any other information besides the face indices.
// The rest of the data is ignored because it will be referenced from the original object
// when being drawn in CreateDisplayList().  Since we are using vertex arrays, we use the
// indices array of face indices to draw the world, verses the faces array.  If you
// don't want to use vertex arrays, go to the end AssignTrianglesToNode() and don't free
// this array, and forget about allocating the indices array.
//
// To create the display lists, after the octree has been creating, we just make a call
// to CreateDisplayList(), which recursively goes through all the end nodes and assigns
// then to a display list ID.  This way we can just call a list ID to draw an end node,
// and not have any loops.  This increases our frame rate drastically, especially accompanied
// with vertex arrays.  
//
// I hope this tutorial doesn't take such a huge leap from the previous that it doesn't
// offer much help.  Let me know if you have any questions, I would love to make anything
// clearer.  Please call me on things.  When creating this much code, I am bound to say
// something wrong or not explain something enough.  
//
// Good luck!
// 
//
// Ben Humphrey
// Game Programmer
// DigiBen@GameTutorials.com
// www.GameTutorials.com
//
// © 2001 GameTutorials
