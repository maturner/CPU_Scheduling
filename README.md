Marcus Turner

I. List of Files:
	1. bin: folder containing some stuff that makes stuff work

	2. src: folder containing all source files
		a. Event.h/Event.cpp: a class holding information regarding a certain event that changes the state of the system.  Revolves heavily around the EVENT_TYPES enum.

		b. Process.h/Process.cpp: a class containing process information such as an ID, a priority number, and a vector of threads that belong to that process

		c. Thread.h/Thread.cpp: a class containing all pertenent information regarding a thread such as ID, arrival time, start time, completion time, vector of bursts, and a few others to aid in statistical calculations of the scheduling algorithm that is used.

		d. Burst.h/Burst.cpp: a simple class containing information on how long a burst is and whether or not it is a cpu or io burst

		e. fcfsSimulator.h/fcfsSimulator.cpp: a class that uses the inputed vector of processes to run a next event simulation based on the first come non-preemptive first serve scheduling algorithm

		f. rrSimulator.h/rrSimulator.cpp: a class that uses the inputed vector of processes to run a next event simulation based on the preemptive round robin scheduling algorithm

		g. prioritySimulator.h/prioritySimulator.cpp: a class that uses the inputed vector of processes to run a next event simulation based on a non-preemptive, priority scheduling algorithm, in which threads are dispatched from several hierarchical queues.

		h. customSimulator.h/customSimulator.cpp:

		i. main.cpp: the entry point for the simulator. This parses that command line, and runs the appropriate simulator using one of the classes from above (default is FCFS).

	3. input.txt: a given text file that is read in and used to populate the processes vector with all the information needed to run the simulation.

	4. input2.txt: a self create text file that is read in and used to populate the processes vector with all the information needed to run the simulation (used solely for additional tests).

	5. makefile: the file that is envoked by the 'make' command that builds (and optionally runs) the project.

	6. README.txt: this file that outlines the basic structure of the project.

	7. simulator: the executable cpp file built from the 'make' command

II. Unusual/Interesting Features:
	Honestly, I'm just happy the thing works right now

III. Approximate Time Spent:
	Deliverable 1: 6-7 hours
	Deliverable 2: 15-16 hours
	Deliverable 3 (final): unknown as of current

IV: Custom Scheduler Explanation:
	
	The custom scheduler settled on is simple combination of the Round Robin Scheduler and the Priority scheduler with a few additions.  Threads will be preempted if run past an alotted time period.  This time quantum is longer than the one specified in the RR simulation (specifically, it is set to 8) to allow more threads to run to completion of a CPU burst.  This is intended as a better balance of time, prevent threads from occupying too much CPU time but also allowing them time adequate time to complete and move to a BLOCKED or EXIT state.

	When it comes time to preempt a thread or move it to an event queue, the dispatcher will look for a thread from the same process to run next. This way, context switch time is minimized since it is attempting to only perform a thread switch instead of an entire process (as the overhead required for switching between threads is much less than the overhead required for switching between processes).  It starts by looking in the ready queue associated with the current thread.  If no more threads of that process are found, it will move to the highest priority queue with a READY thread.