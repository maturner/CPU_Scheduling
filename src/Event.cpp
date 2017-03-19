#include <iostream>
#include "Event.h"
#include "Process.h"
#include "Thread.h"

/**
 Event:
 constructor function to build a new Event
 */
//Event::Event(char* n, int t, Process p, Thread th) {
Event::Event(int n, int t, int pid, int tid, char* p, char* m) {
	eventName = n;
	time = t;
	processID = pid;
	threadID = tid;
	tPriority = p;
	message = m;

}


/**
 toString:
 prints out the required information regarding the event
 */
void Event::toString() {

	printf("\nAt time %d", time);
	printf("\n\t%s", Names[eventName]);
	printf("\n\tThread %d in Process %d [%s]", threadID, processID, tPriority);
	printf("\n\t%s", message);
}