#include <iostream>

#include "Simulator.h"
#include "Event.h"
#include "Thread.h"

/**
 Simulator:
 a constructor that allows the main proggram to make use of the following
 scheduling algorithms
 */
Simulator::Simulator(std::vector<Process> p, int to, int po, int al, bool v) {

	// initialize class variables
	processes = p;
	threadOverhead = to;
	processOverhead = po;
	algorithm = al;
	verbose = v;

	// initialize the event queue
	for(int i = 0; i < processes.size(); i++) {

		// check for new threads from that process
		std::vector<Thread> threads = processes[i].getProcessThreads();
		for(int j = 0; j < threads.size(); j++) {

			// change the thread state
			threads[i].setThreadState("READY");

			// add an event to the event queue
			Event e(EventTypes::THREAD_ARRIVED,
					threads[j].getArrivalTime(),
					processes[i].getProcessID(),
					threads[j].getThreadID(),
					processes[i].getPriorityType(),
					"Transitioned from NEW to READY");
			events.push(e);
		}
	}
}


/**
 firstComeFirstServe:
 a function to schedule the next process to run
 */
void Simulator::run() {

	printf("\nTimeline of Events:");

	while (!events.empty()) {
		Event event = events.top();
		events.pop();

		// display the current event
		if(verbose) event.toString();

        //updateTimes(event);

        switch(event.getType()){
            case EventTypes::THREAD_ARRIVED: {
                threadArrived(event);
                break;
            }

            case EventTypes::DISPATCHER_INVOKED: {
          		std::cout << "got to DISPATCHER_INVOKED" << std::endl;

                break;
            }

            case EventTypes::THREAD_DISPATCH_COMPLETED: {
            	std::cout << "got to THREAD_DISPATCH_COMPLETED" << std::endl;
            	break;
            }

            case EventTypes::PROCESS_DISPATCH_COMPLETED: {
            	std::cout << "got to PROCESS_DISPATCH_COMPLETED" << std::endl;

                break;
            }

            case EventTypes::THREAD_PREEMPTED: {
            	std::cout << "got to THREAD_PREEMPTED" << std::endl;

                break;
            }

            case EventTypes::CPU_BURST_COMPLETED: {
            	std::cout << "got to CPU_BURST_COMPLETED" << std::endl;

                break;
            }

            case EventTypes::IO_BURST_COMPLETED: {
            	std::cout << "got to IO_BURST_COMPLETED" << std::endl;

                break;
            }

            case EventTypes::THREAD_COMPLETED: {
            	std::cout << "got to THREAD_COMPLETED" << std::endl;

                break;
            }
        }
	}
}

/**
 threadArrived:
 builds events around the arrival of a new thread
 */
void Simulator::threadArrived(Event e) {

	// if no running thread:
	//if(currentThread == NULL) {

		// get ready queue size

		// invoke process dispatch
		Event newEvent(EventTypes::DISPATCHER_INVOKED,
							e.getTime(),
							e.getProcessID(),
							e.getThreadID(),
							e.getPriority(),
							"Selected from # threads; will run to completion of burst");
		events.push(newEvent);
	//}

/*
	Event e(EventTypes::DISPATCHER_INVOKED,
		threads[j].getArrivalTime(),
		processes[i].getProcessID(),
		threads[j].getThreadID(),
		processes[i].getPriorityType(),
		"Transitioned from NEW to READY");
*/
}

/*
void Simulator::updateTimes(Event &e) {
    if(event -> get_time() > end_time){
        end_time = event->get_time();
    }

    if(event ->get_time() < start_time){
        start_time = event -> get_time();
    }
}
*/

/**
 displayEventInfo:
 prints out the information regarding each event

void displayEventInfo(std::queue<Event> e) {

	printf("\nTimeline of Events:");

	// display each event in the queue
	while(!e.empty()) {
		Event event = e.front();
		event.toString();
		e.pop();
		printf("\n");
	}
}
*/
