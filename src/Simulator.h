#pragma once

#include <vector>
#include <queue>

#include "Process.h"
#include "Thread.h"
#include "Event.h"

/**
 Simulator:
 a constructor that allows the main proggram to make use of the following
 scheduling algorithms
 */
class Simulator {

private:

	int threadOverhead;
	int processOverhead;
	int startTime = 0;
	int endTime = 0;
	int algorithm = 0;
	bool verbose = false;

	std::vector<Process> processes;
	std::priority_queue<Event> events;
	std::queue<Thread> readyQueue;

	Thread currentThread;

public:

	/**
 	 Algorithm:
	 a constructor that allows the main proggram to make use of the following
	 scheduling algorithms
	 */
	Simulator(std::vector<Process> p, int to, int po, int al, bool v);


	/**
	 getter and setter for the running thread
	 */
	Thread getCurrentThread() { return currentThread; }
	void setCurrentThread(Thread t) { currentThread = t; }


	/**
 	 firstComeFirstServe:
	 a simple scheduling algorithm
	 */
	void run();

	void threadArrived(Event e);


};
