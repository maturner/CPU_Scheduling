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
 customSimulator:
 a class that will schedule and run threads according to a custom scheduling
 algorithm that makes use of preemption via a time quantum, incorporates an ageing
 parameter, and will attempt to run threads of the same process to minimize
 overhead switching.
 */
class customSimulator {

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
	int timeQuantum = 8;

	float cpuUtilization = 0.0;
	float cpuEfficiency = 0.0;

	bool dispatcherActive = false;
	bool cpuBusy = false;
	int pdt;

	std::vector<Process*> processes;
	std::priority_queue<Event*, std::vector<Event*>, comparison> events;
	
	std::queue<Thread*> readyQueue0;
	std::queue<Thread*> readyQueue1;
	std::queue<Thread*> readyQueue2;
	std::queue<Thread*> readyQueue3;

	Thread* nullThread;
	Thread* previousThread;
	Thread* currentThread;

public:

	/**
 	 customSimulator:
	 a constructor that creates a round robin scheduling simulation
	 */
	customSimulator(std::vector<Process*> p, int to, int po, bool v, bool t);


	/**
	 getter and setter for the running thread
	 */
	Thread* getCurrentThread() { return currentThread; }
	int getTimeQuantum() { return timeQuantum; }
	void setCurrentThread(Thread* t) { currentThread = t; }

	/**
 	 run:
	 runs the simulation using the following event functions
	 */
	void run();
	void threadArrived(Event* e);
	void dispatchInvoked(Event* e);
	void processDispatchComplete(Event* e);
	void threadDispatchComplete(Event* e);
	void threadPreempted (Event* e);
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
