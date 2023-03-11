#pragma once

#include "Vector2D.h"
#include "Player.h"

class Spring {
    public:

        Spring(const char *textureLabel, GameObject *p1, GameObject *p2, Vector2D offset = Vector2D(0,0));

        void Update();
        void Render(bool isline=true);
        void HandleClick();

        Vector2D springvelocitytotal;

        float angle, hyp;

    private:
        SDL_Texture* objTexture, *objTexture2;
        SDL_Rect srcRect, destRect;

        Vector2D offset;

        const int spring_width = 120;

        GameObject *p1, *p2;

        Uint32 createdtime;
        



};