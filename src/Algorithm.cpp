#include "Algorithm.h"

#include <iostream>

/**
 Algorithm:
 a constructor that allows the main proggram to make use of the following
 scheduling algorithms
 */
Algorithm::Algorithm(int t, int p) {
	threadOverhead = t;
	processOverhead = p;
}


/**
 firstComeFirstServe:
 a function to schedule the next process to run
 */
void Algorithm::firstComeFirstServe(std::queue<Thread> &readyQueue, std::queue<Event> &e, int time) {
	
	// special case: time = 0
	if(readyQueue.size() > 0 && time == 0) {
		
		// set the running process to the first thread to arrive
		running = readyQueue.front();
		readyQueue.pop();

		// change its state to running
		running.setThreadState("RUNNING");


	}
	

	/*
	if (time == 17) {
		Event event(EventNames::DISPATCHER_INVOKED, 
			time, 
			0, 
			0, 
			0, 
			"just a trial event, see if it works");
		e.push(event);
	}
	*/

}