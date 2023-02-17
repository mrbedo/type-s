///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CInput.h
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
#ifndef CINPUT_H
#define CINPUT_H

//definition for mouse button input array index names
const enum MOUSE_INPUT {L_BUTTON_DOWN, M_BUTTON_DOWN, R_BUTTON_DOWN,
						L_BUTTON_UP,   M_BUTTON_UP,   R_BUTTON_UP,	 
						L_BUTTON_DRAG, M_BUTTON_DRAG, R_BUTTON_DRAG};

class CInput
{
public:
	//////////////////////////////////////////////////////////////////////////////////////+
	// ~CInput()
	// Description:
	//    Destructor
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	~CInput(){};

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
	static CInput* Instance();

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
	void SetKey(int key, bool down);

	//////////////////////////////////////////////////////////////////////////////////////+
	// SetMouseButton()
	// Description:
	//    Sets the mouse array by button index a value of up or down
	//    and stores the currently pressed mouse button
	// Parameters:
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
	void SetMouseButton(int button, bool down);

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
	void SetMouseCoords(int x, int y);

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
	bool Mouse(int m);

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
	bool Key(int k);

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
	int GetKey();

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
	int GetMouseButton();

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
	int GetMouseX();

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
	int GetMouseY();

private:
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
	CInput();

	int currentKeyPressed;											//stores the current pressed key
	int currentMouseButtonPressed;									//stores the current pressed mouse button
	bool keyInput[256];												//keeps track which key was pressed, true= pressed, false= released
	bool mouseInput[9];												//keeps track which mouse button was pressed true=pressed, false= released
	int  mouseCoordX;												//keeps track of mouse X Coordinate
	int  mouseCoordY;												//keeps track of mouse Y Coordinate
};
#endif
