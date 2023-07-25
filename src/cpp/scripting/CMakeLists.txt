target_sources(${TARGET_NAME}
    PUBLIC
    "./lua_utils.h"
    "./lua_utils.cpp"
)

target_sources(${CLI_TARGET_NAME}
    PUBLIC
    "./lua_utils.h"
    "./lua_utils.cpp"
)

add_subdirectory("./extensions")
add_subdirectory("./script_types")