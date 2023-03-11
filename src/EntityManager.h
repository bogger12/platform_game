#pragma once

#include "GameSpace.h"
#include "GameObject.h"

class EntityManager {
    public:
        GameObject* getEntityList() {return *EntityList;};
        void UpdateEntities();
        void RenderEntities();
        int addEntity(GameObject *entity);
    private:
        static const int maxEntities = 16;
        GameObject *EntityList[maxEntities];

};