#include <algorithm>

#include <plog/Log.h>

#include "FileSource.h"
#include "settings/settings_globals.h"


namespace MediaSource {
    std::string FileSource::getType() {
        return "file";
    }

    void FileSource::loadTypeSettings(json& settings_json) {
        if (settings_json.contains("rootPath")) rootPath = settings_json["rootPath"].get<std::string>();
    }
    void FileSource::saveTypeSettings(json& settings_json) {
        settings_json["rootPath"] = rootPath;
    }

    void FileSource::_scan(Scripting::ScriptTypes::MovieParserScript* parser) {
        scanDirectory(fs::path(getRootPath()), parser);
    }
    
    fs::path FileSource::getRootPath() {
        return fs::path(rootPath);
    }

    void FileSource::scanDirectory(fs::path dir, Scripting::ScriptTypes::MovieParserScript* parser){
        for (const fs::directory_entry &entry : fs::directory_iterator(dir)) {
            if (entry.is_directory()) {
                PLOGV.printf("Scanning Directory %s", entry.path().string().c_str());
                scanDirectory(entry, parser);
            } else if (entry.is_regular_file()) {
                PLOGV.printf("Scanning File %s", entry.path().string().c_str());
                
                // Skip this file if it's not a usable extension:
                if(std::find(
                    Global::settings.file_extensions.begin(),
                    Global::settings.file_extensions.end(),
                    entry.path().extension().string()
                ) == Global::settings.file_extensions.end()) {
                    continue;
                }

                // We're still operating on relative paths:
                fs::path rel_path = fs::relative(entry.path(), getRootPath());
                PLOGD << parser->parsePath(rel_path.parent_path().string(), rel_path.filename().string());
            }
        }
    }
}