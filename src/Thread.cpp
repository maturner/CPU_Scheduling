#include <cstring>
#include "Thread.h"

/**
 Thread:
 constructor function to build a new thread
 */
Thread::Thread(int i, int p, int t, std::string s, std::vector<Burst*> b) {
	this->threadID = i;
	this->processID = p;
	this->arrivalTime = t;
	this->threadState = s;
	this->bursts = b;
	this->numIOBursts = 0;
	this->numCPUBursts = 0;

	// get the specific number of each burst
	for(unsigned int i = 0; i < bursts.size(); i++) {
		if (strcmp(bursts[i]->getBurstType(), "CPU") == 0)
			numCPUBursts++;
		if (strcmp(bursts[i]->getBurstType(), "IO") == 0)
			numIOBursts++;
	}

}

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

/*
 getter functions:
 return variable values for display functions
 */
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

/**
 addBurst:
 constructor function to build a new thread
 */
void Thread::addBurst(Burst* b) {
	bursts.push_back(b);
}
