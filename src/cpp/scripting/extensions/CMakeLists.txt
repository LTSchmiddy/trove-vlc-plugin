target_sources(${TARGET_NAME}
    PUBLIC
    lua_json.h
    lua_json.cpp
    lua_log.h
    lua_log.cpp
    lua_web_requests.h
    lua_web_requests.cpp
)

add_subdirectory("./lua_cjson")
