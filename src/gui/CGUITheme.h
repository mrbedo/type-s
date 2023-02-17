#ifndef CGUITHEME_H
#define CGUITHEME_H

#include "../engine/CVector3.h"

class CGUITheme
{
public:
	CGUITheme(){};
	~CGUITheme(){};
	float themeSizeX;
	float themeSizeY;
	CVector3 themeColor[3];
	CVector3 themeBorderColor[3];
	CVector3 themeScale[3];
	CVector3 themeTextColor[3];
	CVector3 themeTextPosition[3];
	CVector3 themeTextBoxColor[3];
	float themeBorderThickness[3];
	int themeTextFont;
};

#endif