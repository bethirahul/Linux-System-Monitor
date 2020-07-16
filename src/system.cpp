#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>
#include <time.h>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

using std::string;
using std::vector;


System::System() {
    startTime_    = static_cast<long>(time(nullptr));
    cachedUpTime_ = LinuxParser::UpTime();
}

// TODO: Return the system's CPU
Processor& System::Cpu() {
    return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for(unsigned i=0; i<pids.size(); i++) {
        Process p(pids[i]);
        processes_.push_back(p);
    }
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    long currentTime = static_cast<long>(time(nullptr));
    return cachedUpTime_ + (currentTime - startTime_);
}