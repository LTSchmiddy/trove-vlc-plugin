
#include <stdio.h>
#include <vector>

#include <plog/Log.h>

#include "globals.h"
#include "logging.h"
#include "media_source/serialize.h"
#include "ns_abbr/fs.h"
#include "ns_abbr/json.h"
#include "settings/root_settings.h"
#include "cli/cli_args.h"


#define SETTINGS_PATH "settings.json"
#define MEDIA_SOURCES_PATH "media_sources.json"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

// Main code
int main(int argc, char** argv) {
    Settings::load_settings(&Global::settings, SETTINGS_PATH);
    // Logging::setup_logs();

    // Loading Media Sources:
    Global::media_sources = new MediaSource::MediaSourceManager();
    Global::media_sources->loadSources(MEDIA_SOURCES_PATH);
    // Loading Library Database:
    Global::library_db = new Library::Database();
    Global::asset_manager = new Assets::AssetManager(); 
    
    // Load and parse args:
    try {
        cxxopts::Options options = CLI::get_args_config();
        cxxopts::ParseResult results = options.parse(argc, argv);

        CLI::process_args(options, results);

    // Error handling:
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what();
    }

    // Cleanup
    delete Global::asset_manager;
    delete Global::library_db;
    Settings::save_settings(&Global::settings, SETTINGS_PATH);
    Global::media_sources->saveSources(MEDIA_SOURCES_PATH);

    return 0;
}