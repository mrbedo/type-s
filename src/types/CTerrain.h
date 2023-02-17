#ifndef CTERRAIN_H
#define CTERRAIN_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTerrain.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2003
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  This class will provide a wrapper for a 3d model used as a world envrionment
//
//	inherits CModel
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../engine/CModel.h"
#include "../engine/CVector3.h"
#include <vector>
#include <string>

class CTerrain: public CModel
{
private:
	int staticTerrainDL;
	C3DModel model;
	CVector3 terrainTranslation;
	CVector3 terrainScale;

public:

	std::vector<float>allTerrainVerteces;

	CTerrain();
	//Post- constructor initalizes member varaibles
	~CTerrain();
	//Post- destructor, cleans up 3D data
	int Init(const std::string& tsiFileName);
	//Pre-	A part object exists
	//Post- Initalizes part parameters
	void DrawTerrain();
	//Pre-  3d model exists
	//Post- Terrain is rendered to the screen
	void GenerateStaticModelDL();
	//Pre-  3d model exists
	//Post- Static portion stored in display list
};
#endif