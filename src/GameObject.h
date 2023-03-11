#pragma once

#include "Vector2D.h"

class GameObject {
    public:

        GameObject(const char *textureLabel, int x, int y, SDL_Rect srcRect, int scale=1);
        ~GameObject();

        void Update();
        void Render();
        void SetDestRect(int scale);
        
        Vector2D getPos() { return position.vround(); };
        Vector2D getExactPos() { return position; };
        Vector2D getV() { return velocity; };
        Vector2D addToV(Vector2D addvector) { velocity += addvector; return velocity; };

        int getW() { return destRect.w; };
        int getH() { return destRect.h; };


    protected:

        Vector2D position = Vector2D(0,0);
        Vector2D velocity = Vector2D(0,0);
        
        SDL_Texture* objTexture;
        SDL_Rect srcRect, destRect;
        int scale;

};