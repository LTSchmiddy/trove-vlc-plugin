target_sources(${TARGET_NAME}
    PUBLIC
    gfx_headers.h
    globals.h
    app_globals.h

    logging.h
    logging.cpp
    main.cpp
)

add_subdirectory("./ui")
add_subdirectory("./settings")
add_subdirectory("./scripting")