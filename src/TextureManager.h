#pragma once

class TextureManager {

    public:
        static SDL_Texture* LoadTexture(const char* fileName);
        static int Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);

};