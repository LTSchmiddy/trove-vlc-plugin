#include <iostream>
#include <string>
#include <exception>

#include <process.hpp>

#include "ns_abbr/json.h"
#include "cli_args.h"
#include "ns_abbr/json.h"
#include "library/library_globals.h"
#include "app_globals.h"

namespace CLI {
    // Configure args:
    cxxopts::Options get_args_config() {
        cxxopts::Options options("Trove CLI", "Trove's command-line utility. Used by VLC for interacting with Trove.");

        options.add_options()
            ("h,help", "Print usage")
            ("l,get-library", "Dump the library database to stdout as Json data.")
            ("m,start-manager", "Start Trove Manager.");

        return options;
    }

    // Process the args:
    void process_args(cxxopts::Options& options, cxxopts::ParseResult& results) {
        if (results.arguments().size() == 0) {
            throw std::invalid_argument("No arguments provided.");
        }

        if (results.count("help")) {
            std::cout << options.help() << std::endl;
            return;
        }

        if (results.count("get-library")) {
            json db_json = Global::library_db->dumpToJson();
            std::cout << db_json.dump(4);
            return;
        }

        if (results.count("start-manager")) {
#ifdef _WIN32
            std::string exec_path = Global::app_path.parent_path().append("trove-manager.exe").string();
#else
            std::string exec_path = Global::app_path.parent_path().append("trove-manager").string();
#endif
            TinyProcessLib::Process manager_proc(exec_path);
        }
    }
}
