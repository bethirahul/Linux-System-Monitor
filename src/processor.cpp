#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
#include <vector>
#include <string>

using std::stoi;
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int user, nice, system, idle, ioWait, irq, softIrq, steal;
    vector<string> sValues = LinuxParser::CpuUtilization();
    if (sValues.size() == 8) {
        user    = stoi(sValues[0]);
        nice    = stoi(sValues[1]);
        system  = stoi(sValues[2]);
        idle    = stoi(sValues[3]);
        ioWait  = stoi(sValues[4]);
        irq     = stoi(sValues[5]);
        softIrq = stoi(sValues[6]);
        steal   = stoi(sValues[7]);

        idle = idle + ioWait;
        int total = idle + user + nice + system + irq + softIrq + steal;

        int totalDelta = total - prevTotal_;
        int idleDelta  = idle  - prevIdle_;

        prevTotal_ = total;
        prevIdle_  = idle;

        return ((float)(totalDelta - idleDelta)) / ((float)totalDelta);
    }
    return 0.0;
}