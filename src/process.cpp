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
    float Hertz = (float)sysconf(_SC_CLK_TCK);
    float uptime = (float)LinuxParser::UpTime();
    vector<long> proc_cpu_util = LinuxParser::ProcessCpuUtilization(pid_);
    float utime  = (float)proc_cpu_util[0] / Hertz;
    float stime = (float)proc_cpu_util[1] / Hertz;
    //float cutime = (float)proc_cpu_util[2] / Hertz;
    //float cstime = (float)proc_cpu_util[3] / Hertz;
    float starttime = (float)proc_cpu_util[4] / Hertz;
    float total_time = utime + stime;
    float elapsed_time_since_the_process_started = uptime - starttime ;
    float cpu_usage_percentage = 100 * (total_time / elapsed_time_since_the_process_started); 
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
    long system_uptime_sec = LinuxParser::UpTime();
    long starttime_ticks = LinuxParser::UpTime(pid_);
    int Hertz = 1; //sysconf(_SC_CLK_TCK);
    long starttime_sec = (long) ((float) starttime_ticks / (float) Hertz);
    return system_uptime_sec - starttime_sec; 
}


// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator< (Process & a) {
    return CpuUtilization() < a.CpuUtilization();
}