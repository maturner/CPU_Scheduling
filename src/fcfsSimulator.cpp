#include <iostream>

#include "fcfsSimulator.h"

/**
 fcfsSimulator:
 a constructor that allows the main proggram to make use of the following
 scheduling algorithms
 */
fcfsSimulator::fcfsSimulator(std::vector<Process*> p, int to, int po, bool v) {

	// initialize class variables
	processes = p;
	threadOverhead = to;
	processOverhead = po;
	verbose = v;
	currentThread = new Thread();

	// initialize the event queue
	for(int i = 0; i < (int)processes.size(); i++) {

		// check for new threads from that process
		std::vector<Thread*> threads = processes[i]->getProcessThreads();
		for(int j = 0; j < (int)threads.size(); j++) {

			// change the thread state
			threads[i]->setThreadState("READY");

			// add an event to the event queue
			Event* newEvent = new Event(EventTypes::THREAD_ARRIVED,
						   				threads[j]->getArrivalTime(),
						   				processes[i]->getProcessID(),
						   				threads[j]->getThreadID(),
						   				processes[i]->getPriorityType(),
						   				"Transitioned from NEW to READY");
			events.push(newEvent);
		}
	}
}


/**
 firstComeFirstServe:
 a function to schedule the next process to run
 */
void fcfsSimulator::run() {

	printf("\nTimeline of Events:");

	while (!events.empty()) {
		Event* event = events.top();
		events.pop();

		// display the current event
		if(verbose) ; event->toString();


		// update the event queue based on the current event
        switch(event->getType()){
            case EventTypes::THREAD_ARRIVED: {
                threadArrived(event);
                break;
            }

            case EventTypes::DISPATCHER_INVOKED: {
          		dispatchInvoked(event);
                break;
            }

            case EventTypes::THREAD_DISPATCH_COMPLETED: {
            	std::cout << "got to THREAD_DISPATCH_COMPLETED" << std::endl;
            	break;
            }

            case EventTypes::PROCESS_DISPATCH_COMPLETED: {
            	processDispatchComplete(event);
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
			//printf("\nReady Queue Size: %d", readyQueue.size());
	}
}

/**
 threadArrived:

 */
void fcfsSimulator::threadArrived(Event* e) {

	// if no running thread and an inactive dispatcher:
	if(currentThread->getProcessID() == -1 && currentThread->getThreadID() == -1 && !dispatcherActive) {

		// get ready queue size for message #

		// invoke process dispatch
		Event* newEvent = new Event(EventTypes::DISPATCHER_INVOKED,
									e->getTime(),
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Selected from # threads; will run to completion of burst");
		events.push(newEvent);
	}

	// push the thread onto the ready queue
	Thread* t = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID() - 1];
	readyQueue.push(t);


}

/**
 dispatchInvoked:

 */
void fcfsSimulator::dispatchInvoked(Event* e) {

	// set the dispatcher flag to true
	dispatcherActive = true;

	// indicate when the process dispatch will be complete
	Event* newEvent = new Event(EventTypes::PROCESS_DISPATCH_COMPLETED,
								e->getTime() + processOverhead,
								e->getProcessID(),
								e->getThreadID(),
								e->getPriority(),
								"Transitioned from READY to RUNNING");
	events.push(newEvent);

	// change the state of the thread

}

/**
 processDispatchComplete:

 */
void fcfsSimulator::processDispatchComplete(Event* e) {

	// indicate that the dispatcher is ready
	dispatcherActive = false;

	// set the current thread and calculate run time
	currentThread = readyQueue.front();
	readyQueue.pop();
	int runTime = currentThread->getBursts().front()->getBurstLength();

	// indicate when the process dispatch will be complete
	Event* newEvent = new Event(EventTypes::CPU_BURST_COMPLETED,
								e->getTime() + runTime,
								e->getProcessID(),
								e->getThreadID(),
								e->getPriority(),
								"Transitioned from RUNNING to BLOCKED");
	events.push(newEvent);

}

/**
 cpuBurstCompleted:

 */
void fcfsSimulator::cpuBurstCompleted(Event* e) {

	

}
