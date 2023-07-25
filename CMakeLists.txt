cmake_minimum_required(VERSION 3.0.0)
project(trove VERSION 0.1.0)

set(TARGET_NAME trove-manager)
set(CLI_TARGET_NAME trove-cli)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

cmake_policy(SET CMP0076 NEW)
set (CMAKE_CXX_STANDARD 20)

include_directories(${TRIPLET_INCLUDE_DIRS}) 

add_executable(${TARGET_NAME})
add_executable(${CLI_TARGET_NAME})

target_compile_definitions(${CLI_TARGET_NAME} PRIVATE CLI_BUILD=1)

function(link_target VAR_TARGET_NAME)
    # Adding SDL2
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME}
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )

    # Adding SDL2_image
    find_package(SDL2_image CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

    # Adding Imgui
    find_package(imgui CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE imgui::imgui)

    # Adding nlohmann_json
    find_package(nlohmann_json CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)

    # Adding Sqlite3
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE unofficial::sqlite3::sqlite3)

    # Adding Lua:
    find_package(Lua REQUIRED)
    target_include_directories(${VAR_TARGET_NAME} PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE ${LUA_LIBRARIES})

    # Adding Sol2
    find_package(sol2 CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE sol2)

    # Adding CxxOpts
    find_package(cxxopts CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE cxxopts::cxxopts)

    # Adding Plog:
    find_package(plog CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE plog::plog)

    # Adding Curlpp:
    find_package(unofficial-curlpp CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE unofficial::curlpp::curlpp)

    # Adding TinyFileDialogs:
    find_package(tinyfiledialogs CONFIG REQUIRED)
    target_link_libraries(${VAR_TARGET_NAME} PRIVATE tinyfiledialogs::tinyfiledialogs)

    set_target_properties(${VAR_TARGET_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "./arc/"
        LIBRARY_OUTPUT_DIRECTORY "./lib/"
        RUNTIME_OUTPUT_DIRECTORY "./bin/"
    )

endfunction()

# Used to add source files to both trove-manager and trove-cli
function(add_sources_to_all_projects P_IFACE)
    # message(${ARGN})

    # Adding to Manager:
    target_sources(${TARGET_NAME}
        ${P_IFACE}
        ${ARGN}
    )
    
    # Adding to CLI:
    target_sources(${CLI_TARGET_NAME}
        ${P_IFACE}
        ${ARGN}
    )
endfunction()

link_target(${TARGET_NAME})
link_target(${CLI_TARGET_NAME})

# Adding sourcecode:
include_directories("./src/cpp")
add_subdirectory("./src/cpp")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
