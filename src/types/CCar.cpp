///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CCar.cpp
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
//  This class provides a general car model description
//
//  inherits CModel
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CCar.h"
#include <iostream>
#include "../engine/CTSIFile.h"

using namespace std;

//initalizes the parts vector
int CCar::InitModels()
{
//partVector.resize(20);	cout<<"initalizing..."<<endl;
	if(numberOfParts > MAX_PARTS)
	{
		cout<<"\n\n\n\nERROR>> Not enough space in partVector["<<MAX_PARTS<<"] to fit "<<numberOfParts<<" parts!\n\n\n\n"<<endl;
		return 0;
	}
	else
	{
		for(unsigned int i=0; i<numberOfParts; ++i)
		{
			partVector[i].GeneratePartDL();
		}
		return 1;
	}
}

//constructor
int CCar::Init(const string& tsiFileName)
{
	CTSIFile		tsi;										// Text loading object
	descriptionPic= tsi.GetTSIString(tsiFileName,"FILES", "descriptionPic");
	carName=		tsi.GetTSIString(tsiFileName,"FILES","objectName"),

	position=CVector3(0.0f, 0.0f, 0.0f);	
	velocity=CVector3(0.0f, 0.0f, 0.0f);

	tireRadius=6911.0f;									//1000th GL units...means glTranslate(0.6971f...)
	wheelRotationConstant=(2.0f*3.14*tireRadius)/360.0f;

	cout<<"\nloading..."<<endl;
	
	/////////////////////////////////////////////////////////
	//
	//  Car Part Geometry
	//
	/////////////////////////////////////////////////////////

	//---------| 0 TIRE | 1 BODY | 2 COUPE | 3 WINDOWS | 4 STEERINGWHEEL |----------
	vector<string> tsiPartList= tsi.GetTSIStringVector(tsiFileName,"PART_LIST","partList");

	string partName;
	char buffer[20];

	for(unsigned int i=0; i<tsiPartList.size(); ++i)
	{
		partName= "part";
		partName += itoa(i,buffer,10);
		cout<<endl<<partName<<endl;


		partVector[i].InitPart(tsiPartList[i], tsi.GetTSIString(tsiFileName,"FILES","texturePath"));
		partVector[i].SetPropertyVector(tsi.GetTSIFloatVector(tsiFileName, "GEOMETRY", partName));
		//		cout<<"pushing.."<<endl;

		switch((int)partVector[i].GetProperty(9))			//store the indeces of the panel types
		{
		case 0: bodyPanelVecIndex.push_back(i);		break;	//bodyPanels
		case 1:	tireVecIndex.push_back(i);			break;	//Tires
		case 2: steeringWheelVecIndex.push_back(i);	break;	//steeringWheels 
		}

	
	}
	//cout<<"\nbodyPanel.Size()="<<bodyPanelVecIndex.size()<<endl;
	for(i=0; i<bodyPanelVecIndex.size(); ++i)
		cout<<bodyPanelVecIndex[i]<<",";

	//cout<<"\ntirePanel.Size()="<<tireVecIndex.size()<<endl;
	for(i=0; i<tireVecIndex.size(); ++i)
		cout<<tireVecIndex[i]<<",";


	numberOfParts=tsiPartList.size();
	cout<<"\n...loaded!"<<endl;

	/////////////////////////////////////////////////////////
	//
	//  Car Camera
	//
	/////////////////////////////////////////////////////////
	SetCameraPositions(CVector3(tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam1x"),
							    tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam1y"),
							    tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam1z")),
					   /*
					   CVector3(tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam2x"),
							    tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam2y"),
							    tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam2z")),


					   CVector3(tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam3x"),
							    tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam3y"),
							    tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam3z")),
						*/
					   CVector3(tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam4x"),
							    tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam4y"),
							    tsi.GetTSIFloat(tsiFileName,"CAMERA_POSITIONS","cam4z"))
					   );
	angle=0;
	steerAngle=0;
	maxSteerAngle=360;
	steerAngleRate=0;
	steerRatio=20;
	wheelAngle=0;
	maxTorque=0;
	throttle=0;
	brake=0;
	rSpeed=0;

	carYaw=0;
	carPitch=0;
	accel=0;

	numberOfCameras=3;
	cameraIndex=0;
	boundingBox=CVector3(1,1,1);
	boundingCyl=CVector3(1,1,1);

	/////////////////////////////////////////////////////////
	//
	//  Car Physics Properties
	//
	/////////////////////////////////////////////////////////
    SetMaxTorque(tsi.GetTSIFloat(tsiFileName, "PHYSICS", "torque"));
	SetMaxSteerAngle(tsi.GetTSIFloat(tsiFileName, "PHYSICS", "maxSteerAngle"));					//maximum steering angle of wheels

	SetPosition(CVector3(tsi.GetTSIFloat(tsiFileName, "PHYSICS", "startPosX"),
						 tsi.GetTSIFloat(tsiFileName, "PHYSICS", "startPosY"),
						 tsi.GetTSIFloat(tsiFileName, "PHYSICS", "startPosZ")));

	SetBoundingBox(CVector3(tsi.GetTSIFloat(tsiFileName, "COLLISION_BOX", "length"),
							tsi.GetTSIFloat(tsiFileName, "COLLISION_BOX", "height"),
							tsi.GetTSIFloat(tsiFileName, "COLLISION_BOX", "width")));

	
	//FIX FIX FIX FIX FIX
	//MAKE A SPHERE FOR WHEEL NOT CYLINDER (MAY FIX PROBLEM)
	SetBoundingCyl(CVector3(1, 0.6f,0.6f));		//car wheel bounding cylinder
	


	cout<<"initdone"<<endl;
	return true;
}

CCar::~CCar()
{
	glDeleteLists(carBodyDL, 1);
	glDeleteLists(wheelDL, 1);
	glDeleteLists(steeringWheelDL, 1);

	for(unsigned int i=0; i< numberOfParts; ++i)
	{
		partVector[i].CleanUp();
	}
}

//renders 3d models using a display list
void CCar::RenderCarModel()
{
	for(unsigned int i=0; i<numberOfParts; ++i)
	{
		switch((int)partVector[i].GetProperty(9)) //check what type of panel it is
		{
		case 0:	//body
			DrawBody(i);
			break;
		case 1: //wheel
			DrawWheel(i);
			break;
		case 2: //steering
			DrawBody(i);
			break;
		}
	}
}

//Determines geometry of the car tires, and draws them to the screen
void CCar::DrawWheel(unsigned int i) const
{
	//short wheelOrientation=0;
/*
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);	//calculate rotation translation
		glRotatef(steerAngle, 0.0f, 1.0f, 0.0f);			//from assigned vertex 

		glPushMatrix();										//transformations must be assigned manually, so RenderPartAsIs is used
			glTranslatef(partVector[i].GetPartTransformVector(0).x,
						 partVector[i].GetPartTransformVector(0).y,
						 partVector[i].GetPartTransformVector(0).z);
			glScalef(partVector[i].GetPartTransformVector(1).x,
					 partVector[i].GetPartTransformVector(1).y,
					 partVector[i].GetPartTransformVector(1).z);

			if(partVector[i].GetProperty(10)==1) //if not static, then rotate wheels	
				glRotatef(-wheelAngle, 0.0f, 1.0f, 0.0f);//first two wheels are rotating wheels
			glRotatef(rSpeed, 0.0f, 0.0f, 1.0f);
			glRotatef(partVector[i].GetPartTransformVector(2).x, 1.0f, 0.0f, 0.0f);
			glRotatef(partVector[i].GetPartTransformVector(2).y, 0.0f, 1.0f, 0.0f);
			glRotatef(partVector[i].GetPartTransformVector(2).z, 0.0f, 0.0f, 1.0f);
			partVector[i].RenderPartAsIs();
			DrawBoundingCyl();
		glPopMatrix();
	glPopMatrix();
	*/
	glPushMatrix();
		glScalef(partVector[i].GetPartTransformVector(1).x,
				 partVector[i].GetPartTransformVector(1).y,
				 partVector[i].GetPartTransformVector(1).z);
		partVector[i].RenderPartWithMatrixTrans();
	glPopMatrix();

	//glFlush();
}


//make the tyre display list
void CCar::GenerateWheelDL()
{
}

//make the steering wheel display list
void CCar::GenerateSteeringWheelDL()
{
}

//make car body display list
void CCar::GenerateCarBodyDL()
{
}


//renders the car body to the screen
void CCar::DrawBody(unsigned int i)	const
{
	glPushMatrix();

		glTranslatef(position.x, position.y, position.z);	//calculate rotation translation
		glRotatef(steerAngle, 0.0f, 1.0f, 0.0f);			//from assigned vertex 

		glPushMatrix();										//move with wheel but not rotate
			glRotatef(carYaw, 1.0f, 0.0f, 0.0f);
//			glRotatef(carPitch, 0.0f, 0.0f, 1.0f);

			//body roll
			glRotatef(carRoll, 1.0f, 0.0f, 0.0f);

			//DrawBoundingBox();

			if(partVector[i].GetProperty(9)==2)             //if it is the steering wheel
			{/*
				glPushMatrix();
					glTranslatef(partVector[i].GetPartTransformVector(0).x,
								 partVector[i].GetPartTransformVector(0).y,
								 partVector[i].GetPartTransformVector(0).z);
					glScalef(partVector[i].GetPartTransformVector(1).x,
							 partVector[i].GetPartTransformVector(1).y,
							 partVector[i].GetPartTransformVector(1).z);
					glRotatef(partVector[i].GetPartTransformVector(2).x, 1.0f, 0.0f, 0.0f);
					glRotatef(partVector[i].GetPartTransformVector(2).y, 0.0f, 1.0f, 0.0f);
					glRotatef(partVector[i].GetPartTransformVector(2).z, 0.0f, 0.0f, 1.0f);
					glRotatef(-wheelAngle*steerRatio, 1.0f, 0.0f, 0.0f);
					partVector[i].RenderPartAsIs();
				
				glPopMatrix();	
				*/

				glPushMatrix();
					glScalef(partVector[i].GetPartTransformVector(1).x,
							 partVector[i].GetPartTransformVector(1).y,
							 partVector[i].GetPartTransformVector(1).z);
					partVector[i].RenderPartWithMatrixTrans();
				glPopMatrix();
			}
			else
			{
			/*	glTranslatef(partVector[i].GetPartTransformVector(0).x,
							 partVector[i].GetPartTransformVector(0).y,
							 partVector[i].GetPartTransformVector(0).z);
				partVector[i].RenderPartWithVectorTrans();
			*/
				glPushMatrix();
					glScalef(partVector[i].GetPartTransformVector(1).x,
							 partVector[i].GetPartTransformVector(1).y,
							 partVector[i].GetPartTransformVector(1).z);
					partVector[i].RenderPartWithMatrixTrans();
				glPopMatrix();
			}
		glPopMatrix();
	glPopMatrix();

	//glFlush();
}

void CCar::ShowCollisionBox()
{
}		

//calculate car physics and behaviour
void CCar::CarPhysics(float deltaTime)
{
}

//Resets the car values
void CCar::Reset()
{
	position=CVector3(0.0f, 0.0f, 0.0f);
	velocity=CVector3(0.0f, 0.0f, 0.0f);
	angle=0;
	steerAngle=0;
	steerAngleRate=0;
	steerRatio=20;
	wheelAngle=0;
	throttle=0;
	brake=0;
	rSpeed=0;
	carYaw=0;
	carPitch=0;
	carRoll=0;
	accel=0;
	cameraIndex=0;

}

//Returns the name of the car
const string& CCar::GetCarName() const
{
	if(carName.empty())
		cout<< "???? (missing model identifier)"<<endl;
	return carName;
}

//Set up the camera positions
void CCar::SetCameraPositions(const CVector3 cam1, const CVector3 cam2)
{
	cameraPositions.push_back(cam1);
	cameraPositions.push_back(cam2);
}

//Returns the camera positions
const vector<CVector3>& CCar::GetCameraPositions()const
{
	return cameraPositions;
}

//sets the max steer angle variable
void CCar::SetMaxSteerAngle(float msa)
{
	maxSteerAngle=msa;
}

//sets the steer angle variable
void CCar::SetSteerAngle(float sa)
{
	if(sa> -maxSteerAngle && sa< maxSteerAngle)
		steerAngle=sa;
}

//returns the max steer angle variable
float CCar::GetMaxSteerAngle() const
{
	return maxSteerAngle;
}

//returns the steer angle variable
float CCar::GetSteerAngle() const
{
	return steerAngle;
}

//returns the cameraIndex for determining the current camera selection
int CCar::GetCameraIndex() const
{
	return cameraIndex;
}

//increments the cameraIndex which determines the current camera selection
void CCar::NextCameraPosition()
{
	if(cameraIndex>numberOfCameras-1)
		cameraIndex=0;
	else
		cameraIndex++;
}

//retun description pic filename
const string& CCar::GetDescriptionPicName() const
{
	return descriptionPic;
}

//set the car's position at center of gravity
void CCar::SetPosition(const CVector3 p)
{
	position=p;
}

//return the car's position vector
const CVector3& CCar::GetPosition() const
{
	return position;
}

//return a vector of indeces for the car body parts
const vector<int>& CCar::GetBodyPanelVecIndex() const
{
	return bodyPanelVecIndex;
}

//return a vector of indeces for the car body parts
const vector<int>& CCar::GetTireVecIndex() const
{
	return tireVecIndex;
}

//return a vector of indeces for the car body parts
const vector<int>& CCar::GetSteeringWheelVecIndex() const
{
	return steeringWheelVecIndex;
}

//set velocity vector
void CCar::SetVelocity(const CVector3 v)
{
	velocity=v;
}

//return velocity vector
const CVector3& CCar::GetVelocity()  const
{
	return velocity;
}

//render bounding box
void CCar::DrawBoundingBox()
{
	gfxUtils.DrawBox(boundingBox.x, boundingBox.y, boundingBox.z);
}

//render bounding cylinder
void CCar::DrawBoundingCyl()
{
	gfxUtils.DrawCylinder(boundingCyl.y, boundingCyl.z);
}

//set bounding box vector
void CCar::SetBoundingBox(const CVector3 bBox)
{
	boundingBox=bBox;
}

//get bounding box vector
const CVector3& CCar::GetBoundingBox() const
{
	return boundingBox;
}

//set bounding cylinder vector
void CCar::SetBoundingCyl(const CVector3 bCyl)
{
	boundingCyl=bCyl;
}

//get bounding cylinder vector
const CVector3& CCar::GetBoundingCyl() const
{
	return boundingCyl;
}

//returns the drive terrain type of the car 0-RWD 1-FWD 2-AWD
unsigned int CCar::GetDriveTerrain() const
{
	return driveTerrain;
}

//set the drive terrain of the car
void CCar::SetDriveTerrain(unsigned int dt)
{
	driveTerrain=dt;
}

//set max torque
void CCar::SetMaxTorque(float maxt)
{
	maxTorque=maxt;
}

//return max torque
float CCar::GetMaxTorque() const
{
	return maxTorque;
}