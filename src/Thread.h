#pragma once

#include <vector>
#include <queue>
#include <string>

#include "Burst.h"

/**
 Thread:
 a simple class containing information regarding a thread
 */
class Thread {

// private class variables and functions
private:
	int threadID;
	int processID;
	int arrivalTime;
	int startTime = -1;
	int endTime = -1;
	int serviceTime;
	int ioTime;
	int numIOBursts;
	int numCPUBursts;
	std::string threadState;
	std::queue<Burst*> bursts;


// public class variables and functions
public:

	/**
	 Thread:
	 default constructor
	 */
	Thread();

	/**
	 Thread:
	 constructor function to build a new thread
	 */
	Thread(int i, int p, int t, std::string s, std::queue<Burst*> b, int st, int it);

	/**
	 Thread:
	 copy constructor
	 */
	Thread(const Thread &t);

	/*
	 getter functions:
	 return variable values for display functions
	 */
	int getThreadID() { return threadID; }
	int getProcessID() { return processID; }
	int getArrivalTime() { return arrivalTime; }
	int getStartTime() { return startTime; }
	int getEndTime() { return endTime; }
	int getNumIOBursts() { return numIOBursts; }
	int getNumCPUBursts() { return numCPUBursts; }
	int getServiceTime() { return serviceTime; }
	int getIOTime() { return ioTime; }
	std::string getThreadState() { return threadState; }
	std::queue<Burst*> getBursts() { return bursts; }

	/*
	 setter functions:
	 set variable values for display functions
	 */
	void setStartTime(int t) { startTime = t; }
	void setEndTime(int t) { endTime = t; }
	void setThreadState(std::string s) { threadState = s; }

	/**
	 addBurst:
	 constructor function to build a new thread
	 */
	void removeBurst();

};
