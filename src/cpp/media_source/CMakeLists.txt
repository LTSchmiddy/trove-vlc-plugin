add_sources_to_all_projects(
    PUBLIC
    "./source_globals.h"

    "./MediaSourceManager.h"
    "./MediaSourceManager.cpp"
    "./serialize.h"
    "./serialize.cpp"
)

add_subdirectory("./source_types")