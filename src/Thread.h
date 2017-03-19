#pragma once

#include <vector>
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
	int startTime;
	int numIOBursts;
	int numCPUBursts;
	std::string threadState;
	std::vector<Burst> bursts;


// public class variables and functions
public:

	/**
	 Thread:
	 constructor function to build a new thread
	 */
	Thread(int i, int p, int t, std::string s, std::vector<Burst> b);

	/**
	 Thread:
	 default constructor
	 */
	Thread();

	/*
	 getter functions:
	 return variable values for display functions
	 */
	int getThreadID() { return threadID; }
	int getProcessID() { return processID; }
	int getArrivalTime() { return arrivalTime; }
	int getStartTime() { return startTime; }
	int getNumIOBursts() { return numIOBursts; }
	int getNumCPUBursts() { return numCPUBursts; }
	std::string getThreadState() { return threadState; }
	std::vector<Burst> getCPUBursts();
	std::vector<Burst> getIOBursts();

	/*
	 setter functions:
	 set variable values for display functions
	 */
	void setStartTime(int t) { startTime = t; }
	void setThreadState(std::string s) { threadState = s; }

	/**
	 addBurst:
	 constructor function to build a new thread
	 */
	void addBurst(Burst b);

};