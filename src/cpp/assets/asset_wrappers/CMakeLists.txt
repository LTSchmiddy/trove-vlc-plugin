target_sources(
    ${TARGET_NAME}
    PUBLIC

    "./SDL_Texture_wrapper.cpp"
    "./SDL_Texture_wrapper.h"
)

# Not everything rrequires an asset wrapper, but for stuff that does, here we are.