#include <dirent.h>
#include <unistd.h>
#include <string>
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
 
    if ((float) memTotal > 0.001) {
      return ((float)1 - (float)memAvailable / (float)memTotal);
    }
  }
  return 0; 
}


// DONE: Read and return the system uptime
float LinuxParser::UpTime() { 
  float uptime1{0}, uptime2{0};
  string word, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime1 >> uptime2;
  }
  return uptime1 + uptime2;
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


// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(vector<long> cpu_util_vect) { 
  long active_jiffies = Jiffies(cpu_util_vect) - IdleJiffies(cpu_util_vect);
  return active_jiffies;
}


// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(vector<long> cpu_util_vect) {
  long idle_jiffies;
  idle_jiffies = cpu_util_vect[3];
  return idle_jiffies;
}

// DONE: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  long user{0}, nice{0}, syst{0}, idle{0}, iowait{0}, irq{0}, softirq{0}, 
         steal{0}, guest{0}, guest_nice{0};
  string line, cpu;
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


// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
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
  return value;
}


// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key;
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
  return value; 
}


// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line{"n/a"};
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value{"0"};
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
  string line, key, value{"0"};
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
  string line, key;
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
vector<long> LinuxParser::CpuUtilization(int pid) {
  long utime{0}, stime{0}, cutime{0}, cstime{0}, starttime{0};
  string line;
  vector<long> data;
  string word;
  long number;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), '(', '_');
    std::replace(line.begin(), line.end(), ')', '_');
    //std::replace(line.begin(), line.end(), '-', '_');
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream linestream(line);
  
    for (int i=1; i<4; i++) { 
        linestream >> word;
        data.push_back(0);
    }
    for (int i=4; i<26; i++) { 
        linestream >> number;
        data.push_back(number);
    }

    utime = data[14-1];
    stime = data[15-1];
    cutime = data[16-1];
    cstime = data[17-1];
    starttime = data[22-1]; 
  }
  vector<long> proc_cpu_util {utime, stime, cutime, cstime, starttime};
  return proc_cpu_util; 
}


// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long starttime{0};
  long number;
  string line, word;
  vector<long> data;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), '(', '_');
    std::replace(line.begin(), line.end(), ')', '_');
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream linestream(line);
  
    for (int i=1; i<4; i++) { 
        linestream >> word;
        data.push_back(0);
    }
    for (int i=4; i<26; i++) { 
        linestream >> number;
        data.push_back(number);
    }
  
    starttime = data[22-1]; 
  }

  return starttime;
}