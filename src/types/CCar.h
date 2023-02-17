#ifndef CCAR_H
#define CCAR_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CCar.h
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

#include "../engine/cmodel.h"
#include "../engine/CGraphicUtils.h"
#include <vector>
#include <string>


class CCar: public CModel
{
private:
	std::string descriptionPic;
	std::string carName;											//stores the name of the car
	CGraphicUtils gfxUtils;

	int carBodyDL,
		wheelDL,
		steeringWheelDL;
		 
	bool wheelListExists;									// (false);
	bool carListExists;										// (false);

	CVector3 boundingBox;									// size in x,y,z direction
	CVector3 boundingCyl;									// x= axis (0=x, 1=y, 2=z), y=radius, z=length
		  

	CVector3 velocity,										//velocity in World Coordinates
			position;										//position in World Coordinates
			
	float	tireRadius,										//stores the radius of the tire
			carYaw,											//yaw angle of car
			carPitch,										//pitch angle of car
			carRoll,
			maxSteerAngle,
			steerAngle,										//angle of steering		 (input)
			steerAngleRate,
			steerRatio,
			oldSteerAngle,
			diffSteerAngle,
			wheelRotationConstant,							// =(2PI*tireRadius/360) for correct wheel rotation representation
			rotAngle,										//angle of rotation
			rSpeed,											//speed of wheel rotation
			wheelAngle,
			oldWheelAngle,
			newWheelAngle,
			angle,											//angle of car body orientation in rads
			maxTorque,										//the maximum allowable torque
			throttle,										//amout of gas applied	 (input)
			brake;											//amout of break applied (input)

	int accel;												//flag for going forwards/backwards

	std::vector<CVector3> cameraPositions;						//contains a list of camera position that the car has
	int numberOfCameras;
	int cameraIndex;

	std::vector<int> bodyPanelVecIndex;							//stores parts based on body panel identifier
	std::vector<int> tireVecIndex;								//stores parts based on tire identifier
	std::vector<int> steeringWheelVecIndex;						//stores parts based on steeringWheel identifier
	
	unsigned int driveTerrain;								//the drive terrain type of the car

public:														//acquired from relative tire position
	CCar(){}
	//Post- default constructor

	~CCar();
	//Post- Destructor

	int InitModels();
	int Init(const std::string& tsiFileName);
	//Pre-	A car object exists
	//Post- Initalizes car parameters

	void GenerateCarBodyDL();
	//Pre-  Car body model exists
	//Post- Stores static translations and scales and geometry in a display list

	void GenerateWheelDL();
	//Pre-  Wheel model exists
	//Post- Stores static translations and scales and geometry in a display list

	void GenerateSteeringWheelDL();
	//Pre-  SteeringWheel model exists
	//Post- Stores static translations and scales and geometry in a display list

	void DrawBody(unsigned int i) const;
	//Pre-  Car body does not exist
	//Post- Polygon indication for car body

	void DrawWheel(unsigned int partIndex) const;
	//Pre-  Wheel body does not exist
	//Post- Wheels are renderd to the screen

	void CarPhysics(float deltaTime);
	//Pre-  Car body and variables are initalized
	//Post- Car behavious is calculated
	//CLoad3DS model;

	void RenderWheelModel(const C3DModel &model);
	//Pre-  3d model object exists
	//Post- 3d model object rendered with display lists

	void RenderCarModel();
	//Pre-  3d model object exists
	//Post- 3d model object rendered without display lists

	void Reset();
	//Pre-	3d model exists with member variables at some value
	//Post- member variables are reset to default values

	void SetVelocity(const CVector3 v);
	//Pre-	Velocity variable
	//Post- Sets the velocity vector

	const CVector3& GetVelocity() const;
	//Pre-  Velocity exists
	//Post-	Returns velocity vector

	const std::string& GetCarName() const;
	//Pre-  carName may or may not exists
	//Post- if carName exists, returns it, else returns default message

	void SetMaxSteerAngle(float msa);
	//Pre-  infinite max steer angle exists, so wheels are allowed to rotate freely
	//Post- sets new max steer angle

	float GetMaxSteerAngle() const;
	//Pre-  maximum steer angle exists
	//Post- returns maximum steer angle

	void SetSteerAngle(float sa);
	//Pre-  steerAngle has some value
	//Post- new value is assigned to steerAngle
	
	float GetSteerAngle() const;
	//Pre-  steerAngle exists
	//Post- returns it;

	void SetCameraPositions(const CVector3 cam1, const CVector3 cam2);
	//Pre-  default values exists for camera 
	//Post- camera will use specified position values when being attached to the car

	const std::vector<CVector3>&  GetCameraPositions() const;
	//Pre-  camera positions vector exists
	//Post- returns the camera positions vector

	int  GetCameraIndex() const;
	//Pre-  camera position index exists
	//Post- camera index is returned

	void NextCameraPosition();
	//Pre-  currentCameraPosition exists
	//Post- currentCameraPosition is now the next item in the camera positions vector

	void ShowCollisionBox();
	//Pre-  collision box exists
	//Post- displays the collision box

	std::vector<float*>& GetCollisionBoxVector();
	//Pre-  a collsion box vector contains arrays of properly ordered verteces for a collision box
	//Post- returns a vector of vertex arrays, so that they can be added to a collision model starting from index 0, to size();
	
	const std::string& GetDescriptionPicName() const;
	//Pre-  description pic name exists
	//Post- returns string of the filename of the description pictures.
	
	void SetPosition(const CVector3 p);
	//Pre-  Position variable
	//Post- Sets the position vector
	
	const CVector3& GetPosition() const;
	//Pre-	Position exists
	//Post- Returns Position vector

	const std::vector<int>& GetBodyPanelVecIndex() const;
	const std::vector<int>& GetTireVecIndex()const;
	const std::vector<int>& GetSteeringWheelVecIndex()const;

	void DrawBoundingBox();
	//Pre-  boundingBox vector with specified size exists
	//Post- draws the bounding box to the screen

	void SetBoundingBox(const CVector3 bBox);
	//Pre-  boundingBox default vector exists
	//Post- new value set to boundingBox

	const CVector3& GetBoundingBox() const;
	//Pre-  boundingBox vector exists
	//Pre-  returns boundingBox vector
	
	void DrawBoundingCyl();
	//Pre-  boundingCyl vector with specified size exists
	//Post- draws the bounding box to the screen

	void SetBoundingCyl(const CVector3 bCyl);
	//Pre-  boundingBox default vector exists
	//Post- new value set to boundingBox

	const CVector3& GetBoundingCyl() const;
	//Pre-  boundingBox vector exists
	//Pre-  returns boundingBox vector

	void SetDriveTerrain(unsigned int dt);
	//Pre-  driveTerrain may be set to some default value
	//Post- sets the drive terrain type of the car
	//		0= Rear  Wheel Drive
	//		1= Front Wheel Drive
	//		2= Four  Wheel Drive

	unsigned int GetDriveTerrain() const;
	//Pre-  driveTerrain is initalized
	//Post- returns driveTerrain
	//		0= Rear  Wheel Drive
	//		1= Front Wheel Drive
	//		2= Four  Wheel Drive

	void SetMaxTorque(float maxt);
	//Pre-  max torque at some value
	//Post- new value for maximum torque

	float GetMaxTorque() const;
	//Pre-  max torque exists
	//Post- return max torque
};
#endif