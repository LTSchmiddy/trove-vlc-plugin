#pragma once

#include <string>

namespace Library::Containers {
    class MovieContainer {
    public:
        // DB values:
        std::string source;
        std::string path;
        std::string title;
        std::string date;
        std::string desc;
        std::string poster_path;

        MovieContainer();
        MovieContainer(std::string p_source, std::string p_path, bool auto_load = true, bool* found = NULL);
        ~MovieContainer();

        bool reloadFromDb();
        bool loadFromDb(std::string p_source, std::string p_path);

        bool existsInDb();

        bool writeToDb();
    };
}