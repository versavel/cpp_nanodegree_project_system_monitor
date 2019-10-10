#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;                         // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process & a) ;            // DONE: See src/process.cpp

  // Constructor
  Process(int pid): pid_(pid) {};

  // DONE: Declare any necessary private members
 private:
  int pid_;
};

#endif