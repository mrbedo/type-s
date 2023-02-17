#ifndef COPENGLHEADER_H
#define COPENGLHEADER_H
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  COpenGLHeader.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Modified by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2003
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//	Includes and defines necessary OpenGL win32, OpenGL extensions, windows and Glut libraries.
//  
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#pragma warning(disable: 4786)								//disable  std string warning
#pragma warning(disable: 4800)								//disable  floating to boolean warning

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glut32.lib")
#include "gl/gl.h"
#include "gl/glu.h"	
#include "gl/glaux.h"
#include "gl/glut.h"
#include "gl/glext.h"

#endif