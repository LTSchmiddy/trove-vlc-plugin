target_sources(${TARGET_NAME}
    PUBLIC
    "./ui_globals.h"

    "./SubWindowHandler.h"
    "./SubWindowHandler.cpp"
)

add_subdirectory("base")
add_subdirectory("dialogs")
add_subdirectory("main_window")
add_subdirectory("menubar")
add_subdirectory("views")