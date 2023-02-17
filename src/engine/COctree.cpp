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
#include "COctree.h"
#include <iostream>
using namespace std;

// This is the final version of our octree code.  It will allow you to take  
// a 3D model, regardless of the file format (as long as it conforms to the C3DStructure struct).
// The difference from the last 2 tutorials is that we store face indices, rather
// than vertices.  Check out the top of Main.cpp for a explanation of why we chose this way.
// 
// Here is a list of the important new functions added to our COctree class:
//
//	//This returns the number of polygons in our entire scene
//	int GetSceneTriangleCount(C3DStructure *pWorld);
//
//  // This adds the current object index to our object list
//	void AddObjectIndexToList(int index);
//
//	// This recursively creates a display list ID for every end node in the octree
//	void CreateDisplayList(COctree *pNode, C3DStructure *pRootWorld, int displayListOffset);
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

///////////////////////////////// RENDER DEBUG LINES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This goes through all of the lines that we stored in our list and draws them
/////
///////////////////////////////// RENDER DEBUG LINES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CDebug::RenderDebugLines()
{
	glDisable(GL_LIGHTING);				

	glColor3ub(255, 255, 0.0f);
	glBegin(GL_LINES);					
		for(unsigned int i = 0; i < m_vLines.size(); ++i)						// Go through the whole list of lines stored in the vector m_vLines.
			glVertex3f(m_vLines[i].x, m_vLines[i].y, m_vLines[i].z);	// Pass in the current point to be rendered as part of a line
	glEnd();						

	if(g_bLighting) 
		glEnable(GL_LIGHTING);
}

///////////////////////////////// ADD DEBUG LINE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This adds a debug LINE to the stack of lines
/////
///////////////////////////////// ADD DEBUG LINE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CDebug::AddDebugLine(CVector3 vPoint1, CVector3 vPoint2)
{
	// Add the 2 points that make up the line into our line list.
	m_vLines.push_back(vPoint1);
	m_vLines.push_back(vPoint2);
}


///////////////////////////////// ADD DEBUG RECTANGLE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This adds a debug RECTANGLE to the stack of lines
/////
///////////////////////////////// ADD DEBUG RECTANGLE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CDebug::AddDebugRectangle(CVector3 vCenter, float width, float height, float depth)
{
	// So we can work with the code better, we divide the dimensions in half.
	// That way we can create the cube from the center outwards.
	width /= 2.0f;	height /= 2.0f;	depth /= 2.0f;

	// Below we create all the 8 points so it will be easier to input the lines
	// of the cube.  With the dimensions we calculate the points.
	CVector3 vTopLeftFront( vCenter.x - width, vCenter.y + height, vCenter.z + depth);
	CVector3 vTopLeftBack(  vCenter.x - width, vCenter.y + height, vCenter.z - depth);
	CVector3 vTopRightBack( vCenter.x + width, vCenter.y + height, vCenter.z - depth);
	CVector3 vTopRightFront(vCenter.x + width, vCenter.y + height, vCenter.z + depth);

	CVector3 vBottom_LeftFront( vCenter.x - width, vCenter.y - height, vCenter.z + depth);
	CVector3 vBottom_LeftBack(  vCenter.x - width, vCenter.y - height, vCenter.z - depth);
	CVector3 vBottomRightBack( vCenter.x + width, vCenter.y - height, vCenter.z - depth);
	CVector3 vBottomRightFront(vCenter.x + width, vCenter.y - height, vCenter.z + depth);

	////////// TOP LINES ////////// 

	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vTopRightFront);
	m_vLines.push_back(vTopLeftBack);  		m_vLines.push_back(vTopRightBack);
	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vTopLeftBack);
	m_vLines.push_back(vTopRightFront);		m_vLines.push_back(vTopRightBack);

	////////// BOTTOM LINES ////////// 

	m_vLines.push_back(vBottom_LeftFront);	m_vLines.push_back(vBottomRightFront);
	m_vLines.push_back(vBottom_LeftBack);	m_vLines.push_back(vBottomRightBack);
	m_vLines.push_back(vBottom_LeftFront);	m_vLines.push_back(vBottom_LeftBack);
	m_vLines.push_back(vBottomRightFront);	m_vLines.push_back(vBottomRightBack);

	////////// SIDE LINES ////////// 

	m_vLines.push_back(vTopLeftFront);		m_vLines.push_back(vBottom_LeftFront);
	m_vLines.push_back(vTopLeftBack);		m_vLines.push_back(vBottom_LeftBack);
	m_vLines.push_back(vTopRightBack);		m_vLines.push_back(vBottomRightBack);
	m_vLines.push_back(vTopRightFront);		m_vLines.push_back(vBottomRightFront);
}


///////////////////////////////// CLEAR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This clears all of the debug lines
/////
///////////////////////////////// CLEAR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CDebug::Clear()						
{
	// Destroy the list using the standard vector clear() function
	m_vLines.clear();
}



//-------------------------------------------------------------------------\\ 



///////////////////////////////// OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	The COctree contstructor which calls our init function
/////
///////////////////////////////// OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// Notice that we got rid of our InitOctree() function and just stuck the
	// initialization code in our constructor.  This is because we no longer need
	// to create the octree in real-time.

	// Initialize our world data to 0.  This stores all the object's
	// face indices that need to be drawn for this node.  

COctree::COctree():
m_bSubDivided(false),				// Set the subdivided flag to false
m_Width(0), 						// Set the dimensions of the box to false
m_TriangleCount(0),				// Initialize the triangle count
m_vCenter(CVector3(0, 0, 0)),		// Initialize the center of the box to the 0
m_pWorld(0)
{
	memset(m_pOctreeNodes, 0, sizeof(m_pOctreeNodes));		// Set the sub nodes to 0
}


///////////////////////////////// ~OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	The COctree destructor which calls our destroy function
/////
///////////////////////////////// ~OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// Notice that we got rid of DestroyOctree() from that last tutorials.  Like
	// the InitOctree(), we didn't need them any more because we will not be able
	// to change the octree restrictions in real-time.

COctree::~COctree()
{

	// If our chopped up world data exists, then we need to free it
		// Go through all the objects that were in our end node
			// Free the triangle data if it's not 0
			// Free the index data if it's not 0
		// Delete our end node face index data and set it to 0
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
		m_pWorld = 0;
	}


	// Go through all of the nodes and free them if they were allocated
		// Make sure this node is valid
			// Free this array index.  This will call the deconstructor which will
			// free the octree data correctly.  This allows us to forget about it's clean up
	for(int i = 0; i < 8; ++i)
	{
		if( m_pOctreeNodes[i] )
		{
			delete m_pOctreeNodes[i];
			m_pOctreeNodes[i] = 0;
		}
	}
}




//////////////////////////// GET SCENE TRIANGLE COUNT \\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the total number of polygons in our scene
/////
//////////////////////////// GET SCENE TRIANGLE COUNT \\\\\\\\\\\\\\\\\\\\\\\\\\*
	// This function is only called once, right before we create our first root node.
	// Basically, we just go through all of the objects in our scene and add up their triangles.


int COctree::GetSceneTriangleCount(C3DStructure *pWorld)
{
	int numberOfTriangles = 0;									// Initialize a variable to hold the total amount of polygons in the scene

	for(int i = 0; i < pWorld->numOfObjects; ++i)				// Go through all the objects and add up their polygon count
		numberOfTriangles += pWorld->objVector[i].numOfFaces;		// Increase the total polygon count

	return numberOfTriangles;
}


///////////////////////////////// OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This sets our initial width of the scene, as well as our center point
/////
///////////////////////////////// OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// This code is still doing the same things as in the previous tutorials,
	// except that we have to go through every object in the scene to find the
	// center point.

	// Go through all of the object's vertices and add them up to eventually find the center
		// Increase the total vertice count
		// Add the current object's vertices up
			// Add the current vertex to the center variable (Using operator overloading)

	// Divide the total by the number of vertices to get the center point.
	// We could have overloaded the / symbol but I chose not to because we rarely use it.

	// Now that we have the center point, we want to find the farthest distance from
	// our center point.  That will tell us how big the width of the first node is.
	// Once we get the farthest height, width and depth, we then check them against each
	// other.  Which ever one is higher, we then use that value for the cube width.

	// This code still does the same thing as in the previous octree tutorials,
	// except we need to go through each object in the scene to find the max dimensions.

	// Go through all of the scene's objects
		// Go through all of the current objects vertices
			// Get the distance in width, height and depth this vertex is from the center.
			// Check if the current width value is greater than the max width stored.
			// Check if the current height value is greater than the max height stored.
			// Check if the current depth value is greater than the max depth stored.

	// Set the member variable dimensions to the max ones found.
	// We multiply the max dimensions by 2 because this will give us the
	// full width, height and depth.  Otherwise, we just have half the size
	// because we are calculating from the center of the scene.

void COctree::GetSceneDimensions(C3DStructure *pWorld)
{
	if(!pWorld) 
		return;

	int numberOfVerts = 0;								// Initialize a variable to hold the total amount of vertices in the scene

	for(int i = 0; i < pWorld->numOfObjects; ++i)
	{
		numberOfVerts += pWorld->objVector[i].numOfVerts;
		for(int n = 0; n < pWorld->objVector[i].numOfVerts; n++)
			m_vCenter = m_vCenter + pWorld->objVector[i].verts[n];
	}

	m_vCenter.x /= numberOfVerts;
	m_vCenter.y /= numberOfVerts;	
	m_vCenter.z /= numberOfVerts;

	int   currentWidth = 0, currentHeight = 0, currentDepth = 0;
	float maxWidth = 0,		maxHeight = 0,	   maxDepth = 0;	// Initialize some temporary variables to hold the max dimensions found


	for(i = 0; i < pWorld->numOfObjects; ++i)
	{
		for(int j = 0; j < pWorld->objVector[i].numOfVerts; j++)
		{
			currentWidth  = abs(pWorld->objVector[i].verts[j].x - m_vCenter.x);	
			currentHeight = abs(pWorld->objVector[i].verts[j].y - m_vCenter.y);		
			currentDepth  = abs(pWorld->objVector[i].verts[j].z - m_vCenter.z);

			if(currentWidth  > maxWidth )	maxWidth  = currentWidth;
			if(currentHeight > maxHeight)	maxHeight = currentHeight;
			if(currentDepth  > maxDepth )	maxDepth  = currentDepth;
		}
	}

	maxWidth  *= 2;		
	maxHeight *= 2;		
	maxDepth  *= 2;

	// Check if the width is the highest value and assign that for the cube dimension
	if(maxWidth > maxHeight && maxWidth > maxDepth)
		m_Width = maxWidth;

	// Check if the height is the heighest value and assign that for the cube dimension
	else if(maxHeight > maxWidth && maxHeight > maxDepth)
		m_Width = maxHeight;

	// Else it must be the depth or it's the same value as some of the other ones
	else
		m_Width = maxDepth;
}


///////////////////////////////// GET NEW NODE CENTER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the center point of the new subdivided node, depending on the ID
/////
///////////////////////////////// GET NEW NODE CENTER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CVector3 COctree::GetNewNodeCenter(CVector3 vCenter, float width, int nodeID)
{
	// I created this function which takes an enum ID to see which node's center
	// we need to calculate.  Once we find that we need to subdivide a node we find
	// the centers of each of the 8 new nodes.  This is what that function does.
	// We just tell it which node we want.

	// Initialize the new node center
	CVector3 vNodeCenter(0, 0, 0);

	// Create a dummy variable to cut down the code size
	CVector3 vCtr = vCenter;

	// Switch on the ID to see which subdivided node we are finding the center
	switch(nodeID)							
	{
		case TOP_LEFT_FRONT:
			// Calculate the center of this new node
			vNodeCenter = CVector3(vCtr.x - width/4, vCtr.y + width/4, vCtr.z + width/4);
			break;

		case TOP_LEFT_BACK:
			// Calculate the center of this new node
			vNodeCenter = CVector3(vCtr.x - width/4, vCtr.y + width/4, vCtr.z - width/4);
			break;

		case TOP_RIGHT_BACK:
			// Calculate the center of this new node
			vNodeCenter = CVector3(vCtr.x + width/4, vCtr.y + width/4, vCtr.z - width/4);
			break;

		case TOP_RIGHT_FRONT:
			// Calculate the center of this new node
			vNodeCenter = CVector3(vCtr.x + width/4, vCtr.y + width/4, vCtr.z + width/4);
			break;

		case BOTTOM_LEFT_FRONT:
			// Calculate the center of this new node
			vNodeCenter = CVector3(vCtr.x - width/4, vCtr.y - width/4, vCtr.z + width/4);
			break;

		case BOTTOM_LEFT_BACK:
			// Calculate the center of this new node
			vNodeCenter = CVector3(vCtr.x - width/4, vCtr.y - width/4, vCtr.z - width/4);
			break;

		case BOTTOM_RIGHT_BACK:
			// Calculate the center of this new node
			vNodeCenter = CVector3(vCtr.x + width/4, vCtr.y - width/4, vCtr.z - width/4);
			break;

		case BOTTOM_RIGHT_FRONT:
			// Calculate the center of this new node
			vNodeCenter = CVector3(vCtr.x + width/4, vCtr.y - width/4, vCtr.z + width/4);
			break;
	}

	// Return the new node center
	return vNodeCenter;
}




///////////////////////////////// CREATE NEW NODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This figures out the new node information and then passes it into CreateNode()
/////
///////////////////////////////// CREATE NEW NODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void COctree::CreateNewNode(C3DStructure *pWorld, vector<tFaceList> pList, int triangleCount,
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
	m_pOctreeNodes[nodeID] = new COctree;

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

}


///////////////////////////////// CREATE NODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is our recursive function that goes through and subdivides our nodes
/////
///////////////////////////////// CREATE NODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// Initialize this node's center point.  Now we know the center of this node.
	// Initialize this nodes cube width.  Now we know the width of this current node.
	// Add the current node to our debug rectangle list so we can visualize it.

	// Check if we have too many triangles in this node and we haven't subdivided
	// above our max subdivisions.  If so, then we need to break this node into
	// 8 more nodes (hence the word OCTree).  Both must be true to divide this node.

	// Since we need to subdivide more we set the divided flag to true.

		// This function pretty much stays the same, except a small twist because
		// we are dealing with multiple objects for the scene, not just an array of vertices.
		// In the previous tutorials, we used a vector<> of booleans, but now we use our
		// tFaceList to store a vector of booleans for each object.

		// Create the list of tFaceLists for each child node
	
		// Create this variable to cut down the thickness of the code below (easier to read)
		// Go through every object in the current partition of the world

		// Store a point to the current object

			// Now, we have a face list for each object, for every child node.
			// We need to then check every triangle in this current object
			// to see if it's in any of the child nodes dimensions.  We store a "true" in
			// the face list index to tell us if that's the case.  This is then used
			// in CreateNewNode() to create a new partition of the world for that child node.

			// Resize the current face list to be the size of this object's face count

			// Go through all the triangles for this object

				// Check every vertice in the current triangle to see if it's inside a child node

					// Store the current vertex to be checked against all the child nodes
					// Check if the point lies within the TOP LEFT FRONT node
					// Check if the point lies within the TOP LEFT BACK node
					// Check if the point lies within the TOP RIGHT BACK node
					// Check if the point lies within the TOP RIGHT FRONT node
					// Check if the point lies within the BOTTOM LEFT FRONT node
					// Check if the point lies within the BOTTOM LEFT BACK node
					// Check if the point lies within the BOTTOM RIGHT BACK node
					// Check if the point lines within the BOTTOM RIGHT FRONT node
void COctree::CreateNode(C3DStructure *pWorld, int numberOfTriangles, CVector3 vCenter, float width)
{
	m_vCenter = vCenter;
	m_Width = width;
	g_Debug.AddDebugRectangle(vCenter, width, width, width);

	if( (numberOfTriangles > g_MaxTriangles) && (g_CurrentSubdivision < g_MaxSubdivisions) )
	{
		m_bSubDivided = true;
		vector<tFaceList> pList1(pWorld->numOfObjects);		// TOP_LEFT_FRONT node list
		vector<tFaceList> pList2(pWorld->numOfObjects);		// TOP_LEFT_BACK node list
		vector<tFaceList> pList3(pWorld->numOfObjects);		// TOP_RIGHT_BACK node list
		vector<tFaceList> pList4(pWorld->numOfObjects);		// TOP_RIGHT_FRONT node list
		vector<tFaceList> pList5(pWorld->numOfObjects);		// BOTTOM_LEFT_FRONT node list
		vector<tFaceList> pList6(pWorld->numOfObjects);		// BOTTOM_LEFT_BACK node list
		vector<tFaceList> pList7(pWorld->numOfObjects);		// BOTTOM_RIGHT_BACK node list
		vector<tFaceList> pList8(pWorld->numOfObjects);		// BOTTOM_RIGHT_FRONT node list
		CVector3 vCtr = vCenter;

		for(int i = 0; i < pWorld->numOfObjects; ++i)
		{
			S3DObject *obj = &(pWorld->objVector[i]);
			pList1[i].pFaceList.resize(obj->numOfFaces);
			pList2[i].pFaceList.resize(obj->numOfFaces);
			pList3[i].pFaceList.resize(obj->numOfFaces);
			pList4[i].pFaceList.resize(obj->numOfFaces);
			pList5[i].pFaceList.resize(obj->numOfFaces);
			pList6[i].pFaceList.resize(obj->numOfFaces);
			pList7[i].pFaceList.resize(obj->numOfFaces);
			pList8[i].pFaceList.resize(obj->numOfFaces);
			for(int j = 0; j < obj->numOfFaces; j++)
			{
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{

					CVector3 vPoint = obj->verts[obj->faces[j].triVertIndex[whichVertex]];

					if( (vPoint.x <= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z >= vCtr.z) ) 
						pList1[i].pFaceList[j] = true;
					if( (vPoint.x <= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z <= vCtr.z) ) 
						pList2[i].pFaceList[j] = true;
					if( (vPoint.x >= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z <= vCtr.z) ) 
						pList3[i].pFaceList[j] = true;
					if( (vPoint.x >= vCtr.x) && (vPoint.y >= vCtr.y) && (vPoint.z >= vCtr.z) ) 
						pList4[i].pFaceList[j] = true;
					if( (vPoint.x <= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z >= vCtr.z) ) 
						pList5[i].pFaceList[j] = true;
					if( (vPoint.x <= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z <= vCtr.z) ) 
						pList6[i].pFaceList[j] = true;
					if( (vPoint.x >= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z <= vCtr.z) ) 
						pList7[i].pFaceList[j] = true;
					if( (vPoint.x >= vCtr.x) && (vPoint.y <= vCtr.y) && (vPoint.z >= vCtr.z) ) 
						pList8[i].pFaceList[j] = true;
				}
			}	

			// Here we initialize the face count for each list that holds how many triangles
			// were found for each of the 8 subdivided nodes.
			pList1[i].totalFaceCount = 0;		
			pList2[i].totalFaceCount = 0;
			pList3[i].totalFaceCount = 0;		
			pList4[i].totalFaceCount = 0;
			pList5[i].totalFaceCount = 0;		
			pList6[i].totalFaceCount = 0;
			pList7[i].totalFaceCount = 0;		
			pList8[i].totalFaceCount = 0;
		}

		// Here we create a variable for each list that holds how many triangles
		// were found for each of the 8 subdivided nodes.
		int triCount1 = 0;	int triCount2 = 0;	int triCount3 = 0;	int triCount4 = 0;
		int triCount5 = 0;	int triCount6 = 0;	int triCount7 = 0;	int triCount8 = 0;
			
		// Go through all of the objects of this current partition
		for(i = 0; i < pWorld->numOfObjects; ++i)
		{
			// Go through all of the current objects triangles
			for(int j = 0; j < pWorld->objVector[i].numOfFaces; j++)
			{
				// Increase the triangle count for each node that has a "true" for the index i.
				// In other words, if the current triangle is in a child node, add 1 to the count.
				// We need to store the total triangle count for each object, but also
				// the total for the whole child node.  That is why we increase 2 variables.
				if(pList1[i].pFaceList[j])	{ pList1[i].totalFaceCount++; triCount1++; }
				if(pList2[i].pFaceList[j])	{ pList2[i].totalFaceCount++; triCount2++; }
				if(pList3[i].pFaceList[j])	{ pList3[i].totalFaceCount++; triCount3++; }
				if(pList4[i].pFaceList[j])	{ pList4[i].totalFaceCount++; triCount4++; }
				if(pList5[i].pFaceList[j])	{ pList5[i].totalFaceCount++; triCount5++; }
				if(pList6[i].pFaceList[j])	{ pList6[i].totalFaceCount++; triCount6++; }
				if(pList7[i].pFaceList[j])	{ pList7[i].totalFaceCount++; triCount7++; }
				if(pList8[i].pFaceList[j])	{ pList8[i].totalFaceCount++; triCount8++; }
			}
		}

		// Next we do the dirty work.  We need to set up the new nodes with the triangles
		// that are assigned to each node, along with the new center point of the node.
		// Through recursion we subdivide this node into 8 more potential nodes.

		// Create the subdivided nodes if necessary and then recurse through them.
		// The information passed into CreateNewNode() are essential for creating the
		// new nodes.  We pass the 8 ID's in so it knows how to calculate it's new center.
		CreateNewNode(pWorld, pList1, triCount1, vCenter, width, TOP_LEFT_FRONT);
		CreateNewNode(pWorld, pList2, triCount2, vCenter, width, TOP_LEFT_BACK);
		CreateNewNode(pWorld, pList3, triCount3, vCenter, width, TOP_RIGHT_BACK);
		CreateNewNode(pWorld, pList4, triCount4, vCenter, width, TOP_RIGHT_FRONT);
		CreateNewNode(pWorld, pList5, triCount5, vCenter, width, BOTTOM_LEFT_FRONT);
		CreateNewNode(pWorld, pList6, triCount6, vCenter, width, BOTTOM_LEFT_BACK);
		CreateNewNode(pWorld, pList7, triCount7, vCenter, width, BOTTOM_RIGHT_BACK);
		CreateNewNode(pWorld, pList8, triCount8, vCenter, width, BOTTOM_RIGHT_FRONT);
	}
	else
	{
		// If we get here we must either be subdivided past our max level, or our triangle
		// count went below the minimum amount of triangles so we need to store them.
		
		// We pass in the current partition of world data to be assigned to this end node
		AssignTrianglesToNode(pWorld, numberOfTriangles);
	}



}




//////////////////////////// ADD OBJECT INDEX TO LIST \\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This adds the index into the model's object list to our object index list
/////
//////////////////////////// ADD OBJECT INDEX TO LIST \\\\\\\\\\\\\\\\\\\\\\\\\\\*

void COctree::AddObjectIndexToList(int index)
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

void COctree::AssignTrianglesToNode(C3DStructure *pWorld, int numberOfTriangles)
{
	m_bSubDivided = false;									// Since we did not subdivide this node we want to set our flag to false
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
		m_pWorld->objVector[i].faces = 0;
	}

	m_DisplayListID = g_EndNodeCount;						// Assign the current display list ID to be the current end node count
	g_EndNodeCount++;										// Increase the amount of end nodes created (Nodes with vertices stored)

}




//////////////////////////////// DRAW OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function recurses through all the nodes and draws the end node's vertices
/////
//////////////////////////////// DRAW OCTREE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*


	// To draw our octree, all that needs to be done is call our display list ID.
	// First we want to check if the current node is even in our frustum.  If it is,
	// we make sure that the node isn't subdivided.  We only can draw the end nodes.



void COctree::DrawOctree(COctree *pNode, C3DStructure *pRootWorld)
{
	//g_Frustum.CalculateFrustum();						// Each frame we calculate the new frustum.  In reality you only need to calculate the frustum when we move the camera.

	if(!pNode) return;	// Make sure a valid node was passed in, otherwise go back to the last node

	/*
	// Check if the current node is in our frustum
	if(!g_Frustum.CubeInFrustum(pNode->m_vCenter.x, pNode->m_vCenter.y, 
								pNode->m_vCenter.z, pNode->m_Width / 2) )
	{
		return;
	}

	if(pNode->IsSubDivided())	// Check if this node is subdivided. If so, then we need to recurse and draw it's nodes
	{
		// Recurse to the bottom of these nodes and draw the end node's vertices
		// Like creating the octree, we need to recurse through each of the 8 nodes.
		DrawOctree(pNode->m_pOctreeNodes[TOP_LEFT_FRONT],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[TOP_LEFT_BACK],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[TOP_RIGHT_BACK],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[TOP_RIGHT_FRONT],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_LEFT_FRONT],	pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_LEFT_BACK],		pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_RIGHT_BACK],	pRootWorld);
		DrawOctree(pNode->m_pOctreeNodes[BOTTOM_RIGHT_FRONT],	pRootWorld);
	}
	else */
	{
		g_TotalNodesDrawn++;								// Increase the amount of nodes in our viewing frustum (camera's view)
		if(!pNode->m_pWorld) return;						// Make sure we have valid data assigned to this node
		glCallList(pNode->m_DisplayListID);					// Call the list with our end node's display list ID
	}
}




//////////////////////////////// CREATE DISPLAY LIST \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function recurses through all the nodes and creates a display list for them
/////
//////////////////////////////// CREATE DISPLAY LIST \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void COctree::CreateDisplayList(COctree *pNode, C3DStructure *pRootWorld, int displayListOffset)
{
	displayListOffset=glGenLists(g_EndNodeCount);
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
	if(pNode->IsSubDivided())
	{
		// Recurse down to each one of the children until we reach the end nodes
		CreateDisplayList(pNode->m_pOctreeNodes[TOP_LEFT_FRONT],	pRootWorld, displayListOffset);
		CreateDisplayList(pNode->m_pOctreeNodes[TOP_LEFT_BACK],		pRootWorld, displayListOffset);
		CreateDisplayList(pNode->m_pOctreeNodes[TOP_RIGHT_BACK],	pRootWorld, displayListOffset);
		CreateDisplayList(pNode->m_pOctreeNodes[TOP_RIGHT_FRONT],	pRootWorld, displayListOffset);
		CreateDisplayList(pNode->m_pOctreeNodes[BOTTOM_LEFT_FRONT],	pRootWorld, displayListOffset);
		CreateDisplayList(pNode->m_pOctreeNodes[BOTTOM_LEFT_BACK],	pRootWorld, displayListOffset);
		CreateDisplayList(pNode->m_pOctreeNodes[BOTTOM_RIGHT_BACK],	pRootWorld, displayListOffset);
		CreateDisplayList(pNode->m_pOctreeNodes[BOTTOM_RIGHT_FRONT],pRootWorld, displayListOffset);
	}
	else 
	{
		// Make sure we have valid data assigned to this node
		if(!pNode->m_pWorld) return;

		// Add our display list offset to our current display list ID
		pNode->m_DisplayListID += displayListOffset;

		// Start the display list and assign it to the end nodes ID
		glNewList(pNode->m_DisplayListID,GL_COMPILE);

		// Create a temp counter for our while loop below to store the objects drawn
		int counter = 0;
		
		// Store the object count and material count in some local variables for optimization
		int objectCount = pNode->objectList.size();
		int materialCount = pRootWorld->matVector.size();

		// Go through all of the objects that are in our end node
		while(counter < objectCount)
		{
			// Get the first object index into our root world
			int i = pNode->objectList[counter];

			// Store pointers to the current face list and the root object 
			// that holds all the data (verts, texture coordinates, normals, etc..)
			S3DObject *obj     = &pNode->m_pWorld->objVector[i];
			S3DObject *pRootObject = &pRootWorld->objVector[i];

			// Check to see if this object has a texture map, if so, bind the texture to it.
			if(pRootObject->bHasTexture) 
			{
				// Turn on texture mapping and turn off color
				glEnable(GL_TEXTURE_2D);

				// Reset the color to normal again
				glColor3ub(255, 255, 255);

				// Bind the texture map to the object by it's materialID
				glBindTexture(GL_TEXTURE_2D, pRootWorld->textureArray[pRootObject->materialID]);
			} 
			else 
			{
				// Turn off texture mapping and turn on color
				glDisable(GL_TEXTURE_2D);


//				cout<<"No texture"<<endl;
				// Reset the color to normal again
				glColor3ub(255, 255, 255);
			}

			// Check to see if there is a valid material assigned to this object
			if(materialCount && pRootObject->materialID >= 0) 
			{
				// Get and set the color that the object is, since it must not have a texture
				BYTE *pColor = pRootWorld->matVector[pRootObject->materialID].color;

				// Assign the current color to this model
//				cout<<"color="<<pColor[0]<<" "<< pColor[1]<<" "<< pColor[2]<<endl;
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
				// Turn on the texture coordinate state
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				// Point OpenGL to our texture coordinate array.
				// We have them in a pair of 2, of type float and 0 bytes of stride between them.
				glTexCoordPointer(2, GL_FLOAT, 0, pRootObject->texVerts);
			}

			// Make sure we have vertices to render
			if(pRootObject->verts)
			{
				// Turn on the vertex array state
				glEnableClientState(GL_VERTEX_ARRAY);

				// Point OpenGL to our vertex array.  We have our vertices stored in
				// 3 floats, with 0 stride between them in bytes.
				glVertexPointer(3, GL_FLOAT, 0, pRootObject->verts);
			}

			// Make sure we have normals to render
			if(pRootObject->normals)
			{
				// Turn on the normals state
				glEnableClientState(GL_NORMAL_ARRAY);

				// Point OpenGL to our normals array.  We have our normals
				// stored as floats, with a stride of 0 between.
				glNormalPointer(GL_FLOAT, 0, pRootObject->normals);
			}

			// Here we pass in the indices that need to be rendered.  We want to
			// render them in triangles, with numOfFaces * 3 for indice count,
			// and the indices are of type UINT (important).
			glDrawElements(GL_TRIANGLES,    obj->numOfFaces * 3, 
						   GL_UNSIGNED_INT, obj->indices);

			// Increase the current object count rendered
			counter++;
		}

		// End the display list for this ID
		glEndList();
	}
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
