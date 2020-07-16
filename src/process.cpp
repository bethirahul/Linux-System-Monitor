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

Process::Process(int pid) {
    pid_          = pid;
    user_         = LinuxParser::User(pid);
    command_      = LinuxParser::Command(pid);
    startTime_    = static_cast<long>(time(nullptr));
    cachedUpTime_ = LinuxParser::UpTime(pid);
    prevActiveJiffies_ = 0;
}

// TODO: Return this process's ID
int Process::Pid() const {
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    long totalCPUJiffies    = LinuxParser::Jiffies();
    long activeJiffies      = LinuxParser::ActiveJiffies(Pid());
    long deltaActiveJiffies = activeJiffies - prevActiveJiffies_;
    prevActiveJiffies_ = activeJiffies;
    if (totalCPUJiffies > 0) {
        return (float)deltaActiveJiffies / (float)totalCPUJiffies;
    }
    return 0.0;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(Pid());
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    long currentTime = static_cast<long>(time(nullptr));
    return cachedUpTime_ + (currentTime - startTime_);
    return 0;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(const Process& a) const {
    if (Pid() > a.Pid()) {
        return false;
    }
    return true;
}