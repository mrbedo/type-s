#ifndef CMODEL_H
#define CMODEL_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CModel.h
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
//  This class will provide a wrapper for a general model composed of CPart objects.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "cpart.h"


class CModel
{
#define MAX_PARTS 50

protected:
	char* modelName;											//stores the tsi fileName of the model descriptor
	unsigned int numberOfParts;

public:
	CPart partVector[MAX_PARTS];								//holds the parts of the model

	CModel();
	//Post- default constructor

	~CModel();
	//Post- default destructor
	
	int Init(const char* tsiFileName);
	//Pre-	A car object exists
	//Post- Initalizes car parameters
	
	int InitModels();
	//Pre-  number of parts to be loaded is known
	//Post- if the number of parts to be loaded can be fitted in the partVector, returns true, and loads
	//		otherwise return false and loads nothing.
	
	void DrawModel() const;
	//Pre-  3d model exists
	//Post- Terrain is rendered to the screen
	
	void ShowcaseDisplay(const CVector3& pos, const CVector3& scale, float angle, float rotateSpeed, float deltaTime) const;
	//Pre-	3d model object exists
	//Post- Draws the model at the position, scale, angle and 
	//		rotates it with the rotation speed for showcasing	//
	
	const char* GetModelName() const;
	//Pre-  carName may or may not exists
	//Post- if carName exists, returns it, else returns default message
	
	void SetPartTransformation(int operation, int partIndex, CVector3& trans);
	//Pre-  a part paremeter array exists
	//Post- at part index, the selected operation (translation/scale/rotation) updates the proper operation array with a new value
	
	void SetPartTransformation(int partIndex, const float* matrix);
	//Pre-  a part paremeter array exists
	//Post- at part index, sets the new transformation matrix

	CVector3 GetPartTransformation(int operation, int partIndex) const;
	//Pre-  a part parameter array exists
	//Post- returns the operation (translation/scale/rotation) coordinates at that index
	
	const GLfloat* GetPartTransformation(int partIndex) const;
	//Pre-  a part parameter array exists
	//Post- returns the transformation matrix at index specified part

	unsigned int GetNumberOfParts() const;
	//Pre-  number or parts exists
	//Post- returns numberOfParts
	
	void Reset();
	//Pre-	3d model exists with member variables at some value
	//Post- member variables are reset to default values

};
#endif