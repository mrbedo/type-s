///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTypeS.cpp
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
//  The common game class descriptor, various game classes will inherit this class
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include"ctypes.h"
#include "../engine/CBitmap.h"
#include <iostream>

using namespace std;

//constructor
CTypeS::CTypeS()
{
}

//destructor
CTypeS::~CTypeS()
{
}

//clean up
int CTypeS::TopCleanUp()
{
	cout<<"\nCleanUP -   CTypeS...";
	
	cout<<"[done]";	
	return true;
}

//toggle text display
void CTypeS::ToggleOutputDisplay()
{
		showText=!showText;
}

//scroll display data up
void CTypeS::ScrollOutputUp()
{
	scrollplace-=0.002f;
}

//scroll display data down
void CTypeS::ScrollOutputDown()
{
	scrollplace+=0.002f;				
}

//set mouse
void CTypeS::MoveMouse(bool flag)
{
	useMouse= flag;
}

//move the camera in desired direction
void CTypeS::MoveCamera(CVector3 velocity)
{
/*	//Limit X travel
	if(c.GetPosition().x > 54)
	{
		velocity.x = 0;
		c.SetPosition(CVector3(54, c.GetPosition().y, c.GetPosition().z));
	}
	else
	if(c.GetPosition().x < -54)
	{
		velocity.y = 0;
		c.SetPosition(CVector3(-54, c.GetPosition().y, c.GetPosition().z));
	}

	//Limit Y travel
	if(c.GetPosition().y > 8)
	{
		velocity.y = 0;
		c.SetPosition(CVector3(c.GetPosition().x, 8, c.GetPosition().z));
	}
	else
	if(c.GetPosition().y < 1)
	{
		velocity.y = 0;
		c.SetPosition(CVector3(c.GetPosition().x, 1, c.GetPosition().z));
	}

	//Limit Z travel
	if(c.GetPosition().z > 54)
	{
		velocity.z = 0;
		c.SetPosition(CVector3(c.GetPosition().x, c.GetPosition().y, 54));
	}
	else
	if(c.GetPosition().z < -54)
	{
		velocity.z = 0;
		c.SetPosition(CVector3(c.GetPosition().x, c.GetPosition().y, -54));
	}
*/
	c.SetVelocity(CVector(velocity.x, velocity.y, velocity.z));
}

//how fast the camera moves
void CTypeS::SetCameraSpeed(float speed)
{
	c.SetSpeed(speed,true);
}

//return camera speed
float CTypeS::GetCameraSpeed()
{
	return c.GetSpeed();
}

//reset the camera to original coordinats
void CTypeS::ResetCamera()
{
	c.ResetCamera();
}

//loading screen
void CTypeS::LoadingScreen()
{
	glClearColor(0,0,0,0);									// Background Color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glLoadIdentity();										// Reset The Current Modelview Matrix
	CBitmap bmp;
	bmp.LoadGLTexture("menu/typeSloadingscreen.bmp");
	bmp.DrawBMPTexture(CVector3( 0.00f, 0.00f,-20.70f), CVector3( 10.0f, 7.6f, 1.00f));
	cout<<"LOADING..."<<endl;
	SwapBuffers(hDC);
	glPopAttrib();
}

//update delta time
void CTypeS::UpdateTime()
{
	time.FramesPerSecond();
	
	deltaTime=time.ComputeDeltaTime(10);
}
