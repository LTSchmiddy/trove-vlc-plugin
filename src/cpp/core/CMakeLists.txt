add_sources_to_all_projects(
    PUBLIC
    "./globals.h"
    "./gfx_headers.h"

    "./logging.h"
    "./logging.cpp"
)

add_subdirectory("./assets")
add_subdirectory("./library")
add_subdirectory("./media_source")
add_subdirectory("./ns_abbr")
add_subdirectory("./scripting")
add_subdirectory("./settings")

