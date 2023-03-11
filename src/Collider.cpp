#include "Collider.h"
#include "TextureManager.h"

#include "utils.cpp"


Collider::Collider(SDL_Rect collider_box, bool cansupport, std::string color) {

    this->collider_box = collider_box; // the actual rect that we calculate collision with
    this->cansupport = cansupport;
    this->color = color;
}

Collider::~Collider() {
    
}

// This converts an SDL_Rect into the sides of the collision box
void Collider::SetBox(SDL_Rect box) {
    top = collider_box.y-box.h;
    bottom = collider_box.y+collider_box.h;
    left = collider_box.x-box.w;
    right = collider_box.x+collider_box.w;
}


bool Collider::checkCollision(Vector2D displacement, SDL_Rect sprite) {
    SetBox(sprite);
    
    Vector2D nextPosition = Vector2D(sprite.x, sprite.y) + displacement;

    bool x_axis_collision = (left < nextPosition.x && nextPosition.x < right);
    bool y_axis_collision = (top < nextPosition.y && nextPosition.y < bottom);

    return (x_axis_collision && y_axis_collision);
}

bool Collider::getSupported(SDL_Rect sprite) {
    if (!cansupport) return false;
    SetBox(sprite);
    return (left < sprite.x && sprite.x < right && sprite.y == top);
}

Vector2D Collider::getDisplacement(Vector2D displacement, SDL_Rect sprite) {
    SetBox(sprite);
    // std::cout << "start: " << displacement << ", ";

    bool topleft = sprite.x == left && sprite.y == top;
    bool topright = sprite.x == right && sprite.y == top;
    bool botleft = sprite.x == left && sprite.y == bottom;
    bool botright = sprite.x == right && sprite.y == bottom;
    
    bool isatcorner = (topleft||topright||botleft||botright) && !getSupported(sprite); // checking if object is at either 4 corners;

    if ((top < sprite.y && sprite.y < bottom)) {
        // if current position within y bounds
        if (displacement.x < 0) displacement.x = right - sprite.x;
        else if (displacement.x > 0) displacement.x = left - sprite.x;
    }
    if ((left < sprite.x && sprite.x < right)) { 
        // if current position within x bounds
        if (displacement.y > 0) {
            displacement.y = top - sprite.y;
        }
        else if (displacement.y < 0) displacement.y = bottom - sprite.y;
    }
    // std::cout << "end: "<< displacement << std::endl;
    return displacement;
}





void Collider::Render() {
    // if (!colliderTexture) colliderTexture = TextureManager::LoadTexture("../assets/collider.png");
    SDL_Rect col_copy = collider_box;
    SDL_Rect* pos_box = GameSpace::camera->transformRect(&col_copy);

    if (color=="red") SDL_SetRenderDrawColor(GameSpace::renderer, 255, 0, 0, 255);
    else if (color=="green") SDL_SetRenderDrawColor(GameSpace::renderer, 0, 255, 0, 255);
    else if (color=="blue") SDL_SetRenderDrawColor(GameSpace::renderer, 0, 0, 255, 255);
    else if (color=="yellow") SDL_SetRenderDrawColor(GameSpace::renderer, 255, 255, 0, 255);

    SDL_Point point1, point2, point3, point4;
    point1.x = point4.x = pos_box->x;
    point2.x = point3.x = pos_box->x+pos_box->w-1;
    point1.y = point2.y = pos_box->y;
    point3.y = point4.y = pos_box->y+pos_box->h-1;
    const SDL_Point points[5] = {point1, point2, point3, point4, point1};
    SDL_RenderDrawLines(GameSpace::renderer, points, 5);

    SDL_SetRenderDrawColor(GameSpace::renderer, 183, 205, 232, 255);

}