///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CCamera.cpp
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

#include "CCamera.h"
#include "../OpenGLHeader.h"

//constructor
CCamera::CCamera():
screen(CScreenManager::Instance()),
input(CInput::Instance()),

view(0),												//inital view 0(inside car 1(free cam

insideCarPos	(CVector3( 1.50f, 1.77f,-0.80f)),
carHoodPos	    (CVector3( 2.70f, 1.77f, 0.00f)),
outsideOnePos	(CVector3(-14.0f, 3.00f, 0.00f)),			//(-7.00f, 4.00f, 0.00f)),
outsideTwoPos   (CVector3(-19.0f, 4.00f, 0.00f)),
freeCarCam		(CVector3( 0.00f, 0.00f, 0.00f)),

cameraLooseness (0.0127f),
startingFollowPositionInitalized(false),

position	(CVector3(0, 0, 0)),
lookAt		(CVector3(0, 0, 0)),

forward		(lookAt),
up			(CVector(0.0, 1.0, 0.0)),
right		(CVector(1.0, 0.0, 0.0)),

velocity	(CVector(0.0, 0.0, 0.0)),
acceleration(CVector(0.0, 0.0, 0.0)),

yaw			(-33.0f),
pitch		(-10.0f),
row			( 0.0f),

cameraSpeed	(1.9f),
cameraSpeedMin(0.0f),
cameraSpeedMax(10.000f),
pitchLimit	(20.0f),
speedLimit	(15.0f),
strafeLimit	(15.0f)
{}

//resets camera to default coordinates
void CCamera::ResetCamera()
{
	position = CVector3(0.0, 0.0, 0.0);
	lookAt = CVector3(0.0, 0.0, 0.0);

	forward = lookAt;
	up = CVector(0.0, 1.0, 0.0);
	right = CVector(1.0, 0.0, 0.0);

	velocity = CVector(0.0, 0.0, 0.0);
	acceleration = CVector(0.0, 0.0, 0.0);

	yaw		=0.0f;
	pitch	=0.0f;
	row		=0.0f;

	startingFollowPositionInitalized=false;
}

//Destructor
CCamera::~CCamera()
{
}

//Set camera with a matrix transformation
void CCamera::MatrixCam(const float orientationMatrix[16], const CVector3& camPosition)
{
	position = camPosition;
	glLoadMatrixf(orientationMatrix);						//replace the current matrix with the orientation matrix
	
	glRotatef(90,0,1,0);
	glTranslatef (-camPosition.x, -camPosition.y, -camPosition.z);
}

//Set the camera position
void CCamera::SetPosition(CVector3& pos)
{
	position=pos;
}

//free camera
void CCamera::MovieCam(const CVector3& lookAtPosition, float deltaTime)
{
     lookAt=lookAtPosition;									//custom LookAt position (lock to car position)
	 FreeCam(true, deltaTime);								//enable LookAt override in FreeCam
}

//free camera
void CCamera::FreeCam(bool overrideLookAt, float deltaTime)
{
	 if		 ( yaw >=  360.0f) yaw-= 720;
	 else if ( yaw <= -360.0f) yaw+= 720;					//reset 360degree cycle

	 if		 (cameraSpeed<=cameraSpeedMin) cameraSpeed=cameraSpeedMin;
	 else if (cameraSpeed>=cameraSpeedMax) cameraSpeed=cameraSpeedMax;

     if		 (pitch >  pitchLimit) pitch =  pitchLimit;
     else if (pitch < -pitchLimit) pitch = -pitchLimit;
     
	 float cosYaw  = cos(DEG2RAD(yaw));
     float sinYaw  = sin(DEG2RAD(yaw));
     float sinPitch= sin(DEG2RAD(pitch));

     float speed = velocity.x* deltaTime;
     float strafeSpeed = velocity.z* deltaTime;

     if		 (speed >  speedLimit) speed =  speedLimit;
	 else if (speed < -speedLimit) speed = -speedLimit;


     if		 (strafeSpeed >  strafeLimit) strafeSpeed = strafeLimit;
     else if (strafeSpeed < -strafeLimit) strafeSpeed = -strafeLimit;

     if (velocity.Length() > 0.0)						
          acceleration = -velocity * 9.0f;

     velocity += acceleration*deltaTime;	
															//calculate position

     position.x += cos(DEG2RAD(yaw + 90.0))*strafeSpeed;
     position.z += sin(DEG2RAD(yaw + 90.0))*strafeSpeed;
     position.x += cosYaw*speed;
     position.z += sinYaw*speed;
	 position.y += velocity.y*deltaTime;

	 if(!overrideLookAt)
	 {
	    lookAt.x = position.x + cosYaw;
	    lookAt.y = position.y + sinPitch;
	    lookAt.z = position.z + sinYaw;
	 }

     gluLookAt(position.x, position.y, position.z,
			   lookAt.x, lookAt.y, lookAt.z,
			   0.0, 1.0, 0.0);

}

//camera position overwritten by specific coordinates
void CCamera::FollowPosition(const CVector3& positionToFollow, const CVector3& cameraPosition, float yawRotationAngle, double deltaTime)
{
	followPosition= positionToFollow+localObjectPosition;		//align with coupe interrior

	if(!startingFollowPositionInitalized)					//if position has not been initalized,
	{														//set it to initial starting value
		localObjectPosition=cameraPosition;
		startingFollowPositionInitalized=true;				//mark the flag
	}

	yaw= yawRotationAngle;

	float cosYaw  = cos(DEG2RAD(-yaw));
    float sinYaw  = sin(DEG2RAD(-yaw));
    float sinPitch= sin(DEG2RAD(pitch));
 
	if (velocity.Length() > 0.0)							//calculate acceleration amount for position						
         acceleration = -velocity * 5.0f;
 
	velocity += acceleration*deltaTime;	
	localObjectPosition.x+=velocity.x*deltaTime;	  
	localObjectPosition.y+=velocity.y*deltaTime;	  
	localObjectPosition.z+=velocity.z*deltaTime;	  

    followPosition.x=cosYaw
			  +positionToFollow.x
			  +(localObjectPosition.x*cosYaw)							//calculate camera position for x axis
			  -(localObjectPosition.z*sinYaw)							//relative to the cooridnates followed position
			  ;
	followPosition.z=sinYaw
		      +positionToFollow.z					
			  +(localObjectPosition.x*sinYaw)							//calculate camera position for x axis
			  +(localObjectPosition.z*cosYaw)							//relative to the car position
			  ;

	position.x=followPosition.x+ cosYaw;
	position.y=followPosition.y+ sinPitch;
	position.z=followPosition.z+ sinYaw;

	gluLookAt(followPosition.x,
			  followPosition.y,
			  followPosition.z,								//lock lookAt to car's x and z positions
              position.x, 
			  position.y, 
			  position.z,
              0.0, 1.0, 0.0);

	//position=positionToFollow;
}

//camera view is controlled by mouse
void CCamera::Mouse(float mouseSensitivity, int CursorPosX, int CursorPosY)
{
	if (mouseSensitivity >= 1.0f)
		mouseSensitivity = 1.0f;
	if (mouseSensitivity <= 0.0f)
		mouseSensitivity = 0.0f;

	if (yaw > 360.0f)
		yaw -= 360.0f;
	if (yaw < 0.0f)
		yaw += 360.0f;

	if (pitch > pitchLimit)									//cap camera pitch
		 pitch= pitchLimit;
	if (pitch < -pitchLimit)
		 pitch= -pitchLimit;
     
	POINT mousePos;											// This is a window structure that holds an X and Y

	int middleX= screen->GetScreenX() >> 1;							// This is a binary shift to get half the width
	int middleY= screen->GetScreenY() >> 1;							// This is a binary shift to get half the height

	mousePos.x= input->GetMouseX();
	mousePos.y= input->GetMouseY();

	SetCursorPos(middleX, middleY);

	float diffX= mousePos.x - middleX;
	float diffY= mousePos.y - middleY;
	
	
	if( diffX== 0)											// if mouse is at a stand still
	{
		return;
	}
	else
	{
		if (diffX > 0)											// compare mouse position with center and update
			yaw += (diffX * mouseSensitivity);					// based on how fast the mouse moved away from 
		if (diffX < 0)											// the center and control it with the mouse sensitivity
			yaw += (diffX * mouseSensitivity);
		if (diffY < 0)
			pitch-= (diffY * mouseSensitivity);
		if (diffY > 0)
			pitch-= (diffY * mouseSensitivity);
	}
}


//sets the amount of yaw  (rot along Y-axis)
void CCamera::SetYaw(float newYaw)
{
	yaw+=newYaw;
}

//sets the amount of pitch (rot along Z-axis)
void CCamera::SetPitch(float newPitch)
{
	pitch+=newPitch;
}

//sets the amount of rolling (rot along X-axis)
void CCamera::SetRow(float newRow)
{
	row+=newRow;
}

//gets the amount of yaw  (rot along Y-axis)
float CCamera::GetYaw() const
{
	return yaw;
}

//gets the amount of pitch (rot along Z-axis)
float CCamera::GetPitch() const
{
	return pitch;
}

//gets the amount of rolling (rot along X-axis)
float CCamera::GetRow() const
{
	return row;
}

//sets view
void CCamera::SetView(int v)
{
	view=v;
}

//gets view
int CCamera::GetView() const
{
	return view;
}

//sets new velocity
void CCamera::SetVelocity(CVector vel)
{
	velocity+=vel;
}

//sets the camera speed
void CCamera::SetSpeed(float camSpeed, bool update)
{
	if(update)
		cameraSpeed+=camSpeed;
	else
		cameraSpeed=camSpeed;
}

//updates position
void CCamera::UpdatePosition(CVector3& pos)
{
	position+=pos;
}

//adds to the current local object position 
void CCamera::UpdateLocalObjectPosition(CVector3& pos)
{
	localObjectPosition+=pos;
}

//returns velocity
CVector& CCamera::GetVelocity()
{
	return velocity;
}

//returns camera speed
float CCamera::GetSpeed() const
{
	return cameraSpeed;
}

//returns camera position vector
CVector3& CCamera::GetPosition()
{
	return position;
}
