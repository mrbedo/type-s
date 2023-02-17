#ifndef CGUIGLOBALEVENTHANDLER_H
#define CGUIGLOBALEVENTHANDLER_H

class CGUIGlobalEventHandler
{
public:
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
	~CGUIGlobalEventHandler();

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
	static CGUIGlobalEventHandler* Instance();

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
	void SetSingleItemActiveMode(bool);

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
	bool GetSingleItemActiveMode();


private:	    
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
	CGUIGlobalEventHandler();

	bool singleItemActiveMode;
};
#endif
