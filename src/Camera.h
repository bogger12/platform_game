#pragma once

#include "GameSpace.h"
#include "Vector2D.h"
#include "Player.h"


class Camera {
    public:
        Camera();

        SDL_Rect* transformRect(SDL_Rect *objectrect);
        void updateCameraOffset(Player *player);

        Vector2D getPos();

        SDL_Rect rect;
    private:
        Vector2D position;
        Vector2D cameravelocity = Vector2D();


};