#ifndef _OCTREE_H
#define _OCTREE_H

#include "C3DSFile.h"
#include "CFrustum.h"


// This is our debug lines class to view the octree visually
class CDebug 
{

public:

	// This adds a line to our list of debug lines
	void AddDebugLine(CVector3 vPoint1, CVector3 vPoint2);

	// This adds a rectangle with a given center, width, height and depth to our list
	void AddDebugRectangle(CVector3 vCenter, float width, float height, float depth);

	// This renders all of the lines
	void RenderDebugLines();		

	void Clear();					
	// This clears all of the debug lines

private:
	bool g_bLighting;									// Turn lighting on initially
	std::vector<CVector3> m_vLines;								// This is the vector list of all of our lines
};


// This is our octree class
class COctree
{
private:
	int g_MaxTriangles;									// The maximum amount of triangles per node
	int g_MaxSubdivisions;								// The maximum amount of subdivisions allow (Levels of subdivision)
	int g_EndNodeCount;									// The amount of end nodes created in the octree (That hold vertices)
	int g_TotalNodesDrawn;								// This stores the amount of nodes that are in the frustum
	int g_CurrentSubdivision;

	CDebug g_Debug;
	CFrustum g_Frustum;
	// This is used for our pLists in CreateNode() to help partition the world into
	// different nodes.
	struct tFaceList
	{
		std::vector<bool> pFaceList;								// This is a vector of booleans to store if the face index is in the nodes 3D Space
		int totalFaceCount;									// This stores the total face count that is in the nodes 3D space (how many "true"'s)
	};

	enum eOctreeNodes										// These values (0 - 7) store the index ID's for the octree node array (m_pOctreeNodes)
	{
		TOP_LEFT_FRONT,
		TOP_LEFT_BACK,
		TOP_RIGHT_BACK,
		TOP_RIGHT_FRONT,
		BOTTOM_LEFT_FRONT,
		BOTTOM_LEFT_BACK,
		BOTTOM_RIGHT_BACK,
		BOTTOM_RIGHT_FRONT
	};

	bool m_bSubDivided;										// This tells us if we have divided this node into more sub nodes
	float m_Width;											// This is the size of the cube for this current node
	int m_TriangleCount;									// This holds the amount of triangles stored in this node


	CVector3 m_vCenter;										// This is the center (X, Y, Z) point in this node
	C3DStructure *m_pWorld;									// This holds all the scene information (verts, normals, texture info, etc..) for this node
	std::vector<int>	objectList;									// This stores the indices into the original model's object list
	int m_DisplayListID;									// This holds the display list ID for the current node, which increases the rendering speed

	COctree *m_pOctreeNodes[8];								// These are the eight nodes branching down from this current node
	
public:

	COctree();
	~COctree();
	// The constructor and deconstructor

	CVector3 GetCenter() {	 return m_vCenter;	}
	// This returns the center of this node

	int GetTriangleCount()  {   return m_TriangleCount;	}
	// This returns the triangle count stored in this node

	float GetWidth() {	 return m_Width;	}
	// This returns the widht of this node (since it's a cube the height and depth are the same)

	bool IsSubDivided()  {   return m_bSubDivided;	}
	// This returns if this node is subdivided or not

	void GetSceneDimensions(C3DStructure *pWorld);
	// This sets the initial width, height and depth for the whole scene

	int GetSceneTriangleCount(C3DStructure *pWorld);
	// This returns the number of polygons in our entire scene

	int GetDisplayListID()		{   return m_DisplayListID;		}
	// This returns this nodes display list ID

	void SetDisplayListID(int displayListID)	{	m_DisplayListID = displayListID;  }
	// This sets the nodes display list ID

	void AddObjectIndexToList(int index);
	// This adds the object index to our end node's list of object indices

	CVector3 GetNewNodeCenter(CVector3 vCenter, float width, int nodeID);
	// This takes in the previous nodes center, width and which node ID that will be subdivided

	void CreateNode(C3DStructure *pWorld, int numberOfTriangles, CVector3 vCenter, float width);
	// This subdivides a node depending on the triangle count and node width

	void CreateNewNode(C3DStructure *pWorld,	std::vector<tFaceList> pList, int triangleCount,
				  	   CVector3 vCenter,	float width,			 int nodeID);
	// This cleans up the new subdivided node creation process, so our code isn't HUGE!

	void AssignTrianglesToNode(C3DStructure *pWorld, int numberOfTriangles);
	// Once we are finished subdividing we need to assign the face indices to the end node.

	void DrawOctree(COctree *pNode, C3DStructure *pRootWorld);
	// This goes through each of the nodes and then draws the end nodes vertices.
	// This function should be called by starting with the root node.  Now it uses display lists.

	void CreateDisplayList(COctree *pNode, C3DStructure *pRootWorld, int displayListOffset);
	// This recursively creates a display list ID for every end node in the octree



};



#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This file added the new tFaceList structure, along with a couple functions
// and member variables to the COctree class.  Most of the functions were changed
// to handle a t3DModel instead of just pure vertices.  Since we are using display
// lists, a bunch of display list data and functions were added.  
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// www.GameTutorials.com
//
//
