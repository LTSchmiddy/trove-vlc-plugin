target_sources(${TARGET_NAME}
    PUBLIC
    "./settings_globals.h"

    "./root_settings.h"
    "./root_settings.cpp"
)

target_sources(${CLI_TARGET_NAME}
    PUBLIC
    "./settings_globals.h"

    "./root_settings.h"
    "./root_settings.cpp"
)