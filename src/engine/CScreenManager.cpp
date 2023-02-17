
#include "CScreenManager.h"
#include "../OpenGLHeader.h"

//////////////////////////////////////////////////////////////////////////////////////+
// CScreenManager()
// Description:
//    Constructor
// Parameters:
//    None
// Return Value:
//    None
// Implementation Notes
//    Private to prevent multiple instances of the CScreenManager object
//	  thus enabling global use of object
//////////////////////////////////////////////////////////////////////////////////////-
CScreenManager::CScreenManager()
{
	//screenSizeX=1;
	//screenSizeY=1;
	//colorDepth=16;
}

//////////////////////////////////////////////////////////////////////////////////////+
// ~CScreenManager()
// Description:
//    Destructor
// Parameters:
//    None
// Return Value:
//    None
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
CScreenManager::~CScreenManager()
{
}

//////////////////////////////////////////////////////////////////////////////////////+
// Instance()
// Description:
//    Returns the instance of the input object
// Parameters:
//    None
// Return Value:
//    CScreenManager*
// Implementation Notes
//    Use this method to initialize all new pointers to this object
//	  so that there is only a single instance of that object, making it global
//////////////////////////////////////////////////////////////////////////////////////-
CScreenManager* CScreenManager::Instance()
{
	static CScreenManager screen;
	return &screen;
}

//////////////////////////////////////////////////////////////////////////////////////+
// SetResolution()
// Description:
//    Sets the keyboard array by key index a value of up or down
//    and stores the currently pressed key
// Parameters:
//    short screenX - value for screen width
//	  short screenY - value for screen length
//	  short colDep  - value for color depth
// Return Value:
//    None
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
void CScreenManager::SetResolution(short screenX, short screenY, short colDep)
{
	screenSizeX=screenX;
	screenSizeY=screenY;
	colorDepth =colDep;
}

//////////////////////////////////////////////////////////////////////////////////////+
// GetScreenX()
// Description:
//    Returns the screen width
// Parameters:
//    None
// Return Value:
//    short mouseCoordX
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
short CScreenManager::GetScreenX()
{
	return screenSizeX;
}

//////////////////////////////////////////////////////////////////////////////////////+
// GetScreenY()
// Description:
//    Returns the screen length
// Parameters:
//    None
// Return Value:
//    short mouseCoordY
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
short CScreenManager::GetScreenY()
{
	return screenSizeY;
}

//////////////////////////////////////////////////////////////////////////////////////+
// GetColorDepth()
// Description:
//    Returns the color depth
// Parameters:
//    None
// Return Value:
//    short colorDepth
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
short CScreenManager::GetColorDepth()
{
	return colorDepth;
}

//Creates a 3D viewport
void CScreenManager::Set3DViewport(unsigned short width, unsigned short height)
{
	if (!height) height=1;								// prevent divide by zero

	glViewport(0,0,width,height);							// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
	glLoadIdentity();										// Reset The Projection Matrix
															// Calculate The Aspect Ratio Of The Window
	gluPerspective(40.0f,width/(float)height,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
	glLoadIdentity();										// Reset The Modelview Matrix
}

//Create a 2D viewport
void CScreenManager::Set2DViewport(unsigned short width, unsigned short height)
{
	glViewport(0,0,width,height);							// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
	glLoadIdentity();										// Reset The Projection Matrix
															// Calculate The Aspect Ratio Of The Window
	gluOrtho2D (0, width, height, 0 );						//position(0,0) is top left corner
	glMatrixMode(GL_MODELVIEW);				
	glLoadIdentity();										// Reset The Modelview Matrix
}

//Store the top left window coordinates relative to the screen
void CScreenManager::SetTopLeftScreenCoords(unsigned short top, unsigned short left)
{
	screenPosX=left;
	screenPosY=top;	
}
