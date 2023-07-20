target_sources(${TARGET_NAME}
    PUBLIC
    "./lua_utils.h"
    "./lua_utils.cpp"
    "./MovieScraperScript.h"
    "./MovieScraperScript.cpp"
)

add_subdirectory("./extensions")