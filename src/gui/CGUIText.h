#ifndef CGUITEXT_H
#define CGUITEXT_H

#include "../OpenGLHeader.h"
#include "../engine/CVector2.h"
#include "../engine/CVector3.h"
#include <sstream>
class CGUIText
{
public:
	//////////////////////////////////////////////////////////////////////////////////////+
	// CGUIText()
	// Description:
	//    Constructor. Initalizes currentFontType to GLUT_BITMAP_8_BY_13
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	CGUIText();
	CGUIText(const CGUIText& t);

	//////////////////////////////////////////////////////////////////////////////////////+
	// ~CGUIText()
	// Description:
	//    Destructor.
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	~CGUIText();

	//////////////////////////////////////////////////////////////////////////////////////+
	// SetFont()
	// Description:
	//    Sets the glut bitmap font type
	// Parameters:
	//    int fontType
	// Return Value:
	//    none
	// Implementation Notes
	//    fontType id corresponds the following way
	//
	//	  value      glut defined type
	//		0 ....... GLUT_BITMAP_8_BY_13
	//		1 ....... GLUT_BITMAP_9_BY_15
	//		2 ....... GLUT_BITMAP_TIMES_ROMAN_10
	//		3 ....... GLUT_BITMAP_TIMES_ROMAN_24
	//		4 ....... GLUT_BITMAP_HELVETICA_10
	//		5 ....... GLUT_BITMAP_HELVETICA_12
	//		6 ....... GLUT_BITMAP_HELVETICA_18
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetFont(int fontType);
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// Print()
	// Description:
	//    Prints a string at a specified x y position
	// Parameters: 
	//	  (For all Overloaded functions)
	//    float x - x raster position
	//	  float y - y raster position
	//	  float z - z raster position
	//	  Point2D pt - x y raster position
	//	  Point3D pt - x y z raster position
	//
	//	  string str - string to be printed
	//
	//    int num - number attached to string
	//
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	void Print(float x, float y,		  const std::string& str) const;
	void Print(float x, float y, float z, const std::string& str) const;
	void Print(const CVector2& pt,		  const std::string& str) const;
	void Print(const CVector3& pt,		  const std::string& str) const;

	void Print(float x, float y,		  std::string str, int num);
	void Print(float x, float y, float z, std::string str, int num);
	void Print(const CVector2& pt,		  std::string str, int num);
	void Print(const CVector3& pt,		  std::string str, int num);

	void Print(float x, float y,		  std::string str, float num);
	void Print(float x, float y, float z, std::string str, float num);
	void Print(const CVector2& pt,		  std::string str, float num);
	void Print(const CVector3& pt,		  std::string str, float num);

	CGUIText& operator=(const CGUIText& t);
private:
	void* currentFontType;									//currently selected font type see SetFont() description for values
};
#endif