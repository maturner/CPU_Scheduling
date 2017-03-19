#pragma once

#include <vector>
#include <queue>

#include "Process.h"
#include "Thread.h"
#include "Event.h"

/**
 Algorithm:
 a class containing several variations of CPU scheduling algorithms
 */
class Algorithm {

public:
	/**
 	 Algorithm:
	 a constructor that allows the main proggram to make use of the following
	 scheduling algorithms
	 */
	Algorithm();

	/**
 	 firstComeFirstServe:
	 a simple scheduling algorithm
	 */
	void firstComeFirstServe(std::queue<Thread> &t, std::queue<Event> &e, int time);


};