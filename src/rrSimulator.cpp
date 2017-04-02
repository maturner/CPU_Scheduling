#include <sstream>

#include "rrSimulator.h"

/**
 rrSimulator:
 a constructor that creates a round robin scheduling simulation
 */
rrSimulator::rrSimulator(std::vector<Process*> p, int to, int po, bool v, bool t) {

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
 run:
 runs the simulation using the following event functions
 */
void rrSimulator::run() {

	while (!events.empty()) {
		Event* event = events.top();
		events.pop();

		// if verbose option chosen, display the current event
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
            	threadDispatchComplete(event);
            	break;
            }

            case EventTypes::PROCESS_DISPATCH_COMPLETED: {
            	processDispatchComplete(event);
                break;
            }

            case EventTypes::THREAD_PREEMPTED: {
            	threadPreempted(event);
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
	if(perThread) displayProcessInfo();

	printf("\nSIMULATION COMPLETED!\n");

	// display the basic thread information
	displayInfo();
}

/**
 threadArrived:
 initially checks for thread in the cpu or if the dispatcher is active: if not, it adds the
 first dispatching event. also will add the thread to the ready queue.
 */
void rrSimulator::threadArrived(Event* e) {

	// if no running thread and an inactive dispatcher -> essentially handles the start case
	if(!cpuBusy && !dispatcherActive) {

		// invoke process dispatch
		Event* newEvent = new Event(EventTypes::DISPATCHER_INVOKED,
									e->getTime(),
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Selected from 1 threads; will run to completion of burst or end of time quantum");
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
void rrSimulator::dispatchInvoked(Event* e) {

	// set the dispatcher flag to true
	dispatcherActive = true;
	pdt = e->getTime();

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

		// if this is the process's first burst, set the start time
		if(processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->getStartTime() < 0)
			processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->setStartTime(e->getTime() + processOverhead);
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
void rrSimulator::processDispatchComplete(Event* e) {

	// if this is the process's first burst, set the start time
	if(processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->getStartTime() < 0)
		processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->setStartTime(e->getTime());

	// indicate that the dispatcher is complete and get its run time
	dispatcherActive = false;
	dispatchTime = dispatchTime + (e->getTime() - pdt);

	// indicate that the cpu is busy
	cpuBusy = true;

	// get the current thread
	currentThread = readyQueue.front(); readyQueue.pop();

	// get the next cpu burst time
	int runTime = currentThread->getBursts().front()->getBurstLength();

	// check to see if the thread will be need to be preempted
	if (runTime > timeQuantum) {

		//printf("\nThis should print if the thread needs to be preempted");
		//printf("\nThe expected service time is %d where as the time quantum is %d.\n", currentThread->getServiceTime(), timeQuantum);

		// decrement the burst length by the set amount
		currentThread->getBursts().front()->decrement(timeQuantum);

		// if so, indicate when the thread will need to be prempted
		Event* newEvent = new Event(EventTypes::THREAD_PREEMPTED,
									e->getTime() + timeQuantum,
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Transitioned from RUNNING to READY");
		events.push(newEvent);
	}

	// if not:
	else {
		
		// remove the burst from the thread's burst queue
		currentThread->removeBurst();

		// if this is not the last burst but it runs to completion:
		if(!currentThread->getBursts().empty()) {
			
			//printf("\nThis should print if the thread has a cpu burst that will start running.\n");

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

			//printf("\nThis should print if the thread will be completed after this cpu burst.\n");

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
}

/**
 threadDispatchComplete:
 sets flags accordingly. creates events based on if the thread will complete on this
 turn or needs to be blocked for an io request.
 */
void rrSimulator::threadDispatchComplete(Event* e){

	// if this is the process's first burst, set the start time
	if(processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->getStartTime() < 0)
		processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->setStartTime(e->getTime());

	// indicate that the dispatcher is complete and add to its total run time
	dispatcherActive = false;
	dispatchTime = dispatchTime + (e->getTime() - pdt);

	// indicate that the cpu is now busy
	cpuBusy = true;

	// get the current thread
	currentThread = readyQueue.front(); readyQueue.pop();

	// get the next cpu burst time
	int runTime = currentThread->getBursts().front()->getBurstLength();

	int cb = ((currentThread->getBursts().size()) / 2) + 1;
	int ib = ((currentThread->getBursts().size()) / 2);

	//printf("\nP%dT%d has %d cpu bursts and %d io bursts remaining", currentThread->getProcessID(), currentThread->getThreadID(), cb, ib);
	//printf("\n\tthe next cpu burst has %d time remaining (time quantum is %d)\n", runTime, timeQuantum);

	// check to see if the thread will be need to be preempted
	if (runTime > timeQuantum) {

		//printf("\nThis should print if the thread needs to be preempted");
		//printf("\nThe expected service time is %d where as the time quantum is %d.\n", currentThread->getServiceTime(), timeQuantum);

		// decrement the burst length by the set amount
		currentThread->getBursts().front()->decrement(timeQuantum);

		// if so, indicate when the thread will need to be prempted
		Event* newEvent = new Event(EventTypes::THREAD_PREEMPTED,
									e->getTime() + timeQuantum,
									e->getProcessID(),
									e->getThreadID(),
									e->getPriority(),
									"Transitioned from RUNNING to READY");
		events.push(newEvent);
	}

	// if not:
	else {
		
		// remove the burst from the thread's burst queue
		currentThread->removeBurst();

		// if this is not the last burst but it runs to completion:
		if(!currentThread->getBursts().empty()) {
			
			//printf("\nThis should print if the thread has a cpu burst that will start running.\n");

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

			//printf("\nThis should print if the thread will be completed after this cpu burst.\n");

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

}

/**
 threadPreempted:
 is called once if the current running process is taking up more than the allocated amount of
 CPU time.  This even places the thread back in the ready queue
*/
void rrSimulator::threadPreempted (Event* e) {

	// set the cpu to not busy
	cpuBusy = false;

	// put the current thread back in the ready queue
	readyQueue.push(currentThread);

	// set the previous thread to the current thread and the current thread to nothing
	previousThread = currentThread;
	currentThread = nullThread;

	// get ready to dispatch the next thread in the ready queue
	Thread* t = readyQueue.front(); 

	// build message
	int size = readyQueue.size();
	std::stringstream ss;
	ss << "Selected from " << size << " threads; will run to completion of burst or end of time quantum";
	std::string message = ss.str();

	//printf("\nThis is inside the threadPreempted function so the new event should be pushed.\n");

	// invoke the dispatcher
	Event* newEvent = new Event(EventTypes::DISPATCHER_INVOKED,
								e->getTime(),
								t->getProcessID(),
								t->getThreadID(),
								processes[t->getProcessID()]->getPriorityType(),
								message);
	events.push(newEvent);	

}

/**
 cpuBurstCompleted:
 only is called if more IO events exist, so it will create an event for when this is completed. also
 begins dispatch of the next thread in the ready queue.
 */
void rrSimulator::cpuBurstCompleted(Event* e) {

	// set the previous thread to what was the current thread
	previousThread = currentThread;

	// set cpu to not busy
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

		// build message
		int size = readyQueue.size();
		std::stringstream ss;
		ss << "Selected from " << size << " threads; will run to completion of burst";
		std::string message = ss.str();

		Event* newEvent2 = new Event(EventTypes::DISPATCHER_INVOKED,
									e->getTime(),
									t->getProcessID(),
									t->getThreadID(),
									processes[t->getProcessID()]->getPriorityType(),
									message);
		events.push(newEvent2);	
	}
	
	// since no thread is running, set the current thread to null
	currentThread = nullThread;
}

/**
 ioBurstCompleted:
 push the process back onto the ready queue and invoke the dispatcher if not already busy
 */
void rrSimulator::ioBurstCompleted(Event* e) {

	// push the thread back onto the queue
	Thread* t = processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()];
	readyQueue.push(t);

	// if no thread is currently running on the cpu and the dispatcher is not already active
	if(!cpuBusy && !dispatcherActive) {

		// build message
		int size = readyQueue.size();
		std::stringstream ss;
		ss << "Selected from " << size << " threads; will run to completion of burst or end of time quantum";
		std::string message = ss.str();

		// dispatch the event
		Event* newEvent = new Event(EventTypes::DISPATCHER_INVOKED,
									e->getTime(),
									t->getProcessID(),
									t->getThreadID(),
									processes[t->getProcessID()]->getPriorityType(),
									message);
		events.push(newEvent);	
	}
}

/**
 threadComplete:
 upon completion of a thread, the dispatcher is once again invoked only if there
 are more threads waiting in the ready queue.
 */
void rrSimulator::threadComplete(Event* e) {

	// reset current and previous threads
	previousThread = currentThread;
	currentThread = nullThread;

	// capture the current end time
	processes[e->getProcessID()]->getProcessThreads()[e->getThreadID()]->setEndTime(e->getTime());

	// continue dispatching events if the ready queue has them
	if(!readyQueue.empty()) {

		// look at the front of the ready queue
		Thread* t = readyQueue.front();

		// build message
		int size = readyQueue.size();
		std::stringstream ss;
		ss << "Selected from " << size << " threads; will run to completion of burstor end of time quantum";
		std::string message = ss.str();

		Event* newEvent = new Event(EventTypes::DISPATCHER_INVOKED,
									e->getTime(),
									t->getProcessID(),
									t->getThreadID(),
									processes[t->getProcessID()]->getPriorityType(),
									message);
		events.push(newEvent);	
	}

	// otherwise, the simulation is over so get the final end time
	else endTime = e->getTime();

}

/**
 calculateMetrics:
 calculates scheduling statistics based on results of the simulation
 */
void rrSimulator::calculateMetrics() {

	// calculate total service time andIO time
	for(int i = 0; i < (int)processes.size(); i++) {
			
		std::vector<Thread*> threads = processes[i]->getProcessThreads();
		for(int j = 0; j < (int)threads.size(); j++) {

			totalServiceTime = totalServiceTime + threads[j]->getServiceTime();
			totalIOTime = totalIOTime + threads[j]->getIOTime();

		}
	}

	// calculate idle time
	idleTime = endTime - (totalServiceTime + dispatchTime);

	// calculate CPU utilization
	cpuUtilization = (((float)(endTime - idleTime)) / ((float)endTime)) * 100;

	// calculate CPU Efficiency
	cpuEfficiency = (((float)totalServiceTime) / ((float)endTime)) * 100;
}


/**
 displayInfo:
 prints out the information regarding each process
 */
void rrSimulator::displayInfo() {

	int sys_threads = 0;  float sys_rt = 0;  float sys_trt = 0;
	int int_threads = 0;  float int_rt = 0;  float int_trt = 0;
	int norm_threads = 0; float norm_rt = 0; float norm_trt = 0;
	int bat_threads = 0;  float bat_rt = 0;  float bat_trt = 0;

	for(int i = 0; i < (int)processes.size(); i++) {
		
		// get SYSTEM thread information
		if(processes[i]->getPriority() == 0) {

			std::vector<Thread*> threads = processes[i]->getProcessThreads();
			
			// get number of threads
			sys_threads = sys_threads + threads.size();
			
			// get the average response time and average turnaround time
			for(int j = 0; j < (int)threads.size(); j++) {
				sys_rt = sys_rt + threads[j]->getResponseTime();
				sys_trt = sys_trt + threads[j]->getTurnaroundTime();
			}
			sys_rt = sys_rt / sys_threads;
			sys_trt = sys_trt / sys_threads;
		}
		
		// get INTEERACTIVE thread information
		if(processes[i]->getPriority() == 1) {

			std::vector<Thread*> threads = processes[i]->getProcessThreads();
			
			// get number of threads
			int_threads = int_threads + threads.size();
			
			// get the average response time and average turnaround time
			for(int j = 0; j < (int)threads.size(); j++) {
				int_rt = int_rt + threads[j]->getResponseTime();
				int_trt = int_trt + threads[j]->getTurnaroundTime();
			}
			int_rt = int_rt / int_threads;
			int_trt = int_trt / int_threads;
		}

		// get NORMAL thread information
		if(processes[i]->getPriority() == 2) {

			std::vector<Thread*> threads = processes[i]->getProcessThreads();
			
			// get number of threads
			norm_threads = norm_threads + threads.size();
			
			// get the average response time and average turnaround time
			for(int j = 0; j < (int)threads.size(); j++) {
				norm_rt = norm_rt + threads[j]->getResponseTime();
				norm_trt = norm_trt + threads[j]->getTurnaroundTime();
			}
			norm_rt = norm_rt / norm_threads;
			norm_trt = norm_trt / norm_threads;
		}

		// get BATCH thread information
		if(processes[i]->getPriority() == 3) {

			std::vector<Thread*> threads = processes[i]->getProcessThreads();
			
			// get number of threads
			bat_threads = bat_threads + threads.size();
			
			// get the average response time and average turnaround time
			for(int j = 0; j < (int)threads.size(); j++) {
				bat_rt = bat_rt + threads[j]->getResponseTime();
				bat_trt = bat_trt + threads[j]->getTurnaroundTime();
			}
			bat_rt = bat_rt / bat_threads;
			bat_trt = bat_trt / bat_threads;
		}
	}

	// calculate the given metrics
	calculateMetrics();

	// display system thread information:
	printf("\nSYSTEM THREADS:\n");
	printf("\tTotal Count:\t\t%d\n", sys_threads);
	printf("\tAvg Response Time:\t%2.2f\n", sys_rt);
	printf("\tAvg Turnaround Time:\t%2.2f\n", sys_trt);

	// display interactive thread information:
	printf("\nINTERACTIVE THREADS:\n");
	printf("\tTotal Count:\t\t%d\n", int_threads);
	printf("\tAvg Response Time:\t%2.2f\n", int_rt);
	printf("\tAvg Turnaround Time:\t%2.2f\n", int_trt);

	// display normal thread information:
	printf("\nNORMAL THREADS:\n");
	printf("\tTotal Count:\t\t%d\n", norm_threads);
	printf("\tAvg Response Time:\t%2.2f\n", norm_rt);
	printf("\tAvg Turnaround Time:\t%2.2f\n", norm_trt);

	// display BATCH thread information:
	printf("\nBATCH THREADS:\n");
	printf("\tTotal Count:\t\t%d\n", bat_threads);
	printf("\tAvg Response Time:\t%2.2f\n", bat_rt);
	printf("\tAvg Turnaround Time:\t%2.2f\n", bat_trt);

	// display calculated times
	printf("\nTotal Elapsed Time:\t\t%d", endTime);
	printf("\nTotal Service Time:\t\t%d", totalServiceTime);
	printf("\nTotal I/O Time:\t\t\t%d", totalIOTime);
	printf("\nTotal Dispatch Time:\t\t%d", dispatchTime);
	printf("\nTotal Idle Time:\t\t%d\n", idleTime);

	// display CPU statistics
	printf("\nCPU Utilization:\t\t%2.2f%%", cpuUtilization);
	printf("\nCPU Efficiency:\t\t\t%2.2f%%\n", cpuEfficiency);

}

/**
 displayProcessInfo:
 prints out the information regarding each process
 */
void rrSimulator::displayProcessInfo() {

	printf("\nProcess Info:\n");
	for(unsigned int i = 0; i < processes.size(); i++) {
		processes[i]->toString();
		printf("\n");
	}
}
