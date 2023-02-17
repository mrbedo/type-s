#include "CCarManager.h"
#include <iostream>
using namespace std;
//constructor
CCarManager::CCarManager():carIndex(0)
{
}

//destructor
CCarManager::~CCarManager()
{
	cout<<"yooooo"<<endl;
	carList.clear();
}

void CCarManager::CleanUp()
{
	cout<<"yourmom"<<endl;
	carList.clear();
}

//singleton initalizer
CCarManager* CCarManager::Instance()
{
	static CCarManager carManager;
	return &carManager;
}

//initalize the cars
bool CCarManager::InitCars()
{
	static bool created=false;

	if(!created)
	{
														cout<<"\n\n--- Setting Up Car List ------------------";
		carTSIFileList=tsi.GetTSIStringVector("models/carList.tsi","TSI_LOCATIONS_VECTOR","objectList");
														cout<<"\n\t+generating car list from carlist.tsi file..."
															<<"\n\t+carTSIFileList.size()="<<carTSIFileList.size()
															<<"\n\n------------------------- carArray.Init() ----------------------";
		if(carTSIFileList.size()<1)
		{
			selectedCar=0;
			return false;
		}
		else
		{
			nCars=carTSIFileList.size();
			carList.resize(nCars);
			for(unsigned int i=0; i<carList.size(); ++i)
			{											cout<<"\nReading TSI -->"<<carTSIFileList[i];
				carList[i].Init(carTSIFileList[i]);		cout<<"\n\n\t\t\t+carList["<<i<<"]: "<<carList[i].GetCarName();
				carList[i].InitModels();
			}		
														cout<<"\n\t+Successfully generated carList vector"															<<"\n\n------------------------- carList.InitModels() ----------------------";
			selectedCar= &carList[0];
		}
		created=true;
	}
	return true;
}

//selects the next car in the vector
void CCarManager::SelectCarNext()
{													
	++carIndex;											//if car speed is between +-0.1
	if(carIndex>=nCars-1)
	{
		carIndex=nCars-1;
	}
	selectedCar= &carList[carIndex];
	cout<<"\ncarList.size():"<<nCars;
	cout<<"\nSelectCarUp carIndex:"<<carIndex;
}

//selects the previous car in the vector
void CCarManager::SelectCarPrev()
{
	--carIndex;		
	if(carIndex<=0)
	{
		carIndex=0;
	}
	selectedCar= &carList[carIndex];
	cout<<"\nSelectCarDown carIndex:"<<carIndex;	
}
