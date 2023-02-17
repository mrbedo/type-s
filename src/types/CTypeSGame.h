#ifndef CTYPESGAME_H
#define CTYPESGAME_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTypeSGame.h
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
//  The main game class
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTypeS.h"
#include "CCarManager.h"
#include "CTerrainManager.h"
#include "../engine/CPhysics.h"
#include "../engine/CCollisionMesh.h"
#include "../engine/CMatrix.h"
#include "../engine/CShadow.h"

class CTypeSGame: public CTypeS
{
public:

	#define TEMP_BODY_OFFSET 1.5

	CCarManager		*carMan;
	CTerrainManager *terrainMan;
	CCar			*currentCar;
	CTerrain		*currentTerrain;

	// dynamics and collision objects (chassis, 3 wheels, environment)
	float LENGTH;	// chassis length
	float WIDTH;	// chassis width
	float HEIGHT;	// chassis height
	float RADIUS;	// wheel radius
	float STARTY;	// starting height of chassis
	float CMASS;	// chassis mass
	float WMASS;	// wheel mass
	float tireWidth;//width of the wheels
	float groundFriction;
	float wheelRateLimit;

	//suspension
	float fSpringSoft; //softness of front suspension
	float rSpringSoft; //softness of rear suspsension

	bool handbrake;



	dMass carMass;
	dMass wheelMass;

	dWorldID world;
	dSpaceID space;
	dBodyID body[5];
	dJointID joint[4];	// joint[0] is the front wheel
	dJointGroupID contactgroup;
	dGeomID ground;
	dSpaceID car_space;
	dGeomID box[1];
	dGeomID sphere[4];
	dGeomID groundBox[20];
	dGeomID groundCyl[20];
	dGeomID boundingWall[6];
	dGeomID terrainDim[5]; //4 walls one ceiling
	dGeomID triMesh;
	dTriMeshDataID triData;
	CCollisionMesh collisionMesh;

	CVector3 gTerrainSize;
	CVector3 gBoxSize;
	CVector3 gCylSize;
	CVector3 gBoundingWallSize;

	int numOfGroundBlocks;

	GLfloat mat[16];									// translation matrix
	CShadow shadow;
	CMatrix matrix;
	CGraphicUtils gfxUtils;


private:
	bool created;


	
	bool clockReset;											//determines if clock is reset
																//if true= quake style, false= incar vievw
	bool quit;													//holds value which singals termination of program

public:
	CTypeSGame(){}
	// default constructor
	
	virtual ~CTypeSGame(){}
	// default destructor
	
	virtual int Init();
	// data members exist uninitalized
	// 3DS model is initalized
	
	virtual void InitMenu();
	// initalization in progress
	// main menu is initalized
	
	virtual void MakeMenuSelection(short state);
	// main menu object exists
	//	if state = 0, main menu selection is only depressed
	//		if state = 1, main menu selection is selected

	virtual int GetMenuSelection();
	// a menu selection exists
	// returns the menu selection from the local mainMenu object


	virtual void RunScene();
	// initalization exists
	// the free drive mode of the game where the user
	//		can freely expore the world driving the car without
	//		limitations

	virtual void LogicLoop();
	// initalization along with FreeDrive() exist
	// performs game logic separate from render loop
	
	virtual int CleanUp();
	// new objects exist
	// frees up any memory and deallocates the new objects
	
	virtual void DisplayText();
	// text is initialized
	// builds openGL fonts and glutOpenGL fonts

	virtual void DisplayMenu();
	// menu is initalized with a number of available choices
	// the main menu of the game
		
	virtual void ChangeCarCamera();
	// a car's camera index exists
	// the next camera index is set

	static void CollisionCallBack(void *data, dGeomID o1, dGeomID o2);
	// LocalCollisionCallBack exists
	// casts *data so that CTypeSGame members are recognized
	//		and calls LocalCollisionCallBack

	void LocalCollisionCallBack(dGeomID o1, dGeomID o2);
	// ode geometries exist
	// manages collision detection between the geometries

	void ResetCamera();
	//Pre-
	//	
	
	void Throttle();
	//Pre-
	// increase car throttle
	
	void Brake();
	//Pre-
	// decrease car throttle
	
	void Decel();
	//Pre-
	// natural car decel
	
	void SteerLeft();
	//Pre-
	// turn car left
	
	void SteerRight();
	//Pre-
	// turn car right
	
	void StraightenSteering();
	//Brings the steering angle back to zero

	void ResetCarPosition();
	//Pre-
	// reset car position
	
	void ReverseGear();
	//Pre-
	// toggle reverse gear
	
	void ReturnToMainMenu();
	//Pre-
	// exit to menu

	//void CreateTransformationMatrix(float m[16], float pos[3], const float r[12]);

	void CreateTransformationMatrix(float m[16], const dReal pos[3], const dReal rot[3]);
	void CreateTransformationMatrix2(float *m, const dReal pos[3], const dReal rot[3]);
	//Sets up the passed in matrix m with the given position and rotation values

	void Steering(float velocity, float steering);
};
#endif