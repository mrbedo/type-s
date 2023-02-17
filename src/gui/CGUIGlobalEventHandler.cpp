#include "CGUIGlobalEventHandler.h"

//////////////////////////////////////////////////////////////////////////////////////+
// CGUIGlobalEventHandler()
// Description:
//    Constructor
// Parameters:
//    None
// Return Value:
//    None
// Implementation Notes
//    Private to prevent multiple instances of the CScreenManager object
//	  thus enabling global use of object
//////////////////////////////////////////////////////////////////////////////////////-
CGUIGlobalEventHandler::CGUIGlobalEventHandler()
{
}

//////////////////////////////////////////////////////////////////////////////////////+
// ~CGUIGlobalEventHandler()
// Description:
//    Destructor
// Parameters:
//    None
// Return Value:
//    None
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
CGUIGlobalEventHandler::~CGUIGlobalEventHandler()
{
}

//////////////////////////////////////////////////////////////////////////////////////+
// Instance()
// Description:
//    Returns the instance of the input object
// Parameters:
//    None
// Return Value:
//    CScreenManager*
// Implementation Notes
//    Use this method to initialize all new pointers to this object
//	  so that there is only a single instance of that object, making it global
//////////////////////////////////////////////////////////////////////////////////////-
CGUIGlobalEventHandler* CGUIGlobalEventHandler::Instance()
{
	static CGUIGlobalEventHandler input;
	return &input;
}

//////////////////////////////////////////////////////////////////////////////////////+
// SetSingleItemActiveMode()
// Description:
//    
// Parameters:
//    
// Return Value:
//    None
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
void CGUIGlobalEventHandler::SetSingleItemActiveMode(bool mode)
{
	singleItemActiveMode=mode;
}

//////////////////////////////////////////////////////////////////////////////////////+
// GetSingleItemActiveMode()
// Description:
//    
// Parameters:
//    None
// Return Value:
//    
// Implementation Notes
//    None
//////////////////////////////////////////////////////////////////////////////////////-
bool CGUIGlobalEventHandler::GetSingleItemActiveMode()
{
	return singleItemActiveMode;
}

