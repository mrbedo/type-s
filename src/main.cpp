#include "types/ctypesmanager.h"
#include <iostream>
using namespace std;
///////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************//
//*      Type-S: Driving Skills required Advanced Car Simulator					*//
//*  Development by SkillTeam under supervision of Bedoworks Visual Studios SS.	*//
//*                           (c)2002-2003										*//
//*		'based on the simple framework of NeheGL http://nehe.gamedev.net		*//
//*******************************************************************************//
///////////////////////////////////////////////////////////////////////////////////

//---windows set up----------------------------------------------------------------
HDC			hDC=NULL;										// Private GDI Device Context
HGLRC		hRC=NULL;										// Permanent Rendering Context
HWND		hWnd=NULL;										// Holds Our Window Handle
HINSTANCE	hInstance;										// Holds The Instance Of The Application

CTypeSManager typeSManager;
CScreenManager *screen;
CInput *input;

int		param;
bool    debug= false;												
bool	active=true;										// Window Active Flag Set To true By Default
bool	fullscreen=true;									// Fullscreen Flag Set To Fullscreen Mode By Default
bool	loadingScreen=true;

CBitmap bmp; //put it somewhere meaningful

//---functions---------------------------------------------------------------------
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		// Declaration For WndProc

int InitScreen()
{
	bool success = true;
	CTSIFile tsi;
	
	screen=CScreenManager::Instance();

	screen->SetResolution((short)tsi.GetTSIFloat("core/config.tsi","SCREEN","resolutionX"),
						  (short)tsi.GetTSIFloat("core/config.tsi","SCREEN","resolutionY"),
						  (short)tsi.GetTSIFloat("core/config.tsi","SCREEN","colorDepth" ));
//	screen->zBufferBits=  (short)tsi.GetTSIFloat("core/config.tsi","SCREEN","zBuffBits" ));

	if(screen->GetScreenX() < 640 || screen->GetScreenY() < 480)
	{
		screen->SetResolution(640,480,16);
		success = false;
	}
	else
	{
		cout<<"resolution = "<<screen->GetScreenX()<<"x"<<screen->GetScreenY()<<" color depth: "<<screen->GetColorDepth()<<endl;
	}

	return success;
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)			// Resize And Initialize The GL Window
{
	screen->Set3DViewport(width, height);
}

//initalize openGL members
int InitGL()												// All Setup For OpenGL Goes Here
{
														cout<<"[standard]";
	//Perspective, Shading, Depth, Culling ----------------------------------------------------
	//glEnable(GL_POLYGON_SMOOTH);							// Enable Antialiasing
//	glEnable(GL_POLYGON_OFFSET_FACTOR);
//	glEnable(GL_POLYGON_OFFSET_UNITS);
//	glEnable(GL_POLYGON_OFFSET_POINT);
//	glEnable(GL_POLYGON_OFFSET_LINE);

//	glEnable(GL_CLIP_PLANE0);
//	glEnable(GL_CLIP_PLANE1);
//	glEnable(GL_CLIP_PLANE2);
//	glEnable(GL_CLIP_PLANE3);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_SCISSOR_BOX);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);									// Enables Culling
	glEnable(GL_DEPTH_TEST);								// Enables Depth Testing
//	glEnable(GL_TEXTURE_2D);
		
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		// Really Nice Perspective Calculations
	glClearDepth(1.0f);										// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);									// The Type Of Depth Testing To Do
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);								// Enable Smooth Shading

	//glScissor();
	//Lighting-------------------------------------------------------------------------------
														cout<<"[lighting]";
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.5);

	float LightAmbient[3]={0.01f, 0.01f, 0.02f};
	float LightDiffuse[3]={0.97f, 0.97f, 0.96f};
	float LightPosition[3]={1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);									// Turn on lighting
	glEnable(GL_LIGHT1);									// Turn on a light with defaults set
	glEnable(GL_COLOR_MATERIAL);							// Allow color
														cout<<"[fog]";
	//Fog----------------------------------------------------------------------------------------
	float fogColor[4] = {0.74f, 0.74f, 0.7f, 1.0f};			// Let's make the Fog Color GREY
	glEnable(GL_FOG);										// This enables our OpenGL Fog
	glFogi(GL_FOG_MODE, GL_EXP2);							// Assign the fog mode to EXP2 (realistic)
	glFogfv(GL_FOG_COLOR, fogColor);						// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.003f);							// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);						// The Fog's calculation accuracy
	glFogf(GL_FOG_START, 0);								// Fog Start Depth
	glFogf(GL_FOG_END, 500.0f);								// Fog End Depth
														cout<<"\nOpenGL Initalized!";
	return true;
}

//initalizes program objects and variables
int InitProgram()
{
	typeSManager.SetHDC(hDC);
	typeSManager.Init();

	input= CInput::Instance();

	bmp.LoadGLTexture("menu/mainLoadingScreen.bmp");

	cout<<"Engine Initalized"<<endl;
	return true;
}
//main loop manager
int DrawGLScene2(GLvoid)										// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();										// Reset The Current Modelview Matrix
	//glClearColor(0.5f, 0.7f, 0.9f, 1.0f);					// Background Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					// Background Color

	bmp.DrawBMPTexture(CVector3( 0.00f, 0.00f,-20.70f), CVector3( 10.0f, 7.6f, 1.00f));

	glPopAttrib();

	return true;
}

//main loop manager
int DrawGLScene(GLvoid)										// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	typeSManager.RunScene();
	return true;
}

//terminates the window
GLvoid KillGLWindow(GLvoid)									// Properly Kill The Window
{
	if (fullscreen)											// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);						// If So Switch Back To The Desktop
		ShowCursor(true);									// Show Mouse Pointer
	}

	if (hRC)												// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))						// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))							// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;											// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))						// Are We Able To Release The DC
	{	
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;											// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))						// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;											// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))				// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;										// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (true) Or Windowed Mode (false)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;								// Holds The Results After Searching For A Match
	WNDCLASS	wc;											// Windows Class Structure
	DWORD		dwExStyle;									// Window Extended Style
	DWORD		dwStyle;									// Window Style
	RECT		WindowRect;									// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;								// Set Left Value To 0
	WindowRect.right=(long)width;							// Set Right Value To Requested Width
	WindowRect.top=(long)0;									// Set Top Value To 0
	WindowRect.bottom=(long)height;							// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;								// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);			// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc Handles Messages
	wc.cbClsExtra		= 0;								// No Extra Window Data
	wc.cbWndExtra		= 0;								// No Extra Window Data
	wc.hInstance		= hInstance;						// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);		// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);		// Load The Arrow Pointer
	wc.hbrBackground	= NULL;								// No Background Required For GL
	wc.lpszMenuName		= NULL;								// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";							// Set The Class Name
	if (!RegisterClass(&wc))								// Attempt To Register The Window Class
	{
		cout<<"Testing RegisterClass(&wc))... FAILED";
		return false;										// Return false
	}
	if (fullscreen)											// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;							// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);	// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;			// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;				// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

															// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			cout<<"\n\nDISPLAY ERROR!"<<endl;
			cout<<"The following fullscreen mode is not supported by your video device"<<endl;
			cout<<"resolution : "<<screen->GetScreenX()<<" x "<<screen->GetScreenY()<<"  colorDepth : "<<screen->GetColorDepth()<<endl<<endl;
			cout<<"Switching to windowed mode"<<endl;
			fullscreen=false;								
		}
	}
	if (fullscreen)											// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;							// Window Extended Style
		dwStyle=WS_POPUP;									// Windows Style
		ShowCursor(false);									// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;		// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;						// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);	// Adjust Window To true Requested Size
	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,					// Extended Style For The Window
								"OpenGL",					// Class Name
								title,						// Window Title
								dwStyle |					// Defined Window Style
								WS_CLIPSIBLINGS |			// Required Window Style
								WS_CLIPCHILDREN,			// Required Window Style
								0, 0,						// Window Position
								WindowRect.right-WindowRect.left,// Calculate Window Width
								WindowRect.bottom-WindowRect.top,// Calculate Window Height
								NULL,						// No Parent Window
								NULL,						// No Menu
								hInstance,					// Instance
								NULL)))						// Dont Pass Anything To WM_CREATE
	{
		cout<<"Testing CreateWindowEx()... FAILED";
		KillGLWindow();										// Reset The Display
		return false;										// Return false
	}
	static	PIXELFORMATDESCRIPTOR pfd=						// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),						// Size Of This Pixel Format Descriptor
		1,													// Version Number
		PFD_DRAW_TO_WINDOW |								// Format Must Support Window
		PFD_SUPPORT_OPENGL |								// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,									// Must Support Double Buffering
		PFD_TYPE_RGBA,										// Request An RGBA Format
		bits,												// Select Our Color Depth
		0, 0, 0, 0, 0, 0,									// Color Bits Ignored
		0,													// No Alpha Buffer
		0,													// Shift Bit Ignored
		0,													// No Accumulation Buffer
		0, 0, 0, 0,											// Accumulation Bits Ignored
		24,													// 16Bit Z-Buffer (Depth Buffer)  
		0,													// No Stencil Buffer
		0,													// No Auxiliary Buffer
		PFD_MAIN_PLANE,										// Main Drawing Layer
		0,													// Reserved
		0, 0, 0												// Layer Masks Ignored
	};	

	if (!(hDC=GetDC(hWnd)))									// Did We Get A Device Context?
	{
		KillGLWindow();										// Reset The Display
		cout<<"Testing hDC=GetDC()... FAILED";
		return false;								
	}
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))			// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();										// Reset The Display
		cout<<"Testing ChoosePixelFormat()... FAILED";
		return false;								
	}
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();										// Reset The Display
		cout<<"Testing SetPixelFormat()... FAILED";
		return false;							
	}
	if (!(hRC=wglCreateContext(hDC)))						// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();										// Reset The Display
		cout<<"Testing wglCreateContext()... FAILED";
		return false;							
	}
	if(!wglMakeCurrent(hDC,hRC))							// Try To Activate The Rendering Context
	{
		KillGLWindow();										// Reset The Display
		cout<<"Testing wglMakeCurrent()... FAILED";
		return false;										
	}
	if(!InitGL())											// Initialize Our Newly Created GL Window
	{
		KillGLWindow();										// Reset The Display
		cout<<"Testing InitGL()... FAILED";
		return false;										// Return false
	}
	if(!loadingScreen)
	{

		if(!InitProgram())
		{
			KillGLWindow();
			MessageBox(NULL,"Program Initalization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}
	}

	ShowWindow(hWnd,SW_SHOW);								// Show The Window
	SetForegroundWindow(hWnd);								// Slightly Higher Priority
	SetFocus(hWnd);											// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);							// Set Up Our Perspective GL Screen
														cout<<"[done]"<<endl;


	return true;											// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,					// Handle For This Window
							UINT	uMsg,					// Message For This Window
							WPARAM	wParam,					// Additional Message Information
							LPARAM	lParam)					// Additional Message Information
{
	switch (uMsg)											// Check For Windows Messages
	{
		case WM_ACTIVATE:									// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))							// Check Minimization State
				active=true;								// Program Is Active
			else
				active=false;								// Program Is No Longer Active
			return 0;										// Return To The Message Loop
		}
		case WM_SYSCOMMAND:									// Intercept System Commands
		{
			switch (wParam)									// Check System Calls
			{
				case SC_SCREENSAVE:							// Screensaver Trying To Start?
				case SC_MONITORPOWER:						// Monitor Trying To Enter Powersave?
				return 0;									// Prevent From Happening
			}
			break;											// Exit
		}
		case WM_CLOSE:										// Did We Receive A Close Message?
		{	
			PostQuitMessage(0);								// Send A Quit Message
			return 0;										// Jump Back
		}	
		case WM_KEYDOWN:									// key held down
		{
			input->SetKey(wParam, true);
			return 0;
		}
		case WM_KEYUP:										// key released
		{
			input->SetKey(wParam, false);
			return 0;
		}
		case WM_SIZE:										// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));	// LoWord=Width, HiWord=Height
			return 0;										// Jump Back
		}
		case WM_MOVE:
		{
			WINDOWPLACEMENT p;										// Get the top left position of the window
			GetWindowPlacement(hWnd, &p);
			screen->SetTopLeftScreenCoords(p.rcNormalPosition.top, p.rcNormalPosition.left);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			input->SetMouseButton(L_BUTTON_DOWN, true);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			input->SetMouseButton(M_BUTTON_DOWN, true);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			input->SetMouseButton(R_BUTTON_DOWN, true);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			input->SetMouseButton(L_BUTTON_UP, true);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			input->SetMouseButton(M_BUTTON_UP, true);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			input->SetMouseButton(R_BUTTON_UP, true);
			return 0;
		}
		case WM_MOUSEMOVE:
		{		
			if(wParam & MK_LBUTTON)
				input->SetMouseButton(L_BUTTON_DRAG, true);
			if(wParam & MK_MBUTTON)
				input->SetMouseButton(M_BUTTON_DRAG, true);
			if(wParam & MK_RBUTTON)
				input->SetMouseButton(R_BUTTON_DRAG, true);
				return 0;
		}
		
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);			// Pass All Unhandled Messages To DefWindowProc
}

//main
int WINAPI WinMain(	HINSTANCE	hInstance,					// Instance
					HINSTANCE	hPrevInstance,				// Previous Instance
					LPSTR		lpCmdLine,					// Command Line Parameters
					int			nCmdShow)					// Window Show State
{
	if(static_cast<string>(lpCmdLine)== "-debug")
	{
		debug=true;
		fullscreen=false;
		loadingScreen=false;
	}

	MSG		msg;											// Windows Message Structure
	BOOL	done=false;										// Bool Variable To Exit Loop

	/*AllocConsole( );										// Create Console Window
	freopen("CONIN$","rb",stdin);							// reopen stdin handle as console window input
	freopen("CONOUT$","wb",stdout);							// reopen stout handle as console window output
	freopen("CONOUT$","wb",stderr);							// reopen stderr handle as console window output
	*/
	cout<<"//////////////////////////////////////////////////////////////"<<endl;
	cout<<"//**********************************************************//"<<endl;
	cout<<"//*        Type-S: Advanced Car Simulation Project         *//"<<endl;
	cout<<"//*                                                        *//"<<endl;
	cout<<"//*<<<<<<<<<<<<<< Bedoworks Visual Studios >>>>>>>>>>>>>>>>*//"<<endl;
	cout<<"//*                                                        *//"<<endl;
	cout<<"//*           Developed by Bedros Magardichian.            *//"<<endl;
	cout<<"//*           (c)2002-2006 All Rights Reserved.            *//"<<endl;
	cout<<"//*          http://www.bedoworks.com/typeS.html           *//"<<endl;
	cout<<"//**********************************************************//"<<endl;
	cout<<"//////////////////////////////////////////////////////////////"<<endl;

	try
	{
		if(!InitScreen())
		{
			cout<<"\nFailed to initalize screen resolution and color depth"<<endl;
			cout<<"Using default values "<<screen->GetScreenX()<<"x"<<screen->GetScreenY()<<" "<<screen->GetColorDepth()<<endl;
		}
																// Create Our OpenGL Window
		if (!CreateGLWindow("Type-S: Driving Simulation",
							screen->GetScreenX(), 
							screen->GetScreenY(),
							screen->GetColorDepth(),
							fullscreen))
		{
			cout<<"Failed to create"<<endl;
			return 0;											// Quit If Window Was Not Created
		}

		//Display initial loading screen
		if(loadingScreen)
		{
			for(int i=0; i<5; ++i)
			{
				if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// Is There A Message Waiting?
				{
					if (msg.message==WM_QUIT)						// Have We Received A Quit Message?
						done=true;									// If So done=true
					else											// If Not, Deal With Window Messages
					{
						TranslateMessage(&msg);						// Translate The Message
						DispatchMessage(&msg);						// Dispatch The Message
					}
				}
				else												// If There Are No Messages
				{
																	// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
					if (active)										// Program Active?
					{
						if(typeSManager.Quit())						// quit message processed?
							done=true;							 
						else										// Not Time To Quit, Update Screen
						{
							DrawGLScene2();							// Draw The Scene
							SwapBuffers(hDC);						// Swap Buffers (Double Buffering)
						}
					}
				}
			}

			SetForegroundWindow(hWnd);								// Slightly Higher Priority
			SetFocus(hWnd);											// Sets Keyboard Focus To The Window

			if(!InitProgram())
			{
				KillGLWindow();
				MessageBox(NULL,"Program Initalization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
				return false;
			}
		}

	}															// Ask The User Which Screen Mode They Prefer
	catch(string s)
	{
		std::cout<<s<<endl;
	}

	
	while(!done)											// Loop That Runs While done=false
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)						// Have We Received A Quit Message?
				done=true;									// If So done=true
			else											// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);						// Translate The Message
				DispatchMessage(&msg);						// Dispatch The Message
			}
		}
		else												// If There Are No Messages
		{
															// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)										// Program Active?
			{
				if(typeSManager.Quit())					// quit message processed?
					done=true;							 
				else										// Not Time To Quit, Update Screen
				{
					DrawGLScene();							// Draw The Scene
					SwapBuffers(hDC);						// Swap Buffers (Double Buffering)
				}
			}
		}
	}														
	KillGLWindow();											// Kill The Window
 	if(debug)
	{
		cout<<"\n Enter a key to shut down: ";
		char x; cin>>x;										// pause to see console
	}

	FreeConsole();											// Free Console Window
	return (msg.wParam);									// Exit The Program


 





return 0;


}
