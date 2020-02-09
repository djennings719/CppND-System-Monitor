#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  enum headers{CPU, USER, NICE, SYSTEM, IDLE, IO_WAIT, IRQ, SOFT_IRQ, STEAL, GUEST, GUEST_NICE};

  // these variables are based on the suggestion of the algorithm on stackoverflow:
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  long idle = 0;  
  long prevIdle;
  
  //PrevIdle = previdle + previowait
  //Idle = idle + iowait

  long nonIdle;
  long prevNonIdle;
  //PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
  //NonIdle = user + nice + system + irq + softirq + steal

  long total;
  long prevTotal;
  //PrevTotal = PrevIdle + PrevNonIdle
  //Total = Idle + NonIdle

  long totalDiff;
  long idleDiff;
  //# differentiate: actual value minus the previous one
  //totald = Total - PrevTotal
  //idled = Idle - PrevIdle

  float cpuPercentage;
  //CPU_Percentage = (totald - idled)/totald
};

#endif