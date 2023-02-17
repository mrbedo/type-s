#ifndef CGUITextBox_H
#define CGUITextBox_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUITextBox.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2004
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  This class provides a text field box in which you can type into and get a sting
//	value out of it
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CGUIButton.h"
#include "../engine/CInput.h"

//text class
class CGUITextBox: public CGUIButton
{
public:
	CGUITextBox();
	//Post- Constructor initalizes member variables

	~CGUITextBox();
	//Post- Destructor

	void TextFieldInit();
	//Pre-
	//Post- initalizes the text field

	void SetMaxChars(unsigned int m);
	//Pre-  maximum charachter length exists
	//Post- new maximum charachter length is assigned

	void SetCursorType(char cursorChar, unsigned int blinkFrequency);
	//Pre-  default cursor charachter and blinking frequency exist
	//Post- updates the cursor type and blinking speed

	void Draw(bool mask);
	//Pre-  text field attributes exist
	//Post- text field is rendered to the screen with specified attributes
	//		if mask then charachters are shown as *

	void SetInput();
	//Pre-  text field string value exists
	//Post- updated with the specified key inserted
	//		if the key is invalid, nothing is updated
	//		if backspace is passed in one charachter is deleted from

	std::string GetStringValue();
	//Pre-  string value exists
	//Post- returns the value of the string
	
	float GetFloatValue();
	//Pre-  string value exists
	//Post- returns the value of the string

	bool IsActive();
	//Pre-  active flag exists
	//Post- returns the active flag

	void SetMask(bool m);
	//Pre-  mask flag exists
	//Post- if true, all charachters are displayed by '*'

	void SetDigitOnly(bool d);
	//Pre-  digitOnly flag exists
	//Post- if true, all charachter input is limited to digits

	bool IsDigitOnly();
	//returns digitOnly flag

	void SetActive(bool a);
	//Pre-  active flag exists
	//Post- sets the active flag
private:
	CInput *input;

	bool active;											//if selected you can type into the text box
	bool digitOnly;											//restricts value to digit values only
	unsigned int maxChars;									//bounds the size of the value string
	unsigned int cursorSpeed;								//how many frames should be waited for the cursor to blink
	char cursor;											//character to represent cursor

	std::string stringValue;											//holds the text in the text field
	float floatValue;

};
#endif