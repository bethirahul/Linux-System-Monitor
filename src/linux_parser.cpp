#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
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
            linestream >> key >> value;
            if (key == "PRETTY_NAME") {
                std::replace(value.begin(), value.end(), '_', ' ');
                return value;
            }
        }
    }
    return value;
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
    }
    return kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    int memTotal, memFree, buffers, cached, sReclaimable, shmem;//, swapTotal, swapFree;
    string line, key, value;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if      (key == "MemTotal:")     { memTotal     = stoi(value); }
            else if (key == "MemFree:")      { memFree      = stoi(value); }
            else if (key == "Buffers:")      { buffers      = stoi(value); }
            else if (key == "Cached:")       { cached       = stoi(value); }
            else if (key == "SReclaimable:") { sReclaimable = stoi(value); }
            else if (key == "Shmem:")        { shmem        = stoi(value); }
        }
        int memUsed = memTotal - memFree - buffers - cached - sReclaimable + shmem;
        return (float)memUsed/(float)memTotal;
    }
    return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    string elapsedTime;
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> elapsedTime;
        return stol(elapsedTime);
    }
    return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    long totalJiffies = 0;
    vector<string> sValues = LinuxParser::CpuUtilization();
    for(int i=0; i<8; i++) {
        totalJiffies += stol(sValues[i]);
    }
    return totalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    long activeJiffies = 0;
    string line, s;
    std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        for(int i=0; i<13; i++) {
            linestream >> s;
        }
        for(int i=0; i<4; i++) {
            linestream >> s;
            activeJiffies += stol(s);
        }
        return activeJiffies;
    }
    return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    vector<string> values;
    string cpu, line, s;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> cpu;
        for(int i=0; i<8; i++) {
            linestream >> s;
            values.push_back(s);
        }
        return values;
    }
    return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line, key, value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "processes") { return stoi(value); }
        }
    }
    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, key, value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "procs_running") { return stoi(value); }
        }
    }
    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) {
    string(line);
    std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        return line;
    }
    return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) {
    string line, key, value;
    std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "VmSize:") {
                float ram = stof(value) / 1000.0f;
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << ram;
                return stream.str();
            }
        }
    }
    return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line, key, value;
    std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "Uid:") { return value; }
        }
    }
    return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string uid = Uid(pid);
    string line, value, x, key;
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> value >> x >> key;
            if (key == uid) { return value; }
        }
    }
    return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) {
    string line, value;
    std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        for(int i=0; i<22; i++) {
            linestream >> value;
        }
        long systemUpTime = UpTime();
        long elapsedTimeBeforeStart = stol(value)/sysconf(_SC_CLK_TCK);
        return systemUpTime - elapsedTimeBeforeStart;
    }
    return 0;
}