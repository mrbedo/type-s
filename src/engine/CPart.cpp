#include "CPart.h"
#include <iostream>
using namespace std;

//constructor
CPart::CPart():	partOctreeCreated(false){}

//destructor
CPart::~CPart()
{
}

//assign the property array
void CPart::InitProperties()
{
	partTranslation	=CVector3(propertyArray[0],propertyArray[1],propertyArray[2]);
	partScale		=CVector3(propertyArray[3],propertyArray[4],propertyArray[5]);
	partRotation	=CVector3(propertyArray[6],propertyArray[7],propertyArray[8]);

	partID			=(int)propertyArray[9];
	dynamic			=(bool)propertyArray[10];
	transparent		=(bool)propertyArray[11];
	reflective		=(bool)propertyArray[12];
	light;

	damageLevel		=(int)propertyArray[13];
	weight			=propertyArray[14];
	dragCoeff		=propertyArray[15];
	frictionCoeff	=propertyArray[16];
}

//return partID
short CPart::GetPartID() const
{
	return partID;
}

//return dynamic
bool CPart::IsDynamic() const
{
	return dynamic;
}

//returns member
bool CPart::IsTransparent() const
{
	return transparent;
}

//returns member
bool CPart::IsReflective() const
{
	return reflective;
}

//returns member
bool CPart::IsLight()  const
{
	return light;
}

//returns member
int CPart::GetDamageLevel() const
{
	return damageLevel;
}

//returns member
float CPart::GetWeight() const
{
	return weight;
}

//sets the part rotation vector
void CPart::SetPartTransformVector(short trans, const CVector3 &coords, bool update)
{
	switch(trans)
	{
	case 0:
		if(update)
			partTranslation+=coords;
		else
			partTranslation=coords;
		break;
	case 1:
		if(update)
			partScale+=coords;
		else
			partScale=coords;
		break;
	case 2:
		if(update)
			partRotation+=coords;
		else
			partRotation=coords;
		break;
	default:
		cout<<"\nERROR>> transformation="<<trans<<endl;
	}
}
//return the part translation vector
CVector3 CPart::GetPartTransformVector(short trans) const
{
	switch(trans)
	{
	case 0:
		return partTranslation;
	case 1:
		return partScale;
	case 2:
		return partRotation;
	default:
		return CVector3(1,1,1);								//return an all 1's vector to prevent divide overfloat
	}
}

//sets the part rotation vector
void CPart::SetPartTransformMatrix(const GLfloat* matrix)
{
	for(int i=0; i<16; ++i)
		transMatrix[i]=matrix[i];
}

//return the part transformation matrix
const GLfloat* CPart::GetPartTransformMatrix() const
{
	return transMatrix;
}

//set the property with indexing
void CPart::SetPropertyArray(unsigned int index, float property)
{
	propertyArray[index]=property;
	InitProperties();
}

//set the property array with another array
void CPart::SetPropertyArray(const float* propArray, unsigned int size)
{
	if(size<MAX_PROP_ARRAY_SIZE)
	{
		for(int i=0; i<MAX_PROP_ARRAY_SIZE; ++i)
		{
			propertyArray[i]=propArray[i];
		}

		InitProperties();
	}
	else
		cout<<"ERROR>> CPart::SetPropertyArray >>propertyArray size exceeded"<<endl;
}

//set the property array with another array
void CPart::SetPropertyVector(const vector<float>& propVector)
{
	for(unsigned int i=0; i<propVector.size(); ++i)
		SetPropertyArray(i,propVector[i]);
	InitProperties();
}

//get a certain property
float CPart::GetProperty(unsigned int index) const
{
	return propertyArray[index];
}

//get the property array
float CPart::GetPropertyArray()	const
{
	return propertyArray[20];
}
//generate a display list
void CPart::GeneratePartDL()
{
	glLoadIdentity();
	cout<<"\nGenerating Display list...";
	partDL=glGenLists(1);
	if(reflective)											//reflective
	{
		if(transparent)
		{
			glNewList(partDL, GL_COMPILE);
			glPushAttrib(GL_ALL_ATTRIB_BITS);
				glEnable(GL_CULL_FACE);
				glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
				glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);
				part.Draw3DModelWithEnviMap();
				glDisable(GL_CULL_FACE);
			glPopAttrib();
			glEndList();
		}
		else
		{
			glNewList(partDL, GL_COMPILE);
				part.Draw3DModelWithEnviMap();
			glEndList();
		}
	}
	else													//dull
	{
		if(transparent)
		{
			glNewList(partDL, GL_COMPILE);
			glPushAttrib(GL_ALL_ATTRIB_BITS);
				glEnable(GL_CULL_FACE);
				glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
				glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA );
				part.Draw3DModel();
				glDisable(GL_CULL_FACE);
			glPopAttrib();
			glEndList();
		}
		else
		{
			glNewList(partDL, GL_COMPILE);
				part.Draw3DModel();
			glEndList();
		}
	}
/*	cout<<"[ok]"<<endl;
	cout<<"\nPropertyArray[ ";
	for(int i=0; i<20; ++i)
		cout<<propertyArray[i]<<" ";
	cout<<"]"<<endl;
	*/
}

//renders part to the screen
void CPart::RenderPart() const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
		glScalef(partScale.x,
				 partScale.y,
				 partScale.z);
		glRotatef(partRotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(partRotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(partRotation.z, 0.0f, 0.0f, 1.0f);

		glCallList(partDL);
		//RenderPartWithOctree();
	glPopMatrix();
	glPopAttrib();
	//glFlush();
}

//renders part to the screen
void CPart::RenderPartWithVectorTrans()	const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
		glTranslatef(partTranslation.x,
					 partTranslation.y,
					 partTranslation.z);
		glScalef(partScale.x,
				 partScale.y,
				 partScale.z);
		glRotatef(partRotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(partRotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(partRotation.z, 0.0f, 0.0f, 1.0f);

		glCallList(partDL);
		//RenderPartWithOctree();
	glPopMatrix();
	glPopAttrib();
	//glFlush();
}

//renders part to the screen
void CPart::RenderPartWithMatrixTrans() const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	  glMultMatrixf (transMatrix);
		glCallList(partDL);
	glPopMatrix();
	glPopAttrib();
	//glFlush();
}

//renders part to the screen without applying default transformations
void CPart::RenderPartAsIs() const
{
	glPushMatrix();
		glCallList(partDL);
	glPopMatrix();
	//glFlush();
}

//render part using the octree
void CPart::RenderPartWithOctree()
{
	if(!partOctreeCreated)
	{
		cout<<"Creating OCTree..."<<endl;
		part.CreateOctree();
		partOctreeCreated=true;
	}
	part.DrawOctree();
	
}

//initalizes the part 
void CPart::InitPart(const string& partPathName, const string& texturePath )
{
	cout<<"part.LoadModel("<<partPathName<<", "<<texturePath<<")"<<endl;
	part.Load3DSFile(partPathName,texturePath);
}

//free up memory
void CPart::CleanUp()
{
	//cout<<"\nCPart::CleanUp()"<<endl;
	glDeleteLists(partDL, 1);
}
