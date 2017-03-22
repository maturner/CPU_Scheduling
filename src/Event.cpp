#include <string>

#include "Event.h"
#include "Process.h"
#include "Thread.h"

/**
 Event:
 constructor function to build a new Event
 */
Event::Event(int n, int t, int pid, int tid, std::string p, std::string m) {
	this->eventType = n;
	this->time = t;
	this->processID = pid;
	this->threadID = tid;
	this->tPriority = p;
	this->message = m;

}


/**
 toString:
 prints out the required information regarding the event
 */
void Event::toString() {

	printf("\nAt time %d", time);
	printf("\n\t%s", Types[eventType]);
	printf("\n\tThread %d in Process %d [%s]", threadID, processID, tPriority.c_str());
	printf("\n\t%s\n", message.c_str());
}

