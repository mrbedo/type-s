///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTypeSCarEditor.cpp
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
//  This class will provides an interface to edit cars by modifying properties
//	of the CModel described objects
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTypeSCarEditor.h"
#include <iostream>

using namespace std;

//Default Constructor
CTypeSCarEditor::CTypeSCarEditor()
{
}

//Default Destructor
CTypeSCarEditor::~CTypeSCarEditor()
{
}

//Initalizes car shop
int CTypeSCarEditor::Init()
{
	static bool created=false;
	c.ResetCamera();

	carMan= CCarManager::Instance();
	currentCar= carMan->selectedCar;

	if(!created)
	{
		showText=true;
		partOperation=0;										//used for determing what to do in the garage
		carShopMenu=new CGUIMenu();

		InitMenu();
	}
	created=true;
	return true;
}


//Initalized Garage Menu
void CTypeSCarEditor::InitMenu()
{
/*														cout<<"\n\n--- Setting Up Garage Menu System ---------------";
	float btnX=0.80f;
	CVector3 buttonPositions[7]=
	{
		CVector3( btnX, 0.80f, -2.40f),						//up     move button
		CVector3( btnX, 0.70f, -2.40f),						//down	 scale button
		CVector3( btnX, 0.60f, -2.40f),						//down	 rotate button
		CVector3( btnX, 0.40f, -2.40f),						//down	 next button
		CVector3( btnX, 0.30f, -2.40f),						//down	 prev button
		CVector3( btnX, 0.10f, -2.40f),						//down	 reset button
		CVector3( btnX, 0.00f, -2.40f)						//down	 quit button
	};

	CVector3 buttonStateScale[3]=
	{
		CVector3( 0.125f, 0.050f, 0.000f),					//up	 scale button
		CVector3( 0.127f, 0.052f, 0.000f),					//select scale button
		CVector3( 0.123f, 0.048f, 0.000f)					//down	 scale button
	};

	CVector3 buttonStateColor[3]=
	{
		CVector3( 0.800f, 0.000f, 0.000f),					//medium bright red
		CVector3( 1.000f, 0.000f, 0.000f),					//full red
		CVector3( 0.300f, 0.000f, 0.000f)					//dark red
	};

	float buttonStateOpacity[3]=
	{
		0.600,												//up	 button opaqueness
		1.000,												//select button opaqueness
		0.300												//down	 button opaqueness
	};

	string buttonText[7]=
	{
		"Move",
		"Scale",
		"Rotate",
		"Next",
		"Prev",
		"Reset",
		"Quit"
	};

	for(int i=0; i<7; ++i)
	{
		carShopMenuButtons[i].Init();
		carShopMenuButtons[i].SetPosition(buttonPositions[i]);
		carShopMenuButtons[i].SetScale(buttonStateScale);
	//	carShopMenuButtons[i].SetColor(buttonStateColor);
		carShopMenuButtons[i].SetOpacity(buttonStateOpacity);
		carShopMenuButtons[i].SetText(buttonText[i]);
		carShopMenu->AddMenuButton(buttonPositions[i],buttonText[i]);
	}

	tf[0].Init();
	tf[0].SetPosition(CVector3( 0.0f,  0.0f, -1.0f));
	tf[0].SetScale(buttonStateScale);
	tf[0].SetColor(CVector3(0.2f, 0.1f, 0.0f));
	tf[0].SetBorderColor(CVector3(0.3f, 0.3f, 0.0f));
	tf[0].SetBorderThickness(2.0f);
	tf[0].SetTextColor(CVector3(0.6f, 1.0f, 0.0f));
	tf[0].SetOpacity(0.5f);
	carShopMenu->AddMenuTextBox(CVector3( 0.0f,  0.0f, -1.0f),"textbox1");

														cout<<"\n\t+buttons created successfully";
*/
}

//displays the menu
void CTypeSCarEditor::DisplayMenu()
{
	glPushMatrix();
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
//		carShopMenu->DisplayMenu();
//		carShopMenu->MouseSelect();
		glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

//return the current menu choice of the scene
int CTypeSCarEditor::GetMenuSelection()
{
	return menuSelection;
}

//process a car shop menu selection
void CTypeSCarEditor::MakeMenuSelection(short state)
{
/*	switch(state)
	{
	case 0:
		carShopMenu->DepressChoice(true);
		break;
	case 1:
		carShopMenu->DepressChoice(false);
		switch(carShopMenu->ReturnChoice())
		{
		case 0:	break; partOperation=0;;break;						//move
		case 1: break; partOperation=1;//currentSceneState=3;							//scales
		case 2:	break; partOperation=2;						//rotates
		case 3: break; SelectPart(true, false);							//selectects next part
		case 4:	break; SelectPart(false, false);									//selectes previous part
		case 5:	break; 											//resets
		case 6: break;//ReturnToMainMenu();break;						//quits
		}
	break;
	}*/
}

//displays the text in the car shop
void CTypeSCarEditor::DisplayText()
{
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0f, 0.0f, 0.0f);
	glScalef(0.9f, 0.9f, 1.0f);
	glTranslatef(0.0f, 0.0f, -0.3f);

	float textAlign=-0.0f;
	float scrollplace=0.0f;

	//-----------------text------------------------
	t.Print(textAlign,scrollplace+0.12f,"CameraPos.x=  %f", c.GetPosition().x);
	t.Print(textAlign,scrollplace+0.11f,"CameraPos.y=  %f", c.GetPosition().y);
	t.Print(textAlign,scrollplace+0.10f,"CameraPos.z=  %f", c.GetPosition().z);
	if(currentCar != 0)
	{
	t.Print(textAlign,scrollplace+0.09f,"CarPosx=      %f", currentCar->GetPosition().x);
/*	t.glutPrint(textAlign,scrollplace+0.08f,"part Trans.x: %f", carInShop->GetPartTransformation(0,selectedCarPart).x);
	t.glutPrint(textAlign,scrollplace+0.07f,"part Trans.y: %f", carInShop->GetPartTransformation(0,selectedCarPart).y);
	t.glutPrint(textAlign,scrollplace+0.06f,"part Trans.z: %f", carInShop->GetPartTransformation(0,selectedCarPart).z);
	t.glutPrint(textAlign,scrollplace+0.05f,"part Scale.x: %f", carInShop->GetPartTransformation(1,selectedCarPart).x);
	t.glutPrint(textAlign,scrollplace+0.04f,"part Scale.y: %f", carInShop->GetPartTransformation(1,selectedCarPart).y);
	t.glutPrint(textAlign,scrollplace+0.03f,"part Scale.z: %f", carInShop->GetPartTransformation(1,selectedCarPart).z);
	t.glutPrint(textAlign,scrollplace+0.02f,"part Rotat.x: %f", carInShop->GetPartTransformation(2,selectedCarPart).x);
	t.glutPrint(textAlign,scrollplace+0.01f,"part Rotat.y: %f", carInShop->GetPartTransformation(2,selectedCarPart).y);
	t.glutPrint(textAlign,scrollplace+0.00f,"part Rotat.z: %f", carInShop->GetPartTransformation(2,selectedCarPart).z);
	t.glutPrint(textAlign,scrollplace-0.02f,"Selected Part:%d", selectedCarPart);
	t.glutPrint(textAlign,scrollplace-0.04f,"wheelAngle:   %f", carInShop->wheelAngle);
*/	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopAttrib();
	glPopMatrix();
}

//Make a car
void CTypeSCarEditor::RunScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glLoadIdentity();										// Reset The Current Modelview Matrix
	glClearColor(0.5f, 0.7f, 0.9f, 1.0f);					// Background Color

	CGraphicUtils	gfxUtils;

//	if(showText)

	if(useMouse)
		c.Mouse(3.0f, 0, 0);

	if(currentCar != 0)
	{
		if(currentCar->GetCameraIndex()==2)
		{
			c.MovieCam(currentCar->GetPosition(), deltaTime);
		}
		else
		if(currentCar->GetCameraIndex()==3)
		{
			c.FreeCam(false, deltaTime);
		}
		else
		{	c.FollowPosition(currentCar->GetPosition(), 
							  currentCar->GetCameraPositions()[currentCar->GetCameraIndex()],
							  currentCar->GetSteerAngle(),
							  deltaTime);
		}
	}
	else													// if no car, use free camera
	{
		c.FreeCam(false, deltaTime);
	}


	gfxUtils.DrawGridLines(10);

	if(currentCar != 0)
		currentCar->DrawModel();

	glPushMatrix();
		glDisable(GL_LIGHTING);
		if(currentCar != 0)
		{
			glTranslatef(currentCar->GetPartTransformation(0,selectedCarPart).x,
						 currentCar->GetPartTransformation(0,selectedCarPart).y,
						 currentCar->GetPartTransformation(0,selectedCarPart).z);
		}
		else
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
		}
		gfxUtils.DrawAxisLines(3);
		glEnable(GL_LIGHTING);
	glPopMatrix();


	glPopAttrib();
	DisplayText();
	DisplayMenu();

	//glFlush();
//	cout<<"in garage"<<endl;

}

void CTypeSCarEditor::LogicLoop()
{
}

//move car part along X,Y, or Z axis
void CTypeSCarEditor::AdjustPart(char axis, float amount)
{
	for(unsigned int i=0; i<selectedCarParts.size(); ++i)			//apply the adjustments to every part in the selected vector
	{
		switch(axis)
		{
		case 'x':
			currentCar->SetPartTransformation(partOperation, selectedCarParts[i], CVector3(amount, 0.0f, 0.0f));
			break;
		case 'y':
			currentCar->SetPartTransformation(partOperation, selectedCarParts[i], CVector3(0.0f, amount, 0.0f));
			break;
		case 'z':
			currentCar->SetPartTransformation(partOperation, selectedCarParts[i], CVector3(0.0f, 0.0f, amount));
			break;
		default:
			break;
		}
	}
}

//selects car parts
void CTypeSCarEditor::SelectPart(bool selectUp, bool addToSelection)
{
/*
	if( selectUp && (selectedCarPart < currentCar->GetNumberOfParts()-1))
		selectedCarPart++;

	if(!selectUp && (selectedCarPart > 0))
		selectedCarPart--;

	if(!addToSelection)
		selectedCarParts.resize(0);							//if not adding to the selection, only push in on the first
	
	selectedCarParts.push_back(selectedCarPart);
	cout<<"\nSelected Car Parts= ";
	for(int i=0; i<selectedCarParts.size(); ++i)
		cout<< selectedCarParts[i] <<" ";*/
}

//change camera view
void CTypeSCarEditor::ChangeCarCamera()
{
	currentCar->NextCameraPosition();
}

// deletes pointer objects
int CTypeSCarEditor::CleanUp()
{
	cout<<"\nCleanUP -  CTypeSCarEditor...";
	TopCleanUp();
	delete carShopMenu;
	carShopMenu=0;

	cout<<"\n           [done]";
	return true;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///////// Unused Inherited Virtual Functions /////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
