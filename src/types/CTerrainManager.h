#ifndef CTERRAINMANAGER_H
#define CTERRAINMANAGER_H

#include "CTerrain.h"

//manages a global selection of terrains so that the currently selected terrain or terrains will
//be accessible through all scenes and can be loaded at any point
class CTerrainManager
{
private:
	int nTerrains;
	int terrainIndex;

	std::vector<std::string> terrainTSIFileList;
	CTerrain terrainList[10];									// used for memory initalization

	CTerrainManager();
	// Constructor

public:
	CTerrain *selectedTerrain;

	~CTerrainManager();
	// Desctor

	static CTerrainManager* Instance();
	// Singleton initalizer

	bool InitTerrains();
	// Initializes the terrains
	
};
#endif