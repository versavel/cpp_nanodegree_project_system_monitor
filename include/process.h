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
  int Pid() const;                        // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // Constructor
  Process(int pid) { 
      pid_ = pid;
      //std::cout << "pid_ is set to " << pid_ << "||";
  }

  // TODO: Declare any necessary private members
 private:
  int pid_;
};

#endif