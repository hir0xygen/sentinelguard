//
// Created by hir0xygen on 5/12/24.
//

#ifndef PROTECTIONS_HPP
#define PROTECTIONS_HPP
#include <string>

auto scanProcFor(const std::string &entry, const std::string &value) -> bool;

auto processTracing() -> bool;

auto processTracing(bool state) -> bool;

auto processIsolation() -> bool;

auto processIsolation(bool state) -> bool;

#endif //PROTECTIONS_HPP
