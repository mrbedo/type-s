///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CGUITextBox.cpp
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
#include "CGUITextBox.h"

using namespace std;



//default constructor
CGUITextBox::CGUITextBox()
{
//	cout<<"TextBox Created"<<endl;
	input= CInput::Instance();
	cursorSpeed=8;
	stringValue="";
	floatValue=0;
	cursor='<';
	active=false;
	digitOnly=false;
	maxChars=80;
}

//default destructor
CGUITextBox::~CGUITextBox()
{
//	cout<<"Text Box Destroyed!"<<endl;
}

//sets the maximum number of allowable charachters
void CGUITextBox::SetMaxChars(unsigned int m)
{
	maxChars=m-1;
}

//set the value of the string
void CGUITextBox::SetInput()
{
	string valueDisp;
	if(active)
	{
		if(input->Key(13))										//pressing enter, deselects the field
		{
			input->SetKey(13,false);
			active= false;
		}
		else
		{
			if(input->Key(8))									//backspace
			{
				input->SetKey(8,false);						//delete text using backspace key
				if(stringValue.length()>0 && stringValue.length()<= maxChars+1)
					stringValue.erase(stringValue.end()-1);
			}
			else
			{
				int key= input->GetKey();
				input->SetKey(key,false);

				if(digitOnly)								//accept only numbers
				{
					if(stringValue.length()<1)				// allow negative sign on first digit only
					{
						if((stringValue.length()<=maxChars) && ((key>='0' && key <='9') || ( key == '-') || (key == '.')))
						{
							input->SetKey(key,false);
							stringValue+=(char)key;			//add charachter
							floatValue= atof(stringValue.c_str());	//convert the string to a float value
						}
					}
					else if((stringValue.length()<=maxChars) && ((key == '.') || (key>='0' && key <='9')))
					{
						input->SetKey(key,false);

						bool decimalRepeat=false;
						if(key == '.')					// only allow a single decimal point
						{
							unsigned int i=0;
							while(!decimalRepeat && i<stringValue.length())
							{
								if(stringValue[i] == '.')
									decimalRepeat=true;
								++i;
							}
						}
						if(!decimalRepeat)
						{
							stringValue+=(char)key;							//add charachter
							floatValue= atof(stringValue.c_str());	//convert the string to a float value
						}
					}
				}
				else										//accept numbers, letters and space bar
				{
					if((stringValue.length()<=maxChars) && (key>='!'&& key <='~') || (input->Key(32)))
					{
						input->SetKey(key, false);
						stringValue+=(char)key;							//add charachter
					}
				}
			}
		}
		static bool blink=true;								//blink the cursor
		static int cs=cursorSpeed;
		if(cs<0)
		{
			blink=!blink;
			cs=cursorSpeed;
		}
		
		blink ? cursor=' ' : cursor='<'; cs--;
		valueDisp=stringValue+cursor;
	}
	else
	{
		valueDisp=stringValue;
	}
	SetText(valueDisp);
}

//sets digitOnly flag
void CGUITextBox::SetDigitOnly(bool d)
{
	digitOnly=d;
}

bool CGUITextBox::IsDigitOnly()
{
	return digitOnly;
}

//return the selected flag value
bool CGUITextBox::IsActive()
{
	return active;
}

//sets the selected flag
void CGUITextBox::SetActive(bool a)
{
	active=a;
}

string CGUITextBox::GetStringValue()
{
	return stringValue;
}

float CGUITextBox::GetFloatValue()
{
	//cout<<"FLOAT VALUE="<<floatValue<<endl;
	return floatValue;
}