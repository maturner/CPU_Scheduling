#pragma once

#include "Process.h"
#include "Thread.h"

enum EventTypes {
	THREAD_ARRIVED,
	THREAD_DISPATCH_COMPLETED,
	PROCESS_DISPATCH_COMPLETED,
	CPU_BURST_COMPLETED,
	IO_BURST_COMPLETED,
	THREAD_COMPLETED,
	THREAD_PREEMPTED,
	DISPATCHER_INVOKED
};

static const char* Types[] = {
	"THREAD_ARRIVED",
	"THREAD_DISPATCH_COMPLETED",
	"PROCESS_DISPATCH_COMPLETED",
	"CPU_BURST_COMPLETED",
	"IO_BURST_COMPLETED",
	"THREAD_COMPLETED",
	"THREAD_PREEMPTED",
	"DISPATCHER_INVOKED"
};

/**
 Event:
 a simple class containing information regarding an Event
 */
class Event {

// private class variables and functions
private:
	int eventType;
	int time;
	int processID;
	int threadID;
	std::string tPriority;
	std::string message;

// public class variables and functions
public:

	/**
	 Event:
	 constructor function to build a new Event
	 */
	Event(int n, int t, int pid, int tid, std::string p, std::string m);

	/**
	 getter functions:
	 */
	int getTime() const { return time; }
	int getProcessID() { return processID; }
	int getThreadID() { return threadID; }
	std::string getPriority() { return tPriority; }
	int getType() { return eventType; }


	/**
	 toString:
	 prints out the required information regarding the Event
	 */
	void toString();
};

/**
 comparison:
 provides the priority queue the operation it needs to determine priorities
 */
struct comparison {
	bool operator()(const Event* e1, const Event* e2) {
		return e1->getTime() >= e2->getTime();
	}
};
