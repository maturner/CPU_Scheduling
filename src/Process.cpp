#include <iostream>
#include "Process.h"

/**
 Process:
 constructor function to build a new process
 */
Process::Process(int i, int p, std::vector<Thread*> th) {
	processID = i;
	priority = p;
	threads = th;
}

/**
 Process:
 default constructor
 */
Process::Process() {
	// do nothing
}

/**
 priorityType:
 returns a string representation of the priority type
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
 toString:
 prints out the process in the required format to visualize basic data
 */
void Process::toString() {

	printf("Process %d [%s]:\n", processID, getPriorityType());

	for(unsigned int i = 0; i < threads.size(); i++) {

		int a = threads[i]->getArrivalTime();
		int e = threads[i]->getEndTime();

		printf("\tThread%3d:   ARR:%3d   CPU:%3d   I/O:%3d   TRT:%4d   END:%4d\n",
			i, 
			a, 
			threads[i]->getServiceTime(), 
			threads[i]->getIOTime(), 
			(e - a), 
			e);
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