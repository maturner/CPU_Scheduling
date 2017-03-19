#include "Algorithm.h"


/**
 Algorithm:
 a constructor that allows the main proggram to make use of the following
 scheduling algorithms
 */
Algorithm::Algorithm() {
	// nothing to do here
}


/**
 Burst:
 constructor function to build a new burst
 */
void Algorithm::firstComeFirstServe(std::queue<Thread> &t, std::queue<Event> &e, int time) {
	
	// 
	while(!t.empty()) {
		
		Thread thread = t.front();

		t.pop();

	}
	

	/*
	if (time == 17) {
		Event event(EventNames::DISPATCHER_INVOKED, 
			time, 
			0, 
			0, 
			0, 
			"just a trial event, see if it works");
		e.push(event);
	}
	*/

}