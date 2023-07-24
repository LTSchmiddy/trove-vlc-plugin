#pragma once

#include <SDL.h>

namespace Assets::AssetWrappers {
    class SdlTextureWrapper {
    public:
        SdlTextureWrapper(SDL_Texture* p_texture);
        ~SdlTextureWrapper();
        SDL_Texture* get();

    private:
        SDL_Texture* _texture = NULL;
    };
}