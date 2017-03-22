#pragma once

#include <vector>
#include <queue>
#include <string>

#include "Process.h"
#include "Thread.h"
#include "Event.h"
#include "Burst.h"
#include "Process.h"


/**
 Simulator:
 a constructor that allows the main proggram to make use of the following
 scheduling algorithms
 */
class fcfsSimulator {

private:

	bool verbose = false;
	bool perThread = false;

	int threadOverhead;
	int processOverhead;
	int startTime = 0;
	int endTime = 0;
	int totalServiceTime = 0;
	int totalIOTime = 0;
	int dispatchTime = 0;
	int idleTime = 0;

	float cpuUtilization = 0.0;
	float cpuEfficiency = 0.0;

	bool dispatcherActive = false;
	bool cpuBusy = false;
	int pdt;

	std::vector<Process*> processes;
	std::priority_queue<Event*, std::vector<Event*>, comparison> events;
	std::queue<Thread*> readyQueue;

	Thread* nullThread;
	Thread* previousThread;
	Thread* currentThread;

public:

	/**
 	 Algorithm:
	 a constructor that allows the main proggram to make use of the following
	 scheduling algorithms
	 */
	fcfsSimulator(std::vector<Process*> p, int to, int po, bool v, bool t);


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
	void threadDispatchComplete(Event* e);
	void cpuBurstCompleted(Event* e);
	void ioBurstCompleted(Event* e);
	void threadComplete(Event* e);

	/**
	 calculateMetrics:
	 calculates scheduling statistics based on results of the simulation
	 */
	void calculateMetrics();

	/**
 	 displayInfo:
 	 prints out the information regarding each process
 	 */
	void displayInfo(); 

	/**
 	 displayProcessInfo:
 	 prints out the information regarding each process
 	 */
	void displayProcessInfo();

};
