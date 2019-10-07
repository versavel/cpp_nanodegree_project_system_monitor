#include "processor.h"
#include <vector>
#include <string>
#include "linux_parser.h"
//#include <iostream>
#include <chrono>
#include <thread>

using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<long> cpu_util_1 = LinuxParser::CpuUtilization();
    long total_jiffies_1 = LinuxParser::Jiffies(cpu_util_1);
    long active_jiffies_1 = LinuxParser::ActiveJiffies(cpu_util_1);
 
    std::this_thread::sleep_for(std::chrono::seconds(1));

    vector<long> cpu_util_2 = LinuxParser::CpuUtilization();
    long total_jiffies_2 = LinuxParser::Jiffies(cpu_util_2);
    long active_jiffies_2 = LinuxParser::ActiveJiffies(cpu_util_2);
 
    float delta_total = total_jiffies_2 - total_jiffies_1;
    float delta_active = active_jiffies_2 - active_jiffies_1;
    float cpu_util = delta_active / delta_total;
 
    return cpu_util;
    }