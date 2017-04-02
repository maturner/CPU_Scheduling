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
	int startTime;
	int endTime;
	int serviceTime;
	int ioTime;
	int numIOBursts;
	int numCPUBursts;

	int lastPreempt;
	int age;
	
	std::string threadState;
	std::queue<Burst*> bursts;


// public class variables and functions
public:

	/**
	 Thread:
	 constructor functions to build a new thread
	 */
	Thread();
	Thread(int i, int p, int t, std::string s, std::queue<Burst*> b, int st, int it);

	/**
	 ~Thread:
	 desructor functions to free up memory
	 */
	~Thread();

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
	int getTurnaroundTime() { return endTime - arrivalTime; }
	int getResponseTime() { return startTime - arrivalTime; }
	int getLastPreempt() { return lastPreempt; }
	int getAge() { return age; }
	std::string getThreadState() { return threadState; }
	std::queue<Burst*> getBursts() { return bursts; }

	/*
	 setter functions:
	 set variable values for display functions
	 */
	void setStartTime(int t) { startTime = t; }
	void setEndTime(int t) { endTime = t; }
	void setThreadState(std::string s) { threadState = s; }
	void setLastPreempt(int t) { lastPreempt = t; }
	void setAge(int a) { age = a; }

	/**
	 addBurst:
	 constructor function to build a new thread
	 */
	void removeBurst();

};
