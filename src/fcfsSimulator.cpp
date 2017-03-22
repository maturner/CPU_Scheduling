#include <iostream>

#include "fcfsSimulator.h"

/**
 fcfsSimulator:
 a constructor that allows the main proggram to make use of the following
 scheduling algorithms
 */
fcfsSimulator::fcfsSimulator(std::vector<Process*> p, int to, int po, bool v, bool t) {

	// initialize class variables
	processes = p;
	threadOverhead = to;
	processOverhead = po;
	verbose = v;
	perThread = t;
	nullThread = new Thread();
	previousThread = new Thread();
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

	while (!events.empty()) {
		Event* event = events.top();
		events.pop();

		// if verbose option chosen, display the current event
		if(verbose) event->toString();


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
            	threadDispatchComplete(event);
            	break;
            }

            case EventTypes::PROCESS_DISPATCH_COMPLETED: {
            	processDispatchComplete(event);
                break;
            }

            case EventTypes::THREAD_PREEMPTED: {
            	// no preemption for first come first serve scheduling
                break;
            }

            case EventTypes::CPU_BURST_COMPLETED: {
            	cpuBurstCompleted(event);
                break;
            }

            case EventTypes::IO_BURST_COMPLETED: {
            	ioBurstCompleted(event);
                break;
            }

            case EventTypes::THREAD_COMPLETED: {
            	threadComplete(event);
                break;
            }
        }
	}

	// if perThread option picked, display:
	if(perThread) ; displayProcessInfo();

	// display the basic thread information
	displayInfo();
}

/**
 threadArrived:
 initially checks for thread in the cpu or if the dispatcher is active: if not, it adds the
 first dispatching event. also will add the thread to the ready queue.
 */
void fcfsSimulator::threadArrived(Event* e) {

	// if no running thread and an inactive dispatcher -> essentially handles the start
	if(!cpuBusy && !dispatcherActive) {

		// get ready queue size for message #
		int size = readyQueue.size();

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
	Thread* t = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()];
	readyQueue.push(t);
}

/**
 dispatchInvoked:
 sets busy flag and determines whether the OS has to perform a full process switch or can 
 perfomr a simple thread switch.
 */
void fcfsSimulator::dispatchInvoked(Event* e) {

	// set the dispatcher flag to true
	dispatcherActive = true;

	// look at the front of the ready queue
	Thread* t = readyQueue.front();

	// if the new thread is not part of the same process:
	if(t->getProcessID() != previousThread->getProcessID()) {
		
		// call OS overhead to switch between processes
		Event* newEvent = new Event(EventTypes::PROCESS_DISPATCH_COMPLETED,
									e->getTime() + processOverhead,
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Transitioned from READY to RUNNING");
		events.push(newEvent);
	}

	// otherwise, they belong to the same process:
	else {

		// call OS overhead to switch between threads of the same process
		Event* newEvent = new Event(EventTypes::THREAD_DISPATCH_COMPLETED,
									e->getTime() + threadOverhead,
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Transitioned from READY to RUNNING");
		events.push(newEvent);
	}
}

/**
 processDispatchComplete:
 sets flags accordingly. creates events based on if the thread will complete on this
 turn or needs to be blocked for an io request.
 */
void fcfsSimulator::processDispatchComplete(Event* e) {

	// indicate that the dispatcher is complete and the cpu is now running
	dispatcherActive = false;
	cpuBusy = true;

	// get the current thread
	currentThread = readyQueue.front(); readyQueue.pop();

	// get the next cpu burst time
	int runTime = currentThread->getBursts().front()->getBurstLength();
	currentThread->removeBurst();

	// if this is not the last burst:
	if(!currentThread->getBursts().empty()) {
	
		// indicate when the process dispatch will be complete
		Event* newEvent = new Event(EventTypes::CPU_BURST_COMPLETED,
									e->getTime() + runTime,
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Transitioned from RUNNING to BLOCKED");
		events.push(newEvent);
	}

	// otherwise:
	else {

		// creat the exit event
		Event* newEvent = new Event(EventTypes::THREAD_COMPLETED,
									e->getTime() + runTime,
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Transitioned from RUNNING to EXIT");
		events.push(newEvent);
	}
}

/**
 threadDispatchComplete:
 sets flags accordingly. creates events based on if the thread will complete on this
 turn or needs to be blocked for an io request.
 */
void fcfsSimulator::threadDispatchComplete(Event* e){

	// indicate that the dispatcher is complete and the cpu is now running
	dispatcherActive = false;
	cpuBusy = true;

	// get the current thread
	currentThread = readyQueue.front(); readyQueue.pop();

	// get the next cpu burst time
	int runTime = currentThread->getBursts().front()->getBurstLength();
	currentThread->removeBurst();

	// if this is not the last burst:
	if(!currentThread->getBursts().empty()) {
	
		// indicate when the process dispatch will be complete
		Event* newEvent = new Event(EventTypes::CPU_BURST_COMPLETED,
									e->getTime() + runTime,
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Transitioned from RUNNING to BLOCKED");
		events.push(newEvent);
	}

	// otherwise:
	else {

		// creat the exit event
		Event* newEvent = new Event(EventTypes::THREAD_COMPLETED,
									e->getTime() + runTime,
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Transitioned from RUNNING to EXIT");
		events.push(newEvent);

	}

}

/**
 cpuBurstCompleted:
 only is called if more IO events exist, so it will create an event for when this is completed. also
 begins dispatch of the next thread in the ready queue.
 */
void fcfsSimulator::cpuBurstCompleted(Event* e) {

	// set the previous thread to what was the current thread and the cpu to not busy
	previousThread = currentThread;
	cpuBusy = false;

	// get the io burst time
	int ioTime = currentThread->getBursts().front()->getBurstLength();
	currentThread->removeBurst();

	// indicate when the process dispatch will be complete
	Event* newEvent1 = new Event(EventTypes::IO_BURST_COMPLETED,
								e->getTime() + ioTime,
								e->getProcessID(),
								e->getThreadID(),
								e->getPriority(),
								"Transitioned from BLOCKED to READY");
	events.push(newEvent1);

	// get the next process in the ready queue
	Thread* t = readyQueue.front();

	// invoke the dispatcher if there are still threads in the ready queue
	if(readyQueue.size() > 0) {

		Event* newEvent2 = new Event(EventTypes::DISPATCHER_INVOKED,
									e->getTime(),
									t->getProcessID(),
									t->getThreadID(),
									processes[t->getProcessID()]->getPriorityType(),
									"Selected from # threads; will run to completion of burst");
		events.push(newEvent2);	
	}
	
	// since no thread is running, set the current thread to null
	currentThread = nullThread;
}

/**
 ioBurstCompleted:
 not really sure what the fuck to do here.
 */
void fcfsSimulator::ioBurstCompleted(Event* e) {

	// push the thread back onto the queue
	Thread* t = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()];
	readyQueue.push(t);

	// if no thread is currently running on the cpu and the dispatcher is not already active
	if(!cpuBusy && !dispatcherActive) {

		// dispatch the event
		Event* newEvent = new Event(EventTypes::DISPATCHER_INVOKED,
									e->getTime(),
									t->getProcessID(),
									t->getThreadID(),
									processes[t->getProcessID()]->getPriorityType(),
									"Selected from # threads; will run to completion of burst");
		events.push(newEvent);	
	}
}

/**
 threadComplete:
 upon completion of a thread, the dispatcher is once again invoked only if there
 are more threads waiting in the ready queue.
 */
void fcfsSimulator::threadComplete(Event* e) {

	// reset current and previous threads
	previousThread = currentThread;
	currentThread = nullThread;

	// capture the current end time
	processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->setEndTime(e->getTime());

	// continue dispatching events if the ready queue has them
	if(!readyQueue.empty()) {

		Thread* t = readyQueue.front();
		Event* newEvent = new Event(EventTypes::DISPATCHER_INVOKED,
									e->getTime(),
									t->getProcessID(),
									t->getThreadID(),
									processes[t->getProcessID()]->getPriorityType(),
									"Selected from # threads; will run to completion of burst");
		events.push(newEvent);	

	}

}

/**
 displayInfo:
 prints out the information regarding each process
 */
void fcfsSimulator::displayInfo() {

	int sys_threads = 0;
	int int_threads = 0;
	int norm_threads = 0;
	int bat_threads = 0;

	for(int i = 0; i < processes.size(); i++) {
		if(processes[i]->getPriority() == 0) sys_threads = sys_threads + processes[i]->getProcessThreads().size();
		if(processes[i]->getPriority() == 1) int_threads = int_threads + processes[i]->getProcessThreads().size();
		if(processes[i]->getPriority() == 2) norm_threads = norm_threads + processes[i]->getProcessThreads().size();
		if(processes[i]->getPriority() == 3) bat_threads = bat_threads + processes[i]->getProcessThreads().size();
	}

	// display system thread information:
	printf("\n\nSYSTEM THREADS:\n");
	printf("\tTotal Count:\t\t%d\n", sys_threads);
	printf("\tAvg Response Time:\t%d\n", 0);
	printf("\tAvg Turnaround Time:\t%d\n", 0);

	// display interactive thread information:
	printf("\nINTERACTIVE THREADS:\n");
	printf("\tTotal Count:\t\t%d\n", int_threads);
	printf("\tAvg Response Time:\t%d\n", 0);
	printf("\tAvg Turnaround Time:\t%d\n", 0);

	// display normal thread information:
	printf("\nNORMAL THREADS:\n");
	printf("\tTotal Count:\t\t%d\n", norm_threads);
	printf("\tAvg Response Time:\t%d\n", 0);
	printf("\tAvg Turnaround Time:\t%d\n", 0);

	// display BATCH thread information:
	printf("\nBATCH THREADS:\n");
	printf("\tTotal Count:\t\t%d\n", bat_threads);
	printf("\tAvg Response Time:\t%d\n", 0);
	printf("\tAvg Turnaround Time:\t%d\n", 0);

	// display calculated times
	printf("\nTotal Elapsed Time:\t\t%d", 0);
	printf("\nTotal Service Time:\t\t%d", 0);
	printf("\nTotal I/O Time:\t\t\t%d", 0);
	printf("\nTotal Dispatch Time:\t\t%d", 0);
	printf("\nTotal Idle Time:\t\t%d\n", 0);

	// display CPU statistics
	printf("\nCPU Utilization:\t\t%2.2f%%", 0.0);
	printf("\nCPU Efficiency:\t\t\t%2.2f%%\n", 0.0);
}

/**
 displayProcessInfo:
 prints out the information regarding each process
 */
void fcfsSimulator::displayProcessInfo() {

	printf("\nProcess Info:\n");
	for(unsigned int i = 0; i < processes.size(); i++) {
		processes[i]->toString();
		printf("\n");
	}
}