///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CSound.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Developed by Luis Miranda
//	Modified by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2003
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  This class provides sound playing capabilities
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CSOUND_H
#define CSOUND_H

#include <string>
#include <vector>

class CSound
{
private:
															//All inline comments begin at COLUMN 61
	std::vector<std::string> supportedSoundType;						//vector used for determining which sound types are supported
	std::string fileName;										//sound file name
	float soundPitch;										//pitch at which a sound is played at

	//ADD ANY NECESSARY PRIVATE MEMBERS (NO PUBLIC MEMBERS ALLOWED) 
public:
	CSound();
	//Post-	constructor, initalize supported file extensions to compare fileName specification
	~CSound();
	//Post- destructor
	void Load(std::string fileName);
	//Pre-  necessary member data not initalized
	//Post- scan the supportedSoundtype exntensions and if a match between an intem in the
	//		supportedSoundType and the last 4 charachters of the string
	//		exists initalize the proper default members
	void Play(float start, float end, int loopNumber);
	//Pre-	a sound file is initalized with the format type known
	//Post- depending on the format call appropriate play function from the
	//		specified start position of the sound lenghth, to the specified end position
	//		of the sound length, and play that sound for the number of loop times specified
	void PlayMP3();
	//Pre-	an mp3 sound exists
	//Post- the mp3 type sound is played based on the specified fileName member and specified pitch
	void PlayWAV();
	//Pre-  a wav sound exists
	//Post- the wav type sound is played based on the specified fileName member and specified pitch
	void SetSoundPitch(float amount, bool increment);
	//Pre-	sound exists at a certain pitch
	//Post- the pitch is incremented (+=) if the increment is true, otherwise
	//		its directly set based on the specified amount
	//		to the current pitch value
	float GetSoundPitch();
	//Pre-  soundPitch exists
	//Post- returns soundPitch
	std::string GetFileName();
	//Pre-  sound file name exists
	//Post- returns fileName

	//ADD ANY NECESSARY FUNCTIONS
};
#endif


	