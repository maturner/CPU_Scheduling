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
	char* tPriority;
	char* message;

// public class variables and functions
public:

	/**
	 Event:
	 constructor function to build a new Event
	 */
	Event(int n, int t, int pid, int tid, char* p, char* m);

	/**
	 getter functions:
	 */
	int getTime() const { return time; }
	int getProcessID() { return processID; }
	int getThreadID() { return threadID; }
	char* getPriority() { return tPriority; }
	int getType() { return eventType; }


	/**
	 toString:
	 prints out the required information regarding the Event
	 */
	void toString();

	/**
	 operator <:
	 overloaded operator to allow for sorting of events by time
	 */
	bool operator<(const Event &e1) const;


};
