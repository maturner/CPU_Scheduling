#pragma once

#include <vector>
#include <queue>

#include "Process.h"
#include "Thread.h"
#include "Event.h"
#include "Burst.h"
#include "Process.h"

#include <vector>
#include <queue>
#include <string>

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
	bool verbose = false;
	bool dispatcherActive = false;

	std::vector<Process*> processes;
	std::priority_queue<Event*, std::vector<Event*>, comparison> events;
	std::queue<Thread*> readyQueue;

	Process* currentProcess;
	Thread* currentThread;

public:

	/**
 	 Algorithm:
	 a constructor that allows the main proggram to make use of the following
	 scheduling algorithms
	 */
	Simulator(std::vector<Process*> p, int to, int po, bool v);


	/**
	 getter and setter for the running thread
	 */
	Thread* getCurrentThread() { return currentThread; }
	void setCurrentThread(Thread* t) { currentThread = t; }


	/**
 	 firstComeFirstServe:
	 a simple scheduling algorithm
	 */
	void run();

	void threadArrived(Event* e);
	void dispatchInvoked(Event* e);
	void processDispatchComplete(Event* e);

};
