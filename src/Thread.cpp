#include <cstring>
#include "Thread.h"

/**
 Thread:
 default constructor
 */
Thread::Thread() {
	threadID = -1;
	processID = -1;
	arrivalTime = -1;
	threadState = "NO_THREAD";
	bursts;
	serviceTime = -1;
	ioTime = -1;
}

/**
 Thread:
 constructor function to build a new thread
 */
Thread::Thread(int i, int p, int t, std::string s, std::queue<Burst*> b, int st, int it) {
	threadID = i;
	processID = p;
	arrivalTime = t;
	threadState = s;
	bursts = b;
	serviceTime = st;
	ioTime = it;

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
	startTime = -1;
	numIOBursts = t.numIOBursts;
	numCPUBursts = t.numCPUBursts;
	threadState = t.threadState;
	bursts = t.bursts;
}


/**
 addBurst:
 constructor function to build a new thread
 */
void Thread::removeBurst() {
	bursts.pop();
}
