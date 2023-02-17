#ifndef CSCREENMANAGER_H
#define CSCREENMANAGER_H

class CScreenManager
{
public:
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
	~CScreenManager();

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
	static CScreenManager* Instance();

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
	void SetResolution(short screenX, short screenY, short colDep);

	//////////////////////////////////////////////////////////////////////////////////////+
	// GetScreenX()
	// Description:
	//    Returns the screen width
	// Parameters:
	//    None
	// Return Value:
	//    int mouseCoordX
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	short GetScreenX();

	//////////////////////////////////////////////////////////////////////////////////////+
	// GetScreenY()
	// Description:
	//    Returns the screen length
	// Parameters:
	//    None
	// Return Value:
	//    int mouseCoordY
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	short GetScreenY();

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
	short GetColorDepth();


	void Set3DViewport(unsigned short width, unsigned short height);
	//Creates an OpenGL 3D Viewport

	void Set2DViewport(unsigned short width, unsigned short height);
	//Creates an OpenGL 2D Viewport

	void SetTopLeftScreenCoords(unsigned short top, unsigned short left);
	//Stores the top left window location coordinates relative to the screen

	short screenPosX;
	short screenPosY;
private:	    
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
	CScreenManager();

	short screenSizeX;
	short screenSizeY;
	short colorDepth;
};
#endif
