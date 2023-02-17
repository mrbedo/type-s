///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTerrain.cpp
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
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTerrain.h"
#include "../engine/CTSIFile.h"
#include <iostream>
using namespace std;
//constructor
CTerrain::CTerrain()
{
}

//destructor
CTerrain::~CTerrain()
{
//	model.Clean3DSModel();
}

int CTerrain::Init(const string& tsiFileName)
{
	CTSIFile tsi;
	cout<<"\nloading..."<<endl;

	vector<string> tsiPartList= tsi.GetTSIStringVector(tsiFileName,"PART_LIST","partList");

	string partName;
	char buffer[20];

	for(unsigned int i=0; i<tsiPartList.size(); ++i)
	{
		partName= "part";
		partName += itoa(i,buffer,10);
		cout<<endl<<partName<<endl;
	
		partVector[i].SetPropertyVector(tsi.GetTSIFloatVector(tsiFileName,"GEOMETRY", partName));
		partVector[i].InitPart(tsiPartList[i],tsi.GetTSIString(tsiFileName,"FILES","texturePath"));

	}
	numberOfParts=tsiPartList.size();
	cout<<"\n...Terrain loaded!"<<endl;
	return 0;
}

//renders the 3d data to the screen
void CTerrain::DrawTerrain()
{
	glCallList(staticTerrainDL);
	//glFlush();
}

//make the static portion of the terrain into a display list
void CTerrain::GenerateStaticModelDL()
{
	staticTerrainDL=glGenLists(1);
	glNewList(staticTerrainDL, GL_COMPILE);
	glPushMatrix();
		glTranslatef(terrainTranslation.x, 
					 terrainTranslation.y,
					 terrainTranslation.z);
		glScalef(terrainScale.x,
				 terrainScale.y,
				 terrainScale.z);

		model.Draw3DModel();
	glPopMatrix();
	glEndList();
}
