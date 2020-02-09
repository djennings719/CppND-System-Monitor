#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

using namespace boost;

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
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memoryUtilization;
  long memTotal, memFree, buffers, cached, value;
  string line, key;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memTotal = value;
        }
        else if (key == "MemFree"){
          memFree = value;
        }
        else if (key == "Buffers"){
          buffers = value;
        }
        else if (key == "Cached"){
          cached = value;
        }
      }
    }
  }

  //long free = memTotal - (memFree + buffers + cached);
  //memoryUtilization = ( memTotal - free ) / memTotal * 100.0; 

  return (memTotal-memFree) / memTotal * 100.0;//memoryUtilization; 

// MemTotal:        6110716 kB
// MemFree:         2076448 kB
// MemAvailable:    3800944 kB
// Buffers:          382240 kB
// Cached:          1496216 kB
  
// Total used memory = MemTotal - MemFree
// Non cache/buffer memory (green) = Total used memory - (Buffers + Cached memory)
// Buffers (blue) = Buffers
// Cached memory (yellow) = Cached + SReclaimable - Shmem
// Swap = SwapTotal - SwapFree

// Memory Utilized = (Total - Free) / Total* 100
  
// Free = Total - Free + Buffer + Cache 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long idletime, uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long jiffies;
  string line, cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> jiffies;
  }

  return jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  //long jiffies;
  int jiffy_index = 13;
  vector <string> fields;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    split(fields, line, is_any_of(" "));
  }

  return stol(fields[jiffy_index]); 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 

  long activeJiffies;
  string line, cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> activeJiffies;
  }
  return activeJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  return 0; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuStats;
  string line, key, value;
  int trim = 6;
  std::ifstream stream (kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    string trimmedLine = line.substr(trim);
    split(cpuStats, trimmedLine, is_any_of(" "));
  }
  return cpuStats; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int processes;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = stoi(value);
          break;
        }
      }
    }
  }

  return processes; 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int runningProcesses;
  string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          runningProcesses = stoi(value);
          break;
        }
      }
    }
  }

  return runningProcesses;  
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line, fqdnCommand, command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  vector<string> parsed;
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> fqdnCommand;
  }
  
  split(parsed, fqdnCommand, is_any_of(" "));
  
  return parsed.back(); 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  int ram;
  string line, key, value;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
  	while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
      	if(key == "VmSize:"){
          ram = stoi(value) / 1024;
          break;
        }
      }
    }
  }
  
  return to_string(ram); 
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, uid, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
  	while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
          break;
        }
      }
    }
  }
  
  return uid; 
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, key, value, username;
  string userId = LinuxParser::Uid(pid);
  vector<string> users;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
  	while(std::getline(stream, line)){
      std::istringstream linestream(line);
      split(users, line, is_any_of(":"));
      if(users[2] == userId){
      	username = users[0];
        break;
      }
    }
  }
  
  return username; 
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long uptime;
  int uptimeIndex = 21;
  string line;
  vector<string> results;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    split(results, line, is_any_of(" "));
  	uptime = stol(results[uptimeIndex]);
  }
  
  return uptime; 
}