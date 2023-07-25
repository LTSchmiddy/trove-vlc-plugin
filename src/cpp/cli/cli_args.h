#pragma once

#include <cxxopts.hpp>

namespace CLI {
    cxxopts::Options get_args_config();

    void process_args(cxxopts::Options& options, cxxopts::ParseResult& results);
}