#pragma once

#include <queue>

#include "Process.h"
#include "Thread.h"
#include "Event.h"

/**
 Scheduler:
 a class containing several variations of CPU scheduling algorithms
 */
class Scheduler {

private:

public:

  /**
   Scheduler:
   default constructor, just builds an object
   */
  Scheduler();

  /**
   firstComeFirstServe:
   a function that will schedule events in the ready queue according to the
   simple first come first serve algorithm
   */
  void firstComeFirstServe(std::queue<Thread> &readyQueue);


};
