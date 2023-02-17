//***********************************************************************//
//																		 //
//		- "Talk to me like I'm a 3 year old!" Programming Lessons -		 //
//                                                                       //
//		$Author:		DigiBen		digiben@gametutorials.com			 //
//																		 //
//		$Program:		Octree3	 										 //
//																		 //
//		$Description:	A working octree with a .3ds file format scene   //
//																		 //
//		$Date:			2/16/02											 //
//																		 //
//***********************************************************************//

#include "C3DSFile.h"
#include <iostream>
#include <cmath>

using namespace std;

// This file handles all of the code needed to load a .3DS file.
// Basically, how it works is, you load a chunk, then you check
// the chunk ID.  Depending on the chunk ID, you load the information
// that is stored in that chunk.  If you do not want to read that information,
// you read past it.  You know how many bytes to read past the chunk because
// every chunk stores the length in bytes of that chunk.

//	This constructor initializes the tChunk data
C3DSFile::C3DSFile()
{
	currentChunk = new tChunk;								// Initialize and allocate our current chunk
	tempChunk = new tChunk;									// Initialize and allocate a temporary chunk
}

C3DSFile::~C3DSFile()
{
}

//	This is called by the client to open the .3ds file, read it, then clean up
bool C3DSFile::Import3DS(C3DStructure *s3D, string strFileName, bool loadTextures)
{
//	cout<<"Loading 3DS file: "<<strFileName<<"..."<<endl;
	char strMessage[255] = {0};
	loadMaterialInfo= loadTextures;

	filePtr = fopen(strFileName.c_str(), "rb");		// Open the 3DS file
	if(!filePtr) 										// Make sure we have a valid file pointer (we found the file)
	{
		cout<<"ERROR>> Unable to find "<<strFileName.c_str()<<". Cannot open file for reading!";
		return false;
	}

	ReadChunk(currentChunk);								// Read the first chuck of the file to see if it's a 3DS file
	if (currentChunk->ID != PRIMARY)						// Make sure this is a 3DS file
	{
		cout<<"ERROR>> Unable to load PRIMARY chuck from "<<strFileName.c_str();
		return false;
	}

	ProcessNextChunk(s3D, currentChunk);					// Begin loading objects, by calling this recursive function
	ComputeNormals(s3D);									// After we have read the whole 3DS file, we want to calculate our own vertex normals.
	CleanUp();												// Clean up after everything
	
//	cout<<"loaded!"<<endl;

	return true;
}

// Free up memory
void C3DSFile::CleanUp()
{

	fclose(filePtr);									// Close the current file pointer
	delete currentChunk;									// Free the current chunk
	delete tempChunk;										// Free our temporary chunk
	currentChunk=0;
	tempChunk=0;
}

//Process next binary chunk of the file
void C3DSFile::ProcessNextChunk(C3DStructure *s3D, tChunk *prevChunk)
{
	S3DObject	  newObject  = {0};			// This is used to add to our object list
	SMaterialInfo newTexture = {0};			// This is used to add to our material list

	unsigned int version = 0;								// This will hold the file version
	int buffer[50000] = {0};								// This is used to read past unwanted data

	currentChunk = new tChunk;								// Allocate a new chunk				

	while (prevChunk->bytesRead < prevChunk->length)
	{
		ReadChunk(currentChunk);							// Read next Chunk
		
		switch (currentChunk->ID)							// Check the chunk ID
		{
		case VERSION:										// This holds the version of the file
			currentChunk->bytesRead += fread(&version, 1, currentChunk->length - currentChunk->bytesRead, filePtr);
			if (version > 0x03)	cout<<"WARNING>> This 3DS file is over version 3 so it may load incorrectly";
			
			break;

		case OBJECTINFO:
			ReadChunk(tempChunk);			// Read the next chunk
			tempChunk->bytesRead += fread(&version, 1, tempChunk->length - tempChunk->bytesRead, filePtr);
			currentChunk->bytesRead += tempChunk->bytesRead;			// Increase the bytesRead by the bytes read from the last chunk
			ProcessNextChunk(s3D, currentChunk);			// Go to the next chunk, which is the object has a texture, it should be MATERIAL, then OBJECT.

			break;

		case MATERIAL:
			if(loadMaterialInfo)// This holds the material information
			{
				s3D->numOfMaterials++;			// Increase the number of materials
				s3D->matVector.push_back(newTexture);			// Add a empty texture structure to our texture list.
				ProcessNextMaterialChunk(s3D, currentChunk);			// Proceed to the material loading function
			}
			break;

		case OBJECT:							// This holds the name of the object being read
			s3D->numOfObjects++;				// Increase the object count
			s3D->objVector.push_back(newObject);// Add a new tObject node to our list of objects (like a link list)
			currentChunk->bytesRead+=GetString(s3D->objVector[s3D->numOfObjects - 1].strName);// Get the name of the object and store it, then add the read bytes to our byte counter.
			ProcessNextObjectChunk(s3D, &(s3D->objVector[s3D->numOfObjects - 1]), currentChunk);// Now proceed to read in the rest of the object information
			
			break;

		//case EDITKEYFRAME:
		//	currentChunk->bytesRead += fread(buffer, 1, currentChunk->length - currentChunk->bytesRead, filePtr);
		//	break;

		default: 
			currentChunk->bytesRead += fread(buffer, 1, currentChunk->length - currentChunk->bytesRead, filePtr);
			break;
		}
		prevChunk->bytesRead += currentChunk->bytesRead;// Add the bytes read from the last chunk to the previous chunk passed in.
		
	}

	delete currentChunk;	// Free the current chunk and set it back to the previous chunk (since it started that way)
	currentChunk = prevChunk;
}


///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles all the information about the objects in the file
/////
///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void C3DSFile::ProcessNextObjectChunk(C3DStructure *s3D, S3DObject *obj, tChunk *prevChunk)
{
	int buffer[50000] = {0};								// This is used to read past unwanted data
	currentChunk = new tChunk;							// Allocate a new chunk to work with

	while (prevChunk->bytesRead < prevChunk->length)	// Continue to read these chunks until we read the end of this sub chunk
	{
		ReadChunk(currentChunk);							// Read the next chunk

		switch (currentChunk->ID)							// Check which chunk we just read
		{
		case OBJECT_MESH:									// This lets us know that we are reading a new object
			ProcessNextObjectChunk(s3D, obj, currentChunk);			// We found a new object, so let's read in it's info using recursion
			break;

		case OBJECT_VERTICES:								// This is the objects vertices
			ReadVertices(obj,currentChunk);
			break;

		case OBJECT_FACES:									// This is the objects face information
			ReadVertexIndices(obj,currentChunk);
			break;

		case OBJECT_MATERIAL:								// This holds the material name that the object has
			
			// This chunk holds the name of the material that the object has assigned to it.
			// This could either be just a color or a texture map.  This chunk also holds
			// the faces that the texture is assigned to (In the case that there is multiple
			// textures assigned to one object, or it just has a texture on a part of the object.
			// Since most of my game objects just have the texture around the whole object, and 
			// they aren't multitextured, I just want the material name.

			ReadObjectMaterial(s3D, obj, currentChunk);	// We now will read the name of the material assigned to this object
			break;

		case OBJECT_UV:										// This holds the UV texture coordinates for the object
			ReadUVCoordinates(obj, currentChunk);		// This chunk holds all of the UV coordinates for our object.  Let's read them in.
			break;

		default:  
			currentChunk->bytesRead += fread(buffer, 1, currentChunk->length - currentChunk->bytesRead, filePtr);			// Read past the ignored or unknown chunks
			break;
		}

		prevChunk->bytesRead += currentChunk->bytesRead;		// Add the bytes read from the last chunk to the previous chunk passed in.
	}

	delete currentChunk;									// Free the current chunk and set it back to the previous chunk (since it started that way)
	currentChunk = prevChunk;
}


///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles all the information about the material (Texture)
/////
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void C3DSFile::ProcessNextMaterialChunk(C3DStructure *s3D, tChunk *prevChunk)
{
	int buffer[50000] = {0};								// This is used to read past unwanted data
	currentChunk = new tChunk;							// Allocate a new chunk to work with

	while (prevChunk->bytesRead < prevChunk->length)	// Continue to read these chunks until we read the end of this sub chunk
	{
		ReadChunk(currentChunk);							// Read the next chunk
		switch (currentChunk->ID)							// Check which chunk we just read in

		{
		case MATNAME:										// This chunk holds the name of the material
			currentChunk->bytesRead += fread(s3D->matVector[s3D->numOfMaterials - 1].strName, 1, currentChunk->length - currentChunk->bytesRead, filePtr);			// Here we read in the material name
			break;

		case MATDIFFUSE:									// This holds the R G B color of our object
			ReadColorChunk(&(s3D->matVector[s3D->numOfMaterials - 1]), currentChunk);
			break;
		
		case MATMAP:										// This is the header for the texture info
			ProcessNextMaterialChunk(s3D, currentChunk);			// Proceed to read in the material information
			break;

		case MATMAPFILE:									// This stores the file name of the material
			currentChunk->bytesRead += fread(s3D->matVector[s3D->numOfMaterials - 1].strFile, 1, currentChunk->length - currentChunk->bytesRead, filePtr);			// Here we read in the material's file name
			break;
		
		default:  											// Read past the ignored or unknown chunks
			currentChunk->bytesRead += fread(buffer, 1, currentChunk->length - currentChunk->bytesRead, filePtr);
			break;
		}

		prevChunk->bytesRead += currentChunk->bytesRead;		// Add the bytes read from the last chunk to the previous chunk passed in.
	}

	delete currentChunk;									// Free the current chunk and set it back to the previous chunk (since it started that way)
	currentChunk = prevChunk;
}

///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in a chunk ID and it's length in bytes
/////
///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// This reads the chunk ID which is 2 bytes.
	// The chunk ID is like OBJECT or MATERIAL.  It tells what data is
	// able to be read in within the chunks section.  
	// Then, we read the length of the chunk which is 4 bytes.
	// This is how we know how much to read in, or read past.

void C3DSFile::ReadChunk(tChunk *pChunk)
{
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, filePtr);
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, filePtr);
}

///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in a string of characters
/////
///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int C3DSFile::GetString(char *c)
{
	int i = 0;
	fread(c, 1, 1, filePtr);							// Read 1 byte of data which is the first letter of the string

	while (*(c + i++) != 0) 								// Loop until we get 0
		fread(c + i, 1, 1, filePtr);					// Read in a character at a time until we hit 0.

	return strlen(c) + 1;										// Return the string length, which is how many bytes we read in (including the 0)
}


///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in the RGB color data
/////
///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void C3DSFile::ReadColorChunk(SMaterialInfo *mat, tChunk *pChunk)
{
	ReadChunk(tempChunk);									// Read the color chunk info
	tempChunk->bytesRead += fread(mat->color, 1, tempChunk->length - tempChunk->bytesRead, filePtr);			// Read in the R G B color (3 bytes - 0 through 255)
	pChunk->bytesRead    += tempChunk->bytesRead;				// Add the bytes read to our chunk
}


///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in the indices for the vertex array
/////
///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// In order to read in the vertex indices for the object, we need to first
	// read in the number of them, then read them in.  Remember,
	// we only want 3 of the 4 values read in for each face.  The fourth is
	// a visibility flag for 3D Studio Max that doesn't mean anything to us.

	// Read in the number of faces that are in this object (int)

	// Alloc enough memory for the faces and initialize the structure
	// Go through all of the faces in this object

		// Next, we read in the A then B then C index for the face, but ignore the 4th value.
		// The fourth value is a visibility flag for 3D Studio Max, we don't care about this.

			// Read the first vertice index for the current face 

				// Store the index in our face structure.
void C3DSFile::ReadVertexIndices(S3DObject *obj, tChunk *prevChunk)
{
	unsigned short index = 0;					// This is used to read in the current face index

	prevChunk->bytesRead += fread(&obj->numOfFaces, 1, 2, filePtr);
	obj->faces =   new SFace[obj->numOfFaces];
	memset(obj->faces, 0, sizeof(SFace) * obj->numOfFaces);

	for(int i = 0; i<obj->numOfFaces; ++i)
	{
		for(int j = 0; j<4; j++)
		{
			prevChunk->bytesRead += fread(&index, 1, sizeof(index), filePtr);
		
			if(j<3) 
				obj->faces[i].triVertIndex[j] = index;
		}
	}
}


///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in the UV coordinates for the object
/////
///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// In order to read in the UV indices for the object, we need to first
	// read in the amount there are, then read them in.

	// Read in the number of UV coordinates there are (int)
	// Allocate memory to hold the UV coordinates
	// Read in the texture coodinates (an array 2 float)

void C3DSFile::ReadUVCoordinates(S3DObject *obj, tChunk *prevChunk)
{
	prevChunk->bytesRead += fread(&obj->numTexVertex, 1, 2, filePtr);
	obj->texVerts = new CVector2 [obj->numTexVertex];
	prevChunk->bytesRead += fread(obj->texVerts, 1, prevChunk->length - prevChunk->bytesRead, filePtr);
}


///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in the vertices for the object
/////
///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// Like most chunks, before we read in the actual vertices, we need
	// to find out how many there are to read in.  Once we have that number
	// we then fread() them into our vertice array.

	// Read in the number of vertices (int)
	// Allocate the memory for the verts and initialize the structure
	// Read in the array of vertices (an array of 3 floats)
	// Now we should have all of the vertices read in.  Because 3D Studio Max
	// Models with the Z-Axis pointing up (strange and ugly I know!), we need
	// to flip the y values with the z values in our vertices.  That way it
	// will be normal, with Y pointing up.  If you prefer to work with Z pointing
	// up, then just delete this next loop.  Also, because we swap the Y and Z
	// we need to negate the Z to make it come out correctly.

	// Go through all of the vertices that we just read and swap the Y and Z values
		// Store off the Y value
		// Set the Y value to the Z value

		// Set the Z value to the Y value, 
		// but negative Z because 3D Studio max does the opposite.

void C3DSFile::ReadVertices(S3DObject *obj, tChunk *prevChunk)
{
	prevChunk->bytesRead += fread(&(obj->numOfVerts), 1, 2, filePtr);

	obj->verts = new CVector3 [obj->numOfVerts];
	memset(obj->verts, 0, sizeof(CVector3) * obj->numOfVerts);
	
	prevChunk->bytesRead += fread(obj->verts, 1, prevChunk->length - prevChunk->bytesRead, filePtr);

	for(int i = 0; i < obj->numOfVerts; ++i)
	{
		float fTempY	= obj->verts[i].y;
		obj->verts[i].y = obj->verts[i].z;
		obj->verts[i].z = -fTempY;
	}
}


///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in the material name assigned to the object and sets the materialID
/////
///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	// *What is a material?*  - A material is either the color or the texture map of the object.
	// It can also hold other information like the brightness, shine, etc... Stuff we don't
	// really care about.  We just want the color, or the texture map file name really.

	// Here we read the material name that is assigned to the current object.
	// strMaterial should now have a string of the material name, like "Material #2" etc..

	// Now that we have a material name, we need to go through all of the materials
	// and check the name against each material.  When we find a material in our material
	// list that matches this name we just read in, then we assign the materialID
	// of the object to that material index.  You will notice that we passed in the
	// model to this function.  This is because we need the number of textures.
	// Yes though, we could have just passed in the model and not the object too.

	// Go through all of the textures
		// If the material we just read in matches the current texture name
			// If the strFile has a string length of 1 and over it's a texture

	// Read past the rest of the chunk since we don't care about shared vertices
	// You will notice we subtract the bytes already read in this chunk from the total length.
void C3DSFile::ReadObjectMaterial(C3DStructure *s3D, S3DObject *obj, tChunk *prevChunk)
{
	char strMaterial[255] = {0};			// This is used to hold the objects material name
	int buffer[50000] = {0};				// This is used to read past unwanted data

	prevChunk->bytesRead += GetString(strMaterial);

	for(int i = 0; i < s3D->numOfMaterials; ++i)
	{
		if(strcmp(strMaterial, s3D->matVector[i].strName) == 0)
		{
			obj->materialID = i;						// Set the material ID to the current index 'i' and stop checking
	
			if(strlen(s3D->matVector[i].strFile) > 0)	// Now that we found the material, check if it's a texture map.
			{	
				obj->bHasTexture = true;				// Set the object's flag to say it has a texture map to bind.
			}
			break;
		}
		else
		{
			obj->materialID = -1;						// Set the ID to -1 to show there is no material for this object
		}
	}
	prevChunk->bytesRead += fread(buffer, 1, prevChunk->length - prevChunk->bytesRead, filePtr);
}			

///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//	This function computes the normals and vertex normals of the objects
//
// What are vertex normals?  And how are they different from other normals?
// Well, if you find the normal to a triangle, you are finding a "Face Normal".
// If you give OpenGL a face normal for lighting, it will make your object look
// really flat and not very round.  If we find the normal for each vertex, it makes
// the smooth lighting look.  This also covers up blocky looking objects and they appear
// to have more polygons than they do.    Basically, what you do is first
// calculate the face normals, then you take the average of all the normals around each
// vertex.  It's just averaging.  That way you get a better approximation for that vertex.
//
// Go through each of the objects to calculate their normals
//		Get the current object
//		Here we allocate all the memory we need to calculate the normals
//		Go though all of the faces of this object
//			To cut down LARGE code, we extract the 3 points of this face
//			Now let's calculate the face normals (Get 2 vectors and find the cross product of those 2)
///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
void C3DSFile::ComputeNormals(C3DStructure *s3D)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];

	if(s3D->numOfObjects <= 0)							// If there are no objects, we can skip this part
		return;

	for(int index = 0; index < s3D->numOfObjects; ++index)
	{
		S3DObject *obj = &(s3D->objVector[index]);

		CVector3 *normals		= new CVector3[obj->numOfFaces];
		CVector3 *tempNormals	= new CVector3[obj->numOfFaces];
		obj->normals			= new CVector3[obj->numOfVerts];

		for(int i=0; i < obj->numOfFaces; ++i)
		{												
			vPoly[0] = obj->verts[obj->faces[i].triVertIndex[0]];
			vPoly[1] = obj->verts[obj->faces[i].triVertIndex[1]];
			vPoly[2] = obj->verts[obj->faces[i].triVertIndex[2]];


			vVector1 = vPoly[0] - vPoly[2];				// Get the vector of the polygon (we just need 2 sides for the normal)
			vVector2 = vPoly[2] - vPoly[1];				// Get a second vector of the polygon

			vNormal  = Cross(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
			tempNormals[i] = vNormal;					// Save the un-normalized normal for the vertex normals
			vNormal  = Normalize(vNormal);				// Normalize the cross product to give us the polygons normal

			normals[i] = vNormal;							// Assign the normal to the list of normals


		}


		CVector3 vSum(0.0, 0.0, 0.0);						// Get The Vertex Normals

		CVector3 vZero = vSum;
		int shared=0;

		for (i = 0; i < obj->numOfVerts; ++i)			// Go through all of the vertices
		{
			for (int j = 0; j < obj->numOfFaces; ++j)	// Go through all of the triangles
			{												// Check if the vertex is shared by another face
				if (obj->faces[j].triVertIndex[0] == i || 
					obj->faces[j].triVertIndex[1] == i || 
					obj->faces[j].triVertIndex[2] == i)
				{
					vSum = vSum + tempNormals[j];			// Add the un-normalized normal of the shared face
					shared++;								// Increase the number of shared triangles
				}
			}      
			obj->normals[i] = DivideVectorByScaler(vSum, float(-shared));
			obj->normals[i] = Normalize(obj->normals[i]);	

			vSum = vZero;									// Reset the sum
			shared = 0;										// Reset the shared
		}
	
		delete [] tempNormals;								// Free our memory and start over on the next object
		delete [] normals;
		tempNormals=0;
		normals=0;
	}
}


// This divides a vector by a single number (scalar) and returns the result
CVector3 C3DSFile::DivideVectorByScaler(const CVector3 &vVector1, const float scaler)
{
	CVector3 vResult;							// The variable to hold the resultant vector
	
	vResult.x = vVector1.x / scaler;			// Divide Vector1's x value by the scaler
	vResult.y = vVector1.y / scaler;			// Divide Vector1's y value by the scaler
	vResult.z = vVector1.z / scaler;			// Divide Vector1's z value by the scaler

	return vResult;								// Return the resultant vector
}

// This returns the cross product between 2 vectors
CVector3 C3DSFile::Cross(CVector3 &vVector1, CVector3 &vVector2)
{
	CVector3 vCross;								// The vector to hold the cross product
												// Get the X value
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
												// Get the Y value
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
												// Get the Z value
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;								// Return the cross product
}

// This returns the normal of a vector
CVector3 C3DSFile::Normalize(CVector3 &vNormal)
{
	double Magnitude;							// This holds the magitude			

	Magnitude = Mag(vNormal);					// Get the magnitude

	vNormal.x /= (float)Magnitude;				// Divide the vector's X by the magnitude
	vNormal.y /= (float)Magnitude;				// Divide the vector's Y by the magnitude
	vNormal.z /= (float)Magnitude;				// Divide the vector's Z by the magnitude

	return vNormal;								// Return the normal
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This was a HUGE amount of knowledge and probably the largest tutorial yet!
// In the next tutorial we will show you how to load a text file format called .obj.
// This is the most common 3D file format that almost ANY 3D software will import.
//
// Once again I should point out that the coordinate system of OpenGL and 3DS Max are different.
// Since 3D Studio Max Models with the Z-Axis pointing up (strange and ugly I know! :), 
// we need to flip the y values with the z values in our vertices.  That way it
// will be normal, with Y pointing up.  Also, because we swap the Y and Z we need to negate 
// the Z to make it come out correctly.  This is also explained and done in ReadVertices().
//
// CHUNKS: What is a chunk anyway?
// 
// "The chunk ID is a unique code which identifies the type of data in this chunk 
// and also may indicate the existence of subordinate chunks. The chunk length indicates 
// the length of following data to be associated with this chunk. Note, this may 
// contain more data than just this chunk. If the length of data is greater than that 
// needed to fill in the information for the chunk, additional subordinate chunks are 
// attached to this chunk immediately following any data needed for this chunk, and 
// should be parsed out. These subordinate chunks may themselves contain subordinate chunks. 
// Unfortunately, there is no indication of the length of data, which is owned by the current 
// chunk, only the total length of data attached to the chunk, which means that the only way 
// to parse out subordinate chunks is to know the exact format of the owning chunk. On the 
// other hand, if a chunk is unknown, the parsing program can skip the entire chunk and 
// subordinate chunks in one jump. " - Jeff Lewis (werewolf@worldgate.com)
//
// In a short amount of words, a chunk is defined this way:
// 2 bytes - Stores the chunk ID (OBJECT, MATERIAL, PRIMARY, etc...)
// 4 bytes - Stores the length of that chunk.  That way you know when that
//           chunk is done and there is a new chunk.
//
// So, to start reading the 3DS file, you read the first 2 bytes of it, then
// the length (using fread()).  It should be the PRIMARY chunk, otherwise it isn't
// a .3DS file.  
//
// Below is a list of the order that you will find the chunks and all the know chunks.
// If you go to www.wosit.org you can find a few documents on the 3DS file format.
// You can also take a look at the 3DS Format.rtf that is included with this tutorial.
//
//
//
//      MAIN3DS  (0x4D4D)
//     |
//     +--EDIT3DS  (0x3D3D)
//     |  |
//     |  +--EDIT_MATERIAL (0xAFFF)
//     |  |  |
//     |  |  +--MAT_NAME01 (0xA000) (See mli Doc) 
//     |  |
//     |  +--EDIT_CONFIG1  (0x0100)
//     |  +--EDIT_CONFIG2  (0x3E3D) 
//     |  +--EDIT_VIEW_P1  (0x7012)
//     |  |  |
//     |  |  +--TOP            (0x0001)
//     |  |  +--BOTTOM         (0x0002)
//     |  |  +--LEFT           (0x0003)
//     |  |  +--RIGHT          (0x0004)
//     |  |  +--FRONT          (0x0005) 
//     |  |  +--BACK           (0x0006)
//     |  |  +--USER           (0x0007)
//     |  |  +--CAMERA         (0xFFFF)
//     |  |  +--LIGHT          (0x0009)
//     |  |  +--DISABLED       (0x0010)  
//     |  |  +--BOGUS          (0x0011)
//     |  |
//     |  +--EDIT_VIEW_P2  (0x7011)
//     |  |  |
//     |  |  +--TOP            (0x0001)
//     |  |  +--BOTTOM         (0x0002)
//     |  |  +--LEFT           (0x0003)
//     |  |  +--RIGHT          (0x0004)
//     |  |  +--FRONT          (0x0005) 
//     |  |  +--BACK           (0x0006)
//     |  |  +--USER           (0x0007)
//     |  |  +--CAMERA         (0xFFFF)
//     |  |  +--LIGHT          (0x0009)
//     |  |  +--DISABLED       (0x0010)  
//     |  |  +--BOGUS          (0x0011)
//     |  |
//     |  +--EDIT_VIEW_P3  (0x7020)
//     |  +--EDIT_VIEW1    (0x7001) 
//     |  +--EDIT_BACKGR   (0x1200) 
//     |  +--EDIT_AMBIENT  (0x2100)
//     |  +--EDIT_OBJECT   (0x4000)
//     |  |  |
//     |  |  +--OBJ_TRIMESH   (0x4100)      
//     |  |  |  |
//     |  |  |  +--TRI_VERTEXL          (0x4110) 
//     |  |  |  +--TRI_VERTEXOPTIONS    (0x4111)
//     |  |  |  +--TRI_MAPPINGCOORS     (0x4140) 
//     |  |  |  +--TRI_MAPPINGSTANDARD  (0x4170)
//     |  |  |  +--TRI_FACEL1           (0x4120)
//     |  |  |  |  |
//     |  |  |  |  +--TRI_SMOOTH            (0x4150)   
//     |  |  |  |  +--TRI_MATERIAL          (0x4130)
//     |  |  |  |
//     |  |  |  +--TRI_LOCAL            (0x4160)
//     |  |  |  +--TRI_VISIBLE          (0x4165)
//     |  |  |
//     |  |  +--OBJ_LIGHT    (0x4600)
//     |  |  |  |
//     |  |  |  +--LIT_OFF              (0x4620)
//     |  |  |  +--LIT_SPOT             (0x4610) 
//     |  |  |  +--LIT_UNKNWN01         (0x465A) 
//     |  |  | 
//     |  |  +--OBJ_CAMERA   (0x4700)
//     |  |  |  |
//     |  |  |  +--CAM_UNKNWN01         (0x4710)
//     |  |  |  +--CAM_UNKNWN02         (0x4720)  
//     |  |  |
//     |  |  +--OBJ_UNKNWN01 (0x4710)
//     |  |  +--OBJ_UNKNWN02 (0x4720)
//     |  |
//     |  +--EDIT_UNKNW01  (0x1100)
//     |  +--EDIT_UNKNW02  (0x1201) 
//     |  +--EDIT_UNKNW03  (0x1300)
//     |  +--EDIT_UNKNW04  (0x1400)
//     |  +--EDIT_UNKNW05  (0x1420)
//     |  +--EDIT_UNKNW06  (0x1450)
//     |  +--EDIT_UNKNW07  (0x1500)
//     |  +--EDIT_UNKNW08  (0x2200)
//     |  +--EDIT_UNKNW09  (0x2201)
//     |  +--EDIT_UNKNW10  (0x2210)
//     |  +--EDIT_UNKNW11  (0x2300)
//     |  +--EDIT_UNKNW12  (0x2302)
//     |  +--EDIT_UNKNW13  (0x2000)
//     |  +--EDIT_UNKNW14  (0xAFFF)
//     |
//     +--KEYF3DS (0xB000)
//        |
//        +--KEYF_UNKNWN01 (0xB00A)
//        +--............. (0x7001) ( viewport, same as editor )
//        +--KEYF_FRAMES   (0xB008)
//        +--KEYF_UNKNWN02 (0xB009)
//        +--KEYF_OBJDES   (0xB002)
//           |
//           +--KEYF_OBJHIERARCH  (0xB010)
//           +--KEYF_OBJDUMMYNAME (0xB011)
//           +--KEYF_OBJUNKNWN01  (0xB013)
//           +--KEYF_OBJUNKNWN02  (0xB014)
//           +--KEYF_OBJUNKNWN03  (0xB015)  
//           +--KEYF_OBJPIVOT     (0xB020)  
//           +--KEYF_OBJUNKNWN04  (0xB021)  
//           +--KEYF_OBJUNKNWN05  (0xB022)  
//
// Once you know how to read chunks, all you have to know is the ID you are looking for
// and what data is stored after that ID.  You need to get the file format for that.
// I can give it to you if you want, or you can go to www.wosit.org for several versions.
// Because this is a proprietary format, it isn't a official document.
//
// I know there was a LOT of information blown over, but it is too much knowledge for
// one tutorial.  In the animation tutorial that I eventually will get to, some of
// the things explained here will be explained in more detail.  I do not claim that
// this is the best .3DS tutorial, or even a GOOD one :)  But it is a good start, and there
// isn't much code out there that is simple when it comes to reading .3DS files.
// So far, this is the best I have seen.  That is why I made it :)
// 
// I would like to thank www.wosit.org and Terry Caton (tcaton@umr.edu) for his help on this.
//
// Let me know if this helps you out!
// 
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// www.GameTutorials.com
//
//

