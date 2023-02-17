#ifndef CGUIButton_H
#define CGUIButton_H

#include "CGUIText.h"

class CGUIButton
{
public:
	//////////////////////////////////////////////////////////////////////////////////////+
	// CGUIButton()
	// Description:
	//    Constructor.
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	CGUIButton();
	CGUIButton(const CGUIButton& b);

	//////////////////////////////////////////////////////////////////////////////////////+
	// ~CGUIButton()
	// Description:
	//    Destructor.
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	~CGUIButton();

	//////////////////////////////////////////////////////////////////////////////////////+
	// Init()
	// Description:
	//    Button created with default values and position and scale.
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	void Init();

	//////////////////////////////////////////////////////////////////////////////////////+
	// CleanUp()
	// Description:
	//    deletes newed stuff
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	int CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////+
	// SetSize()
	// Description:
	//    new widht and height are set for button
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//    None
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetSize(float w, float h);

	//////////////////////////////////////////////////////////////////////////////////////+
	// SetPosition()
	// Description:
	//    Button assigned new position per state or all states
	// Parameters: Overloaded
	//    Point3D - 3 value structure
	//    --or--
	//    Point3D* - Array of size 3 of type Point3D
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  Two versions for updating variable, one with a single <type> for all states
	//	  the other takes an array <type>* to update each specific state [up,over,down]
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetPosition(CVector3 *p);
	void SetPosition(const CVector3& p);
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// SetScale()
	// Description:
	//    Button assigned new scale per state or all states
	// Parameters: Overloaded
	//    Point3D - 3 value structure
	//    --or--
	//    Point3D* - Array of size 3 of type Point3D
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  Two versions for updating variable, one with a single <type> for all states
	//	  the other takes an array <type>* to update each specific state [up,over,down]
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetScale(CVector3 *s);
	void SetScale(const CVector3& s);
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// SetText()
	// Description:
	//    Button assigned new text per state or all states
	// Parameters: Overloaded
	//    string - text value structure
	//    --or--
	//    string* - Array of size 3 of type string
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  Two versions for updating variable, one with a single <type> for all states
	//	  the other takes an array <type>* to update each specific state [up,over,down]
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetText(std::string *t);
	void SetText(std::string t);
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// SetTextColor()
	// Description:
	//    Button assigned new text color per state or all states
	// Parameters: Overloaded
	//    Point3D - 3 value structure
	//    --or--
	//    Point3D* - Array of size 3 of type Point3D
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  Two versions for updating variable, one with a single <type> for all states
	//	  the other takes an array <type>* to update each specific state [up,over,down]
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetTextColor(CVector3 *c);
	void SetTextColor(const CVector3& c);
	
	void SetTextShadowColor(CVector3 *c);
	void SetTextShadowColor(const CVector3& c);
	//Pre-  Button text shadow color is initalized to default
	//Post- Button text shadow color is assigned the same specified value for every state
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// SetTextPosition()
	// Description:
	//    Button assigned new text position per state or all states
	//    Text position will be rendered relative to button center
	// Parameters: Overloaded
	//    Point3D - 3 value structure
	//    --or--
	//    Point3D* - Array of size 3 of type Point3D
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  Two versions for updating variable, one with a single <type> for all states
	//	  the other takes an array <type>* to update each specific state [up,over,down]
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetTextPosition(CVector3 *p);
	void SetTextPosition(const CVector3& p);

	//////////////////////////////////////////////////////////////////////////////////////+
	// SetLabel()
	// Description:
	//    Button assigned new label text
	//    label position is also set
	// Parameters: Overloaded
	//	  text-		string text for label
	//    position- CVector3 position of label, relative to button position
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetLabel(const CVector3& position, std::string text);

	//////////////////////////////////////////////////////////////////////////////////////+
	// SetColor()
	// Description:
	//    Button assigned new button color per state or all states
	// Parameters: Overloaded
	//    Point3D - 3 value structure
	//    --or--
	//    Point3D* - Array of size 3 of type Point3D
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  Two versions for updating variable, one with a single <type> for all states
	//	  the other takes an array <type>* to update each specific state [up,over,down]
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetColor(const CVector3 *c);
	void SetColor(const CVector3& c);
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// SetBorderColor()
	// Description:
	//    Button assigned new button border color per state or all states
	// Parameters: Overloaded
	//    Point3D - 3 value structure
	//    --or--
	//    Point3D* - Array of size 3 of type Point3D
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  Two versions for updating variable, one with a single <type> for all states
	//	  the other takes an array <type>* to update each specific state [up,over,down]
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetBorderColor(CVector3* c);
	void SetBorderColor(const CVector3& c);
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// SetBorderThickness()
	// Description:
	//    Button assigned new button border color per state or all states
	// Parameters: Overloaded
	//    float - thickness value
	//    --or--
	//    float* - Array of size 3 of type float
	//
	// Return Value:
	//    None
	// Implementation Notes
	//	  Two versions for updating variable, one with a single <type> for all states
	//	  the other takes an array <type>* to update each specific state [up,over,down]
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetBorderThickness(float *c);
	void SetBorderThickness(float c);

	void SetOpacity(float *o);
	void SetOpacity(float o);
	//Pre-  Button will have be completely opaque
	//Post- Blend function is applied to button for every state
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// SetTextFont()
	// Description:
	//    New font is assigned to text object
	// Parameters:
	//    int f - CGUIText type font
	// Return Value:
	//    None
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetTextFont(int f);

	//////////////////////////////////////////////////////////////////////////////////////+
	// SetState()
	// Description:
	//    Button is assigned a state 0=up, 1=over, 2=down
	// Parameters:
	//    short s - the state
	// Return Value:
	//    None
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	void SetState(short s);
	
	//////////////////////////////////////////////////////////////////////////////////////+
	// GetLabel()
	// Description:
	//    Returns the text of the label of the current state of the button
	// Parameters:
	//    none
	// Return Value:
	//    label text for specified state
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	std::string GetLabel();

	//////////////////////////////////////////////////////////////////////////////////////+
	// GetText()
	// Description:
	//    Returns the text of the current state of the button
	// Parameters:
	//    none
	// Return Value:
	//    text for specified state
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	std::string GetText();

	//////////////////////////////////////////////////////////////////////////////////////+
	// GetState()
	// Description:
	//    Returns the current state of the button
	// Parameters:
	//    None
	// Return Value:
	//    state variable
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	short GetState();
 
	//////////////////////////////////////////////////////////////////////////////////////+
	// DrawRectangle()
	// Description:
	//    Draws an empty rectangle indicating the button
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	void DrawRectangle();

	//////////////////////////////////////////////////////////////////////////////////////+
	// DrawBorder()
	// Description:
	//    Draws a border for the button
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	void DrawBorder();

	//////////////////////////////////////////////////////////////////////////////////////+
	// DisplayInState()
	// Description:
	//    Button is displayed in specifically selected state, 
	//	  not necessarily the current state.
	// Parameters:
	//    short s  - state type
	// Return Value:
	//    None
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	void DisplayInState(short s);

	//////////////////////////////////////////////////////////////////////////////////////+
	// Display()
	// Description:
	//    Button is displayed in its current state
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	void Display();

	//////////////////////////////////////////////////////////////////////////////////////+
	// SelectionDisplay()
	// Description:
	//    Renders only non visual aspects of button for area selection purposes
	// Parameters:
	//    None
	// Return Value:
	//    None
	// Implementation Notes
	//	  None
	//////////////////////////////////////////////////////////////////////////////////////-
	void SelectionDisplay();

	CGUIButton& operator=(const CGUIButton& b);


protected:
	CGUIText t;											//text object

	float width;
	float height;
	
	CVector3 position[3];									//size of button
	CVector3 scale[3];										//scale of button
	CVector3 color[3];										//color of actual button
	float opacity[3];										//opacity of button

	CVector3 borderColor[3];
	float borderThickness[3];

	std::string text[3];											//text on the button
	std::string label;											//text for lable on button
	CVector3 labelPosition;									 //label position
	CVector3 textColor[3];									//color of text on button
	CVector3 textShadowColor[3];							//color of text shadow
	CVector3 textPosition[3];								//relative position of text to button width and height
	std::string toolTip[3];										//text for the pop up text on a button

	short state;											//0=up, 1=over, 2=down
	bool withTextShadow[3];
	bool withImage[3];										//if the button is defined with bitmaps or just plain color
	bool withText[3];										//if the button will have text in it
	bool withToolTip[3];
	bool withBorder[3];
	bool withLabel;											//if the button will have a label text
};
#endif