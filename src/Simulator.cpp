#include <iostream>

#include "Simulator.h"

/**
 Simulator:
 a constructor that allows the main proggram to make use of the following
 scheduling algorithms
 */
Simulator::Simulator(std::vector<Process*> p, int to, int po, bool v) {

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
void Simulator::run() {

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
        
	}
	//displayReadyQueue();
}

/**
 threadArrived:
 
 */
void Simulator::threadArrived(Event* e) {

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

	
	// place the thread in the in the ready queue
	int tid = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->getThreadID();
	int pid = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->getProcessID();
	int art = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->getArrivalTime();
	std::string state = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->getThreadState();
	std::vector<Burst*> bursts = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->getBursts();
	
	Thread* t = new Thread(tid, pid, art, state, bursts);
	readyQueue.push(t);

	//printf("\n\nDispatcher is busy, so P%dT%d is going to be pushed to the ready queue\n\n", pid, tid);
	

}

/**
 dispatchInvoked:
 
 */
void Simulator::dispatchInvoked(Event* e) {

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
void Simulator::processDispatchComplete(Event* e) {

	// set the current thread
	//currentThread = 

}








void Simulator::displayReadyQueue() {

	printf("\nReady Queue:\n");
	while(!readyQueue.empty()) {
		Thread* t = readyQueue.front();
		printf("P%dT%d\n", t->getProcessID(), t->getThreadID());
		readyQueue.pop();
	}
}
