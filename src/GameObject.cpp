#include "GameObject.h"
#include "TextureManager.cpp"

GameObject::GameObject(const char *textureLabel, int x, int y, SDL_Rect srcRect, int scale) {

    objTexture = TextureManager::LoadTexture(textureLabel);

    position = Vector2D(x,y);

    this->srcRect = srcRect;
    this->scale = scale;
    

};

GameObject::~GameObject() {
    NULL;
};


void GameObject::Update() {

}

void GameObject::SetDestRect(int scale) {
    destRect = makeSDL_Rect(position.x, position.y, srcRect.w * scale, srcRect.h * scale);
}


void GameObject::Render() {

    SetDestRect(scale);
    SDL_RenderCopy(GameSpace::renderer, objTexture, &srcRect, GameSpace::camera->transformRect(&destRect));
}