#include "Burst.h"

/**
 Burst:
 constructor function to build a new burst
 */
Burst::Burst(int l, const char* t) {
	this->burstLength = l;
	this->burstType = t;
}

/**
 decrement:
 decrements the burst length by a set amount upon completion of a burst
 */
void Burst::decrement(int i) {
	burstLength = burstLength - i;
}