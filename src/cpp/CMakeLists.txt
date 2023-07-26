add_sources_to_all_projects(
    PUBLIC
    "./gfx_headers.h"
    "./globals.h"
    "./app_globals.h"

    "./logging.h"
    "./logging.cpp"
)

target_sources(${TARGET_NAME}
    PUBLIC
    "./main.cpp"
)

target_sources(${CLI_TARGET_NAME}
    PUBLIC
    "./cli_main.cpp"
)

add_subdirectory("./assets")
add_subdirectory("./cli")
add_subdirectory("./extern")
add_subdirectory("./library")
add_subdirectory("./media_source")
add_subdirectory("./ns_abbr")
add_subdirectory("./scripting")
add_subdirectory("./settings")
add_subdirectory("./ui")
