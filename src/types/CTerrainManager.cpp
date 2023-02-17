#include "CTerrainManager.h"
#include "../engine/CTSIFile.h"
#include <iostream>

using namespace std;


//constructor
CTerrainManager::CTerrainManager()
{
	terrainIndex=0;
}

//destructor
CTerrainManager::~CTerrainManager()
{
}

//singleton initalizer
CTerrainManager* CTerrainManager::Instance()
{
	static CTerrainManager terrainManager;
	return &terrainManager;
}

//initalize the terrains
bool CTerrainManager::InitTerrains()
{														cout<<"\n\n--- Setting Up Terrain List ------------------";
	static bool created=false;

	if(!created)
	{
		CTSIFile tsi;
														cout<<"\n\n--- Setting Up Terrain List ------------------";
		terrainTSIFileList=tsi.GetTSIStringVector("models/terrainList.tsi","LOCATIONS_VECTOR","objectList");
															cout<<"\n\t+terrainList.size()="<<terrainTSIFileList.size()
																<<"\n\n------------------------- Initalizing Models ----------------------";
		if(terrainTSIFileList.size()<1)
		{
			cout<<"\n\n Empty Model List Found"<<endl;
			return false;
		}
		else
		{
			for(unsigned int i=0; i<terrainTSIFileList.size(); ++i)
			{												cout<<"\nReading TSI -->"<<terrainTSIFileList[i];
				terrainList[i].Init(terrainTSIFileList[i]);			cout<<"\n\n\t\t\t+terrainList["<<i<<"]: "<<tsi.GetTSIString(terrainTSIFileList[i],"FILES","objectName");
				if(!terrainList[i].InitModels())
					cout<<"ERROR >> No terrain loaded"<<endl;
			}		
															cout<<"\n\t+Successfully generated terrainList vector"															<<"\n\n------------------------- carList.InitModels() ----------------------";
			selectedTerrain= &terrainList[0];
		}
		created=true;
	}
	return true;
}