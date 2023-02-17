///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CModel.cpp
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
//  This class will provide a wrapper for a general 3ds model
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "cmodel.h"

//constructor
CModel::CModel()
{
}

CModel::~CModel()
{
}

int CModel::Init(const char* tsiFileName)
{
	strcpy(modelName, tsiFileName);
	//cout<<"CModel Default Initalizes Nothing"<<endl;
	return 0;
}

//initalizes the parts vector
int CModel::InitModels()
{
	//cout<<"initalizing..."<<endl;
	if(numberOfParts > MAX_PARTS)
	{
		//cout<<"\n\n\n\nERROR>> Not enough space in partVector["<<MAX_PARTS<<"] to fit "<<numberOfParts<<" parts!\n\n\n\n"<<endl;
		return 0;
	}
	else
	{
		//partVector.resize(numberOfParts);
		for(unsigned int i=0; i<numberOfParts; ++i)
		{
			partVector[i].GeneratePartDL();
		}
	}
	return 1;
}

//draws all the parts of the model
void CModel::DrawModel() const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonOffset(0.2f, 1.0f);

	for(unsigned int i=0; i<numberOfParts; ++i)
	{
		partVector[i].RenderPartWithVectorTrans();
		//partVector[i].RenderPartWithOctree();
	}
	glPopAttrib();
}
//rotates a car in a showcase fashion
void CModel::ShowcaseDisplay(const CVector3& pos, const CVector3& scale, float angle, float rotateSpeed, float deltaTime)const
{
	static float showcaseRotationSpeed=0.0f;

	if(showcaseRotationSpeed>360) 
		showcaseRotationSpeed-=360.0f;
	else if(showcaseRotationSpeed< -360)
		showcaseRotationSpeed+=360.0f;

	glTranslatef(pos.x,
			     pos.y,
				 pos.z);
	glScalef(scale.x,
			 scale.y,
			 scale.z);

	glRotatef(angle, 1.0f, 0.0f, 0.0f);
	glRotatef(showcaseRotationSpeed, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

		glTranslatef(-1,0,0);
		DrawModel();											//normal view

		glTranslatef(0.0f, -1.4-pos.y, 0.0f);				//translate below car
		glScalef( 1.0f,-1.0f,1.0f);							//invert
		glCullFace(GL_FRONT);								//invert culling since
		DrawModel();										//normals are switched during invert

		glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
		glColor4f(0.0f, 0.0f, 0.0f, 0.3);
		glBegin(GL_QUADS);
			glVertex3f(-100,-0.7,-100);
			glVertex3f( 100,-0.7,-100);
			glVertex3f( 100,-0.7, 100);
			glVertex3f(-100,-0.7, 100);
		glEnd();
		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
	glPopAttrib();
	glPopMatrix();
	//glFlush();
	showcaseRotationSpeed+=rotateSpeed*deltaTime;
}

//set up the part translation vectors
void CModel::SetPartTransformation(int operation, int partIndex, CVector3& trans)
{
	partVector[partIndex].SetPartTransformVector(operation,trans, false); //set with update
}

//return the specifed transformation vector
CVector3 CModel::GetPartTransformation(int operation, int partIndex) const
{
	return partVector[partIndex].GetPartTransformVector(operation);
}

//set up the part translation matrix
void CModel::SetPartTransformation(int partIndex, const GLfloat* matrix)
{
	partVector[partIndex].SetPartTransformMatrix(matrix);
}

//return the transformation matrix
const GLfloat* CModel::GetPartTransformation(int partIndex)	const
{
	return partVector[partIndex].GetPartTransformMatrix();
}

//return the number of parts
unsigned int CModel::GetNumberOfParts() const
{
	return numberOfParts;
}

//returns the name of the tsi file that describes the model
const char* CModel::GetModelName() const
{
	return modelName;
}