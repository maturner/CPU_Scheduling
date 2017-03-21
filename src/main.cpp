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
#include <getopt.h>
#include <cstring>
#include <queue>

#include "fcfsSimulator.h"


/**
 * get_flag:
 * checks for user command lind flags every loop of the main program
 */
void get_flag(int argc, char** argv, bool* per_thread, bool* verbose, int* algorithm, int* help) {

	// make a struct that defines user options
	static struct option long_options[] = {
		{"per_thread", 	no_argument, 		NULL, 	't'},
		{"verbose", 	no_argument, 		NULL, 	'v'},
		{"algorithm", 	required_argument, 	NULL, 	'a'},
		{"help", 		no_argument, 		NULL, 	'h'},
		{0, 0, 0, 0}
	};

	//
	while(true) {

		// stores the option index
		int option_index = 0;

		// get the flag character
		int c = getopt_long(argc, argv, "t:v:a:h", long_options, &option_index);

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
		if (optind < argc) {
			std::cout << "non-option ARGV-elements:" << std::endl;
			while(optind < argc) printf("%s ", argv[optind++]);
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
	unsigned int arrivalTime, numBursts, cBurst, iBurst;

	inFile >> arrivalTime
		   >> numBursts;

	// get a queue of CPU and IO Bursts
	std::queue<Burst*> bursts;

	for(unsigned int i = 0; i < numBursts - 1; i++) {
		inFile >> cBurst
			   >> iBurst;

		Burst* cpuBurst = new Burst(cBurst, "CPU");	bursts.push(cpuBurst);
		Burst* ioBurst = new Burst(iBurst, "IO");	bursts.push(ioBurst);
	}

	// get the last cpu burst
	inFile >> cBurst;
	Burst* cpuBurst = new Burst(cBurst, "CPU");	bursts.push(cpuBurst);

	Thread* t = new Thread(tid, pid, arrivalTime, "NEW", bursts);
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
		Thread* tempThread = getThread(inFile, i + 1, processID);
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

/**
 displayProcessInfo:
 prints out the information regarding each process
 */
void displayProcessInfo(std::vector<Process*> p) {

	printf("\nProcess Info:\n");
	for(unsigned int i = 0; i < p.size(); i++) {
		p[i]->toString();
		printf("\n");
	}
}

/**
 displayInfo:
 prints out the information regarding each process
 */
void displayInfo(std::vector<Process*> p) {

	int sys_threads = 0;
	int int_threads = 0;
	int norm_threads = 0;
	int bat_threads = 0;


	for(unsigned int i = 0; i < p.size(); i++) {
		if(p[i]->getPriority() == 0) sys_threads = sys_threads + p[i]->getProcessThreads().size();
		if(p[i]->getPriority() == 1) int_threads = int_threads + p[i]->getProcessThreads().size();
		if(p[i]->getPriority() == 2) norm_threads = norm_threads + p[i]->getProcessThreads().size();
		if(p[i]->getPriority() == 3) bat_threads = bat_threads + p[i]->getProcessThreads().size();
	}

	// display system thread information:
	printf("\n\n\nSYSTEM THREADS:\n");
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
	int h = 0;

	// get a command line flags
	get_flag(argc, argv, &t, &v, &a, &h);

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

	// create an object for scheduling
	fcfsSimulator fcfs(processes, threadOverhead, processOverhead, v);

	// call a scheduling scheme
	fcfs.run();

	// display basic system information
	displayInfo(processes);

	// display additional information based on user input flags
	if(t) ; displayProcessInfo(processes);
	//if(v) ; displayEventInfo(events);

	printf("\n\n\nEverything seemed to run okey doky!\n");
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------------------------
