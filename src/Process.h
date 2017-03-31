#pragma once

#include "Thread.h"
#include <vector>

/**
 Process:
 a simple class containing information regarding a process
 */
class Process {

// private class variables and functions
private:
	int processID;
	int priority;
	std::vector<Thread*> threads;


// public class variables and functions
public:

	/**
	 Process:
	 constructor functions to build a new process
	 */
	Process();
	Process(int i, int p, std::vector<Thread*> th);

	/**
	 ~Process:
	 desructor functions to free up memory
	 */
	~Process();

	/**
	 getter functions:
	 return variables related to the process
	 */
	int getProcessID() { return processID; }
	int getPriority() { return priority; }
	std::vector<Thread*> getProcessThreads() { return threads; }

	/**
	 priorityType:
	 returns a string representation of the priority type
	 */
	std::string getPriorityType();

	/**
	 toString:
	 prints out the required information regarding the process
	 */
	void toString();

	/**
	 operator =:
	 assignment operator for hte process class
	 */
	Process operator=(const Process& other);

};