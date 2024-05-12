#include "protections.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>

namespace args = boost::program_options;

void checkSentinel() {
  std::cout << ((processIsolation() && processTracing()) ? "[SENTINEL ARMED]" : "[SENTINEL DISARMED]") << std::endl;
}

void toggleSentinel(const bool state) {
  if (!processTracing(state))
    std::cout << "Failed setting protection 'process tracing' to state " << (state ? "enabled" : "disabled") << std::endl;
  if (!processIsolation(state))
    std::cout << "Failed setting protection 'process isolation' to state " << (state ? "enabled" : "disabled") << std::endl;

  checkSentinel();
}

int main(const int argc, char *argv[]) {
  std::string input;
  std::string output;

  args::options_description desc("Allowed options");
  desc.add_options()("help", "Print usage message")("check,c", "Check sentinel state")("enable,e", "Enable sentinel guarding")("disable,d", "Disable sentinel guarding");

  args::variables_map vm;
  store(args::command_line_parser(argc, argv).options(desc).run(), vm);
  notify(vm);

  if (vm.contains("help") || !vm.contains("enable") && !vm.contains("disable") && !vm.contains("check")) {
    std::cerr << desc << std::endl;
    return -1;
  }

  if (vm.contains("check"))
    checkSentinel();
  else if (vm.contains("enable"))
    toggleSentinel(true);
  else if (vm.contains("disable"))
    toggleSentinel(false);

  return 0;
}
