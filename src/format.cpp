#include <string>
#include <iostream>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string Format::ElapsedTime(long seconds) { 
    long hours, minutes;
    hours = seconds / 3600;
    seconds = seconds - 3600 * hours;
    minutes = seconds / 60;
    seconds = seconds - 60 * minutes;

    std::ostringstream oss;
    if (hours > 99) { return "99:59:59"; }
    if (hours < 10) { oss << "0"; }
    oss << hours << ":" ;
    if (minutes < 10) { oss << "0"; }
    oss << minutes << ":";
    if (seconds < 10) { oss << "0"; }
    oss << seconds ;
    return oss.str(); 
}