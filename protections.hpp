//
// Created by hir0xygen on 5/12/24.
//

#ifndef PROTECTIONS_HPP
#define PROTECTIONS_HPP
#include <string>

bool scanProcFor(const std::string &entry, const std::string &value);

bool processTracing();

bool processTracing(bool state);

bool processIsolation();

bool processIsolation(bool state);

#endif //PROTECTIONS_HPP
