#include "Spring.h"

#define PI 3.14159265

#define K 0.4 // spring constant
#define EP 40 // equilibrium point
#define OBJ_MASS 1 // object on spring mass
#define DAMP 0.9 // damping constant

#define NO_CLICK true

float getAngle(float x, float y) {
    return atan2(-x,y);
}
float getHyp(float x, float y) {
    return sqrt(x*x+y*y); // pythagorus
}

Spring::Spring(const char *textureLabel, GameObject *p1, GameObject *p2, Vector2D offset) {

    objTexture = TextureManager::LoadTexture(textureLabel);

    srcRect.w = 3834;
    srcRect.h = 2875;
    srcRect.x = 0;
    srcRect.y = 0;

    this->p1 = p1;
    this->p2 = p2;
    
    createdtime = GameSpace::framecount;

};
// Gets mouse position
void Spring::HandleClick() {
    int x, y;
    
    if (NO_CLICK || SDL_GetMouseState(&x, &y)&SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (NO_CLICK) SDL_GetMouseState(&x, &y);
        //start_position = Vector2D(x-(spring_width/2), y);
        //start_position = player->position;
        //velocity = Vector2D(0,0);
    }
}


void Spring::Update() {
    float force, a;

    int x = p2->getPos().x -(p1->getPos().x+offset.x);
    int y = p2->getPos().y -(p1->getPos().y+offset.y);

    angle = getAngle(x,y);
    hyp = getHyp(x,y);


    float distance = hyp-EP;
    force = -K*(distance) * (distance >= 0);
    a = force/OBJ_MASS; // acceleration

    Vector2D acc_vector = Vector2D(a*sin(angle), -a*cos(angle));

    //acc_vector += Vector2D(0,9.81); // apply gravity constant

    springvelocitytotal += acc_vector/60;

    p1->addToV(acc_vector/60);
}


void Spring::Render(bool isline) {
    destRect = makeSDL_Rect(p1->getPos().x, p1->getPos().y, spring_width, hyp);

    if (isline) {
        Vector2D camerapos = GameSpace::camera->getPos();
        // Have to manually subtract Camera position as no SDL_Rect dest to modify
        SDL_SetRenderDrawColor(GameSpace::renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(
            GameSpace::renderer, 
            p1->getPos().x+offset.x - camerapos.x, 
            p1->getPos().y+offset.y - camerapos.y, 
            p2->getPos().x+p2->getW()/2 - camerapos.x,
            p2->getPos().y+p2->getH()/2 - camerapos.y
        );
        SDL_SetRenderDrawColor(GameSpace::renderer, 183, 205, 232, 255);
    }
    else {
        // The point around which the spring is flipped
        SDL_Point center;
        center.x = spring_width/2;
        center.y = 0; 
        
        SDL_RenderCopyEx(GameSpace::renderer, objTexture, &srcRect, &destRect, (angle * 180 / PI), &center, SDL_FLIP_VERTICAL);
    }
}