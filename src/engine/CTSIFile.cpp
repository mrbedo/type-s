#include "CTSIFile.h"
#include <iostream>
#include <fstream>


using namespace std;
//writes a string
void CTSIFile::WriteTSI(const string& filename, const string& section, const string& key, const char * value) const
{
/*	string sectionInFile="sd ";								//designate section buffer
	string keyInFile="";									//designate key buffer

	section="["+section+"]";								//format the section word
	key+="=";												//format the key word

//	fstream file(filename);
	ofstream out(filename);
out.
	out<< value;
//	ifstream in(filename);

//	while(section!=sectionInFile && !file.eof())
//	{
//		cout<<"section:"<<section<<endl;
//		cout<<"sectionInFile:"<<sectionInFile<<endl;
//		if(sectionInFile.empty())
//			cout<<"found some emtpy space"<<endl;
	
//		file.in>>sectionInFile;
//	}

//	out.seekp(in.tellg());

//	in.close();
	out.close();
*/	
}

//returns a float
void CTSIFile::WriteTSIFloat(const string& filename, const string& section, const string& key, float floatValue) const
{
	string csection="["+section+"]",								//format the section word
		   ckey= key+"=",
		   sectionInFile="",
		   keyInFile="";									//designate key buffer

	ifstream in(filename.c_str());							//open the file stream
	ofstream out(filename.c_str());							//write to the file
	if(in.fail())
	{
		cout<<"\nTSI initalization file '"<<filename<<"' is missing or name is not correct.";
		return;
	}
	
	while(section!=sectionInFile && !in.eof())				//search the section until there is a match
		in>>sectionInFile;									//or end of file is reached

	while(key!=keyInFile && !in.eof())						//now search for the key in that section
		in>>keyInFile;										//or end of file is reached

	in>>floatValue;											//assign the next word which is a float value

	in.close();												//close the file stream
}

//writes a float
void CTSIFile::WriteTSI(const string& filename, const string& section, const string& key, float value) const
{
	string sectionInFile="sd ",
		   keyInFile="",
		   csection="["+section+"]",
		   ckey= key+"=";												//format the key word

	ofstream out(filename.c_str());
	out<< value;

	out.close();
}

//returns a string
string CTSIFile::GetTSIString(const string& filename, const string& section, const string& key) const
{
	if(!filename.c_str())
		cout<<"\nThis TSI file "<< filename <<"was not found! Cannot create object!";

	string stringValue="",
		   csection="["+section+"]",
		   ckey= key+"=";												//format the key word

	string sectionInFile="";								//designate section buffer
	string keyInFile="";									//designate key buffer

	ifstream in(filename.c_str());									//open the file stream
	
	while(csection!=sectionInFile && !in.eof())				//search the section until there is a match
		in>>sectionInFile;									//or end of file is reached

	while(ckey!=keyInFile && !in.eof())						//now search for the key in that section
		in>>keyInFile;										//or end of file is reached

	in>>stringValue;										//assign the next word which is a string value

	in.close();												//close the file stream

	return stringValue;
}

//returns a float
float CTSIFile::GetTSIFloat(const string& filename, const string& section, const string& key) const
{
	float floatValue=1.0f;
	string csection="["+section+"]",
	       ckey= key+"=",
		   sectionInFile="",
		   keyInFile="";									//designate key buffer

	ifstream in(filename.c_str());									//open the file stream
	if(in.fail())
	{
		cout<<"\nTSI initalization file '"<<filename<<"' is missing or name is not correct.";
		return floatValue;
	}
	
	while(csection!=sectionInFile && !in.eof())				//search the section until there is a match
		in>>sectionInFile;									//or end of file is reached

	while(ckey!=keyInFile && !in.eof())						//now search for the key in that section
		in>>keyInFile;										//or end of file is reached

	in>>floatValue;											//assign the next word which is a float value

	in.close();												//close the file stream
	return floatValue;
}

//returns a vector of strings
vector<string> CTSIFile::GetTSIStringVector(const string& filename, const string& section, const string& key) const
{
	vector<string> stringVector;
	string passedInString,
		   csection="["+section+"]",
		   ckey= key+"=",
		   sectionInFile="",
		   keyInFile="";									//designate key buffer

	ifstream in(filename.c_str());									//open the file stream
	if(in.fail())
	{
		cout<<"\nTSI initalization file '"<<filename<<"' is missing or name is not correct.";
		return stringVector;
	}
	
	while(csection!=sectionInFile && !in.eof())				//search the section until there is a match
		in>>sectionInFile;									//or end of file is reached

	while(ckey!=keyInFile && !in.eof())						//now search for the key in that section
		in>>keyInFile;										//or end of file is reached

	while(!in.eof())
	{
		if(passedInString=="#endVector")
		{
			stringVector.pop_back();						//get rid of the #endVector keyword
			in.close();												//close the file stream
			return stringVector;
		}
		else
		{
			in>>passedInString;									//assign the next word which is a string value
			if (passedInString.find(";",0)== string::npos)
				stringVector.push_back(passedInString);				//push it onto the vector
		}
	}
	in.close();												//close the file stream
	cout<<"**************************FILE CLOSED*********************************"<<endl;
	return stringVector;
}

//returns a vector of floats
vector<float> CTSIFile::GetTSIFloatVector(const string& filename, const string& section, const string& key) const
{
	vector<float> stringVector;
	string passedInString,
		   csection="["+section+"]",
		   ckey= key+"=",
		   sectionInFile="",
		   keyInFile="";									//designate key buffer

	ifstream in(filename.c_str());									//open the file stream
	if(in.fail())
	{
		cout<<"\nTSI initalization file '"<<filename<<"' is missing or name is not correct.";
		return stringVector;
	}
	
	while(csection!=sectionInFile && !in.eof())				//search the section until there is a match
		in>>sectionInFile;									//or end of file is reached

	while(ckey!=keyInFile && !in.eof())						//now search for the key in that section
		in>>keyInFile;										//or end of file is reached

	while(!in.eof())
	{
		if(passedInString=="#endVector")
		{
			stringVector.pop_back();						//get rid of the #endVector keyword
			in.close();												//close the file stream
			return stringVector;
		}
		else
		{
			in>>passedInString;									//assign the next word which is a string value
			if (passedInString.find(";",0)== string::npos)
				stringVector.push_back((float)atof(passedInString.c_str()));				//push it onto the vector
		}
	}
	in.close();												//close the file stream
	cout<<"**************************FILE CLOSED*********************************"<<endl;
	return stringVector;
}