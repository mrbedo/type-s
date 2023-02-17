#include "CTypeSMenu.h"
#include <iostream>

using namespace std;
//Initalize objects and member variables
int CTypeSMenu::Init()										// All Setup For OpenGL Goes Here
{
	static bool created=false;
	if(!created)
	{
															cout<<"\nLoading Type-S Module";
															cout<<"\n\t+member data initalization";
//		t.BuildTextureFont();									//create textured fonts

		guiMenu= new CGUIMenu();
		panel=	 new CGUIPanel();
		btnTheme= new CGUITheme();
		btnThemeDisabled = new CGUITheme();



		textAlign=-0.00f;
		scrollplace=0.0f;


		showText=false;											//displays the text on the screen
															cout<<"\n\t+member data initalized";
		InitMenu();												//make main menu

		carMan=CCarManager::Instance();
		carMan->InitCars();

	//	carMan->selectedCar= carMan->selectedCar;
		screen= CScreenManager::Instance();
	}
	created=true;
	return true;
}

//Initalized Main Menu
void CTypeSMenu::InitMenu()
{
														cout<<"\n\n--- Setting Up Menu System ---------------";
	menuBMP.LoadGLTexture("menu/typesmenu3.bmp");			//menu background pic
	

	CVector3 tBorderColor[3]=	{	CVector3(0.8f, 0.8f, 0.0f),
									CVector3(0.6f, 0.6f, 0.8f),
									CVector3(0.9f, 0.0f, 0.0f) };

	CVector3 tBorderColorGrayed[3]=	{ CVector3(0.6f, 0.6f, 0.6f),
									  CVector3(0.6f, 0.6f, 0.6f),
									  CVector3(0.6f, 0.6f, 0.6f) };

	CVector3 tColor[3]=			{	CVector3(0.4f, 0.1f, 0.0f),
									CVector3(0.5f, 0.0f, 0.0f),
									CVector3(0.3f, 0.0f, 0.0f) };

	CVector3 tColorGrayed[3]=	{	CVector3(0.4f, 0.4f, 0.4f),
									CVector3(0.4f, 0.4f, 0.4f),
									CVector3(0.4f, 0.4f, 0.4f) };
	
	CVector3 tScale[3]=			{	CVector3( 1, 1, 1),	
									CVector3( 1, 1, 1),			
									CVector3( 1, 1, 1) };

	CVector3 tTextColor[3]=		{	CVector3(0.8f, 0.8f, 0.6f),
									CVector3(1.0f, 1.0f, 1.0f),
									CVector3(0.5f, 0.5f, 0.5f) };

	CVector3 tTextBoxColor[3]=	{	CVector3(1.0f, 1.0f, 1.0f),
									CVector3(1.0f, 1.0f, 1.0f),
									CVector3(1.0f, 1.0f, 1.0f) };

	CVector3 tTextPosition[3]=	{	CVector3(10.0f, 25.0f, 0.0f),
									CVector3(10.0f, 25.0f, 0.0f),
									CVector3(10.0f, 25.0f, 0.0f) };


	btnTheme->themeSizeX= btnThemeDisabled->themeSizeX= 140.0f;
	btnTheme->themeSizeY= btnThemeDisabled->themeSizeY= 40.0f;

	for(unsigned int i=0; i<3; ++i)
	{
		btnThemeDisabled->themeBorderColor[i]=	tBorderColorGrayed[i];
		btnThemeDisabled->themeColor[i]=		tColorGrayed[i];
		btnThemeDisabled->themeScale[i]=		tScale[i];
		btnThemeDisabled->themeTextColor[i]=	tBorderColorGrayed[i];
		btnThemeDisabled->themeTextBoxColor[i]=	tTextBoxColor[i];
		btnThemeDisabled->themeTextPosition[i]=	tTextPosition[i];
		btnThemeDisabled->themeBorderThickness[i]= 1.0f;
	}

	for(unsigned int i=0; i<3; ++i)
	{
		btnTheme->themeBorderColor[i]=	tBorderColor[i];
		btnTheme->themeColor[i]=		tColor[i];
		btnTheme->themeScale[i]=		tScale[i];
		btnTheme->themeTextColor[i]=	tTextColor[i];
		btnTheme->themeTextBoxColor[i]=	tTextBoxColor[i];
		btnTheme->themeTextPosition[i]=	tTextPosition[i];
		btnTheme->themeBorderThickness[i]= 1.0f;
	}

	CVector3 btnPos[4];

	for(unsigned int j=0; j<4; j++)
		btnPos[j]=CVector3(100*j+ j*btnTheme->themeSizeX,50,0);

	panel->Init();
//	panel->PrintLine(5,11,"Type-S Menu");
//	panel->SetSize(0,0);
	panel->SetPosition(CVector3(30,30,0));
//	panel->SetTextPosition(CVector3(5,15,-1));
//	panel->SetTextColor(CVector3(0.5f, 0.6f, 0.0f));
//	panel->SetScale(CVector3(1,1,1));
///	panel->SetColor(CVector3(0.1f, 1.2f, 0.1f));
//	panel->SetBorderColor(CVector3(1.0f, 0.3f, 0.0f));
//	panel->SetBorderThickness(2.0f);

	guiMenu->AddPanel("menu",panel);
	guiMenu->AddPanelButton("menu", btnPos[0],"Free Drive" , *btnTheme);
	guiMenu->AddPanelButton("menu", btnPos[1],"Car Editor" , *btnThemeDisabled);
	guiMenu->AddPanelButton("menu", btnPos[2],"Credits"	   , *btnThemeDisabled);
	guiMenu->AddPanelButton("menu", btnPos[3],"Exit"	   , *btnTheme);


														cout<<"\n\t+menu buttons set up";
														cout<<"\n\t+menu textures loaded successfully";
}

//process a main menu selection
void CTypeSMenu::MakeMenuSelection(short state)
{
	guiMenu->MakeMenuSelection(state);
}

//return the current menu choice of the scene
int CTypeSMenu::GetMenuSelection()
{
	return guiMenu->GetPanelSelectedButton("menu");
}

void CTypeSMenu::DisplayMenu()
{
	glPushMatrix();
	glPushAttrib(GL_COLOR_BUFFER_BIT);//GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	screen->Set2DViewport(screen->GetScreenX(), screen->GetScreenY());

	guiMenu->DisplayMenu();

	screen->Set3DViewport(screen->GetScreenX(), screen->GetScreenY());


	glEnable(GL_LIGHTING);
	glPopAttrib();
	glPopMatrix();
}

//shows the text on the screen
void CTypeSMenu::DisplayText()
{
	//time.FramesPerSecond();
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);//GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.3f);
	
	t.Print(0.09f, 0.10f,"FPS: ", time.GetFPS());
	time.FramesPerSecond();
	
	//-----------------text------------------------
/*	t.glutPrint(-textAlign-0.025f, 0.10f," ", time.GetHour()); 
	t.glutPrint(-textAlign-0.020f, 0.10f,":", time.GetMinute()); 
	t.glutPrint(-textAlign-0.015f, 0.10f,":", time.GetSecond());
	t.glutPrint(-textAlign-0.010f, 0.10f,".", time.GetTensSecond());
*/	

/*	t.glutPrint(textAlign,scrollplace+0.0f,"CameraPos.x=       ", c.GetPosition().x);
	t.glutPrint(textAlign,scrollplace+0.11f,"CameraPos.y=       ", c.GetPosition().y);
	t.glutPrint(textAlign,scrollplace+0.10f,"CameraPos.z=       ", c.GetPosition().z);
	t.glutPrint(textAlign,scrollplace+0.09f,"Velocity.x:        ", carMan->selectedCar->velocity.x);
	t.glutPrint(textAlign,scrollplace+0.08f,"Velocity.z:        ", carMan->selectedCar->velocity.z);
	t.glutPrint(textAlign,scrollplace+0.07f,"car position.x:    ", carMan->selectedCar->position.x);
	t.glutPrint(textAlign,scrollplace+0.06f,"car position.y:    ", carMan->selectedCar->position.y);
	t.glutPrint(textAlign,scrollplace+0.05f,"car position.z:    ", carMan->selectedCar->position.z);
	t.glutPrint(textAlign,scrollplace+0.04f,"steerAngle:        ", carMan->selectedCar->steerAngle);
	t.glutPrint(textAlign,scrollplace+0.03f,"DELTA TIME:        ", (float)time.GetDeltaTime());
	t.glutPrint(textAlign,scrollplace+0.02f,"Menu Choice:       ", mainMenu->ReturnChoice());
	t.glutPrint(textAlign,scrollplace+0.01f,"Scene State:       ", currentSceneState);
	t.glutPrint(textAlign,scrollplace+0.00f,"CarIndex:          ", carIndex);
	t.glutPrint(textAlign,scrollplace-0.01f,"carHasBeenInit:    ", (int)carHasBeenInitalized);
	t.glutPrint(textAlign,scrollplace-0.02f,"carListSize:       ", (int)carList.size());
	t.glutPrint(textAlign,scrollplace-0.03f,"steerAngleRate:    ", carMan->selectedCar->steerAngleRate);
	t.glutPrint(textAlign,scrollplace-0.04f,"wheelAngle:        ", carMan->selectedCar->wheelAngle);
//	t.glutPrint(textAlign,scrollplace-0.05f,"Terrain Vtx        ", (int)terrain.model.allVerteces.size());//allTerrainVerteces.size());
	t.glutPrint(textAlign,scrollplace-0.06f,"rSpeed:            ", carMan->selectedCar->rSpeed);
	t.glutPrint(textAlign,scrollplace-0.07f,"throttle:          ", carMan->selectedCar->throttle);
	t.glutPrint(textAlign,scrollplace-0.08f,"Camera YAW:        ", c.GetYaw());
	t.glutPrint(textAlign,scrollplace-0.09f,"Camera Pitch:      ", c.GetPitch());
	t.glutPrint(textAlign,scrollplace-0.10f,"Camera speed:      ", c.GetCameraSpeed());
//	t.glutPrint(textAlign,scrollplace-0.10f,"EndNodes:          ", g_Octree.g_EndNodeCount);
//	t.glutPrint(textAlign,scrollplace-0.11f,"NodesDraw:         ", g_Octree.g_TotalNodesDrawn);  
*/
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopAttrib();
	glPopMatrix();
}

//Main menu of game
void CTypeSMenu::RunScene()
{
	glClearColor(0,0,0,0);									// Background Color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	DisplayMenu();
	glLoadIdentity();										// Reset The Current Modelview Matrix


	glPushAttrib(GL_ALL_ATTRIB_BITS);

	ShowCursor(true);

	menuBMP.DrawBMPTexture(CVector3( 0.00f, 0.00f,-20.70f), CVector3( 10.0f, 7.6f, 1.00f));
	if(carMan->selectedCar)
	{
//		carDescriptionPic[carIndex].DrawBMPImage(CVector3(0.63f, 0.0f, -2.0f), CVector3(0.25,0.35,1));
		carMan->selectedCar->ShowcaseDisplay(CVector3( -0.03f, 0.02f,-0.90f),
											 CVector3( 0.05f, 0.05f, 0.05f),
											 18.0f, 5.0f, 0.2f);
	}
	glPopAttrib();
	DisplayText();



	glFlush();												// Everything Went OK
}

//Handles the non graphical aspect of the main scene
void CTypeSMenu::LogicLoop()
{
}

//selects the next car in the vector
void CTypeSMenu::SelectCarUp()
{													
}

//selects the previous car in the vector
void CTypeSMenu::SelectCarDown()
{
}

void CTypeSMenu::NextMainMenuChoice()
{
//	mainMenu->NextChoice();
}

//selects the previous menu choice
void CTypeSMenu::PreviousMainMenuChoice()
{
//	mainMenu->PreviousChoice();				
}

//scroll display data up
void CTypeSMenu::ScrollOutputUp()
{
	scrollplace-=0.002f;
}

//scroll display data down
void CTypeSMenu::ScrollOutputDown()
{
	scrollplace+=0.002f;				
}

//deletes pointers and objects and frees up memory
int CTypeSMenu::CleanUp()
{
	carMan->CleanUp();
	cout<<"\nCleanUP -  CTypeSMenu...";
	TopCleanUp();

	delete guiMenu;
	delete panel;
	delete btnTheme;
	delete btnThemeDisabled;

	guiMenu=0;
	panel=0;
	btnTheme=0;

	cout<<"\n           [done]";
	return true;
}	

//delegate tasks to specific identified functions
void CTypeSMenu::ContextSpecificFunction(int id)
{
	switch(id)
	{
	case 1:	SelectCarUp();		break;
	case 2: SelectCarDown();	break;
	}
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///////// Unused Inherited Virtual Functions /////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


//change car camera	
void CTypeSMenu::ChangeCarCamera()
{
}