target_sources(${TARGET_NAME}
    PUBLIC
    "./asset_globals.h"
    
    "./AssetManager.h"
    "./AssetManager.cpp"
)

target_sources(${CLI_TARGET_NAME}
    PUBLIC
    "./asset_globals.h"
    
    "./AssetManager.h"
    "./AssetManager.cpp"
)

add_subdirectory("./asset_wrappers")