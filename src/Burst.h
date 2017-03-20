#pragma once

#ifndef Burst_h
#define Burst_h

/**
 * Burst:
 * a simple class containing information regarding a bust
 */
class Burst {

// private class variables and functions
private:
	int burstLength;
	const char* burstType;


// public class variables and functions
public:

	/**
	 getter functions:
	 return variable values for display functions
	 */
	int getBurstLength() { return burstLength; }
	const char* getBurstType() { return burstType; }

	/**
	 Burst:
	 constructor function to build a new burst
	 */
	Burst(int l, const char* t);

	/**
	 decrement:
	 decrements the burst length by a set amount upon completion of a burst
	 */
	void decrement(int i);

};

#endif