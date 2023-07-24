#include <iostream>

#include "SDL_Texture_wrapper.h"

namespace Assets::AssetWrappers {

    SdlTextureWrapper::SdlTextureWrapper(SDL_Texture* p_texture) {
        _texture = p_texture;
    }

    SdlTextureWrapper::~SdlTextureWrapper() {
        SDL_DestroyTexture(_texture);
    }

    SDL_Texture* SdlTextureWrapper::get() {
        return _texture;
    }

}