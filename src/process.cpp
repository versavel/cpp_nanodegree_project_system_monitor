#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


// DONE: Return this process's ID
int Process::Pid() const  { return pid_; }

// DONE: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_); }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float Hertz = (float) sysconf(_SC_CLK_TCK);
    float uptime = LinuxParser::UpTime();
    vector<long> cpu_data = LinuxParser::CpuUtilization(pid_);
    float utime  = (float) cpu_data[0] / Hertz;
    float stime = (float) cpu_data[1] / Hertz;
    //float cutime = (float) cpu_data[2] / Hertz;
    //float cstime = (float) cpu_data[3] / Hertz;
    float starttime = (float) cpu_data[4] / Hertz;
    float cpu_time = utime + stime;
    float elapsed_time_since_the_process_started = uptime - starttime ;
    float cpu_usage_percentage{0};
    if (elapsed_time_since_the_process_started > 2) {
        cpu_usage_percentage = cpu_time / elapsed_time_since_the_process_started; 
    }
    return cpu_usage_percentage;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }


// DONE: Return this process's memory utilization
string Process::Ram() {
    string mem_util_kb = LinuxParser::Ram(pid_);
    string mem_util_mb = to_string((int)((float) stol(mem_util_kb, nullptr, 10) / 1024.0));
    return mem_util_mb;
}


// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
    float system_uptime_sec = LinuxParser::UpTime();
    long starttime_ticks = LinuxParser::UpTime(pid_);
    int Hertz = sysconf(_SC_CLK_TCK);
    float starttime_sec =  (float) starttime_ticks / (float) Hertz;
    return (long) (system_uptime_sec - starttime_sec); 
}


// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator< (Process & a) {
    return Process::CpuUtilization() < a.Process::CpuUtilization();
}