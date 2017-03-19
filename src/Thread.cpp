#include <cstring>
#include "Thread.h"

/**
 Thread:
 constructor function to build a new thread
 */
Thread::Thread(int i, int p, int t, std::string s, std::vector<Burst> b) {
	threadID = i;
	processID = p;
	arrivalTime = t;
	threadState = s;
	bursts = b;
	numIOBursts = 0;
	numCPUBursts = 0;

	// get the specific number of each burst
	for(unsigned int i = 0; i < bursts.size(); i++) {
		if (strcmp(bursts[i].getBurstType(), "CPU") == 0)
			numCPUBursts++;
		if (strcmp(bursts[i].getBurstType(), "IO") == 0)
			numIOBursts++;
	}

}

/**
 Thread:
 default constructor
 */
Thread::Thread() {
	// do nothing
}

/*
 getter functions:
 return variable values for display functions
 */
std::vector<Burst> Thread::getCPUBursts() {
	std::vector<Burst> cBursts;
	char key[] = "CPU";
	for(unsigned int i = 0; i < bursts.size(); i++) {
		if (strcmp(bursts[i].getBurstType(), key) == 0)
			cBursts.push_back(bursts[i]);
	}
	return cBursts;
}

std::vector<Burst> Thread::getIOBursts() {
	std::vector<Burst> iBursts;
	char key[] = "IO";
	for(unsigned int i = 0; i < bursts.size(); i++) {
		if (strcmp(bursts[i].getBurstType(), key) == 0)
			iBursts.push_back(bursts[i]);
	}
	return iBursts;
}

/**
 addBurst:
 constructor function to build a new thread
 */
void Thread::addBurst(Burst b) {
	bursts.push_back(b);
}