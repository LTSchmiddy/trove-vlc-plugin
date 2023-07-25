target_sources(
    ${TARGET_NAME}
    PUBLIC

    "./SDL_Texture_wrapper.cpp"
    "./SDL_Texture_wrapper.h"
)

target_sources(
    ${CLI_TARGET_NAME}
    PUBLIC

    "./SDL_Texture_wrapper.cpp"
    "./SDL_Texture_wrapper.h"
)

# Not everything requires an asset wrapper, but for stuff that does, here we are.