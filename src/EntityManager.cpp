#include "EntityManager.h"


void EntityManager::UpdateEntities() {
    for (auto& e: EntityList) e->Update();
};

void EntityManager::RenderEntities() {
    for (auto& e: EntityList) e->Render();
};

int EntityManager::addEntity(GameObject *entity) {
    for (auto& e: EntityList) {
        if (e==nullptr) {
            e = entity;
        }
    }
}
