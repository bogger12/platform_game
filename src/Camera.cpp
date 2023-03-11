#include "Camera.h"

// Camera position is designed to mimic position of camera looking at level, even though the camera function is moving the level itself.
Camera::Camera() {
    rect = makeSDL_Rect(-100,0,WIN_WIDTH,WIN_HEIGHT);
    position = Vector2D(0,0);
};

SDL_Rect* Camera::transformRect(SDL_Rect* objectrect) {
    objectrect->x -= (int) position.x;
    objectrect->y -= (int) position.y;
    return objectrect;
}

Vector2D Camera::getPos() {
    return position.vround();
}


#define K2 0.1// spring constant
#define EP2 10 // equilibrium point
#define OBJ_MASS 1 // object on spring mass
//#define DAMP 0.9 // damping constant

void Camera::updateCameraOffset(Player *player) {

    //check if player is out of boundaries relative to the screenspace, not level space.

    //make a spring connected to the player and the camera (horizontal) that will determine the position of the camera

    //position of player relative to the screen
    int screenxpos = player->getPos().x - this->position.x;
    int screenypos = player->getPos().y - this->position.y;

    std::cout << position << std::endl;

    //position of camera relative to the screen
    int cameramidx = rect.w/2;
    int cameramidy = rect.h/2;

    SDL_Rect destRect;
    destRect.x = cameramidx;
    destRect.y = cameramidy;
    destRect.w = destRect.h = 1;
    SDL_Texture* tex = TextureManager::LoadTexture("../assets/grabblepoint.png");

    SDL_RenderCopy(GameSpace::renderer, tex, NULL, &destRect);

    int x = screenxpos - cameramidx;
    int y = screenypos - cameramidy;

    float angle = atan2(-x,y);
    float hyp = sqrt(x*x+y*y);

    float distance = hyp-EP2;
    float force = -K2*(distance) * (distance >= 0);
    float a = force/OBJ_MASS; // acceleration
    Vector2D acc_vector = Vector2D(a*sin(angle), -a*cos(angle));
    position += acc_vector;
}