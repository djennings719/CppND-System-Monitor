#include<vector>
#include<iostream>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using namespace std;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  vector<string> cpuParams = LinuxParser::CpuUtilization();

  prevIdle = idle;
  idle = stol(cpuParams[headers::IDLE]) + stol(cpuParams[headers::IO_WAIT]);
//   //PrevIdle = previdle + previowait
//   //Idle = idle + iowait

  prevNonIdle = nonIdle;
  nonIdle = stol(cpuParams[headers::USER])
            + stol(cpuParams[headers::NICE])
            + stol(cpuParams[headers::SYSTEM])
            + stol(cpuParams[headers::IRQ])
            + stol(cpuParams[headers::SOFT_IRQ])
            + stol(cpuParams[headers::STEAL]);
//   //PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
//   //NonIdle = user + nice + system + irq + softirq + steal

  prevTotal = total;
  total = idle + nonIdle;
//   //PrevTotal = PrevIdle + PrevNonIdle
//   //Total = Idle + NonIdle

  totalDiff = total - prevTotal;
  idleDiff = idle - prevIdle;
//   //# differentiate: actual value minus the previous one
//   //totald = Total - PrevTotal
//   //idled = Idle - PrevIdle

  cpuPercentage = ( totalDiff - idleDiff ) / totalDiff;
// //  CPU_Percentage = (totald - idled)/totald

  return cpuPercentage;
}