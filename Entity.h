#pragma once

#include "lab_m1/Tema1/Renderer.h"
#include "components/simple_scene.h"
#include "lab_m1/Tema1/EntityManager.h"

class Entity
{
public:
    Entity();
    ~Entity();
    virtual std::vector<Renderer> Render(float deltaTime);
    void setId(int id);
    glm::ivec2 getPosition();
    float getRadius();

protected:
    int id;
    glm::ivec2 pos;
    float radius;
};