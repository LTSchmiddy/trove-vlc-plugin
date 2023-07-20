#pragma once

#include <string>
#include <vector>
#include "ns_abbr/json.h"

namespace MediaSource {
    class SourceBase {

        std::string name;

        virtual void loadSettings(json settings_json);
        virtual void saveSettings(json settings_json);
        virtual void scanForFiles();

    };
}