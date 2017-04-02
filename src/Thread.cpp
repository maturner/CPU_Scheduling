#include "Thread.h"

/**
 Thread:
 default constructor
 */
Thread::Thread() {
	threadID = -1;
	processID = -1;
	arrivalTime = -1;
	startTime = -1;
	endTime = -1;
	serviceTime = -1;
	ioTime = -1;
	threadState = "NO_THREAD";
	bursts;
	lastPreempt = -1;
	age = -1;
}

/**
 Thread:
 constructor function to build a new thread
 */
Thread::Thread(int i, int p, int t, std::string s, std::queue<Burst*> b, int st, int it) {
	threadID = i;
	processID = p;
	arrivalTime = t;
	startTime = -1;
	endTime = -1;
	serviceTime = st;
	ioTime = it;
	threadState = s;
	bursts = b;
	lastPreempt = -1;
	age = -1;

	// get the specific number of each burst
	numIOBursts = bursts.size() / 2;
	numCPUBursts = (bursts.size() / 2) + 1;
}

/**
 Thread:
 copy constructor
 */
Thread::Thread(const Thread &t) {
	threadID = t.threadID;
	processID = t.processID;
	arrivalTime = t.arrivalTime;
	startTime = t.startTime;
	endTime = t.endTime;
	numIOBursts = t.numIOBursts;
	numCPUBursts = t.numCPUBursts;
	threadState = t.threadState;
	bursts = t.bursts;
	lastPreempt = t.lastPreempt;
	age = t.age;
}

/**
 ~Thread:
 desructor functions to free up memory
 */
Thread::~Thread() {

	for(int i = 0; i < (int)bursts.size(); i++) {
		//delete bursts[i];
	}
}


/**
 addBurst:
 constructor function to build a new thread
 */
void Thread::removeBurst() {
	bursts.pop();
}
