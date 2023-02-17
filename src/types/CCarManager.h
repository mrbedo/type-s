#ifndef CCARMANAGER_H
#define CCARMANAGER_H

#include "CCar.h"
#include "../engine/CTSIFile.h"

//manages a global selection of cars so that the currently selected car or cars will
//be accessible through all scenes and can be loaded at any point
class CCarManager
{
private:
	CTSIFile      tsi;											// Text loading object

	std::vector<std::string> carTSIFileList;								// stores a list of car .tsi files to be loaded
	std::vector<CCar> carList;									    	// maximum cars and terrains loaded is 1000
	int	nCars;
	int carIndex;


	CCarManager();
	// Constructor

public:
	CCar *selectedCar;

	~CCarManager();
	// Desctor

	void CleanUp();

	static CCarManager* Instance();
	// Singleton initalizer

	bool InitCars();
	// Initializes the cars
	
	void SelectCarNext();
	// increments the current car in the car array

	void SelectCarPrev();
	// decrements the current car in the car array
};
#endif