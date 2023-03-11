#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture) {

    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(GameSpace::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

int TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) {
    return SDL_RenderCopy(GameSpace::renderer, tex, &src, &dest);
}