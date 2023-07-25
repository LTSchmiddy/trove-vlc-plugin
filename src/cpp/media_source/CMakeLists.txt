target_sources(${TARGET_NAME}
    PUBLIC
    "./source_globals.h"

    "./MediaSourceManager.h"
    "./MediaSourceManager.cpp"
    "./serialize.h"
    "./serialize.cpp"
)

target_sources(${CLI_TARGET_NAME}
    PUBLIC
    "./source_globals.h"

    "./MediaSourceManager.h"
    "./MediaSourceManager.cpp"
    "./serialize.h"
    "./serialize.cpp"
)


add_subdirectory("./source_types")