//
// Created by hir0xygen on 5/12/24.
//

#include "protections.hpp"

#include <csignal>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/mount.h>

auto scanProcFor(const std::string &entry, const std::string &value) -> bool {
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

auto processTracing() -> bool {
  return scanProcFor("/sys/kernel/yama/ptrace_scope", "2");
}

auto processTracing(const bool state) -> bool {
  std::ofstream sysctl_file("/proc/sys/kernel/yama/ptrace_scope");
  if (!sysctl_file) {
    std::cerr << "YAMA setting not found" << std::endl;
    return false;
  }

  sysctl_file << (state ? 2 : 0);
  if (sysctl_file.fail()) {
    std::cerr << "Failed updating YAMA setting" << std::endl;
    return false;
  }

  sysctl_file.close();
  if (sysctl_file.fail()) {
    std::cerr << "Failed closing YAMA setting" << std::endl;
    return false;
  }

  return true;
}

auto processIsolation() -> bool {
  return scanProcFor("/mounts", "hidepid=invisible");
}

auto processIsolation(const bool state) -> bool {
  if (-1 == mount("proc", "/proc", "proc", MS_REMOUNT | MS_RELATIME | MS_NODEV | MS_NOSUID | MS_NOEXEC | MS_PRIVATE, state ? "hidepid=2" : "hidepid=0")) {
    std::cerr << "Failed remounting proc - " << errno << " (" << std::strerror(errno) << ")" << std::endl;
    return false;
  }

  return true;
}