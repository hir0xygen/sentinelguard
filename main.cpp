#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_set>

#include "protections.hpp"
#include "termcolor.hpp"

namespace tc = termcolor;
const std::string program_version = "v0.0.6";

auto checkSentinel() -> void {
  if (processIsolation() && processTracing())
    std::cout << tc::color<100, 210, 170> << "[SENTINEL ARMED]" << tc::reset << std::endl;
  else
    std::cout << tc::color<255, 80, 110> << "[SENTINEL DISARMED]" << tc::reset << std::endl;
}

auto toggleSentinel(const bool state) -> void {
  if (!processTracing(state))
    std::cout << tc::color<255, 80, 110>  << "Failed setting protection 'process tracing' to state " << (state ? "enabled" : "disabled") << tc::reset << std::endl;
  if (!processIsolation(state))
    std::cout << tc::color<255, 80, 110>  << "Failed setting protection 'process isolation' to state " << (state ? "enabled" : "disabled") << tc::reset << std::endl;

  checkSentinel();
}

auto processRunning(const std::string &process_name) -> pid_t {
  auto pid = 0;
  
  for (const auto &entry: std::filesystem::__cxx11::directory_iterator("/proc")) {
    if (!entry.is_directory())
      continue;

    std::ifstream command_file(std::string(entry.path()) + "/cmdline");
    std::string command_line;
    std::getline(command_file, command_line);

    if (command_line.find(process_name) != std::string::npos) {
      pid = std::stoi(entry.path().filename());
      break;
    }
  }
  return pid;
}

auto embeddedLaunch() -> void {
  std::cout << "[SENTINEL] starting in embedded mode" << std::endl;
  if (processRunning("r5apex.exe") == 0) {
    toggleSentinel(false);
  } else {
    std::cout << "[SENTINEL] Apex already running, failure" << std::endl;
    exit(EXIT_FAILURE);
  }

  while (processRunning("r5apex.exe") == 0)
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

  std::cout << "[SENTINEL] Apex launched" << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(2500)); // wait for EAC to truly be done
  toggleSentinel(true);

  while (processRunning("r5apex.exe") != 0)
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  std::cout << "[SENTINEL] Apex closed" << std::endl;

  toggleSentinel(false);

  exit(EXIT_SUCCESS);
}

auto parseArgs(const int argc, char *argv[]) -> std::unordered_set<std::string> {
  std::unordered_set<std::string> args;
  for (auto i = 1; i < argc; ++i) {
    if (std::string arg = argv[i]; arg[0] == '-') {
      if (arg[1] == '-') {
        args.insert(arg.substr(2)); // long option
      } else {
        for (size_t j = 1; j < arg.length(); ++j)
          args.insert(std::string(1, arg[j])); // short option
      }
    }
  }
  return args;
}

void printUsage(const char* program_name) {
  std::cerr << "Usage: " << program_name << " [options]\n"
            << "Allowed options:\n"
            << "  --help           Print usage message\n"
            << "  -c, --check      Check sentinel state\n"
            << "  -e, --enable     Enable sentinel guarding\n"
            << "  -d, --disable    Disable sentinel guarding\n"
            << "  -s, --steam      For use when launching from Steam\n\n"
            << "sentinelguard - " << program_version << "\n";
}

auto main(const int argc, char *argv[]) -> int {
  if (geteuid() != 0) {
    std::cerr << "Not running as root, did setuid fail?" << std::endl;
    return -1;
  }

  const auto args = parseArgs(argc, argv);

  if (args.count("help") || (!args.count("enable") && !args.count("e") && !args.count("disable") && !args.count("d") && !args.count("check") && !args.count("c") && !args.count("steam") && !args.count("s"))) {
    printUsage(argv[0]);
    return -1;
  }

  if (args.count("check") || args.count("c"))
    checkSentinel();
  else if (args.count("enable") || args.count("e"))
    toggleSentinel(true);
  else if (args.count("disable") || args.count("d"))
    toggleSentinel(false);
  else if (args.count("steam") || args.count("s"))
    embeddedLaunch();

  return 0;
}