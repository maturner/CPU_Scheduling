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
}

/**
 Thread:
 constructor function to build a new thread
 */
Thread::Thread(int i, int p, int t, std::string s, std::queue<Burst*> b) {
	threadID = i;
	processID = p;
	arrivalTime = t;
	threadState = s;
	bursts = b;

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

/*
 getter functions:
 return variable values for display functions
 */
 /*
std::vector<Burst*> Thread::getCPUBursts() {
	std::vector<Burst*> cBursts;
	char key[] = "CPU";
	for(unsigned int i = 0; i < bursts.size(); i++) {
		if (strcmp(bursts[i]->getBurstType(), key) == 0)
			cBursts.push_back(bursts[i]);
	}
	return cBursts;
}

std::vector<Burst*> Thread::getIOBursts() {
	std::vector<Burst*> iBursts;
	char key[] = "IO";
	for(unsigned int i = 0; i < bursts.size(); i++) {
		if (strcmp(bursts[i]->getBurstType(), key) == 0)
			iBursts.push_back(bursts[i]);
	}
	return iBursts;
}
*/

/**
 addBurst:
 constructor function to build a new thread
 */
void Thread::addBurst(Burst* b) {
	bursts.push(b);
}
