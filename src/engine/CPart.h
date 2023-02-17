#ifndef CPART_H
#define CPART_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CPart.h
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
//  This class provides a wrapper for loading a .3ds file and applying specified matrix
//	transformations and user defined properties
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CVector3.h"
#include "C3DModel.h"
#include <string>
#include <vector>


class CPart  
{
private:

	enum { MAX_PROP_ARRAY_SIZE = 20 };

	CVector3 partTranslation;
	CVector3 partScale;
	CVector3 partRotation;
	float transMatrix[16];				//for matrix wise transformations

	short partID;						//describes part 0=body panel, 1=car wheel, 2=steering
	bool  dynamic;						//does the part conform to its local rotation behavior?(used for making a brake caliper for wheels when declared as a wheel partID)
	bool  transparent;					//is the part transparent?
	bool  reflective;					//does the surface take on a reflection map?
	bool  light;						//does the object glow like al light bulb?

	int		damageLevel;				//holds a degree of damage
	float   weight;						//weight of the object
	float   dragCoeff;					//drag coefficient
	float   frictionCoeff;				//friction coefficient

	bool partOctreeCreated;

	float propertyArray[MAX_PROP_ARRAY_SIZE];					//each index is a type of property
	//------------------------
	// 0- translation X
	// 1- translation Y
	// 2- translation Z
	// 3- scale X
	// 4- scale Y
	// 5- scale Z
	// 6- rotation X
	// 7- rotation Y
	// 8- rotation Z
	//
	// 9- ID
	//		0= body panel
	//		1= car tire
	//		2= steering wheel
	//		
	//
	//    0                  1
	// 10- dynamic          static
	// 11- transparent      opaque
	// 12- reflective       dull
	// 13-light			   not a light
	// 
	// 14- damage level 0-100
	// 15- weight
	// 16- drag coefficient
	// 17- frictional coefficient
	// 18- 
	// 19-
 
	int partDL;

public:
	C3DModel part;

	CPart();
	//Post- Default constructor

	~CPart();
	//Post- Default destructor
	
	void InitProperties();
	//Pre-  property array exists with values
	//Post- takes the values of the property array and assigns them
	//		to their corresponding variables
	
	void SetPartTransformVector(short trans, const CVector3 &value, bool update);
	//Pre-  partTransVector scale and rotation exists
	//Post- selected transformation updated with new trans coordinate

	void SetPartTransformMatrix(const GLfloat* matrix);
	//Pre-  transformation matrix exists
	//Post- overwrites existing transformation matrix
	
	void SetPropertyArray(unsigned int index, float property);
	//Pre-  property array exists
	//Post- a new property is assigned to a specified index
	
	void SetPropertyArray(const float* propArray, unsigned int size);
	//Pre-  property array exists
	//Post- property array replaced by passed in array
	
	void SetPropertyVector(const std::vector<float> &propVector);
	//Pre-  propertyArray exists
	//Post- vector contnets are put into the property array
	
	float GetProperty(unsigned int index) const;
	//Pre-  property exists in property array
	//Post- returns propertyArray at index i
	
	float GetPropertyArray() const;
	//Pre-  property array exists
	//Post- returns property array
	
	CVector3 GetPartTransformVector(short trans) const;
	//Pre-  part transformation vectors exists
	//Post- returns selected transformation vector

	const GLfloat* GetPartTransformMatrix() const;
	//Pre-  part transformation matrix exists
	//Post- returns a 4x4 matrix
	
	short GetPartID() const;
	//Pre-  partID exists
	//Post- returns it
	
	bool IsDynamic() const;
	//Pre-  dynamic flag exists
	//Post- returns it
	
	bool IsReflective() const;
	//Pre-  reflective flag exists
	//Post- returns it
	
	bool IsTransparent() const;
	//Pre-  transparent flag exists
	//Post- returns it
	
	bool IsLight() const;
	//Pre-  light flag exists
	//Post- returns it
	
	int GetDamageLevel() const;
	//Pre-  damage level exists
	//Post- returns it
	
	float GetWeight() const;
	//Pre-  weight exists;
	//Post- returns it	
	
	void GeneratePartDL();
	//Pre-  part is initalized and loaded
	//Post- part is rendered in a display list
	
	void RenderPartWithVectorTrans() const;
	//Pre-  part display list exists
	//Post- part is rendered with default specified vector transformations (marginally faster)
	
	void RenderPartWithMatrixTrans()const;
	//Pre-  part display list exists
	//Post- part is rendered by a matrix transformation applied

	void RenderPart()const;
	//Pre-  part display list exists
	//Post- part is rendered with a predefined scale but no translation is defined

	void RenderPartAsIs()const;
	//Pre-  part display list exists
	//Post- part is rendered without the default transformations specified, useful for manual transformations
	
	void RenderPartWithOctree();
	// Render the part using the octree feature of C3DModel

	void InitPart(const std::string& partPathName, const std::string& texturePath);
	//Pre-  3DS model ready to initalize
	//Post- initalizes 3DS model and generates display list

	void CleanUp();
	//Pre-  pointers and display lists exists
	//Post- deletes unused pointers and display lists
};
#endif