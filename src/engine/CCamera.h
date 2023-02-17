#ifndef CAMERA_H
#define CAMERA_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CCamera.h
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
//  This class provides an openGL camera with various functionality
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CInput.h"
#include "CScreenManager.h"
#include "Cvector3.h"
#include "CVector.h"

////////////////////////////////////////////////////
//	Camera Class loosely based on Quake III engine
////////////////////////////////////////////////////

class CCamera
{
private:
//	CWindowsGLWindow myWindow;
	// these are used for moving and changing camera orientation
	// through the MoveTo/LookTo methods
	CVector initPosition, finalPosition;
	CVector initLookAt, finalLookAt;

	CVector lookAtVel;										// velocity for looking at objects
	CVector lookAtAccel;									// acceleration for looking at objects

	void UpdateLookAt();
	void UpdateMoveTo();

	float pitchLimit;
	float speedLimit;
	float strafeLimit;

	int view;												// keeps track of current camera view

	CVector3 insideCarPos;
	CVector3 carHoodPos;
	CVector3 outsideOnePos;
	CVector3 outsideTwoPos;
	float cameraLooseness;

	float cameraSpeed;										// how fast camera reacts to imput
	float cameraSpeedMin;									// miniumum speed for camera
	float cameraSpeedMax;									// maximum speed for camera
	CVector3 position;										// position of camera
	CVector3 localObjectPosition;							// position relative to a current object
	CVector3 followPosition;								// position of camera following a current object
	CVector3 lookAt;										// lookat vector
	CVector3 freeCarCam;									// free camera position within car local coordinates
	CVector velocity;										// velocity of camera
	CVector acceleration;									// acceleration of camera

	CVector up;												// up, forward, right vectors
	CVector forward;
	CVector right;

	float yaw;												// yaw and pitch angles
	float pitch;
	float row;

	bool startingFollowPositionInitalized;					// flag for initalizing the starting position of the followObject camera position

	CScreenManager *screen;
	CInput *input;

public:
	CCamera();
	//Post-Default constructor
	//CCamera(int width, int height) {}
	//Post-
	CCamera(CVector *look);
	//Post-
	CCamera(CVector *pos, CVector *look);
	//Post-
	~CCamera();
	//Post- Destructor
	
	void MatrixCam(const float orientationMatrix[16], const CVector3& camPosition);
	//Pre-  an orientation matrix and a position is passed
	//Post- multiply the current viewmatrix by the orienation matirx and 
	//		translate the entire scene by the camPosition
	//		Note: This is a substitute for glLookAt function

	void SetPosition(CVector3& pos);
	//Pre-  default camera position exists
	//Post- camera position is set to new value

	void RotateYaw(scalar_t radians);
	//Pre-  Camera exists
	//Post- Rotates along the Y axis

	void RotatePitch(scalar_t radians);
	//Pre-  Camera exists
	//Post- Rotates along the X axis
	
	void RotateRoll(scalar_t radians);
	//Pre-  Camera exists
	//Post- Rotates along the Z axis

	void FollowPosition(const CVector3& positoinToFollow, const CVector3& camPosition, float yawRotationAngle, double deltaTime);
	//Pre-  Camera has default gluLookAt values
	//Post- Camera follows specified position from a specific location

	void MovieCam(const CVector3& lookAtPosition, float deltaTime);
	//Pre-  Camera has default gluLookAt values
	//Post- Camera gluLookAt paramters are set to view
	//		the position of the car
	
	void FreeCam(bool overrideLookAt, float deltaTime);
	//Pre-  Camera object exists
	//Post- Camera is in free world coordinates acts as Quake style
	//		if LookAt is not override (false) then LookAt is inputted from mouse
	
	void Mouse(float mouseSensitivity, int x, int y);
	//Pre-  Camera object exists
	//Post- Camera yaw and pitch member variables get
	//		imput from windows mouse by position comparison
	
	void SetView(int v);
	//Pre-  Camera view is not initalized or not selected
	//Post- Cemara view is selected
	
	int GetView() const;
	//Pre-  Camera view exists
	//Post- Returns camera view
	
	void ResetCamera();
	//Pre-  Camera is at some coordinate
	//Post- Camera is reset to original default position
	
	void SetYaw(float newYaw);
	//Pre-  A yaw angle amount exists
	//Post- new angle amount added to current angle
	
	void SetPitch(float newPitch);
	//Pre-  A pitch angle amount exists
	//Post- new angle amount added to current angle
	
	void SetRow(float newRow);
	//Pre-  A row angle amount exists
	//Post- new angle amount added to current angle
	
	void SetVelocity(CVector vel);
	//Pre-  current velocity exists for camera
	//Post- parameter vector is added to velocity
	
	void SetSpeed(float camSpeed, bool update);
	//Pre-  cameraSpeed exists
	//Post- parameter added to cameraSpeed 
	
	void UpdatePosition(CVector3& pos);
	//Pre- position exists
	//Post- parameter added to position
	
	void UpdateLocalObjectPosition(CVector3& pos);
	//Pre-  localObjectPosition exists
	//Post- updates the localObjectPosition by adding to it.
	
	CVector& GetVelocity();
	//Pre-  velocity vector exists
	//Post- returns velocity vector
	
	float GetSpeed() const;
	//Pre-  cameraSpeed exists
	//Post- returns cameraSpeed;
	
	CVector3& GetPosition();
	//Pre- position exists
	//Post- returns position
	
	float GetYaw() const;
	//Pre- yaw exists
	//Post- returns yaw
	
	float GetPitch() const;
	//Pre- pitch exists
	//Post- returns pitch
	
	float GetRow() const;
	//Pre- row exists
	//Post- returns row
};

#endif