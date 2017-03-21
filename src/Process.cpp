#include <iostream>
#include "Process.h"

/**
 Process:
 constructor function to build a new process
 */
Process::Process(int i, int p, std::vector<Thread*> th) {
	this->processID = i;
	this->priority = p;
	this->threads = th;
}

/**
 Process:
 default constructor
 */
Process::Process() {
	// do nothing
}

/**
 * priorityType:
 * returns a string representation of the priority type
 */
char* Process::getPriorityType() {

	char* t;

	if (priority == 0) t = "SYSTEM";
	else if (priority == 1) t = "INTERACTIVE";
	else if (priority == 2) t = "NORMAL";
	else if (priority == 3) t = "BATCH";
	else t = "UNIDENTIFIED";
	
	return t;
}

/**
 * toString:
 *
 */
void Process::toString() {

	printf("Process %d [%s]:\n", processID, getPriorityType());

	for(unsigned int i = 0; i < threads.size(); i++) {
		printf("\tThread %d:\tARR: %d\tCPU: %d\tI/O: %d\tTRT: %d\tEND: %d\n",
			i, 
			threads[i]->getArrivalTime(), 
			threads[i]->getNumCPUBursts(), 
			threads[i]->getNumIOBursts(), 
			4, 
			5);
	}
}

/**
 operator =:
 assignment operator for hte process class
 */
Process Process::operator=(const Process& other) {

	Process p;

	p.processID = other.processID;
	p.priority = other.priority;
	p.threads = other.threads;

	return p;

}