#pragma once
#include "Vector2D.h"


// using Corners = std::array<std::unique_ptr<Vector2D>, 4>;
class Collider {
    public:
        Collider(SDL_Rect collider_box, bool cansupport, std::string color = "red");
        ~Collider();
        void SetBox(SDL_Rect box);

        bool checkCollision(Vector2D displacement, SDL_Rect sprite);
        bool getSupported(SDL_Rect sprite);
        Vector2D getDisplacement(Vector2D displacement, SDL_Rect sprite);
        void Render();

        SDL_Rect collider_box;
    private:
        int top, bottom, left, right;

        bool cansupport = false;

        std::string color;
};