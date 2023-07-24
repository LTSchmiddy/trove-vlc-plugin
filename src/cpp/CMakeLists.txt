target_sources(${TARGET_NAME}
    PUBLIC
    "./gfx_headers.h"
    "./globals.h"
    "./app_globals.h"

    "./logging.h"
    "./logging.cpp"
    "./main.cpp"
)

add_subdirectory("./assets")
add_subdirectory("./library")
add_subdirectory("./media_source")
add_subdirectory("./ns_abbr")
add_subdirectory("./scripting")
add_subdirectory("./settings")
add_subdirectory("./ui")
