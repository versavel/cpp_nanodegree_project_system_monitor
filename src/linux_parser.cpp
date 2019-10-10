#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <vector>
#include <iostream>
#include <stdlib.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return "OS Name not found";
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    return kernel;
  }
  return "Kernel info not found";
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  long memTotal, memFree, memAvailable;
  string word, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream1(line);
    linestream1 >> word >> memTotal;
  
    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> word >> memFree;
   
    std::getline(stream, line);
    std::istringstream linestream3(line);
    linestream3 >> word >> memAvailable;
  }
  return ((float)1 - (float)memAvailable / (float)memTotal);  
}


// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  float uptime;
  string word, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return (long) uptime;
}


// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(vector<long> cpu_util_vect) {
  long jiffies{0};
  for (int i=0; i<10; i++) {
    jiffies += cpu_util_vect[i];
  }
  return jiffies; 
} 


// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function

//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(vector<long> cpu_util_vect) { 
  long active_jiffies = Jiffies(cpu_util_vect) - IdleJiffies(cpu_util_vect);
  return active_jiffies;
}


// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(vector<long> cpu_util_vect) {
  long idle_jiffies;
  idle_jiffies = cpu_util_vect[3];
  return idle_jiffies;
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  std::string cpu;
  long user, nice, syst, idle, iowait, irq, softirq, 
         steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> syst >> idle
               >> iowait >> irq >> softirq  
               >> steal >> guest >> guest_nice;
  }
  vector<long> cpu_utilization {user, nice, syst, idle,
                                iowait, irq, softirq, steal,
                                guest, guest_nice};
  return cpu_utilization;
  }


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', '_');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return 0;
}


// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string line;
  string key;
  int value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', '_');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return 0; 
}


// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value{" "};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return value;
        }
      }
    }
  }  
  return value;
}


// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return value;
}


// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;
  string key;
  string username, dummy, user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> dummy >> user) {
        if (user == uid) {
          return username;
        }
      }
    }
  }
  return "n/a";  
}

// DONE: Read and return the CPU data associated with a process
vector<long> LinuxParser::ProcessCpuUtilization(int pid) {
  long utime, stime, cutime, cstime, starttime;
  string line, dummy;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), '(', '_');
    std::replace(line.begin(), line.end(), ')', '_');
    std::replace(line.begin(), line.end(), '-', '_');
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream linestream(line);
  
    for (int i=0; i<12; i++) { linestream >> dummy;}
    linestream >> utime >> stime >> cutime >> cstime;
    for (int i=0; i<5; i++) { linestream >> dummy; }
    linestream >> starttime;
    }

  vector<long> proc_cpu_util {utime, stime, cutime, cstime, starttime};
  
  return proc_cpu_util;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }