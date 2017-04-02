#include "Process.h"

/**
 Process:
 constructor function to build a new process
 */
Process::Process() {
	// do nothing
}
Process::Process(int i, int p, std::vector<Thread*> th) {
	processID = i;
	priority = p;
	threads = th;
}

/**
 ~Process:
 desructor functions to free up memory
 */
Process::~Process() {

	// delete each thread pointer
	for(int i = 0; i < threads.size(); i++) {
		delete threads[i];
	}
}

/**
 priorityType:
 returns a string representation of the priority type
 */
std::string Process::getPriorityType() {

	std::string t;

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

	printf("Process %d [%s]:\n", processID, getPriorityType().c_str());

	for(unsigned int i = 0; i < threads.size(); i++) {

		printf("\tThread%3d:    ARR:%3d   CPU:%3d   I/O:%3d   TRT:%4d   END:%4d\n",
			i, 
			threads[i]->getArrivalTime(), 
			threads[i]->getServiceTime(), 
			threads[i]->getIOTime(), 
			threads[i]->getTurnaroundTime(), 
			threads[i]->getEndTime());
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