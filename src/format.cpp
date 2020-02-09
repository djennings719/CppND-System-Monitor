#include <string>

#include "format.h"

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  int hrs, mins, secs;

  hrs = seconds / 3600;
  mins = seconds / 60;
  secs = seconds % 60;
  
  return to_string(hrs) + ":" + Format::TimeFormatter(mins % 60) + ":" + Format::TimeFormatter(secs); 
}

string Format::TimeFormatter(int time){
  if(time >= 10){
    return to_string(time);
  }
  return "0" + to_string(time);
}