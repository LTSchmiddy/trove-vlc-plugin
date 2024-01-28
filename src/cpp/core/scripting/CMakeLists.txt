add_sources_to_all_projects(
    PUBLIC
    "./lua_utils.h"
    "./lua_utils.cpp"
)


add_subdirectory("./extensions")
add_subdirectory("./script_types")