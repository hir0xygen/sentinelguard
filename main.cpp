#include "protections.hpp"
#include "termcolor.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <boost/program_options.hpp>

namespace args = boost::program_options;
namespace tc = termcolor;

auto checkSentinel() -> void {
  if (processIsolation() && processTracing())
    std::cout << tc::color<100, 210, 170> << "[SENTINEL ARMED]" << std::endl;
  else
    std::cout << tc::color<255, 80, 110> << "[SENTINEL DISARMED]" << std::endl;
}

auto toggleSentinel(const bool state) -> void {
  if (!processTracing(state))
    std::cout << tc::color<255, 80, 110>  << "Failed setting protection 'process tracing' to state " << (state ? "enabled" : "disabled") << std::endl;
  if (!processIsolation(state))
    std::cout << tc::color<255, 80, 110>  << "Failed setting protection 'process isolation' to state " << (state ? "enabled" : "disabled") << std::endl;

  checkSentinel();
}

auto apexRunning() -> pid_t {
  auto pid = 0;
  
  for (const auto &entry: std::filesystem::__cxx11::directory_iterator("/proc")) {
    if (!entry.is_directory())
      continue;

    std::ifstream command_file(std::string(entry.path()) + "/cmdline");
    std::string command_line;
    std::getline(command_file, command_line);

    if (command_line.find("r5apex.exe") != std::string::npos) {
      pid = std::stoi(entry.path().filename());
      break;
    }
  }
  return pid;
}

auto embeddedLaunch() -> void {
  if (apexRunning() == 0)
    toggleSentinel(false);
  else
    exit(EXIT_FAILURE);

  while (apexRunning() == 0)
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

  std::this_thread::sleep_for(std::chrono::milliseconds(2500)); // wait for EAC to truly be done
  toggleSentinel(true);

  while (apexRunning() != 0)
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  toggleSentinel(false);

  exit(EXIT_SUCCESS);
}

auto main(const int argc, char *argv[]) -> int {
  std::string input;
  std::string output;

  args::options_description desc("Allowed options");
  desc.add_options()("help", "Print usage message")("check,c", "Check sentinel state")("enable,e", "Enable sentinel guarding")("disable,d", "Disable sentinel guarding")("steam,s", "For use when launching from Steam");

  args::variables_map vm;
  store(args::command_line_parser(argc, argv).options(desc).run(), vm);
  notify(vm);

  if (vm.contains("help") || !vm.contains("enable") && !vm.contains("disable") && !vm.contains("check") && !vm.contains("steam")) {
    std::cerr << desc << std::endl;
    return -1;
  }

  if (vm.contains("check"))
    checkSentinel();
  else if (vm.contains("enable"))
    toggleSentinel(true);
  else if (vm.contains("disable"))
    toggleSentinel(false);
  else if (vm.contains("steam"))
    embeddedLaunch();

  return 0;
}
