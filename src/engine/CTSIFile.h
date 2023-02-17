#ifndef CTSIFILE_H
#define CTSIFILE_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CTSIFile.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2003
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  This class provides the ability to extract data from a .tsi text file based on
//	key values similar to windows INI file
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <string>
#include <vector>


//Text Style Initalization file

class CTSIFile
{
public:
	CTSIFile()
	{}
	//Post- Default constructor

	~CTSIFile()
	{}
	//Post- Default destructor
	void WriteTSI(const std::string& filename, const std::string& section, const std::string& key, const char * value) const;
	//Pre-	no TSI file exists
	//Post- writes to the specified file, in the specified section, in the specified
	//		key the specified value, if such file exists, or section or key exists,
	//		they are created
	void WriteTSI(const std::string& filename, const std::string& section, const std::string& key, float value) const;
	//Pre-	no TSI file exists
	//Post- writes to the specified file, in the specified section, in the specified
	//		key the specified value, if such file exists, or section or key exists,
	//		they are created
	void WriteTSIFloat(const std::string& filename, const std::string& section, const std::string& key, float floatValue) const;
	//Pre-  tsi file exists
	//Post- writes to the specified file, int the specified section, in the specified
	//		key the floatValue.  If cannot find the the location to write to, displays 
	//		error message
	std::string GetTSIString(const std::string& filename, const std::string& section, const std::string& key) const;
	//Pre-	a TSI file must exist
	//Post- gets the specifed string value from the specific filename section and key
	//		if neither exist returns false and does nothing	
	float GetTSIFloat(const std::string& filename, const std::string& section, const std::string& key) const;
	//Pre-	a TSI file must exist
	//Post- gets the specifed float value from the specific filename section and key
	//		if neither exist returns false and does nothing	
	std::vector<std::string> GetTSIStringVector(const std::string& filename, const std::string& section, const std::string& key) const;
	//Pre-  a TSI file must exist
	//Post- returns a vector of string items from a specified key name location, in a specified secion
	//      from a specified file
	std::vector<float> GetTSIFloatVector(const std::string& filename, const std::string& section, const std::string& key) const;
	//Pre-  a TSI file must exist
	//Post- returns a vector of float items from a specified key name location, in a specified secion
	//      from a specified file

};
#endif