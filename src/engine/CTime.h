//Time Class
#ifndef CTIME_H
#define CTIME_H

#include <string>

class CTime
{
private:
	double timerStartTime,
		   timerStopTime,
		   accumulatedTime;

	int framesPerSecond;									// Holds the current Frames Per Second value
	double deltaTime;										// Holds the frame interval for time based movement

	bool clockReset;
	int tensSecond;
	int second;
	int minute;
	int hour;
	double currentTime;

public:
	CTime();
	//Post- constructor
	
	virtual ~CTime();
	//Post- destructor
	
	void FramesPerSecond();
	//Pre-  frame vars are initalized
	//Post- frames per second are calculated based on system time
	
	void Clock();
	//Pre-  gets system clock pulse
	//Post- calculates seconds minutes hours etc.
	
	int GetFPS() const;
	//Pre-  fps (frames per second) exists
	//Post- returns fps
	
	double GetDeltaTime() const;
	//Pre-  a frame interval is calculated
	//Post- returns frameInterval
	
	double ComputeDeltaTime(int frameInterval);
	//Pre-  system time exist
	//Post- the delta time is calculated over the specified number of
	//		frames, and returns the averaged delta time for that frame interval

	int GetHour()const;
	//Pre-  hour in clock calculated
	//Post- returns hour
	
	int GetMinute()const;
	//Pre-  minute in clock calculated
	//Post- returns minute
	
	int GetSecond() const;
	//Post- second in clock calculated
	//Post- returns second
	
	int GetTensSecond() const;
	//Pre-  tens seconds in clock calculated
	//Post- return tensSecond

	void StartTimer();
	//Pre-  system timer exists
	//Post- current system time is stored

	void StopTimer();
	//Pre-  system timer exists
	//Post- difference of current time and start time is stored into accumulated time

	void ResetTimer();
	//Pre-  accumulatedTime exists
	//Post- resets accumulated time

	void PrintTimer() const;
	//Pre-  accumulated time exists
	//Post- formats accumulated time into hours, minutes seconds
	//		and prints out the result

	std::string GetTimerResultString() const;
	//Pre-  accumulated time exists
	//Post- returns a formatted string of h:m:s:mss of the accumulated time
	//		between a StartTimer() and StopTimer() call

	double GetAccumulatedTime() const;
	//Pre-  accumulated time exists
	//Post- returns accumulatedTime
};
#endif