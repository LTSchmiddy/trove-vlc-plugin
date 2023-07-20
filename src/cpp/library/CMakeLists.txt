target_sources(${TARGET_NAME}
    PUBLIC
    "./library_globals.h"
    
    "./database.h"
    "./database.cpp"
)

add_subdirectory("./containers")