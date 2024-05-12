//
// Created by hir0xygen on 5/12/24.
//

#include "protections.hpp"

#include <csignal>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>
#include <sys/mount.h>

bool scanProcFor(const std::string &entry, const std::string &value) {
  std::ifstream proc_entry("/proc" + entry);
  if (!proc_entry.is_open()) {
    std::cerr << "Failed to open /proc" + entry << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(proc_entry, line)) {
    if (line.find(value) != std::string::npos) {
      proc_entry.close();
      return true;
    }
  }

  proc_entry.close();
  return false;
}

bool processTracing() {
  return scanProcFor("/sys/kernel/yama/ptrace_scope", "2");
}

bool processTracing(const bool state) {
  std::ofstream sysctl_file("/proc/sys/kernel/yama/ptrace_scope");
  if (!sysctl_file) {
    return false;
  }

  sysctl_file << (state ? 2 : 0);
  sysctl_file.close();

  if (sysctl_file.fail()) {
    return false;
  }

  return true;
}

bool processIsolation() {
  return scanProcFor("/mounts", "hidepid=invisible");
}

bool processIsolation(const bool state) {
  return -1 != mount("proc", "/proc", "proc", MS_REMOUNT | MS_RELATIME | MS_NODEV | MS_NOSUID | MS_NOEXEC | MS_PRIVATE, state ? "hidepid=2" : "hidepid=0");
}
