#include "CTime.h"

#include <windows.h>
//#include <iostream>
#include <sstream>

using namespace std;

//Initialize timer variables
CTime::CTime():timerStartTime(0),timerStopTime(0),accumulatedTime(0){}

//destructor
CTime::~CTime()
{
}

//Start timer
void CTime::StartTimer()
{
	timerStartTime=timeGetTime();
}

//Stop timer
void CTime::StopTimer()
{
	timerStopTime=timeGetTime();
	accumulatedTime= timerStopTime-timerStartTime;
}

//Print formated time in hours minutes and seconds
void CTime::PrintTimer() const
{
	//VERY BAD... MUST FIX
/*	int hrs=0,min=0,sec=0, mss=0;

	int accTime= static_cast<int>(accumulatedTime*0.001f);

	if(accTime>3600)
	{
		hrs= static_cast<int>(floor(static_cast<float>(accTime/3600)));
		hrs%=24;
	}
	if(accTime>60)
	{
		min= static_cast<int>(floor(static_cast<float>(accTime/60)));
		min%=60;
	}
	sec= accTime%60;
	mss= (int)accumulatedTime%1000;
	
	cout<<"Timer took: "<<hrs<<"h:"<<min<<"m:"<<sec<<"s."<<accumulatedTime<<endl;
*/
}

//returns a string containing a format of h:m:s:mss
string CTime::GetTimerResultString() const
{
	/*  TERRIBLE... TERRIBLE!!!
	int hrs=0,min=0,sec=0, mss=0;
	int accTime= static_cast<int>(accumulatedTime*0.001f);

	if(accTime>3600)
	{
		hrs= static_cast<int>(floor(static_cast<float>(accTime/3600)));
		hrs%=24;
	}
	if(accTime>60)
	{
		min= static_cast<int>(floor(static_cast<float>(accTime/60)));
		min%=60;
	}
	sec= accTime%60;
	mss= (int)accumulatedTime%1000;
	
	ostringstream ostr;
	ostr <<hrs<<"h : "<<min<<"m : "<<sec<<"s . "<<mss;											//convert num to string
	return ostr.str();
	*/
	return "";
}

//Reset the accumulated time
void CTime::ResetTimer()
{
	accumulatedTime=0;
}

//Calculates the Frames per Second
void CTime::FramesPerSecond()
{
	static int calculatedFPS	=0;							//This will store calculated fps
	static double lastFrameTime	=0.0f;						//Stores the last frame's time
    
    if((timeGetTime()-lastFrameTime) >= 1000)
    {
		framesPerSecond=calculatedFPS;
		calculatedFPS=0;
		lastFrameTime=timeGetTime();
    }
    else
		++calculatedFPS;
}

//Compute averge delta time for a particual frame interval
double CTime::ComputeDeltaTime(int frameInterval)
{
	static int frameCount=1;
	static double averageDeltaTime=0;
	static double computedAverageDeltaTime=0;    
	static double lastFrameTime	=0.0f;						//Stores the last frame's time
    
	averageDeltaTime+=(timeGetTime()-lastFrameTime);
	lastFrameTime=static_cast<double>(timeGetTime());
	++frameCount;

	if(frameCount == frameInterval)
	{
		frameCount=1;
		computedAverageDeltaTime=averageDeltaTime/frameInterval;
		averageDeltaTime=0;
	}
	return computedAverageDeltaTime*0.001f;
}

void CTime::Clock()
{
	static int staticTensSec=0;
	static int staticSec=0;
	static int staticMin=0;
	static int staticHour=0;
	static double tack=0.0f;									//old time
	double tick=timeGetTime()*0.001;						//current time

	if(clockReset)											//reset the clock
	{
		staticTensSec=0;
		staticSec=0;
		staticMin=0;
		staticHour=0;
	}
	if(staticTensSec>=9)									//calculate 10th of a second
	{
		staticSec++;
		staticTensSec=0;
	}
	if(staticSec>59)										//calculate a second
	{
		staticMin++;
		staticSec=0;
	}
	if(staticMin>59)										//calculate a minute
	{
		staticHour++;
		staticMin=0;
	}
	if(tick-tack> 0.1f)										//for every second advanced, accuracy range(.990 slower, .999 faster)
	{														//increment second
		tack=tick;
		staticTensSec++;
	}
	//glRasterPos2f(-textAlign-0.05f, 0.08f);				//position of text, and print text
	hour		=staticHour; 
	minute		=staticMin; 
	second		=staticSec;
	tensSecond	=staticTensSec;
}

//Return a converted to int fps
int CTime::GetFPS() const
{
	return (int)framesPerSecond;
}

//Returns the frame interval as delta Time
double CTime::GetDeltaTime() const
{
	return deltaTime;
}

//Returns the clock components
int CTime::GetHour() const
{
	return hour;
}

int CTime::GetMinute() const
{
	return minute;
}

int CTime::GetSecond() const
{
	return second;
}

int CTime::GetTensSecond() const
{
	return tensSecond;
}

double CTime::GetAccumulatedTime() const
{
	return accumulatedTime;
}
