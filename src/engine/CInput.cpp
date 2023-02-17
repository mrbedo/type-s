///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CInput.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2003,2004,2005
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  This class provides an interface for keyboard and mouse input
//	It utilizes a singleton pattern, there can only be one instance
//	at any time of this class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "CInput.h"


//////////////////////////////////////////////////////////////////////////////////////+
// CInput()
// Description:
//    Constructor
// Parameters:
//    None
// Return Value:
//    None
// Implementation Notes
//    Private to prevent multiple instances of the CInput object
//	  thus enabling global use of object
//////////////////////////////////////////////////////////////////////////////////////-
CInput::CInput():currentKeyPressed(-9999){}

//////////////////////////////////////////////////////////////////////////////////////+
// Instance()
// Description:
//    Returns the instance of the input object
// Parameters:
//    None
// Return Value:
//    CInput*
// Implementation Notes
//    Use this method to initialize all new pointers to this object
//	  so that there is only a single instance of that object, making it global
//////////////////////////////////////////////////////////////////////////////////////-
CInput* CInput::Instance()
{
	static CInput input;
	return &input;
}

//////////////////////////////////////////////////////////////////////////////////////+
// SetKey()
// Description:
//    Sets the keyboard array by key index a value of up or down
//    and stores the currently pressed key
// Parameters:
//    int key - the value of the key pressed
//	  bool down - determine if the key is pressed.  Set this flag to false to create
//				  a pause before autorepeat kicks in.  That way if the key is held
//				  down continously for more than a second, only a single input will be
//				  processed.
//				  A true value indicates that there is an input processed each iteration
//				  through the main loop.
//				  This flag is stored in the keyInput Array where the index corresponds
//				  to the int key value.
// Return Value:
//    None
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
void CInput::SetKey(int key, bool down)
{
	if(!down)
		currentKeyPressed= -9999;
	else
		currentKeyPressed=key;
	
	if(key>=0 && key<256)
		keyInput[key]=down;
}

//////////////////////////////////////////////////////////////////////////////////////+
// SetMouseButton()
// Description:
//    Sets the mouse array by button index a value of up or down
//    and stores the currently pressed mouse button
// Parameters:
//	  int x - x screen coordinate of mouse
//	  int y - y screen coordinate of mouse
//    int button - the button ID of the mouse, use the MOUSE_INPUT enumeration above
//				   to set this parameter
//    bool down -  sets the mouseInput array flag for determining of the key is held
//				   or not.  If false, the currentMouseButtonPressed ID is set
//				   to -9999
// Return Value:
//    None
// Implementation Notes
//    Due to the nature of the mouse inputs, since there is
//	  a separate int button ID which identifies a held mouse button
//	  this parameter is not as useful as in the SetKey function for
//	  the keyboard input.
//////////////////////////////////////////////////////////////////////////////////////-
void CInput::SetMouseButton(int button, bool down)
{
	currentMouseButtonPressed=button;
	if(!down) currentMouseButtonPressed= -9999;
	mouseInput[button]=down;
}

//////////////////////////////////////////////////////////////////////////////////////+
// SetMouseCoords()
// Description:
//    Sets the mouse X and Y coordinate variables
// Parameters:
//	  int x - x screen coordinate of mouse
//	  int y - y screen coordinate of mouse
// Return Value:
//    None
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
void CInput::SetMouseCoords(int x, int y)
{
	mouseCoordX=x;
	mouseCoordY=y;
}

//////////////////////////////////////////////////////////////////////////////////////+
// GetKey()
// Description:
//    Returns the currently pressed key
// Parameters:
//    None
// Return Value:
//    int currentKeyPressed
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
int CInput::GetKey()
{
	return currentKeyPressed;
}

//////////////////////////////////////////////////////////////////////////////////////+
// Key()
// Description:
//    Returns the state of the specified key index
// Parameters:
//    int k - the ID of the keyboard key being tested
// Return Value:
//    Returns true if the parameter specified key is the currently pressed key
// Implementation Notes
//	  This should be used when more than one key is pressed at a time
//////////////////////////////////////////////////////////////////////////////////////-
bool CInput::Key(int k)
{
	if(k>=0 && k<256)
		return keyInput[k];
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////+
// Mouse()
// Description:
//    Returns the state of the specified mousebutton index
// Parameters:
//    int m - the ID of the mouse button being tested
// Return Value:
//    Returns true if the parameter specifed button is the currently pressed mouse button
// Implementation Notes
//	  This should be used when more than one button is pressed at a time
//	  On some systems, creates conflicts on which button is pressed
//////////////////////////////////////////////////////////////////////////////////////-
bool CInput::Mouse(int m)
{
	if(m>=0 && m<9)
		return mouseInput[m];
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////+
// GetMouseButton()
// Description:
//    Returns the currently pressed mouse button
// Parameters:
//    None
// Return Value:
//    int currentMouseButtonPressed
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
int CInput::GetMouseButton()
{
	return currentMouseButtonPressed;
}

//////////////////////////////////////////////////////////////////////////////////////+
// GetMouseX()
// Description:
//    Returns the mouse X coordinate
// Parameters:
//    None
// Return Value:
//    int mouseCoordX
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
int CInput::GetMouseX()
{
	return mouseCoordX;
}

//////////////////////////////////////////////////////////////////////////////////////+
// GetMouseY()
// Description:
//    Returns the mouse Y coordinate
// Parameters:
//    None
// Return Value:
//    int mouseCoordY
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
int CInput::GetMouseY()
{
	return mouseCoordY;
}
