target_sources(${TARGET_NAME}
    PUBLIC
    lua_utils.h
    lua_utils.cpp
    scraper_script.h
    scraper_script.cpp
)

add_subdirectory("extensions")