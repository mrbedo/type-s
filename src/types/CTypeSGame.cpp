#include "CTypeSGame.h"
#include <iostream>

using namespace std;
//Initalize objects and member variables
int CTypeSGame::Init()				
{
	static bool created=false;
	ResetCamera();

	
	if(!created)
	{
		carMan= CCarManager::Instance();
		carMan->InitCars();
		currentCar=carMan->selectedCar;

		terrainMan= CTerrainManager::Instance();
		terrainMan->InitTerrains();
		currentTerrain=terrainMan->selectedTerrain;

		/////////////////////////////////////////
		//
		//	PHYSICS SETUP
		//
		/////////////////////////////////////////

		// dynamics and collision objects (chassis, 4 wheels, environment)
		LENGTH= currentCar->GetBoundingBox().x;					// chassis length
		WIDTH=	currentCar->GetBoundingBox().y;					// chassis width
		HEIGHT= currentCar->GetBoundingBox().z;					// chassis height
		RADIUS= currentCar->GetBoundingCyl().y*1.25,			// wheel radius
		STARTY= 0.9;					// starting height of chassis
		CMASS= 0.0070;											// chassis mass
		WMASS= 0.0001;											// wheel mass
		wheelRateLimit= 140.0f;
		tireWidth= 	currentCar->GetBoundingCyl().z;				// width of the wheels
		currentCar->SetDriveTerrain(3);
		ResetCamera();

		groundFriction= 0.8;
		//suspension
		fSpringSoft= 21.0f;										// softness of front suspension
		rSpringSoft= 16.0f;										// softness of rear suspsension
		handbrake=false;

		world = dWorldCreate();
		space = dHashSpaceCreate(0);
		contactgroup = dJointGroupCreate(0);

		dWorldSetGravity(world,0,-39.8,0);

		dWorldSetCFM(world, 0.4);
		dWorldSetERP(world, 0.8);

		ground = dCreatePlane (space,0,1,0,0);

		// chassis body
		body[0] = dBodyCreate(world);
		dBodySetPosition(body[0],currentCar->GetPartTransformation(0,5).x+TEMP_BODY_OFFSET-0.6, STARTY+currentCar->GetPartTransformation(0,5).y, currentCar->GetPartTransformation(0,5).z);
		dMassSetBox(&carMass,1,currentCar->GetBoundingBox().x,
							   currentCar->GetBoundingBox().y,
							   currentCar->GetBoundingBox().z);
		dMassAdjust (&carMass,CMASS);
		dBodySetMass (body[0],&carMass);
		box[0] = dCreateBox (0, currentCar->GetBoundingBox().x,
								currentCar->GetBoundingBox().y,
								currentCar->GetBoundingBox().z);
		dGeomSetBody (box[0],body[0]);

		// wheel bodies
		for (unsigned int i=1; i<=4; ++i)
		{
			body[i] = dBodyCreate (world);
			dQuaternion q;
			dQFromAxisAndAngle (q,0,0,1,M_PI*0.5);
			dBodySetQuaternion (body[i],q);
			dMassSetSphere (&wheelMass,1,RADIUS);
			dMassAdjust (&wheelMass,WMASS);
			dBodySetMass (body[i],&wheelMass);
			sphere[i-1] = dCreateSphere(0,RADIUS);
			dGeomSetBody (sphere[i-1],body[i]);
		}
		dBodySetPosition(body[1], currentCar->GetPartTransformation(0,1).x, STARTY, currentCar->GetPartTransformation(0,1).z);
		dBodySetPosition(body[2], currentCar->GetPartTransformation(0,3).x, STARTY, currentCar->GetPartTransformation(0,3).z);
		dBodySetPosition(body[3], currentCar->GetPartTransformation(0,2).x, STARTY, currentCar->GetPartTransformation(0,2).z);
		dBodySetPosition(body[4], currentCar->GetPartTransformation(0,0).x, STARTY, currentCar->GetPartTransformation(0,0).z);
		
		dBodySetFiniteRotationMode (body[1], 1);		//1=finate rotation mode
		dBodySetFiniteRotationMode (body[2], 1);		//1=finate rotation mode
		dBodySetFiniteRotationMode (body[3], 1);		//1=finate rotation mode
		dBodySetFiniteRotationMode (body[4], 1);		//1=finate rotation mode
				
		const dReal *a;
		for (i=0; i<4; ++i)										// front and back wheel hinges
		{
			joint[i]=dJointCreateHinge2 (world,0);
			dJointAttach(joint[i],body[0],body[i+1]);
			a=dBodyGetPosition(body[i+1]);
			dJointSetHinge2Anchor(joint[i],a[0],a[1],a[2]);
			dJointSetHinge2Axis1(joint[i],0,1,0);
			dJointSetHinge2Axis2(joint[i],0,0,1);

		}

		// set front suspension
		dJointSetHinge2Param (joint[0],dParamSuspensionERP,1);
		dJointSetHinge2Param (joint[0],dParamSuspensionCFM,fSpringSoft);
		dJointSetHinge2Param (joint[3],dParamSuspensionERP,1);
		dJointSetHinge2Param (joint[3],dParamSuspensionCFM,fSpringSoft);

// set back suspension
		dJointSetHinge2Param (joint[1],dParamSuspensionERP,1);
		dJointSetHinge2Param (joint[1],dParamSuspensionCFM,rSpringSoft);
		dJointSetHinge2Param (joint[2],dParamSuspensionERP,1);
		dJointSetHinge2Param (joint[2],dParamSuspensionCFM,rSpringSoft);

		
		// lock back wheels along the steering axis
		dJointSetHinge2Param (joint[0],dParamStopERP,1);
		dJointSetHinge2Param (joint[0],dParamStopCFM,0);
		
		dJointSetHinge2Param (joint[3],dParamStopERP,1);
		dJointSetHinge2Param (joint[3],dParamStopCFM,0);
		
		dJointSetHinge2Param (joint[1],dParamLoStop,-0.0001f);
		dJointSetHinge2Param (joint[1],dParamHiStop, 0.0001f);
		dJointSetHinge2Param (joint[1],dParamStopERP,1);
		dJointSetHinge2Param (joint[1],dParamStopCFM,1.0);

		dJointSetHinge2Param (joint[2],dParamLoStop,-0.0001f);
		dJointSetHinge2Param (joint[2],dParamHiStop, 0.0001f);
		dJointSetHinge2Param (joint[2],dParamStopERP,1);
		dJointSetHinge2Param (joint[2],dParamStopCFM,1.0);
		
		dJointSetHinge2Param (joint[0],dParamFMax2,0);
		dJointSetHinge2Param (joint[3],dParamFMax2,0);	   
		dJointSetHinge2Param (joint[1],dParamFMax2,0);
		dJointSetHinge2Param (joint[2],dParamFMax2,0);
		
		dJointSetHinge2Param (joint[0],dParamFudgeFactor,0);
		dJointSetHinge2Param (joint[1],dParamFudgeFactor,0);
		dJointSetHinge2Param (joint[2],dParamFudgeFactor,0);
		dJointSetHinge2Param (joint[3],dParamFudgeFactor,0);


		// create car space and add it to the top level space
		car_space = dSimpleSpaceCreate (space);
		dSpaceSetCleanup (car_space,0);
		dSpaceAdd (car_space,box[0]);
		dSpaceAdd (car_space,sphere[0]);
		dSpaceAdd (car_space,sphere[1]);
		dSpaceAdd (car_space,sphere[2]);
		dSpaceAdd (car_space,sphere[3]);

		// environment
		numOfGroundBlocks = 5;
		gTerrainSize = CVector3(100,100,20);
		gBoxSize= CVector3(3.0, 0.35, 0.15);		// width length height
		gCylSize= CVector3(1,20,2);					// radius, height, center axis
		gBoundingWallSize = CVector3(150, 20 ,0);

		terrainDim[0] = dCreatePlane(0, -1, 0,  0, -gTerrainSize.x); // x,y,z,d
		terrainDim[1] = dCreatePlane(0,  1, 0,  0, -gTerrainSize.x); // x,y,z,d
		terrainDim[2] = dCreatePlane(0,  0, 0,  1, -gTerrainSize.y); // x,y,z,d
		terrainDim[3] = dCreatePlane(0,  0, 0, -1, -gTerrainSize.y); // x,y,z,d
		terrainDim[4] = dCreatePlane(0,  0,-1,  0, -gTerrainSize.z); // x,y,z,d


		dMatrix3 R;
		dRFromAxisAndAngle (R,1,0,0, 1.57);
/*
		//Walls
		//Left	(relative to cars starting position)
		boundingWall[0] = dCreateBox(space, gBoundingWallSize.x, gBoundingWallSize.y, gBoundingWallSize.z);
		dGeomSetPosition(boundingWall[0], 0, -0.05,-56);

		//Right	(relative to cars starting position)
		boundingWall[1] = dCreateBox(space, gBoundingWallSize.x, gBoundingWallSize.y, gBoundingWallSize.z);
		dGeomSetPosition(boundingWall[1], 0, -0.05, 58);

		//Front	(relative to cars starting position)
		boundingWall[2] = dCreateBox(space, gBoundingWallSize.z, gBoundingWallSize.y, gBoundingWallSize.x);
		dGeomSetPosition(boundingWall[2], 58, -0.05, 0);

		//Behind	(relative to cars starting position)
		boundingWall[3] = dCreateBox(space, gBoundingWallSize.z, gBoundingWallSize.y, gBoundingWallSize.x);
		dGeomSetPosition(boundingWall[3],-58, -0.05, 0);

		for(i=0; i<numOfGroundBlocks; ++i)
		{
				groundBox[i] = dCreateBox(space, gBoxSize.x, gBoxSize.y, gBoxSize.z);
				dGeomSetPosition(groundBox[i],10,-0.05, i*10);
		}

		for(i=0; i<10; ++i)
		{
			float trans= 20.0f;
			if(i & 0x1) trans= -45.0f;
			
           	groundCyl[i] = dCreateCCylinder(space, gCylSize.x, gCylSize.y);
			dGeomSetPosition(groundCyl[i], trans, 3, (i-5)*10);
			dGeomSetRotation(groundCyl[i],R);

		}
*/
/*
		boundingWall[1] = dCreateBox(space, gBoundingWallSize.x, gBoundingWallSize.y, gBoundingWallSize.z);
		dGeomSetPosition(boundingWall[0],0,-0.05, -100);

		boundingWall[2] = dCreateBox(space, gBoundingWallSize.y, gBoundingWallSize.x, gBoundingWallSize.z);
		dGeomSetPosition(boundingWall[0],100,-0.05, 100);

		boundingWall[3] = dCreateBox(space, gBoundingWallSize.y, gBoundingWallSize.x, gBoundingWallSize.z);
		dGeomSetPosition(boundingWall[0],-100,-0.05, -100);
*/
		//////////////////
		//MESH
		//////////////////
		/*
		triData = dGeomTriMeshDataCreate();
		collisionMesh.Load("models/terrain/garage/garage.3ds");
		cout<<"yourmom"<<endl;
		dGeomTriMeshDataBuildSingle(triData, 
									collisionMesh.vertexArray, 
									3*sizeof(float), 
									collisionMesh.vertexCount, 
									collisionMesh.indexArray, 
									collisionMesh.indexCount, 
									3*sizeof(unsigned int));
		cout<<"yourmom"<<endl;
		triMesh = dCreateTriMesh(space, triData, 0,0,0);
		cout<<"yourmom"<<endl;
		*/


	}

	created=true;
	return true;
}

void CTypeSGame::InitMenu()
{
}

void CTypeSGame::MakeMenuSelection(short state)
{
}

//return the current menu choice of the scene
int CTypeSGame::GetMenuSelection()
{
	return menuSelection;
}

void CTypeSGame::DisplayMenu()
{
}

//shows the text on the screen
void CTypeSGame::DisplayText()
{
	time.FramesPerSecond();
	t.SetFont(6);

	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 0.0f);
	glScalef(0.9f, 0.9f, 1.0f);
	glTranslatef(0.0f, 0.0f, -0.3f);
	//-----------------text------------------------
/*	t.glutPrint(-textAlign-0.025f, 0.10f," %d", time.GetHour()); 
	t.glutPrint(-textAlign-0.020f, 0.10f,":%d", time.GetMinute()); 
	t.glutPrint(-textAlign-0.015f, 0.10f,":%d", time.GetSecond());
	t.glutPrint(-textAlign-0.010f, 0.10f,".%d", time.GetTensSecond());
*/	t.Print(-textAlign-0.02f, 0.09f,"FPS: ", time.GetFPS());
	t.Print(textAlign,scrollplace+0.12f,"CameraPos.x=    ", c.GetPosition().x);
	t.Print(textAlign,scrollplace+0.11f,"CameraPos.y=    ", c.GetPosition().y);
	t.Print(textAlign,scrollplace+0.10f,"CameraPos.z=    ", c.GetPosition().z);
	if(currentCar != 0)
	{
	t.Print(textAlign,scrollplace+0.09f,"Velocity.x:     ", currentCar->GetVelocity().x);
	t.Print(textAlign,scrollplace+0.08f,"Velocity.z:     ", currentCar->GetVelocity().z);
	t.Print(textAlign,scrollplace+0.07f,"car position.x: ", currentCar->GetPosition().x);
	t.Print(textAlign,scrollplace+0.06f,"car position.y: ", currentCar->GetPosition().y);
	t.Print(textAlign,scrollplace+0.05f,"car position.z: ", currentCar->GetPosition().z);
	t.Print(textAlign,scrollplace+0.04f,"steerAngle:     ", currentCar->GetSteerAngle());
	t.Print(textAlign,scrollplace+0.03f,"DELTA TIME:     ", deltaTime);
	t.Print(textAlign,scrollplace+0.02f,"rate w0         ", (float)dJointGetHinge2Angle2Rate(joint[0]));
	t.Print(textAlign,scrollplace+0.01f,"rate w3         ", (float)dJointGetHinge2Angle2Rate(joint[3]));
	t.Print(textAlign,scrollplace+0.00f,"rate w1         ", (float)dJointGetHinge2Angle2Rate(joint[1]));
	t.Print(textAlign,scrollplace-0.01f,"rate w2         ", (float)dJointGetHinge2Angle2Rate(joint[2]));
	t.Print(textAlign,scrollplace-0.02f,"angle w0        ", (float)dJointGetHinge2Angle1(joint[0]));
	t.Print(textAlign,scrollplace-0.03f,"angle w3        ", (float)dJointGetHinge2Angle1(joint[3]));
	t.Print(textAlign,scrollplace-0.04f,"angle w1        ", (float)dJointGetHinge2Angle1(joint[1]));
	t.Print(textAlign,scrollplace-0.05f,"angle w2        ", (float)dJointGetHinge2Angle1(joint[2]));
//	t.glutPrint(textAlign,scrollplace+0.02f,"Throttle:          %f", currentCar->throttle);
	}
/*	t.glutPrint(textAlign,scrollplace+0.02f,"Menu Choice:       %d", mainMenu->ReturnChoice());
	t.glutPrint(textAlign,scrollplace+0.01f,"Scene State:       %d", currentSceneState);
	t.glutPrint(textAlign,scrollplace+0.00f,"CarIndex:          %d", carIndex);
	t.glutPrint(textAlign,scrollplace-0.01f,"carHasBeenInit:    %d", (int)carHasBeenInitalized);
	t.glutPrint(textAlign,scrollplace-0.02f,"carListSize:       %d", (int)carList.size());
*///	t.glutPrint(textAlign,scrollplace-0.03f,"steerAngleRate:    %f", currentCar->steerAngleRate);
//	t.glutPrint(textAlign,scrollplace-0.04f,"wheelAngle:        %f", currentCar->wheelAngle);
/*	t.glutPrint(textAlign,scrollplace-0.05f,"Terrain Vtx        %d", (int)terrain.model.allVerteces.size());//allTerrainVerteces.size());
	t.glutPrint(textAlign,scrollplace-0.06f,"rSpeed:            %f", currentCar->rSpeed);
	t.glutPrint(textAlign,scrollplace-0.07f,"throttle:          %f", currentCar->throttle);
	t.glutPrint(textAlign,scrollplace-0.08f,"Camera YAW:        %f", c.GetYaw());
	t.glutPrint(textAlign,scrollplace-0.09f,"Camera Pitch:      %f", c.GetPitch());
	t.glutPrint(textAlign,scrollplace-0.10f,"Camera speed:      %f", c.GetCameraSpeed());
*///	t.glutPrint(textAlign,scrollplace-0.10f,"EndNodes:          %d", g_Octree.g_EndNodeCount);
//	t.glutPrint(textAlign,scrollplace-0.11f,"NodesDraw:         %d", g_Octree.g_TotalNodesDrawn);  
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glPopAttrib();
	glPopMatrix();
}

//Game Scene
void CTypeSGame::RunScene()									
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();										// Reset The Current Modelview Matrix
	glClearColor(0.5f, 0.7f, 0.9f, 1.0f);					// Background Color
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	if(useMouse)
		c.Mouse(10.0f, 0, 0);
		
	if(currentCar)
	{
		if(currentCar->GetCameraIndex()==0)
		{
			c.MovieCam(currentCar->GetPosition(), deltaTime);
		}
		else
		if(currentCar->GetCameraIndex()==1)
		{
			c.FreeCam(false, deltaTime);
		}
		else
		{
			CVector3 vec(dBodyGetPosition(body[0])[0],
						 dBodyGetPosition(body[0])[1]+1,
						 dBodyGetPosition(body[0])[2]);
			const dReal camPos[3]={vec.x, vec.y, vec.z}  ;

			
			CreateTransformationMatrix2(const_cast<float*>(matrix.gl_mtr()), camPos,dBodyGetRotation(body[0]));
			c.MatrixCam(matrix.transpose().gl_mtr(),vec );
			/*
			c.FollowPosition(currentCar->GetPosition(), 
							 currentCar->GetCameraPositions()[currentCar->GetCameraIndex()],
							 currentCar->GetSteerAngle(),
							 deltaTime);
							 */
		}
	}
	else													// if no car, use free camera
	{
		c.FreeCam(false, deltaTime);
	}
	if(showText)
	{
		glPushMatrix();											// City is in separate matrix
			//TEMP
			gfxUtils.DrawGridLines(1000);
		glPopMatrix();

		collisionMesh.Draw();
	}

	glPopAttrib();
	//glFlush();
}


// Modified by NateW for joystick support
void CTypeSGame::Steering(float velocity, float steering)
{
	//static const dReal steeringRate = 1;


	// a little bit of deadband seems to make it a bit easier to control
	if(fabs(steering) < 1.1) steering=0;

	float desiredPosition = steering/57;			//convert actual angle into ode motor angle units
	float actualPositionR = dJointGetHinge2Angle1(joint[0]);
	float actualPositionL = dJointGetHinge2Angle1(joint[3]);

	float steeringVelocityR = (desiredPosition - actualPositionR)*20;
	float steeringVelocityL = (desiredPosition - actualPositionL)*20;

	dJointSetHinge2Param (joint[3],dParamVel, steeringVelocityR);
	dJointSetHinge2Param (joint[3],dParamLoStop, desiredPosition-0.01f);
	dJointSetHinge2Param (joint[3],dParamHiStop, desiredPosition+0.01f);
	
	dJointSetHinge2Param (joint[0],dParamVel, steeringVelocityL);
	dJointSetHinge2Param (joint[0],dParamLoStop, desiredPosition-0.01f);
	dJointSetHinge2Param (joint[0],dParamHiStop, desiredPosition+0.01f);
}

//Handles the non graphical aspect of the main scene
void CTypeSGame::LogicLoop()
{
	CGraphicUtils gfxUtils;

	if(currentCar)
	{
		if(handbrake)
		{
			cout<<"Handbrake"<<endl;
			//cut power to front wheels and lock rear wheels
			//dJointSetHinge2Param (joint[0],dParamVel2, currentCar->GetVelocity().x);
	
			//dJointSetHinge2Param (joint[3],dParamVel2, currentCar->GetVelocity().x);
			
            //dJointSetHinge2Param (joint[1], dParamVel2, 0);
 			//dJointSetHinge2Param (joint[2], dParamVel2, 0);

		}

		for(unsigned int i=1; i<5; ++i)
			dBodySetFiniteRotationAxis (body[i],0,0,0);		//realign all axis for stability	
		
		float torque=currentCar->GetVelocity().x;
		
		if(torque < -0.05)
			torque= -0.05;
		else 
		if(torque > 0.05)
			torque= 0.05;
	  

		dBodyAddRelTorque (body[1], 0, 0, torque*.32);//FF
		dBodyAddRelTorque (body[4], 0, 0, torque*.32);//FF
		dBodyAddRelTorque (body[2], 0, 0, torque*.68);//RR
		dBodyAddRelTorque (body[3], 0, 0, torque*.68);//RR


		Steering(currentCar->GetVelocity().x, currentCar->GetSteerAngle());
	
		//dSpaceCollide (space,this,CollisionCallBack);
      	double stepSize= deltaTime;
		//if	   (stepSize<0.034) stepSize=0.034;
		//else if(stepSize>0.05) stepSize=0.05;

		if(stepSize<=0.000) 
		{
			stepSize=0.0001;
		}
		else if(stepSize>0.05)
		{
			stepSize=0.05;
		}
	   // dWorldStep (world, stepSize/2);
       // dJointGroupEmpty(contactgroup);						// remove all contact joints


		for(unsigned int i=0; i<4; ++i)
		{
			if(dJointGetHinge2Angle2Rate(joint[i]) > wheelRateLimit)
			{
				dJointSetHinge2Param (joint[i],dParamFMax2, 1 );
				dJointSetHinge2Param (joint[i], dParamVel2, wheelRateLimit);
			}
			else if(dJointGetHinge2Angle2Rate(joint[i]) < -wheelRateLimit)
			{
				dJointSetHinge2Param (joint[i],dParamFMax2, 1 );
				dJointSetHinge2Param (joint[i], dParamVel2, -wheelRateLimit);
			}
			else
				dJointSetHinge2Param (joint[i],dParamFMax2,0);

		}

		dSpaceCollide (space,this,CollisionCallBack);
		dWorldStep (world, stepSize/2);
        dJointGroupEmpty(contactgroup);						// remove all contact joints

		//garage
		glPushMatrix();
		//	glMultMatrixf(mat);
		glTranslatef(currentTerrain->GetPartTransformation(0,0).x,
					 currentTerrain->GetPartTransformation(0,0).y,
					 currentTerrain->GetPartTransformation(0,0).z);
			currentTerrain->partVector[0].RenderPart();
			//gfxUtils.DrawBox(gTerrainSize.x, gTerrainSize.y, gTerrainSize.z);
		glPopMatrix();
/*
		//ground blocks
		for(unsigned int i=0; i<numOfGroundBlocks; ++i)
		{
				glPushMatrix();
					CreateTransformationMatrix(mat, dGeomGetPosition(groundBox[i]),dGeomGetRotation(groundBox[i]));
					glMultMatrixf(mat);
					currentTerrain->partVector[2].RenderPart();
					//gfxUtils.DrawBox(gBoxSize.x, gBoxSize.y, gBoxSize.z);
				glPopMatrix();
		}

		
		//garage columns
		for(i=0; i<10; ++i)
		{
				glPushMatrix();
					CreateTransformationMatrix(mat, dGeomGetPosition(groundCyl[i]),dGeomGetRotation(groundCyl[i]));
					glMultMatrixf(mat);
					currentTerrain->partVector[1].RenderPart();
				//	gfxUtils->DrawCylinder(gCylSize.x, gCylSize.y);
				glPopMatrix();
		}
*/
		float lodLevel=0.5f;

		//Wheels
		glPushMatrix();
			CreateTransformationMatrix(mat, dBodyGetPosition(body[1]),dBodyGetRotation(body[1]));
			glMultMatrixf(mat);
			currentCar->partVector[1].RenderPart();
			if(showText)currentCar->DrawBoundingCyl();
		glPopMatrix();

		glPushMatrix();
			CreateTransformationMatrix(mat, dBodyGetPosition(body[2]),dBodyGetRotation(body[2]));
			glMultMatrixf(mat);
			currentCar->partVector[3].RenderPart();
			if(showText)currentCar->DrawBoundingCyl();
		glPopMatrix();

		glPushMatrix();
			CreateTransformationMatrix(mat, dBodyGetPosition(body[3]),dBodyGetRotation(body[3]));
			glMultMatrixf(mat);
			currentCar->partVector[2].RenderPart();
			if(showText)currentCar->DrawBoundingCyl();
		glPopMatrix();

		glPushMatrix();
			CreateTransformationMatrix(mat, dBodyGetPosition(body[4]),dBodyGetRotation(body[4]));
			glMultMatrixf(mat);
			currentCar->partVector[0].RenderPart();
			if(showText)currentCar->DrawBoundingCyl();
		glPopMatrix();

		//Body
		glPushMatrix();
			CreateTransformationMatrix(mat, dBodyGetPosition(body[0]),dBodyGetRotation(body[0]));
			glMultMatrixf(mat);
			/*currentCar->partVector[4].part.DrawLODModel(lodLevel);//.RenderPartWithVectorTrans();
			currentCar->partVector[5].part.DrawLODModel(lodLevel);//RenderPartWithVectorTrans();
			currentCar->partVector[6].part.DrawLODModel(lodLevel);//RenderPartWithVectorTrans();
			currentCar->partVector[7].part.DrawLODModel(lodLevel);//RenderPartWithVectorTrans();
*/
			if(showText)currentCar->DrawBoundingBox();

			glTranslatef(-TEMP_BODY_OFFSET+0.6,0,0);
			//currentCar->partVector[4].RenderPart();
			currentCar->partVector[5].RenderPart();
			//currentCar->partVector[6].RenderPart();
			//currentCar->partVector[7].RenderPart();
		glPopMatrix();


		//Draw Shadow
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
			
			shadow.ShadowProjection(CVector3(-500,1000,0),
									CVector3(dBodyGetPosition(body[0])[0],
											 0.01,
											 dBodyGetPosition(body[0])[2]),
									CVector3(0,-1,0));
			CreateTransformationMatrix(mat, dBodyGetPosition(body[0]),dBodyGetRotation(body[0]));
			glMultMatrixf(mat);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0,0,0,0.5);
			gfxUtils.DrawCarShadowPolygon(currentCar->GetBoundingBox().z+0.3,
									currentCar->GetBoundingBox().x-0.4, 1);

		glEnable(GL_LIGHTING);
		glEnable(GL_FOG);
		glPopAttrib();
		glPopMatrix();

		currentCar->SetPosition(CVector3(dBodyGetPosition(body[0])[0],
										 dBodyGetPosition(body[0])[1],
										 dBodyGetPosition(body[0])[2]));
	}

	if(showText)											// Determine if the text is to be shown
		DisplayText();
}

//creates an openGL transformation matrix from the physics generated position coordinates
//and rotational matrix
//void CTypeSGame::CreateTransformationMatrix(float m[16] , const float pos[3], const float r[12])
void CTypeSGame::CreateTransformationMatrix(float m[16] ,const dReal pos[3], const dReal rot[10])
{
	m[ 0]=rot[ 0]; m[ 4]=rot[ 1]; m[ 8]=rot[ 2]; m[12]=pos[0];		//Set up the translation transformation matrix
	m[ 1]=rot[ 4]; m[ 5]=rot[ 5]; m[ 9]=rot[ 6]; m[13]=pos[1];
	m[ 2]=rot[ 8]; m[ 6]=rot[ 9]; m[10]=rot[10]; m[14]=pos[2];
	m[ 3]=0;	   m[ 7]=0;       m[11]=0;		 m[15]=1;		
}

void CTypeSGame::CreateTransformationMatrix2(float* m ,const dReal pos[3], const dReal rot[10])
{
	m[ 0]=rot[ 0]; m[ 4]=rot[ 1]; m[ 8]=rot[ 2]; m[12]=pos[0];		//Set up the translation transformation matrix
	m[ 1]=rot[ 4]; m[ 5]=rot[ 5]; m[ 9]=rot[ 6]; m[13]=pos[1];
	m[ 2]=rot[ 8]; m[ 6]=rot[ 9]; m[10]=rot[10]; m[14]=pos[2];
	m[ 3]=0;	   m[ 7]=0;       m[11]=0;		 m[15]=1;		
}

//Callback function for collision
void CTypeSGame::LocalCollisionCallBack(dGeomID o1, dGeomID o2)
{
    int i, n;
    dBodyID b1 = dGeomGetBody (o1);
    dBodyID b2 = dGeomGetBody (o2);
    if (b1 && b2 && dAreConnected (b1, b2)) return;

    const int N = 10;
    dContact contact[N];
    n = dCollide (o1, o2, N, &contact[0].geom, sizeof (dContact));
    if (n > 0)
    {
        for (i = 0; i < n; i++)
        {
            contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;
            if (dGeomGetClass (o1) == dSphereClass || dGeomGetClass (o2) == dSphereClass)
                contact[i].surface.mu = 0.0;
            else
                contact[i].surface.mu = groundFriction;
            
			contact[i].surface.slip1 = 0.1;
            contact[i].surface.slip2 = 0.2;
            contact[i].surface.soft_erp = 0.2;
            contact[i].surface.soft_cfm = 0.7;
			dJointID c = dJointCreateContact (world,contactgroup, contact+i);
            dJointAttach (c, dGeomGetBody(contact[i].geom.g1),
							 dGeomGetBody(contact[i].geom.g2));
        }
    }
}

//have static callback function recognize internal data members
void CTypeSGame::CollisionCallBack(void* data, dGeomID o1, dGeomID o2)
{
	CTypeSGame* dataPtr= static_cast<CTypeSGame*>(data);
	dataPtr->LocalCollisionCallBack(o1,o2);
}


//Sets the scene state to 1 which is the main menu
void CTypeSGame::ReturnToMainMenu()
{
	c.ResetCamera();
}	

void CTypeSGame::ResetCamera()
{
	c.ResetCamera();
	c.SetPosition(CVector3(-10,5,0));
}

//increase car throttle
void CTypeSGame::Throttle()
{
	float torque= currentCar->GetVelocity().x-100.0f;
	currentCar->SetVelocity(CVector3(torque,0,0));

	/*
	switch(currentCar->GetDriveTerrain())
	{
	case 2: //AWD
		dJointSetHinge2Param (joint[0],dParamFMax2,1);
		dJointSetHinge2Param (joint[3],dParamFMax2,1);	   
		dJointSetHinge2Param (joint[1],dParamFMax2,1);
		dJointSetHinge2Param (joint[2],dParamFMax2,1);
		break;
	case 1: //FWD
 		dJointSetHinge2Param (joint[0],dParamFMax2,1);
		dJointSetHinge2Param (joint[3],dParamFMax2,1);	   
		dJointSetHinge2Param (joint[1],dParamFMax2,0);
		dJointSetHinge2Param (joint[2],dParamFMax2,0);
		break;
	case 0: //RWD
 		dJointSetHinge2Param (joint[0],dParamFMax2,0);
		dJointSetHinge2Param (joint[3],dParamFMax2,0);	   
		dJointSetHinge2Param (joint[1],dParamFMax2,1);
		dJointSetHinge2Param (joint[2],dParamFMax2,1);
		break;
	default: //Disable powe to wheels
 		dJointSetHinge2Param (joint[0],dParamFMax2,0);
		dJointSetHinge2Param (joint[3],dParamFMax2,0);	   
		dJointSetHinge2Param (joint[1],dParamFMax2,0);
		dJointSetHinge2Param (joint[2],dParamFMax2,0);
	}
		*/  
}

//decrease car throttle
void CTypeSGame::Brake()
{

	float torque= currentCar->GetVelocity().x+100.0f;
	currentCar->SetVelocity(CVector3(torque,0,0));

	
	//	currentCar->SetVelocity(CVector3(2*currentCar->GetVelocity().x, 0.0f, 0.0f));
	/*
	dJointSetHinge2Param (joint[0],dParamFMax2,1);
	dJointSetHinge2Param (joint[3],dParamFMax2,1);	   
	dJointSetHinge2Param (joint[1],dParamFMax2,1);
	dJointSetHinge2Param (joint[2],dParamFMax2,1);
	*/

}


//natural car decel
void CTypeSGame::Decel()
{
	currentCar->SetVelocity(CVector3(0,0,0));
	/*
	float w0 = dJointGetHinge2Angle2Rate(joint[0]);
	float w1 = dJointGetHinge2Angle2Rate(joint[0]);
	float w2 = dJointGetHinge2Angle2Rate(joint[0]);
	float w3 = dJointGetHinge2Angle2Rate(joint[0]);
	float velTot=(w0+w1+w2+w3)/4;

	currentCar->SetVelocity(CVector3(velTot, 0.0f, 0.0f));
	*/
}

//turn car left
void CTypeSGame::SteerLeft()
{
	currentCar->SetSteerAngle(currentCar->GetSteerAngle()-1);
}

//turn car right
void CTypeSGame::SteerRight()
{
	currentCar->SetSteerAngle(currentCar->GetSteerAngle()+1);
}

void CTypeSGame::StraightenSteering()
{
	float sa=currentCar->GetSteerAngle();
	if(sa >= 1) 
		currentCar->SetSteerAngle(sa-=1);
	else if(sa <= 1)
		currentCar->SetSteerAngle(sa+=1);
}
//reset car position
void CTypeSGame::ResetCarPosition()
{
	dMatrix3 R;
	for(unsigned int i=0; i<5; ++i)
		dBodySetRotation(body[i], R);
	
	dBodySetPosition(body[0], currentCar->GetPartTransformation(0,5).x, STARTY+currentCar->GetPartTransformation(0,5).y, currentCar->GetPartTransformation(0,5).z);
	dBodySetPosition(body[1], currentCar->GetPartTransformation(0,1).x, STARTY, currentCar->GetPartTransformation(0,1).z);
	dBodySetPosition(body[2], currentCar->GetPartTransformation(0,3).x, STARTY, currentCar->GetPartTransformation(0,3).z);
	dBodySetPosition(body[3], currentCar->GetPartTransformation(0,2).x, STARTY, currentCar->GetPartTransformation(0,2).z);
	dBodySetPosition(body[4], currentCar->GetPartTransformation(0,0).x, STARTY, currentCar->GetPartTransformation(0,0).z);
	
	currentCar->SetVelocity(CVector3(0,0,0));
}

//change camera view
void CTypeSGame::ChangeCarCamera()
{
	currentCar->NextCameraPosition();
}

//deletes pointers and objects and frees up memory
int CTypeSGame::CleanUp()
{
	cout<<"\nCleanUP -  CTypeSGame...";
	TopCleanUp();
	//currentTerrain=0;


	if(created)
	{
	dJointGroupDestroy (contactgroup);
	dSpaceDestroy (space);
	dWorldDestroy (world);
	dGeomDestroy (box[0]);
	dGeomDestroy (sphere[0]);
	dGeomDestroy (sphere[1]);
	dGeomDestroy (sphere[2]);
	dGeomDestroy (sphere[3]);
	dGeomTriMeshDataDestroy(triData);
	}

	cout<<"\n           [done]";
	return true;
}
