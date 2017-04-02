/*************************************************************************
* Program: CpuScheduler
* Description:
* Author: Marcus Turner
* Date: March 5, 2017
*************************************************************************/

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <getopt.h>
#include <queue>

#include "fcfsSimulator.h"

/**
 * get_flag:
 * checks for user command lind flags every loop of the main program
 */
void get_flag(int argc, char** argv, bool* per_thread, bool* verbose, int* algorithm) {

	// make a struct that defines user options
	static struct option long_options[] = {
		{"per_thread", 	0, 		NULL, 	't'},
		{"verbose", 		0, 		NULL, 	'v'},
		{"algorithm", 	1, 		NULL, 	'a'},
		{"help", 				0, 		NULL, 	'h'},
		{0, 0, 0, 0}
	};

	//
	while(true) {

		// stores the option index
		int option_index = 0;

		// get the flag character
		int c = getopt_long(argc, argv, "tva:h", long_options, &option_index);

		// if returned -1, invalid character entered, break the loop
		if(c == -1) break;

		// otherwise, switch between different cases of flags
		switch(c) {

			// for 'per_thread':
			case 't':

				// set to true
				*per_thread = true;
				break;

			// for 'verbose':
			case 'v':

				// set to true
				*verbose = true;
				break;

			// for 'algorithm':
			case 'a':

				// choose which algorithm to use
				if (strcmp(optarg, "FCFS") == 0) *algorithm = 0;
				else if(strcmp(optarg, "RR") == 0) *algorithm = 1;
				else if(strcmp(optarg, "PRIORITY") == 0) *algorithm = 2;
				else if(strcmp(optarg, "CUSTOM") == 0) *algorithm = 3;

				// if invalid argument, diplay error and exit
				else {
					printf("Invalid option detected.\nDefaulting to FCFS.\n");
					*algorithm = 0;
				}
				break;

			// for 'help':
			case 'h':

				// display options
				printf("\nCommand line Flags:\n");
				printf("./simulator [flags] simulation_file.txt\n");
				printf("\t-t, --per_thread:\n\tOutput additional per-thread statistics for arrival time, service time, ect.\n\n");
				printf("\t-v, --verbose:\n\tOutput information about every state-changing event and scheduling decision.\n\n");
				printf("\t-a, --algorithm:\n\tThe scheduling algorithm to use. Can be FCFS, RR, PRIORITY, or CUSTOM.\n\n");
				printf("\t-h, --help:\n\tDisplay a help message about the command line flags and exit.\n\n");

				// exit (no error)
				exit(0);

			// default case: unrecognized argument
			default:
				exit(-1);
		}

		// print any remaining command line arguments (not options)
		if (option_index < argc) {
			std::cout << "non-option ARGV-elements:" << std::endl;
			while(option_index < argc) printf("%s ", argv[option_index++]);
			putchar('\n');
		}
	}
}


/**
 getThread:
 uses the passed filestream object to read in the thread info
 */
Thread* getThread(std::ifstream &inFile, int tid, int pid) {

	// get the process header information
	int arrivalTime, numBursts, cBurst, iBurst;
	int serviceTime = 0;
	int ioTime = 0;

	inFile >> arrivalTime
		   >> numBursts;

	// get a queue of CPU and IO Bursts
	std::queue<Burst*> bursts;

	for(int i = 0; i < numBursts - 1; i++) {
		inFile >> cBurst
			   >> iBurst;

		// capture service and ioTime (easiest place to do it is here)
		serviceTime = serviceTime + cBurst;
		ioTime = ioTime + iBurst;

		Burst* cpuBurst = new Burst(cBurst, "CPU");	bursts.push(cpuBurst);
		Burst* ioBurst = new Burst(iBurst, "IO");	bursts.push(ioBurst);
	}

	// get the last cpu burst
	inFile >> cBurst;
	serviceTime = serviceTime + cBurst;
	Burst* cpuBurst = new Burst(cBurst, "CPU");	bursts.push(cpuBurst);

	Thread* t = new Thread(tid, pid, arrivalTime, "NEW", bursts, serviceTime, ioTime);
	return t;
}

/**
 getProcess:
 uses the passed filestream object to read in the process info
 */
Process* getProcess(std::ifstream &inFile) {

	std::vector<Thread*> threads;

	// get the process header information
	int processID, processType, numThreads;
	inFile >> processID
		   >> processType
		   >> numThreads;

	for(int i = 0; i < numThreads; i++) {
		Thread* tempThread = getThread(inFile, i, processID);
		threads.push_back(tempThread);
	}

	Process* p = new Process(processID, processType, threads);
	return p;
}

/**
 readFile:
 returns a struct containing all the information read in from the text file
 */
struct fileData {
	std::vector<Process*> processes;
	int threadOverhead;
	int processOverhead;
};

fileData readFile(std::string inputFile) {

	// open a new file stream object
	std::ifstream inFile(inputFile);

	// declare struct variables
	std::vector<Process*> processes;
	int numProcesses, threadOverhead, processOverhead;

	// check for error when opening the file
	if (inFile.fail()) {
		std::cerr << "Error opening file" << std::endl;
	}

	// if successful:
	else {

		// get the first 3 elements of the file
		inFile >> numProcesses
			   >> threadOverhead
			   >> processOverhead;

		// get the processes
		for (int i = 0; i < numProcesses; i++) {
			Process* tempProcess = getProcess(inFile);
			processes.push_back(tempProcess);
		}

		// close the file stream
		inFile.close();
	}

	return {processes, threadOverhead, processOverhead};
}


//-----------------------------------------------------------------------------------------------
/**
 main:
 entry point for the program
 */
int main(int argc, char** argv) {

	// command line arguments
	bool t = false;
	bool v = false;
	int a = 0;

	// get a command line flags
	get_flag(argc, argv, &t, &v, &a);

	// scheduling variables
	std::vector<Process*> processes;	// a vector that holds the hierarchy of data from the input file
	std::queue<Event*> events;			// the event queue that stores all changes to the system

	// given overhead times for context switching
	unsigned int threadOverhead, processOverhead;

	// get the filename -> last argument in the command line
	char* f = argv[argc - 1];
	std::string file = f;

	// read in the text file and store data in variables
	auto fileData = readFile(file);
	processes = fileData.processes;
	threadOverhead = fileData.threadOverhead;
	processOverhead = fileData.processOverhead;

	// call the desired algorithm and run the simulation
	if(a == 1) {
		rrSimulator rr(processes, threadOverhead, processOverhead, v, t);
		rr.run();
	}
	else if(a == 2) {
		prioritySimulator p(processes, threadOverhead, processOverhead, v, t);
		p.run();
	}
	else if(a == 3) {
		//customSimulator c(processes, threadOverhead, processOverhead, v, t);
		//c.run();
	}
	else {
		fcfsSimulator fcfs(processes, threadOverhead, processOverhead, v, t);
		fcfs.run();


	printf("\n\n\nEverything seemed to run okey doky!\n");
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------------------------
